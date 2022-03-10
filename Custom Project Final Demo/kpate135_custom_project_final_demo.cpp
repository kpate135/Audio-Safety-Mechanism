/*         Your Name & E-mail: Krishaan Patel kpate135@ucr.edu

*          Lab Section: 23

 *         Assignment: Custom Lab Project Final Demo Video 

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

int ledTimer = 0;

int buzzer = 9;
bool buzzerEnabled = true; //true for now, later implement menu within nokia 5110 lcd to toggle buzzerEnabled.
bool threshold_reached = false;
bool buzzer_threshold = false;

bool intensityValue = false;
int micLevel = 0;

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
} task;

const unsigned short tasksNum = 3;
task tasks[tasksNum];

enum MIC_States {MIC_START, MIC_CHECK} state;
int TickFct_MIC(int state);

enum LED_States {LED_START, LED_OFF, LED_ON};
int TickFct_LED(int state);

enum BUZZER_STATES {BUZZER_START, BUZZER_OFF, BUZZER_ON};
int TickFct_BUZZER(int state);

void setup() {
  
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
  tasks[i].state = LED_START;
  tasks[i].period = 50;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_LED;
  i++;
  tasks[i].state = BUZZER_START;
  tasks[i].period = 1000;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_BUZZER;
  i++;


}

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
          threshold_reached = true;
          buzzer_threshold = true;
        }
        else {
          threshold_reached = false;
        }
        

        micLevel = analogRead(micSensor_level); //uncomment after below testing to revert
        //micLevel = map(micLevel, 0, 700, 0, 10);
        Serial.println(micLevel); //uncomment after below testing to revert
        
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

int TickFct_LED(int state) {
  switch(state){ // Transition States
    case LED_START:
        state = LED_OFF;
    break;

    case LED_OFF:
        if (threshold_reached) {
          digitalWrite(ledPin, HIGH);
          ledTimer = 0;
          state = LED_ON;
        }
        else {
          digitalWrite(ledPin, LOW);
          state = LED_OFF;
        }
    break;

    case LED_ON:
      if (ledTimer <= 20) {
        state = LED_ON;
      }
      else{
        digitalWrite(ledPin, LOW);
        state = LED_OFF;
      }
    break;
    
    default:
      break;
  }


  switch(state){ // Action States
    case LED_START:
      
    break;

    case LED_OFF:
      
    break;

    case LED_ON:
      ++ledTimer;
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
        if (buzzer_threshold && buzzerEnabled) {
          state = BUZZER_ON;
        }
        else {
          state = BUZZER_OFF;
        }
    break;

    case BUZZER_ON:
      buzzer_threshold = false;
      state = BUZZER_OFF;
    break;
    
    default:
      break;
  }


  switch(state){ // Action States
    case BUZZER_START:
      
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
