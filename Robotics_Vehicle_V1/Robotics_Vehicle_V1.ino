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
int motor1 = 8;
int redLED = 2;
//#define null 6;
int motor2 = 9;
int motor3 = 10;
int motor4 = 11;
int IRPin = 5;
int blueLED = 4;
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

bool hasIRRecv = false;

bool isFoward = false;
bool isBackward = false;
bool isLeft = false;
bool isRight = false;

#pragma endregion



void setup()
{
    Serial.begin(11520);
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
    IRDecode();
    MotorMaster();
}


void IRReceive()
{
    if (IrReceiver.decode())
    {
        hasIRRecv = true;
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            digitalWrite(redLED, HIGH);
            digitalWrite(blueLED, LOW);
            Serial.println("||IR: Received noise or an unknown (or not yet enabled) protocol");
            IrReceiver.printIRResultRawFormatted(&Serial, true);
            IrReceiver.resume();
        }
        else {
            digitalWrite(redLED, LOW);
            digitalWrite(blueLED, HIGH);
            IrReceiver.resume();
            Serial.print("||IR: ");
            IrReceiver.printIRResultShort(&Serial);
            IrReceiver.printIRSendUsage(&Serial);
        }
    }
    else
    {
        hasIRRecv = false;
    }
}

void IRDecode()
{
 
#pragma region ButtonCommands


    int but1 = 0x45;
    int but2 = 0x46;
    int but3 = 0x47;
    int but4 = 0x44;
    int but5 = 0x40;
    int but6 = 0x43;
    int but7 = 0x7;
    int but8 = 0x15;
    int but9 = 0x9;
    int but0 = 0x19;

    int butUp = 0x18;
    int butLeft = 0x8;
    int butRight = 0x5A;
    int butDown = 0x52;
    int butOk = 0x1C;
    int butStar = 0x16;
    int butHash = 0xD;

#pragma endregion


if (!hasIRRecv) { return; }

int IrData = IrReceiver.decodedIRData.command;

    if (IrData == butUp)
    {
        Serial.println("IR: BUTTON Up RECV");
        isFoward = true;
        isLeft = false;
        isRight = false;
        isBackward = false;


    }
    else if(IrData == butDown)
    {
        Serial.println("IR: BUTTON Down RECV");
        isFoward = false;
        isLeft = false;
        isRight = false;
        isBackward = false;


    }
    else if (IrData == butLeft)
    {
        Serial.println("IR: BUTTON Left RECV");
        isLeft = true;
        isFoward = false;
        isRight = false;

    }
    else if (IrData == butRight)
    {
        Serial.println("IR: BUTTON Right RECV");
        isRight = true;
        isFoward = false;
        isLeft = false;
        isBackward = false;


    }
    else if (IrData == butOk)
    {
        Serial.println("IR: BUTTON OK RECV");
        isBackward = true;
        isRight = false;
        isFoward = false;
        isLeft = false;
    }
    else if (IrData == but1)
    {
        Serial.println("IR: BUTTON 1 RECV");
    }
    else if (IrData == but2)
    {
        Serial.println("IR: BUTTON 2 RECV");
    }
    else if (IrData == but3)
    {
        Serial.println("IR: BUTTON 3 RECV");
    }
    else if (IrData == but4)
    {
        Serial.println("IR: BUTTON 4 RECV");
    }
    else if (IrData == but5)
    {
        Serial.println("IR: BUTTON 5 RECV");
    }
    else if (IrData == but6)
    {
        Serial.println("IR: BUTTON 6 RECV");
    }
    else if (IrData == but6)
    {
        Serial.println("IR: BUTTON 6 RECV");
    }
	else if (IrData == but7)
	{
		Serial.println("IR: BUTTON 7 RECV");
	}
	else if (IrData == but8)
	{
		Serial.println("IR: BUTTON 8 RECV");
	}
	else if (IrData == but9)
	{
		Serial.println("IR: BUTTON 9 RECV");
	}
	else if (IrData == but0)
	{
		Serial.println("IR: BUTTON 0 RECV");
	}
	else if (IrData == butStar)
	{
		Serial.println("IR: BUTTON * RECV");
	}
	else if (IrData == butHash)
	{
		Serial.println("IR: BUTTON # RECV");
	}
	else
	{
		Serial.println("IR: BUTTON UNKNOWN RECV");
        isLeft = false;
        isRight = false;
	}

    
    
    
    
    
    /*
    switch (IrReceiver.decodedIRData.command)
    {
    default:
        digitalWrite(redLED, HIGH);
        digitalWrite(blueLED, HIGH);
        break;
    but1:
        Serial.println("IR BUTTON 1 RECV");
        break;
    but:
        Serial.println("IR BUTTON 2 RECV");
        break;
    but3:
        break;
    }*/


}

void MotorMaster()
{
    Serial.println(isFoward);
    if (isFoward)
    {
        Serial.println("FOWARAD ACTIONED");
        MotorControl(HIGH, LOW, HIGH, LOW);

    }
    else if (isBackward)
    {
        MotorControl(0, 1, 0, 1);

    }
    else if (isLeft)
    {
        MotorControl(1, 0, 0, 0);
    }
    else if (isRight)
    {
        MotorControl(0, 0, 1, 0);
    }
    else
    {
        MotorControl(0, 0, 0, 0);
        //digitalWrite(motor1, LOW);
        //digitalWrite(motor2, LOW);
        //digitalWrite(motor3, LOW);
        //digitalWrite(motor4, LOW);
    }
}

void MotorControl(int motor1Int, int motor2Int, int motor3Int, int motor4Int)
{
    Serial.println(motor1Int + motor2Int + motor3Int + motor4Int);
	digitalWrite(motor1, motor2Int);
	digitalWrite(motor2, motor1Int);
	digitalWrite(motor3, motor3Int);
	digitalWrite(motor4, motor4Int);
}


/*    if (IrReceiver.decodedIRData.command == but1)
    {
        Serial.println("IR BUTTON 1 RECV");
        //IrReceiver.decodedIRData;
    }*/