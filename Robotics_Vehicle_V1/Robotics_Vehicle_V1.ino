/*
    Name:       Robotics_Vehicle_V1.ino
    Created:	24/06/2024 9:18:57 am
    Author:     MIMAS\reube
*/

#define DECODE_NEC  

#include <Ultrasonic.h>
#include <Servo.h>
#include <IRremote.hpp>

bool isDebuging = true;

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
int ultraTrig = 12;
int ultraEcho = 13;

#pragma endregion

#pragma region OtherNonFunctionSetup

Servo myServo;
int motorPins[4] = { motor1, motor2, motor3, motor4 };
Ultrasonic ultrasonic(ultraTrig, ultraEcho, 20000UL);

#pragma endregion

#pragma region RuntimeVariables

int servoAngle = 90;

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

    Serial.println("Setup Complete");
    Serial.println("--------------");
    digitalWrite(blueLED, LOW);
}

void TestLEDSetup()
{
    pinMode(redLED, OUTPUT);
    pinMode(blueLED, OUTPUT);
    digitalWrite(blueLED, HIGH);

    Serial.println("LED Setup Complete");
}

void IRInitilise()
{
    IrReceiver.begin(IRPin, ENABLE_LED_FEEDBACK);
    Serial.print("Receiving IR at pin ");
    Serial.println(IRPin);
    Serial.println("IR Setup Complete");
}

void ServoInitilise()
{
    myServo.attach(servoPin);
    myServo.write(90);

    Serial.println("Servo Setup Complete");
}

void MotorInitilise()
{
    for (int i = 0; i <= 4; i++)
    {
        pinMode(motorPins[i], OUTPUT);
        digitalWrite(motorPins[i], LOW);
    }

    Serial.println("Motor Setup Complete");
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
    /*
    Serial.print("Servo Angle: ");
    Serial.print(servoAngle);
    Serial.print(" | Ultrasonic: ");
    Serial.print(ultrasonic.read());
    Serial.println(" |");

    if (isDebuging)
    {
        digitalWrite(motor1, HIGH);
        digitalWrite(motor2, LOW);
        digitalWrite(motor3, HIGH);
        digitalWrite(motor4, LOW);
        delay(1500);
        digitalWrite(motor2, HIGH);
        digitalWrite(motor1, LOW);
        digitalWrite(motor4, HIGH);
        digitalWrite(motor3, LOW);
    }
    */
    IRReceive();
}


void IRReceive()
{
    if (IrReceiver.decode())
    {
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            Serial.println("||IR: Received noise or an unknown (or not yet enabled) protocol");
            IrReceiver.printIRResultRawFormatted(&Serial, true);
            IrReceiver.resume();
        }
        else {
            IrReceiver.resume();
            Serial.print("||IR: ");
            IrReceiver.printIRResultShort(&Serial);
            IrReceiver.printIRSendUsage(&Serial);
        }

    }
}

void IRDecode()
{
    
}