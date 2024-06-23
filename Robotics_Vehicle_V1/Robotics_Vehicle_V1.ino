/*
    Name:       Robotics_Vehicle_V1.ino
    Created:	24/06/2024 9:18:57 am
    Author:     MIMAS\reube
*/

#include <IRremote.hpp>

#pragma region PinDefine

#define RX 0;
#define TX 1;
//#define null 2;
#define servo 3;
#define motor1 4;
#define redLED 5;
//#define null 6;
#define motor2 7;
#define motor3 8;
#define motor4 9;
#define IRPin 10;
#define blueLED 11;
#define ultraTrig 12;
#define ultraEcho 13;

#pragma endregion

void setup()
{
    Serial.begin(9600);
    Serial.println("Begun Setup");

    IRInitilise();
}

void IRInitilise()
{
    IrReceiver.begin(IRPin, ENABLE_LED_FEEDBACK);
}

void loop()
{


}
