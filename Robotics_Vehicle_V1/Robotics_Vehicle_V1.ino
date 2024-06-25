/*
    Name:       Robotics_Vehicle_V1.ino
    Created:	24/06/2024 9:18:57 am
    Author:     MIMAS\reube
*/

#include <Servo.h>
#include <IRremote.hpp>


#pragma region PinDefine

#define RX 0;
#define TX 1;
//#define null 2;
int servoPin = 3;
int motor1 = 4;
int redLED = 5;
//#define null 6;
int motor2 = 7;
int motor3 = 8;
int motor4 = 9;
int IRPin = 10;
int blueLED = 11;
#define ultraTrig 12;
#define ultraEcho 13;

#pragma endregion

#pragma region OtherNonFunctionSetup

Servo myServo;
int motorPins[4] = { motor1, motor2, motor3, motor4 };

#pragma endregion


void setup()
{
    Serial.begin(9600);
    Serial.println("Begun Setup");

    TestLEDSetup();
    //Btooth Setup
    IRInitilise(); //IR Setup
    ServoInitilise();// Servo Setup
    MotorInitilise();
}

void TestLEDSetup()
{
    pinMode(redLED, OUTPUT);
    pinMode(blueLED, OUTPUT);
    digitalWrite(BlueLED, HIGH);
}

void IRInitilise()
{
    IrReceiver.begin(IRPin, ENABLE_LED_FEEDBACK);
    Serial.print("Receiving IR at pin ");
    Serial.println(IRPin);
}

void ServoInitilise()
{
    myServo.attach(servoPin);
    myServo.write(90);
}

void MotorInitilise()
{
    for (int i = 0; i <= 4; i++)
    {
        pinMode(motorPins[i], OUTPUT);
        digitalWrite(motorPins[i], LOW);
    }
}

void UltrasonicInitilise()
{

}

void GenericPinSetup(int pin, int type)
{
    digitalWrite(pin, type);
    if (type == 0)
    {
        digitalWrite(pin, LOW);
    }
}

void loop()
{


}
