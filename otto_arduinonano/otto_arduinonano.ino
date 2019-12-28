#include <EEPROM.h>
#include <EnableInterrupt.h>
#include <Adafruit_NeoPixel.h> // Library to manage the Neopixel RGB led
#include <OttoSerialCommand.h> //-- Library to manage serial commands
OttoSerialCommand SCmd;  // The SerialCommand object
#include <Otto9Humanoid.h> //-- Otto Library version 9
Otto9Humanoid Otto;  //This is Otto!
//---------------------------------------------------------
//-- First step: Configure the pins where the servos are attached
/*
         --------------- 
        |     O   O     |
        |---------------|
RA 7==> |               | <== LA 6
        |               |
LR 3==> |               | <== LL 2
         --------------- 
            ||     ||
            ||     ||
FR 5==>   -----   ------  <== FL 4
         |-----   ------|
*/
// SERVO PINs //////////////////////////////////////////////////////////////////////////////
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
#define PIN_LA 6 //servo[4]  Left arm if enabled
#define PIN_RA 7 //servo[5]  Right arm if enabled
// ULTRASONIC PINs /////////////////////////////////////////////////////////////////////////
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
// BUZZER PIN //////////////////////////////////////////////////////////////////////////////
#define PIN_Buzzer  13 //BUZZER pin (13)
// SOUND SENSOR PIN //////////////////////////////////////////////////////////////////////////
#define PIN_NoiseSensor A6  //SOUND SENSOR   ANALOG pin (A6)
// LED MATRIX PINs //////////////////////////////////////////////////////////////////////////
#define DIN_PIN    A3   //DIN pin (A3)
#define CS_PIN     A2   //CS pin (A2)
#define CLK_PIN    A1   //CLK pin (A1)
#define LED_DIRECTION  1// LED MATRIX CONNECTOR position (orientation) 1 = top 2 = bottom 3 = left 4 = right  DEFAULT = 1
// BATTERY SENSE PIN //////////////////////////////////////////////////////////////////////////
boolean BATTcheck = false;    // SET TO FALSE IF NOT USING THIS OPTION
#define PIN_Battery   A7  //3v7 BATTERY MONITOR   ANALOG pin (A7)
// TOUCH SENSOR or PUSH BUTTON /////////////////////////////////////////////////////////////////
#define PIN_Button   A0 // TOUCH SENSOR Pin (A0) PULL DOWN RESISTOR MAYBE REQUIRED to stop false interrupts (interrupt PIN)
// RGB NEOPIXEL LED PIN   /////////////////////////////////////////////////////////////////////
// SERVO ASSEMBLY PIN   /////////////////////////////////////////////////////////////////////
// to help assemble Otto's feet and legs - wire link between pin 10 and GND
#define PIN_ASSEMBLY    10   //ASSEMBLY pin (10) LOW = assembly    HIGH  = normal operation
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////

const char programID[] = "OttoHumanoid_V9"; //Each program will have a ID
const char message1[] = "I AM OTTO"; //9 characters MAXIMUM
//-- Movement parameters
int T = 1000;            //Initial duration of movement
int moveId = 0;          //Number of movement
int modeId = 0;          //Number of mode
int moveSize = 15;       //Asociated with the height of some movements
volatile bool buttonPushed=false;  //Variable to remember when a button has been pushed
//---------------------------------------------------------
//-- Otto has 5 modes:
//--    * MODE = 0: Otto is awaiting
//--    * MODE = 1: Dancing mode!
//--    * MODE = 2: Obstacle detector mode
//--    * MODE = 3: Battery chek mode for Otto with LED matrix mouth
//--    * MODE = 4: OttoPAD or any Teleoperation mode (listening SerialPort).
//---------------------------------------------------------
volatile int MODE = 0; //State of Otto in the principal state machine.

unsigned long previousMillis = 0;
int randomDance = 0;
int randomSteps = 0;
bool obstacleDetected = false;
int REDled = 0;
int GREENled = 0;
int BLUEled = 0;
double batteryCHECK = 0;
unsigned long int matrix;
unsigned long timerMillis = 0;

double Duration = 0; //受信した間隔
double Distance = 0; //距離

