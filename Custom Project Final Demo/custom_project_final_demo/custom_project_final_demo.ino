/*         Your Name & E-mail: Krishaan Patel kpate135@ucr.edu

*          Lab Section: 23

 *         Assignment: Custom Lab Project Final Demo Video 

 *         Exercise Description: [optional - include for your own benefit]

 *        

 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.

 *

 *         Demo Link: https://www.youtube.com/watch?v=BEUl_NvDw6c
 */

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Declare LCD object for software SPI
// Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

int rotatetext = 1;

int ledPin = 2;
int micSensor_level = A0; //analog gives real-time voltage signal
int micSensor_intensity = 11; //digital gives LOW or HIGH sound intensity

int ledTimer = 0;

int button1 = A1;

int buzzer = 9;
bool buzzerEnabled = true; //toggle buzzerEnabled via button press
bool threshold_reached = false;
bool buzzer_threshold = false;

bool intensityValue = false;
int micLevel = 0;

//string nokiaDecibel; 

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
} task;

const unsigned short tasksNum = 5;
task tasks[tasksNum];

enum MIC_States {MIC_START, MIC_CHECK} state;
int TickFct_MIC(int state);

enum LED_States {LED_START, LED_OFF, LED_ON};
int TickFct_LED(int state);

enum BUZZER_TOGGLE_STATES {BUZZER_TOGGLE_START, BUZZER_TOGGLE_WAIT, BUZZER_TOGGLE};
int TickFct_BUZZER_TOGGLE(int state);

enum BUZZER_STATES {BUZZER_START, BUZZER_OFF, BUZZER_ON};
int TickFct_BUZZER(int state);

enum NOKIA_STATES {NOKIA_START, NOKIA_DISPLAY};
int TickFct_NOKIA(int state);

void setup() {
  
  pinMode(micSensor_level, INPUT);
  pinMode(micSensor_intensity, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
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
  tasks[i].state = BUZZER_TOGGLE_START;
  tasks[i].period = 50;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_BUZZER_TOGGLE;
  i++;
  tasks[i].state = BUZZER_START;
  tasks[i].period = 1000;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_BUZZER;
  i++;
  tasks[i].state = NOKIA_START;
  tasks[i].period = 50;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_NOKIA;
  i++;

  //Initialize Display
  display.begin();

  //Set Contrast
  display.setContrast(57);

  //Clear the buffer.
  display.clearDisplay();

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
        

        micLevel = analogRead(micSensor_level); 
        micLevel = map(micLevel, 500, 538, 0, 10);
        micLevel = abs(10 - micLevel);
        Serial.println(micLevel); 
        
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

int TickFct_BUZZER_TOGGLE(int state) {
  switch(state){ // Transition States
    case BUZZER_TOGGLE_START:
        state = BUZZER_TOGGLE_WAIT;
    break;

    case BUZZER_TOGGLE_WAIT:
        if (digitalRead(button1)) { //not pressed 
          state = BUZZER_TOGGLE_WAIT;
        }
        else if (!digitalRead(button1)) {
          buzzerEnabled = !buzzerEnabled;
          state = BUZZER_TOGGLE;
        }
    break;

    case BUZZER_TOGGLE:
      if (!digitalRead(button1)) { //button remains pressed
        state = BUZZER_TOGGLE;
      }
      else {
        state = BUZZER_TOGGLE_WAIT;
      }
    break;
    
    default:
      break;
  }


  switch(state){ // Action States
    case BUZZER_TOGGLE_START:
      
    break;

    case BUZZER_TOGGLE_WAIT:
      
    break;

    case BUZZER_TOGGLE:
      
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

int TickFct_NOKIA(int state) {
  switch(state){ // Transition States
    case NOKIA_START:
        state = NOKIA_DISPLAY;
    break;

    case NOKIA_DISPLAY:
        state = NOKIA_DISPLAY;
    break;
    
    default:
      break;
  }


  switch(state){ // Action States
    case NOKIA_START:
      
    break;

    case NOKIA_DISPLAY:
      display.clearDisplay();
      display.setTextColor(BLACK);
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.println("Sound Level: ");
      display.println(micLevel);
      display.println("");
      display.println("Intensity: ");
      if (threshold_reached) {
        display.println("HIGH");
      }
      else {
        display.println("LOW");
      }
      display.display();
    break;
    
    default:
      break;
  }
  return state;
}
