#include <Arduino.h>

// IO defines
#define RED_LED_PIN 2
#define YELLOW_LED_PIN 3
#define GREEN_LED_PIN 4

// state machine defines
#define STATE_STARTED 1
#define STATE_WAITRED_END 2
#define STATE_WAIT_REDYELLOW_END 3
#define STATE_WAIT_GREEN_END 4
#define STATE_WAIT_YELLOW_END 5
#define STATE_WAIT_GORED 6

#define REDGREENTIME 2000
#define YELLOWTIME 1000
#define BLINKTIME 1000

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  static uint8_t currentState = STATE_STARTED;
  static unsigned long startTimeMillis;
  static unsigned long blinkTimeMillis;
  static uint8_t blinkToggle = 0;

  unsigned long currentTimeMillis = millis();
  
  if(currentTimeMillis - blinkTimeMillis >= BLINKTIME){
	  blinkToggle ^= 1;	//uisng XOR operation
	  blinkTimeMillis = currentTimeMillis;	//reset the currentTimeMillis for output
	  digitalWrite(LED_BUILTIN, blinkToggle);
  }
  
  switch(currentState){
	case STATE_STARTED:
		digitalWrite(RED_LED_PIN, HIGH);	// start the red light
		startTimeMillis = millis();	// remember the start time
		currentState = STATE_WAITRED_END;	//wait for the next state to the end
	break;
	
	case STATE_WAITRED_END:
		if(currentTimeMillis - startTimeMillis >= REDGREENTIME){	// previous state will run in 1s
			digitalWrite(YELLOW_LED_PIN, HIGH);	//after the red light run in 1s, next to the yellow light run
			digitalWrite(RED_LED_PIN, LOW);
			startTimeMillis = millis();	// update the start time
			currentState = STATE_WAIT_REDYELLOW_END;	//move on the next state
		} 
	break;
	
	case STATE_WAIT_REDYELLOW_END:
		if(currentTimeMillis - startTimeMillis >= YELLOWTIME){	// previous state will run in 1s
			digitalWrite(YELLOW_LED_PIN, LOW);
			digitalWrite(RED_LED_PIN, LOW);
			digitalWrite(GREEN_LED_PIN, HIGH);	//after the red and yellow light turn off, the green light will turn on
			startTimeMillis = millis();
			currentState = STATE_WAIT_GREEN_END;
		}	
	break;
	
	case STATE_WAIT_GREEN_END:
		if(currentTimeMillis - startTimeMillis >= REDGREENTIME){	// time is up, turn off the green light and turn on the yellow light
			digitalWrite(GREEN_LED_PIN, LOW);
			digitalWrite(YELLOW_LED_PIN, HIGH);
			startTimeMillis = millis();
			currentState = STATE_WAIT_YELLOW_END;
		}
	break;
	
	case STATE_WAIT_YELLOW_END:
		if(currentTimeMillis - startTimeMillis >= YELLOWTIME){	// return the first state after 1s
			digitalWrite(YELLOW_LED_PIN, LOW);
			currentState = STATE_STARTED;
		}
	break;
  }
}
