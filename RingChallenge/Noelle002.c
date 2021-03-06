#pragma config(Sensor, port2,  distance,       sensorVexIQ_Distance)
#pragma config(Sensor, port3,  horizontalGyro, sensorVexIQ_Gyro)
#pragma config(Sensor, port7,  Led,            sensorVexIQ_LED)
#pragma config(Sensor, port8,  ColorSensor,    sensorVexIQ_ColorGrayscale)
#pragma config(Motor,  motor1,          leftMotor,     tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor4,          slideMotor,    tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor6,          rightMotor,    tmotorVexIQ, PIDControl, reversed, driveRight, encoder)
#pragma config(Motor,  motor10,         armLift,       tmotorVexIQ, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
int  CalibrateGYROS(tSensors nGyro1,TI2CGyroCalibrateSamples nCalibrationSetting){
	/*
	Calibration of GYROS appears to be neccessary at the begining of any program
	where you are going to make use of a GYRO. The amount of time to complete the
	calibration process varys based on the number of samples specified.

	During calibration the robot mus be kept prefectly still.
	So we are going to wait a bit before starting calibration to
	insure know one is touching the robot.
	*/

	string str;

	displayString(1,"Calibrating Gyro....");
	sleep(2000);
	stringFormat(str, "Gyro1CaliFlag: %d", getGyroCalibrationFlag(nGyro1)); displayString(2, str);
	//stringFormat(str, "Gyro2CaliFlag: %d", getGyroCalibrationFlag(nGyro2)); displayString(3, str);

	/*This actually begins the gyro calibration process*/
	startGyroCalibration(nGyro1, nCalibrationSetting);
	//startGyroCalibration(nGyro2, nCalibrationSetting);

	sleep(10); //This appears to be need for debug but doesn't hurt anything.

	/*Loop until the calibration process is complete on both GYROS*/
	//while(getGyroCalibrationFlag(nGyro1)||getGyroCalibrationFlag(nGyro2))
	while(getGyroCalibrationFlag(nGyro1))
	{
		stringFormat(str, "Gyro1CaliFlag: %d", getGyroCalibrationFlag(nGyro1)); displayString(2, str);
		//	stringFormat(str, "Gyro2CaliFlag: %d", getGyroCalibrationFlag(nGyro2)); displayString(3, str);
		sleep(10);
	}
	/*###########################################
	*# GYRO Calibration is complete.
	*###########################################*/
	return 0;
}

task main()
{
	//This is calibrating the gyros.
setTouchLEDColor(Led,colorRed);
	CalibrateGYROS(horizontalGyro,gyroCalibrateSamples4096);
	setGyroSensitivity(horizontalGyro,gyroHighSensitivity);
setTouchLEDColor(Led,colorYellow);

	waitUntil(getTouchLEDValue(Led)== 1);

	resetGyro(horizontalGyro);

	int iMaxSpeed = 45;
	int iTurningSpeed = 5;
	float fHeading = 0;
	float fTarget=1.5;
	int iStoppingDistance = 100;

	//	setDistanceMaxRange(distance,100);
	repeatUntil(getDistanceValue(distance) <= iStoppingDistance)
	{
		displaySensorValues(line2, distance);
		fHeading = getGyroHeading(horizontalGyro);
		if (fHeading < fTarget && fHeading >(fTarget*-1))  ///This like its going straight
		{
			setMotorSpeed(rightMotor,iMaxSpeed);
			setMotorSpeed(leftMotor,iMaxSpeed);

			setTouchLEDColor(Led,colorGreen);		}
		else if (fHeading >=fTarget) { //ITs veering off to the right

			setMotorSpeed(rightMotor,iTurningSpeed);
			setMotorSpeed(leftMotor,iMaxSpeed);
			setTouchLEDColor(Led,colorRed);

		}
		else{ //its veering off to left
			setMotorSpeed(leftMotor,iTurningSpeed);
			setMotorSpeed(rightMotor,iMaxSpeed);
			setTouchLEDColor(Led,colorBlue);
		}

	}

	backward(1,rotations, 50);
	/*fHeading = getGyroHeading(horizontalGyro);
	while(fHeading >fTarget || fHeading <(fTarget*-1)){
		if (fHeading >=fTarget) { //ITs veering off to the right
			setMotorSpeed(leftMotor,25);
			setMotorSpeed(rightMotor,0);
			setTouchLEDColor(Led,colorRed);
		}
		else{ //its veering off to left
			setMotorSpeed(rightMotor,25);
			setMotorSpeed(leftMotor,0);
			setTouchLEDColor(Led,colorBlue);
		}
		fHeading = getGyroHeading(horizontalGyro);
	}
*/
	setMotorSpeed(slideMotor, 75);
	sleep(1000);
	stopAllMotors();
	forward(1.05, rotations, 100);
	stopAllMotors();
	backward(1.5,rotations, 50);
	setMotorSpeed(slideMotor, 100);
	sleep(1500);
	stopAllMotors();
	setTouchLEDColor(Led,colorGreen);

}
