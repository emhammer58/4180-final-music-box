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