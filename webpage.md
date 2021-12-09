# 4180-Final-Music-Box

#### Emma Hammer & Mark Nicoletti
#### ECE 4180 Section A

------------------------------------------------------------------------------------------

# Overview

This Music Box is Emma Hammer and Mark Nicoletti's ECE 4180 Final Project for Fall 2021.
Upon opening, the music box begins to play one of three songs, a small star icon begins 
to spin, and an RGB LED lights up. The music box utilizes Bluetooth to pause/play the 
music, skip to the previous/next song, increase/decrease the volume, change the RGB LED's 
color, and stop/start the spinning icon.

# Parts Required

#### ARM Mbed LPC1768:

![Mbed](https://a.pololu-files.com/picture/0J3125.1200.jpg?28bb58b9f86b823615475f062ba3ed2b)

#### 2 Speakers:

![Speaker](https://media.digikey.com/Photos/Adafruit%20Industries%20LLC/3968.jpg)

#### Class D Audio Amplifier:

![AudioAmp](https://os.mbed.com/media/uploads/4180_1/classdamp.jpg)

#### Adafruit Bluefruit BLE:

![Bluetooth](https://cdn-shop.adafruit.com/1200x900/2479-01.jpg)

#### RGB LED:

![LED](https://cdn.sparkfun.com//assets/parts/6/5/5/7/11120-Diffused_LED_-_RGB_10mm-01.jpg)

#### DC Motor:

![motor](https://media.digikey.com/Photos/Adafruit%20Industries%20LLC/3777.JPG)

#### Mosfet Driver for DC Motor:

![mosfet](https://ae01.alicdn.com/kf/H7148e89b39e14b7ab82051feea12887bn/5PCS-Top-Mosfet-Button-IRF520-Mosfet-Driver-Module-For-Arduino-MCU-ARM-Raspberry-Pi-3-3v.jpg_Q90.jpg_.webp)

#### Limit Switch:

![switch](https://media.digikey.com/photos/Honeywell%20Photos/V7-2B17D8-022.jpg)

#### External Power Source:

![batteries](https://media.digikey.com/Photos/Memory%20Protection%20Photos/BH26AAL.jpg)

#### On/Off Switch:

![switch](https://res.cloudinary.com/rsc/image/upload/b_rgb:FFFFFF,c_pad,dpr_1.0,f_auto,h_337,q_auto,w_600/c_pad,h_337,w_600/F7347154-01?pgw=1)

# Wiring

|mbed           |Audio Amplifier   |Speaker  | 10K Potentiometer |
|:-------------:|:----------------:|:-------:|:-----------------:|
| Vout          | PWR+             |         |
| gnd           | PWR-, IN-        |         |
| p25           | IN+              |         |
|               | OUT+             | red     |
|               | OUT-             | black   |
|               | VOL+             |         | +
|               | VOLin            |         | in
|               | VOL-             |         | -
| p9            | S                |         | +

|mbed           |Bluefruit BLE   |
|:-------------:|:--------------:|
| p27           | TX             |         
| p28           | Rx             |         
| gnd           | CTS, GND       |         
| Vout          | V              |        
 
|mbed           |RGB LED         |Resistors  |
|:-------------:|:--------------:|:---------:|
|p13            |R               |180 ohms   |
|p14            |G               |100 ohms   |
|p15            |B               |100 ohms   |
|Vout           |+               |           |

|mbed           |DC Motor           |Mosfet   |External Power  |
|:-------------:|:-----------------:|:-------:|:--------------:|
|Vin            |                   |IN+      |+9V             |
|gnd            |                   |IN-      |                |
|               |Red                |OUT+     |                |
|               |Black              |OUT-     |                |

|mbed           |Limit Switch       |Resistor   |
|:-------------:|:-----------------:|:---------:|
|p9             |+                  |10 KOhms   |
|gnd            |-                  |           |

|mbed           |On/Off Switch      |External Power   |
|:-------------:|:-----------------:|:---------------:|
|               |+                  |+9V              |
|Vin            |-                  |                 |
|gnd            |gnd                |                 |

# How It Works

If the On/Off Switch is on when the music box is opened, the limit switch is released and the microcontroller unit will begin
playing music and the star icon will begin spinning. Additionally, the RGB LED will be white. 
Using the Bluefruit LE Phone Application's control pad, the user can Pause/Play the song, skip
to the next song, play the previous song, start/stop the spinning, and change the color of the
RGB LED. 

When a song ends, after a short pause, the same song will start playing from the beginning. There 
are three songs that the user can toggle between. Each song is stored in arrays on the mbed, and 
the PlayNote function in mbed's Speaker.h file is used to play the songs.

The mbed uses RTOS threads to run the speakers and Bluetooth simultaneously. To see the music box
in action, watch our demonstration at the link below:

> https://www.youtube.com/watch?v=n-1rNmdZwjg&ab_channel=MarkNicoletti

## RTOS Threading

The music block implements RTOS threading to run the speakers and Bluetooth simultaneously. There is
a speaker thread called Speaker_Thread to play the songs, and a main thread taking in Bluetooth inputs.

The Speaker Thread checks a volatile variable called SongNumber to determine what song is currently 
playing. Then, it uses a while loop to play the notes in that song until a Bluetooth button is pressed 
and the song is either paused or changed. 

The main thread constantly checks for Bluetooth inputs from the control pad on the Bluefruit LE app. 
Depending on which control pad button is pressed, various functionalities will occur (i.e., pause song,
change RGB LED to red). 

## Bluefruit BLE 

As previously mentioned, the main thread checks for Bluetooth inputs from the Bluefruit LE application's
control pad. The Bluefruit LE app can be downloaded to any smartphone, and connects to the Bluefruit BLE
module. The control pad has buttons numbered 1-4, and up/down/left/right arrows as seen below.

![controlpad](https://www.digikey.com/maker-media/72d5f0ce-c609-4d67-826b-78f716837bc6)

In the music box code, the buttons have the following functions:

- '1': Change LED to Red
- '2': Change LED to Blue
- '3': Change LED to Green
- '4': Turn off LED
- Up Arrow: Pause/Play Music
- Down Arrow: Pause/Play Spinning Star
- Left Arrow: Play Previous Song
- Right Arrow: Play Next Song

To use the Bluefruit LE app to control the Bluefruit BLE module, download the app from the app store and
open it. Upon opening the app, make sure your device's bluetooth is turned on. When the Bluefruit BLE module
is turned on and in range, the module's name (Adafruit Bluefruit LE) will show up on the app's homepage. Click
connect, then click the "controller mode" button. Finally, select "control pad" and the control pad will be
displayed. At this point, you can click the buttons and control the music box.

# Main.cpp Code

The code used to implement the music box and its various functionalities is displayed below.
Note that in order to correctly implement this code, you will need to import the mbed, rtos, 
and PinDetect libraries, along with the Speaker.h file from the mbed website into your project.

      #include "mbed.h"
      #include "Speaker.h"
      #include "rtos.h"
      #include "PinDetect.h"

      //-----------------------------------------------------
      //Component Declarations

      DigitalOut red(p13);            // RGB LED Red
      DigitalOut blue(p14);           // RGB LED Blue
      DigitalOut green(p15);          // RGB LED Green
      DigitalOut m(p12);              // DC Motor

      RawSerial bluetooth(p28,p27);   // Bluetooth adafruit serial port
      PinDetect limitSwitch(p9);      // Limit Switch
      Speaker mySpeaker(p25);         // Speakers

      //Songs
      float rowboat[27] = {261.63, 261.63, 261.63, 293.66, 329.63, 329.63, 293.66, 329.63, 349.23, 392, 261.63,261.63,261.63, 392,392,392, 329.63,329.63,329.63, 261.63,261.63,261.63, 392, 349.23, 329.63, 293.66, 261.63};
      float lamb[26] = {329.63, 293.66, 261.63, 293.66, 329.63, 329.63, 329.63, 293.66, 293.66, 293.66, 329.63, 329.63, 329.63, 329.63, 293.66, 261.63, 293.66, 329.63, 329.63, 329.63, 329.63, 293.66, 293.66, 329.63, 293.66, 261.63};
      float star[42] = {392, 392, 293.66, 293.66, 329.63, 329.63, 293.66, 261.63, 261.63, 493.88, 493.88, 440, 440, 392, 293.66, 293.66, 261.63, 261.63, 493.88, 493.88, 440, 293.66, 293.66, 261.63, 261.63, 493.88, 493.88, 440, 392, 392, 293.66, 293.66, 329.63, 329.63, 293.66, 261.63, 261.63, 493.88, 493.88, 440, 440, 392};

      //Variables
      static volatile int isPaused = 0;   // Tracks if the song has been paused
      volatile int boxOpen = 0;           // Tracks if the box is open or not
      volatile int spinningEnable = 0;    // Tracks if spinning is enabled
      volatile int songNumber = 1;        // Tracks which song is playing (1-3)
      volatile int note = 0;              // Tracks which note is playing
      volatile bool downPressed = 0;      // Tracks if down arrow is pressed
      volatile bool upPressed = 0;        // Tracks if up arrow is pressed
      volatile bool rightPressed = 0;     // Tracks if right arrow is pressed
      volatile bool leftPressed = 0;      // Tracks if left arrow is pressed

      //Interrupt Function for limit switch: Tracks if box is opened
      void turnOn()
      {
          boxOpen = 1;
          m = 0;
      }

      //Interrupt Function for limit switch: Tracks if box is closed
      void turnOff()
      {
          boxOpen = 0;
          m = 1;
      }

      //Speaker Thread: plays music to speakers
      void speaker_thread(void const *argument)
      {
          while(1)
          {
              led3 = boxOpen;
              led4 = isPaused;
              if(songNumber == 1)
              {
                  while(!isPaused)
                  {
                      mySpeaker.PlayNote(rowboat[note],0.5,1.0);
                      if(note < 26)
                      {
                          note++;
                      }
                      else
                      {
                          note = 0;
                          Thread::wait(1000);
                      }
                  }
              }
              if(songNumber == 2)
              {
                  while(!isPaused)
                  {
                      mySpeaker.PlayNote(lamb[note],0.5,1.0);
                      if(note < 25)
                      {
                          note++;
                      }
                      else
                      {
                          note = 0;
                          Thread::wait(1000);
                      }
                  }
              }
              if(songNumber == 3)
              {
                  while(!isPaused)
                  {
                      mySpeaker.PlayNote(star[note],0.5,1.0);
                      if(note < 41)
                      {
                          note++;
                      }
                      else
                      {
                          note = 0;
                          Thread::wait(1000);
                      }
                  }
              }
          Thread::wait(50);
          }
      }

      int main()
      {
          m = 0;

          //Set limit switch to pullup mode
          limitSwitch.mode(PullUp);

          //Setup interrupt callbacks for limit switch
          limitSwitch.attach_deasserted(&turnOn);
          limitSwitch.attach_asserted(&turnOff);

          //Set Sample Rate Frequency to check limit switch
          limitSwitch.setSampleFrequency(500);

          //Start Speaker Thread
          Thread thread1(speaker_thread);

          //Main Thread
          while (1)
          {
              char buttonPressed = 0;
              while(1)
              {
                  led = isPaused;
                  if(bluetooth.getc()=='!') //Check for serial input & ensure box is still open
                  {
                      if(bluetooth.getc()=='B') //Check for button press & ensure box is still open
                      {
                          buttonPressed = bluetooth.getc(); //Store which button was pressed in buttonPressed
                          if(buttonPressed == '1') //Change RGB LED Color 1
                          {
                              red = 0;
                              blue = 1;
                              green = 1;
                          }
                          if(buttonPressed == '2') //Change RGB LED Color 2
                          {
                              red = 1;
                              blue = 0;
                              green = 1;
                          }
                          if(buttonPressed == '3') //Change RGB LED Color 3
                          {
                              red = 1;
                              blue = 1;
                              green = 0;
                          }
                          if(buttonPressed == '4') //Change RGB LED Color 4
                          {
                              red = 1;
                              blue = 1;
                              green = 1;
                          }
                          if(buttonPressed == '5') //Up Arrow - Pause/Play Song
                          {
                              if(!upPressed)
                              {
                                  if(isPaused)
                                  {
                                      isPaused = 0;
                                  }
                                  else
                                  {
                                      isPaused = 1;
                                  }
                              }
                              upPressed = !upPressed;
                          }
                          if(buttonPressed == '6') //Down Arrow - Pause/Play Motor
                          {
                              led = !led;
                              if(!downPressed)
                              {
                                  if(spinningEnable == 1)
                                  {
                                      spinningEnable = 0; //Turn off Spin Enable
                                      m = 0; //Turn off Motor
                                  }
                                  else
                                  {
                                      spinningEnable = 1; //Turn on Spin Enable
                                      m = 1; //Turn on Motor
                                  }
                              }
                              downPressed = !downPressed;
                          }
                          if(buttonPressed == '7') //Left Arrow - Play Prev. Song
                          {
                              if(!leftPressed)
                              {
                                  songNumber--; //Decrement Song # Tracker
                                  if(songNumber < 1)
                                  {
                                      songNumber = 3;
                                  }
                              }
                              leftPressed = !leftPressed;
                              isPaused = 1;
                              Thread::wait(1000);
                              isPaused = 0;
                              note = 0;
                          }
                          if(buttonPressed == '8') //Right Arrow - Play Next Song
                          {
                              if(!rightPressed)
                              {
                                  songNumber++; //Increment Song # Tracker
                                  if(songNumber > 3)
                                  {
                                      songNumber = 1;
                                  }
                              }
                              rightPressed = !rightPressed;
                              isPaused = 1;
                              Thread::wait(1000);
                              isPaused = 0;
                              note = 0;
                          }
                      }
                  }
              Thread::wait(50);
              }
          }
      }
