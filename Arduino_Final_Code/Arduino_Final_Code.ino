/*
 This code is for an Arduino project that controls four servos using a joystick and potentiometer.
 And it plays MP3 files if a button is pushed.
 
 Sources:
 - Controlling a servo position using a potentiometer by Michal Rinott: <http://people.interaction-ivrea.it/m.rinott>
 - Servo Motor Basics with Arduino: http://www.arduino.cc/en/Tutorial/Knob
 - DF player project: http://educ8s.tv/arduino-mp3-player/
*/

#include "SoftwareSerial.h"       // Include the software serial library.
SoftwareSerial mySerial(12, 13);  // Declare the pins on the Arduino to which the Dfplayer mini is connected. 12 = output, 13 = input
# define Start_Byte 0x7E          // DF player constants and variables (see source above)
# define Version_Byte 0xFF        // DF player constants and variables (see source above)
# define Command_Length 0x06      // DF player constants and variables (see source above)
# define End_Byte 0xEF            // DF player constants and variables (see source above)
# define Acknowledge 0x00         // Returns info with command 0x41 [0x01: info, 0x00: no info]

#include <Servo.h>                //Include the servo library.

// Creates four servo objects for controlling servo motors.
Servo x;  // This servo moves the eye in the X position
Servo y;  // This servo moves the eye in the Y position
Servo b;  // This servo moves the under eyelid (bottom)
Servo t;  // This servo moves the upper eyelid (top)

// Define analog input pins used to connect a potentiometer,joystick and pushbutton.
int xpotpin = 0;      // Connected to a joystick that controls X
int ypotpin = 1;      // Connected to a joystick that controls Y
int opin = 2;         // Connected to a potentiometer
int bpin = 4;         // Connected to a joystick pushbutton
int buttonNext = 3;   // Connected to a push button


// Declare variables for storing values read from analog pins and other variables used in the code.
int xval;       // Value of the eye in the X position
int yval;       // Value of the eye in the Y position
int bval;       // Value of the under eyelid (bottom)
int tval;       // Value of the upper eyelid (top)
int xxval;      // Re-mapped value of eye X position
int yyval;      // Re-mapped value of eye Y position
int oval;       // Value of the potentiometer & re-mapped value
int blinkval;   // Value of the joystick pushbutton

boolean isPlaying = false;


void setup() {
  // Sets pin modes, attaches servo motors to their respective pins, 
  pinMode(bpin, INPUT);     // Joystick pushbutton
  x.attach(5);              // Servomotor for the eye in the X position
  y.attach(6);              // Servomotor for the eye in the Y position
  b.attach(9);              // Servomotor for the under eyelid
  t.attach(10);             // Servomotor for the upper eyelid
  Serial.begin(9600);       // Initializes the serial communication for the hardware serial (Serial) 

  // Configure buttonNext (a push button) to be ready for receiving input signals
  pinMode(buttonNext, INPUT);
  digitalWrite(buttonNext,HIGH);

mySerial.begin (9600);      // Initializes serial communication for the software serial (mySerial),

delay(100);                 // Wait 100 miliseconnds
setVolume(35);              // Set volume of DF player mini
}

void loop() {
// Controls the DF player playback based on the pushbutton's state
Serial.print(digitalRead(buttonNext));
  if(isPlaying == false) {
    pause();                // Pause the execution of the program
  }
  if (digitalRead(buttonNext) == HIGH) {
      Serial.print("test"); // Test if it's working
      playNext();           // Play the next track in the folder
      isPlaying = true;     // When it's true, the execution of the program will not be paused
  }

 // Reads analog values from pins and performs various servo control operations based on those values.
 blinkval = analogRead(bpin);            // Analog value of the joystick pushbutton
  oval = analogRead(opin);               // Analog value of the of the potentiometer (value between 0 and 1023)
  xval = analogRead(xpotpin);            // Analog value of the of the Joystick X eye
  yval = analogRead(ypotpin);            // Analog value of the of the Joystick Y eye
  oval = map(oval, 0, 1023, -45, 60);    // Re-mapped value of the potentiometer to use it with the servo
  xxval = map(xval, 0, 1023, 85, 105);   // Re-mapped value of Joystick X to use it with the servo (value between 0 and 180 degrees)
  yyval = map(yval, 0, 1023, 100, 20);   // Re-mapped value of Joystick Y to use it with the servo (value between 0 and 180 degrees)
  
  if (blinkval == 0){ // If  the Joystick button is pressed, move upper and under eyelid
    bval = 25;        // Value for blinking under eyelid
    tval = 35;        // Value for blinking upper eyelid
    b.write(bval);    // Move under eyelid
    t.write(tval);    // Move upper eyelid
   
    
  } else { // Eyelids follow the value of the potentiometer
  tval =  map(yval, 0, 1023, 0, 180);    // Re-mapped value of under eyelid to use it with the servo 
  bval =  map(yval, 0, 1023, 70, 0 );    // Re-mapped value of upper eyelid to use it with the servo 
  b.write(bval + oval); // Move under eyelid
  t.write(tval + oval); // Move upper eyelid
    }

  x.write(xxval); // Sets the servo position (for Eye X) according to the re-mapped value
  y.write(yyval); // Sets the servo position (for Eye Y) according to the re-mapped value


  delay(5); // Pauses the program for 5 miliseconds
}

// From here it's DF Player code. (see source above)
void playFirst()
{
  execute_CMD(0x3F, 0, 0);  // Specify the specific folder playback mode
  delay(500);
  execute_CMD(0x11,0,1);    // Play the first track in the folder
  delay(500);
}

void pause()
{
  execute_CMD(0x0E,0,0);    // Pause the DF player playback
  delay(500);
}

void play()
{
  execute_CMD(0x0D,0,1);    // Resume playback after it was paused
  delay(500);
}

void playNext()
{
  execute_CMD(0x01,0,1);    // Play the next track in the folder
  delay(500);
}



void setVolume(int volume)
{
  execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
  delay(2000);
}

void execute_CMD(byte CMD, byte Par1, byte Par2) // constructs and sends commands to the MP3 player module
// Excecute the command and parameters
{
// Calculate the checksum (2 bytes)
word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
// Build the command line
byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
//Send the command line to the module
for (byte k=0; k<10; k++)
{
mySerial.write( Command_line[k]);
}
}

