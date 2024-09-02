/*
    Name:       Robotics_Vehicle_V1.2.ino
    Created:	02/09/2024 2:40:32 Pm
    Author:     MIMAS\reube
    ReadMe:     ./README.md
    Description: This code is for a simple robot vehicle that can be controlled via bluetooth or IR remote with an option for a full auto mode
    Project wide development notes:
        The pragma regions are simply for organisation and do not affect the code in any way, it makes it easier to look at and debug, splitting the code up into differnt sections
*/
#define DECODE_NEC  //IR protocal defintion

//Libraries
#include <Dabble.h>
#include <Ultrasonic.h>
#include <Servo.h>
//#include <IRremote.hpp> //IR througout the project may be commented out due to a conflict with the Dabble library. Only one method of communcation can be active at once

bool isAutoFull = false; //Toggle for auto mode
bool isBluetooth = true; //Toggle for bluetooth mode vs IR mode

#pragma region PinDefine

// Variables for the board digital pins so the pin number can be changed here and it changes throughout the project
// These could be const variables since these variables are not being modified and the project is relatively small, it is not needed.
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

// Gloabl variables changed during runtime

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

bool isAuto = false;

#pragma endregion

// Setup function called by the arduino at the start of the program
void setup()
{
    Serial.begin(11520);
    Serial.println("Begun Setup");

    LEDSetup();             // LED setup
    BluetoothInitilise();   // Bluetooth Setup
    //IRInitilise();        // IR Setup
    ServoInitilise();       // Servo Setup
    MotorInitilise();       // Motor Setup

    Serial.println("Setup Complete");
    Serial.println("--------------");
    digitalWrite(blueLED, LOW);
}

void LEDSetup()
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
    // for loop to cycle through the motor pins and set them to output and low
    for (int i = 0; i <= 4; i++)
    {
        pinMode(motorPins[i], OUTPUT);
        digitalWrite(motorPins[i], LOW);
    }

    Serial.println("Motor Setup Complete");
}

// Main loop function called by the arduino after the setup function
void loop()
{
    if (isAutoFull || isAuto) // if auto mode is enabled, or auto has been triggered by bluetooth, run the auto control loop rather than the manual control loop
    {
        AutoControlLoop();
        ServoMove();
    }
    else
    {
        if (isBluetooth) // if bluetooth is enabled, run the bluetooth loop else run the IR loop
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

    // A sequence of motor pin changes and delays to create a rough square pattern
    int delay1 = 1000;
    int delay2 = 800;

    // The robot will move forward for 1000ms, then turn left for 800ms, this will repeat 4 times to create a square pattern
    for (int i = 0; i < 4; i++) {
        isFoward = true;
        isLeft = false;
        MotorMaster();
        delay(delay1);

        isFoward = false;
        isLeft = true;
        MotorMaster();
        delay(delay2);
    }

    isAuto = false;

}

void ServoMove()
{
    delay(20);
    myServo.write(nextAngle);
    Serial.println(nextAngle);

    if (!isNextAngleNeg) // if the next angle is not negative, increase the angle by 3 degrees but if it is negative, decrease the angle by 3 degrees
    {
        if (nextAngle >= 150) // if the servo has reached the maximum desired radius, switch the direction to start it moving in the opposite direction.
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

void BluetoothLoop()
{
    Dabble.processInput();

    // Checks the input received via the bluetooth libary and sets the relative movement command to true and the others to false.

    // This could be a case statement but the if statements are more readable and easier to debug.
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
    // Receives and processes the boolean values of the relevent movement commands and runs the MotorControl function with the data passed through.

    if (isFoward)
    {
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
    // The method responsable for setting the motor pins to the correct values to move the robot in the desired direction.
    // It takes in 4 boolean values and sets the motor pins to the corresponding values.
    digitalWrite(motor1, motor1Int);
    digitalWrite(motor2, motor2Int);
    digitalWrite(motor3, motor3Int);
    digitalWrite(motor4, motor4Int);
}


// -----IR------ //

/*
void IRReceive()
{
// If the IR receiver has decoded a signal, print the signal to the serial monitor, set the hasIRRecv variable to true and resume the IR receiver.
// If the signal is noisy then print an error and light up the error LED.
// If no signal is received set the hasIRRecv variable to false. This ensures that the robot will only move if currently receiving a signal.
// Otherwise the IR decoded data will remian as the most recent command and the motor control will interpretthat as a continued press.

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

// Button commands for the IR remote

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


if (!hasIRRecv) { return; } // If we have not received a signal, break out of the function and do not decode the signal.

// Goes through all the commands and if the command is received, set the relative movement command to true and the others to false.

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