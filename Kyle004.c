#pragma config(Sensor, port2,  ,               sensorVexIQ_Distance)
#pragma config(Sensor, port7,  ,               sensorVexIQ_Gyro)
#pragma config(Motor,  motor1,           ,             tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor4,           ,             tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor6,           ,             tmotorVexIQ, PIDControl, driveRight, encoder)
#pragma config(Motor,  motor10,          ,             tmotorVexIQ, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()

{

int distval= getDistanceValue(port2);
//Kyle: This program will only execute the statement 15-24 once. You need to add a Loop(While, Repeat)
if (distval>500)
{
	setMultipleMotors(100, leftMotor,rightMotor); //Kyle: leftMort and rightMotor are not defined. I believe you actually want motor1 and motor6.
	}

 else {
   playSound(soundTada);

}

}//end (task main)





