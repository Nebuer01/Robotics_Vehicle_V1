# Robotics_Vehicle_V1
2024 11ROB Vehicle code version 1
This program is for a simple robot vehicle that can be controlled via bluetooth or IR remote with an option for a full auto mode. 
It utilizes and Arduino Uno R3 for control. The robot has 2 motors powered by a motor controlle with four pins, 1Forward 1Reverse 2Foward 3Reverse, as well as a 5V for control board power and a higher voltage pin up to 12v for motor power.
The robot also features an ultrasonic sensor on a servo which is used to ensure the robot will avoid objects when using the auto mode. This has however not been fully implemented yet as the IR and especially Bluetooth control modes supersede the auto mode.
The vehicle is powered off 9V using 6 AA batteries (6x 1.5V). This is fed into a power bus dedicated for all 9V electronics such as the motor and Arduino VIN. There is also a 5V power bus powered by the Arduino’s 5V pin.
For control, it is using a generic IR receiver and remote with 17 buttons and an HM-10 Bluetooth module. 