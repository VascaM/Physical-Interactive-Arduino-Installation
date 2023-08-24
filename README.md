# Physical interactive installation (Arduino programming language/C++)

The assignment: Make an interactive product with on sensors and actuators (hardware) to control your own application (software).

Finished product: I made an animatronic based on the character Bill Cipher from the TV show Gravity falls. 
There are 4 servo motors, a Df player mini, a speaker, a potentiometer, a push button and a joystick attached to a custom Arduino shield. By writing my own Arduino code, I can control various input and output components. In this case the code works like this:

- When the push button is pressed, it triggers the playback of the next track in the folder of MP3 files stored on the DFPlayer Mini module.
- The potentiometer is used to provide a user-adjustable parameter that influences the movement of the upper and lower eyelids of the "eye" mechanism driven by servo motors. It also determines the extent of the eyelid movement in response to the joystick input and button press.
- The joystick's X and Y values are used to control the horizontal and vertical movement of an eye by manipulating the position of the corresponding two servos. Additionally, the code includes logic for blinking the eye's upper and lower eyelids (another two servos) based on the state of the joystick's pushbutton.

Github files:
- I have added the functional code in the “…” file.  The code in this example is written in C++. The Arduino programming language is built upon C++ and includes specific functions and libraries that simplify programming for microcontrollers like Arduino boards. Essentially, Arduino code is C++ with added tools for easier interaction with hardware.
- And a video with the end result.
