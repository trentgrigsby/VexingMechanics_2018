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
	setTouchLEDColor(Led,colorRed);

	waitUntil(getTouchLEDValue(Led)==true);

	CalibrateGYROS(horizontalGyro,gyroCalibrateSamples4096);
	//	setGyroSensitivity(horizontalGyro,gyroHighSensitivity);

	setTouchLEDColor(Led,colorBlue);

	//If telling the gyro to turn negative degrees use the < symbol. Ex <=-100
	//If telling the gyro to turn positive degres use the > symbol. Ex >=100

	waitUntil(getTouchLEDValue(Led)==true);

	resetGyro(horizontalGyro);

	forward(740,degrees,75); //Moving forward to blue rings.

	repeatUntil(getGyroDegrees(horizontalGyro)<=-80) //Spinnig so clamps are pointed at blue peg.
	{
		setMotor(leftMotor,50);

		setMotor(rightMotor,-50);
	}

	stopAllMotors();

	backward(225,degrees,75); //Backing up into blue rings

	sleep(750);

	forward(400,milliseconds,25);

	stopAllMotors();

	setMotor(armLift,-100);

	sleep(1000);

	setMotorBrakeMode(armLift,motorHold);

	forward(225,75); //Away from rings before spinning

	stopAllMotors();

	resetGyro(horizontalGyro);

	repeatUntil(getGyroDegrees(horizontalGyro)>=170) //Turning so that the ring clamps are facing the poles.
	{
		setMotor(leftMotor,-50);

		setMotor(rightMotor, 50);

	}

	stopAllMotors();

	setMotor(armLift,-100); //Lifting the ARM up because it tends to lower down

	sleep(1000);

	setMotorBrakeMode(armLift,motorHold);

	stopAllMotors();

	setMotor(slideMotor,100); //sliding so that the ring clamp is on par with the pole.

	sleep(600);

	stopAllMotors();


		repeatUntil(getGyroDegrees(horizontalGyro)>=170) //Turning so that the ring clamps are facing the poles.
	{
		setMotor(leftMotor,-50);

		setMotor(rightMotor, 50);

	}

	stopAllMotors();


	backward(800,degrees,100); //Backing up to put rings on pole.

	stopAllMotors();

	setMotor(armLift,20);

	sleep(1500);

	stopAllMotors();

	forward(360,degrees,100); //Pulling away so that the robot isn't touching the rings.

	setTouchLEDColor(Led,colorRed);

}