void setup() {
  Serial.begin(9600);
  Otto.initHUMANOID(PIN_YL, PIN_YR, PIN_RL, PIN_RR, PIN_LA, PIN_RA, true, PIN_NoiseSensor, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins
  Otto.initMATRIX( DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);   // set up Matrix display pins = DIN pin,CS pin, CLK pin, MATRIX orientation 
  Otto.matrixIntensity(1);// set up Matrix display intensity
  Otto.initBatLevel(PIN_Battery);// set up Battery percent read pin - MUST BE AN ANALOG PIN
  randomSeed(analogRead(PIN_NoiseSensor));   //Set a random seed
  pinMode(PIN_ASSEMBLY,INPUT_PULLUP); // - Easy assembly pin - LOW is assembly Mode
  pinMode(PIN_Button,INPUT); // - ensure pull-down resistors are used

  pinMode( PIN_Echo, INPUT );
  pinMode( PIN_Trigger, OUTPUT );

  //Interrupts
  //enableInterrupt(PIN_Button, ButtonPushed, RISING);
  if (enableRGB == true) {  // ONLY IF RGB NEOPIXEL OPTION IS SELECTED
  NeopixelLed.begin();
  NeopixelLed.show(); // Initialize all pixels to 'off'
  NeopixelLed.setBrightness(64); // Op Brightness
  }

  //Otto wake up!
  Otto.sing(S_connection);
  Otto.home();
  // Animation Uuuuuh - A little moment of initial surprise
  //-----
  for (int i = 0; i < 2; i++) {
    for (int i = 0; i < 8; i++) {
      Otto.putAnimationMouth(littleUuh, i);
      delay(150);
    }
  }
  //Smile for a happy Otto :)
  Otto.putMouth(smile);
  Otto.sing(S_happy);
  delay(200);
  previousMillis = millis();
// if Pin 10 is LOW then place OTTO's servos in home mode to enable easy assembly, 
// when you have finished assembling Otto, remove the link between pin 10 and GND
  while (digitalRead(PIN_ASSEMBLY) == LOW) {
    Otto.home();
    Otto.sing(S_happy_short);   // sing every 5 seconds so we know OTTO is still working
    delay(500);
  }
delay (500);
Otto.clearMouth();
// test for matrix
 matrix = 0b000000000000000000000000000001;// set the variable to the first LED bottom RHS again
  for (int i = 0; i < 30; i++) { // this FOR NEXT LOOP repeats the code following it 30 times
      Otto.putMouth(matrix, false); // display the single LED
      matrix = matrix << 1 ;// shift the single LED one to the left (to the next LED)
      delay(250);// wait for 1/4 second, this is so that you can see the image on the Matrix 
      Otto.clearMouth();// clear the Matrix display so that it is ready for the next image
    }
  delay(500);// wait for 1 second, this is so that you can see the image on the Matrix for 1 second before repeating
  Otto.clearMouth();  
// write a text string of no more than nine limited characters and scroll at a speed between 50 and 150 (FAST and SLOW)
 // limited characters are : CAPITALS A to Z   NUMBERS 0 to 9    'SPACE'  : ; < >  = @ 
Otto.writeText (message1, 70);
delay (200);
Otto.clearMouth();
Otto.putMouth(smile);
}

void loop() {
  digitalWrite(PIN_Trigger, LOW);
  delayMicroseconds(2);
  digitalWrite( PIN_Trigger, HIGH ); //超音波を出力
  delayMicroseconds( 10 );
  digitalWrite( PIN_Trigger, LOW );
  Duration = pulseIn( PIN_Echo, HIGH ); //センサからの入力
  if (Duration > 0) {
    Duration = Duration/2; //往復距離を半分にする
    Distance = Duration*340*100/1000000; // 音速を340m/sに設定
    Serial.print("Distance:");
    Serial.print(Distance);
    Serial.println(" cm");
  }

  if(Distance <= 5.0){
    Serial.println("back");
    Otto.walk(1, 1300, -1);
  } else {
    Serial.println("straight");
    Otto.walk(1, 1000, 1);
  }
  delay(50);
}