/*
    Name:       Robotics_Vehicle_V1.ino
    Created:	24/06/2024 9:18:57 am
    Author:     MIMAS\reube
*/
#define DECODE_NEC  //IR protocal defintion

//Libraries
#include <Dabble.h>
#include <Ultrasonic.h>
#include <Servo.h>
//#include <IRremote.hpp> //IR througout the project may be commented out due to a conflict with the Dabble library. Only one method of communcation can be active at once

bool isAuto = false; //Toggle for auto mode
bool isBluetooth = true; //Toggle for bluetooth mode vs IR mode

#pragma region PinDefine

// Variables for the board digital pins so I can change the pin number here and it changes throughout the project
int servoPin = 6;
int motor1 = 9;
int redLED = 7;
int motor2 = 8;
int motor3 = 10;
int motor4 = 11;
int IRPin = 5;
int blueLED = 4;
int ultraTrig = 12;
int ultraEcho = 13;

#pragma endregion

#pragma region AliasVariables

Servo myServo;
int motorPins[4] = { motor1, motor2, motor3, motor4 };
Ultrasonic ultrasonic(ultraTrig, ultraEcho, 20000UL);

#pragma endregion

#pragma region RuntimeVariables

//Variables changed during runtime

int servoAngle = 90;

bool hasIRRecv = false;

bool isFoward = false;
bool isBackward = false;
bool isLeft = false;
bool isRight = false;

int currentAngle = myServo.read();
int nextAngle = 180;
int cycleCount = 0;
bool isNextAngleNeg = false;

#pragma endregion



void setup()
{
    Serial.begin(11520);
    Serial.println("Begun Setup");

    TestLEDSetup(); //Test LED Setup
    BluetoothInitilise(); //Bluetooth Setup
    //IRInitilise(); //IR Setup
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
    pinMode(redLED, LOW);
    digitalWrite(blueLED, HIGH);

    Serial.println("LED Setup Complete");
}

void BluetoothInitilise()
{
   Dabble.begin(9600); //Begin bluetooth and set bluetooth baud rate
   Serial.println("Bluetooth Setup Complete");
}
/*
void IRInitilise()
{
    IrReceiver.begin(IRPin, ENABLE_LED_FEEDBACK); //Begin IR communication, set pin and enable the onboard LED
    Serial.print("Receiving IR at pin ");
    Serial.println(IRPin);
    Serial.println("IR Setup Complete");
}
*/
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



void loop()
{
    if (isAuto)
	{
		AutoControlLoop();
        ServoMove();
	}
    else 
    {
        if (isBluetooth)
        {
			BluetoothLoop();
		}
        else
        {
            //IRReceive();
            //IRDecode();
        }
        MotorMaster();
    }
}

void AutoControlLoop()
{
    int delay1 = 1000;
    int delay2 = 800;

    isFoward = true;
    MotorMaster();
    delay(delay1);
    isFoward = false;
    isLeft = true;
    MotorMaster();
    delay(delay2);
    isFoward = true;
    isLeft = false;
    MotorMaster();
    delay(delay1);
    isFoward = false;
    isLeft = true;
    MotorMaster();
    delay(delay2);
    isFoward = true;
    isLeft = false;
    MotorMaster();
    delay(delay1);
    isFoward = false;
    isLeft = true;
    MotorMaster();
    delay(delay2);
    isFoward = true;
    isLeft = false;
    MotorMaster();
    delay(delay1);
    isFoward = false;
    isLeft = true;
    MotorMaster();
    delay(delay2);

    if(isBluetooth)
	{
		isAuto = false;
	}
}


void ServoMove()
{
    delay(20);
    myServo.write(nextAngle);
    Serial.println(nextAngle);

    if (!isNextAngleNeg)
    {
        if (nextAngle >= 150)
        {
            isNextAngleNeg = true;
        }
        else
        {
            nextAngle = nextAngle + 3;
        }
    }
    else
    {
        if (nextAngle <= 30)
        {
            isNextAngleNeg = false;
        }
        else
        {
            nextAngle = nextAngle - 3;
        }
    }
}

// IR
/*
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
}
*/

void BluetoothLoop()
{
    Dabble.processInput();

    if (GamePad.isCrossPressed())
    {
        Serial.println("CROSS");
        isFoward = false;
        isLeft = false;
        isRight = false;
        isBackward = false;
    }
    else if (GamePad.isUpPressed())
	{
		Serial.println("UP");
		isFoward = true;
		isLeft = false;
		isRight = false;
		isBackward = false;
	}
    else if (GamePad.isLeftPressed())
    {
        Serial.println("LEFT");
        isFoward = false;
        isLeft = true;
        isRight = false;
        isBackward = false;
    }
    else if (GamePad.isRightPressed())
	{
		Serial.println("RIGHT");
		isFoward = false;
		isLeft = false;
		isRight = true;
		isBackward = false;
	}
	else if (GamePad.isDownPressed())
	{
		Serial.println("DOWN");
		isFoward = false;
		isLeft = false;
		isRight = false;
		isBackward = true;
	}
    else if (GamePad.isStartPressed())
    {
        Serial.println("START");
        isFoward = false;
        isLeft = false;
        isRight = false;
        isBackward = false;
        isAuto = !isAuto;
    }
	else
	{
		isFoward = false;
		isLeft = false;
		isRight = false;
		isBackward = false;
	}
}


void MotorMaster()
{
    if (isFoward)
    {
        Serial.println("FOWARAD ACTIONED");
        MotorControl(1, 0, 1, 0);
    }
    else if (isBackward)
    {
        MotorControl(0, 1, 0, 1);
    }
    else if (isLeft)
    {
        MotorControl(0, 0, 1, 0);
    }
    else if (isRight)
    {
        MotorControl(1, 0, 0, 0);
    }
    else
    {
        MotorControl(0, 0, 0, 0);
    }
}

void MotorControl(int motor1Int, int motor2Int, int motor3Int, int motor4Int)
{
	digitalWrite(motor1, motor1Int);
	digitalWrite(motor2, motor2Int);
	digitalWrite(motor3, motor3Int);
	digitalWrite(motor4, motor4Int);
}

