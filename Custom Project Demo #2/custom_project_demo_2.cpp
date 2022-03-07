/*         Your Name & E-mail: Krishaan Patel kpate135@ucr.edu

*          Lab Section: 23

 *         Assignment: Custom Lab Project Demo Video #2

 *         Exercise Description: [optional - include for your own benefit]

 *        

 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.

 *

 *         Demo Link: PUT VIDEO HERE
 */


/*
const int b0 = 2;
const int b1 = 3;
const int b2 = 4;
const int b3 = 5;
const int b4 = 6;
const int b5 = 7;
const int b6 = 8;
const int b7 = 9;
int a = 0x00;
int a0, a1, a2, a3, a4, a5, a6, a7;
*/

int ledPin = 3;
int micSensor_level = A0; //analog gives real-time voltage signal
int micSensor_intensity = 11; //digital gives LOW or HIGH sound intensity

int buzzer = 9;
bool buzzerEnabled = true; //true for now, later implement menu within nokia 5110 lcd to toggle buzzerEnabled.
bool buzzer_threshold_reached = false;

bool intensityValue = false;
int micLevel = 0;

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
} task;

const unsigned short tasksNum = 2;
task tasks[tasksNum];

enum MIC_States {MIC_START, MIC_CHECK} state;
int TickFct_MIC(int state);

enum BUZZER_STATES {BUZZER_START, BUZZER_OFF, BUZZER_ON};
int TickFct_BUZZER(int state);

void setup() {
  // put your setup code here, to run once:
  /*
  pinMode(b0, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(b3, OUTPUT);
  pinMode(b4, OUTPUT);
  pinMode(b5, OUTPUT);
  pinMode(b6, OUTPUT);
  pinMode(b7, OUTPUT);
  */

  pinMode(micSensor_level, INPUT);
  pinMode(micSensor_intensity, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  Serial.begin(9600);
  
  unsigned char i = 0;
  tasks[i].state = MIC_START;
  tasks[i].period = 50;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_MIC;
  i++;
  tasks[i].state = BUZZER_START;
  tasks[i].period = 1000;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_BUZZER;
  i++;

}

/*                
void readData()
{
  if(Serial.available()){
    a = Serial.parseInt();
  }
  a7 = a >> 7;
  a6 = (a >> 6) & 0x01;
  a5 = (a >> 5) & 0x01;
  a4 = (a >> 4) & 0x01;
  a3 = (a >> 3) & 0x01;
  a2 = (a >> 2) & 0x01;
  a1 = (a >> 1) & 0x01;
  a0 = (a >> 0) & 0x01;
}

void resetB()
{
  digitalWrite(b7,LOW);
  digitalWrite(b6,LOW);
  digitalWrite(b5,LOW);
  digitalWrite(b4,LOW);
  digitalWrite(b3,LOW);
  digitalWrite(b2,LOW);
  digitalWrite(b1,LOW);
  digitalWrite(b0,LOW);
}

void writeToB(unsigned char temp)
{
  if ((temp>>7) & 0x01){digitalWrite(b7,HIGH);}
  if ((temp>>6) & 0x01){digitalWrite(b6,HIGH);}
  if ((temp>>5) & 0x01){digitalWrite(b5,HIGH);}
  if ((temp>>4) & 0x01){digitalWrite(b4,HIGH);}
  if ((temp>>3) & 0x01){digitalWrite(b3,HIGH);}
  if ((temp>>2) & 0x01){digitalWrite(b2,HIGH);}
  if ((temp>>1) & 0x01){digitalWrite(b1,HIGH);}
  if (temp & 0x01){digitalWrite(b0,HIGH);}
}
*/

void loop() {
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ((millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }
  
  delay(50);
}

int TickFct_MIC(int state) {
  switch(state){ // Transition States
    case MIC_START:
        state = MIC_CHECK;
    break;

    case MIC_CHECK:
        state = MIC_CHECK;
    break;
    
    
    default:
      break;
  }


  switch(state){ // Action States
    case MIC_START:
    
    break;

    case MIC_CHECK:
        intensityValue = digitalRead(micSensor_intensity);
        Serial.println(intensityValue);
        
        if (intensityValue == HIGH) {
          digitalWrite(ledPin, HIGH);
          buzzer_threshold_reached = true;
          delay(1000); //figure out better method than this for how to get LCD to stay on for a bit once this is triggered
          //maybe can do
          //tasks[0].period = 1000;
          //OR I can just set a shared bool variable to be true here and false in the else statement below.
          //Then in a separate LIGHT_LED task, I can have maybe a 1000ms period that the LED lights up when this shared bool variable goes true.
        }
        else {
          digitalWrite(ledPin, LOW);
          buzzer_threshold_reached = false;
          //tasks[0].period = 50;
        }
        

        micLevel = analogRead(micSensor_level); //uncomment after below testing to revert
        //micLevel = map(micLevel, 0, 700, 0, 10);
        Serial.println(micLevel); //uncomment after below testing to revert

        /*
        if (micLevel > 110) {
          digitalWrite(ledPin, HIGH);
        }
        else {
          digitalWrite(ledPin, LOW);
        }
        */
        
        /*
        //Testing with getting mic level values
        int sum = 0;
        int measurements = 128;
        for (int i = 0; i < measurements; ++i) {
          micLevel = analogRead(micSensor_level);
          sum += micLevel;
        }
        micLevel = sum / measurements;
        Serial.print("Sound Level: ");
        Serial.println(micLevel - 38);
        */

        /*
        int sensor_value = 0;
        int threshold = 509;
        int abs_value = 0;
        int ledCount = 10;

        sensor_value = analogRead(micSensor_level);
        abs_value = abs(sensor_value - threshold);
        int ledLevel = map(abs_value, 0, (1024 - threshold), 0, ledCount);
        Serial.print("ledLevel: ");
        Serial.println(ledLevel);
        */
    break;
    
    
    default:
      break;
  }
  return state;
}

int TickFct_BUZZER(int state) {
  switch(state){ // Transition States
    case BUZZER_START:
        state = BUZZER_OFF;
    break;

    case BUZZER_OFF:
        if (buzzer_threshold_reached && buzzerEnabled) {
          state = BUZZER_ON;
        }
        else {
          state = BUZZER_OFF;
        }
    break;

    case BUZZER_ON:
      state = BUZZER_OFF;
    break;
    
    default:
      break;
  }


  switch(state){ // Action States
    case BUZZER_START:
      state = BUZZER_OFF;
    break;

    case BUZZER_OFF:
      noTone(buzzer);
    break;

    case BUZZER_ON:
      tone(buzzer, 200);
    break;
    
    default:
      break;
  }
  return state;
}
