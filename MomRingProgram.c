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

void  Turn(bool bLeft, int iSpeed,int iTurnSpeed){
	const int iMaxTurnSpeed=20;
	while(getGyroHeading(horizontalGyro)!=0){
		if(bLeft)
			setMotorSpeed(rightMotor,iSpeed-iTurnSpeed);
		else
			setMotorSpeed(rightMotor,iSpeed+iTurnSpeed);

		iTurnSpeed++;
		if(iTurnSpeed >iMaxTurnSpeed) iTurnSpeed=iMaxTurnSpeed;
		sleep(100);
	}

}
task TaskTurnLeft(){
	setTouchLEDColor(Led,colorBlueGreen);
	Turn(true,45,5);
}
task TaskTurnRight(){
	setTouchLEDColor(Led,colorYellow);
	Turn(false,45,5);
}

bool bTaskTurnLeftRunning=false;
bool bTaskTurnRightRunning=false;

void  StartTurn(bool bLeft){
	//Only Starts Will not restart.
	if(bLeft){
		if(bTaskTurnRightRunning) stopTask(TaskTurnRight);
		if(!bTaskTurnLeftRunning) startTask(TaskTurnLeft);
		bTaskTurnLeftRunning=true;
		abortTimeslice();
	}
	else{
		if(bTaskTurnLeftRunning) stopTask(TaskTurnLeft);
		if(!bTaskTurnRightRunning) startTask(TaskTurnRight);
		bTaskTurnRightRunning=true;
		abortTimeslice();
	}

}

void StopTurn(){
	if(bTaskTurnLeftRunning) stopTask(TaskTurnLeft);
	if( bTaskTurnRightRunning)stopTask(TaskTurnRight);
	bTaskTurnLeftRunning=bTaskTurnRightRunning=false;
}
int HoldHeading(int iWantedHeading,int iSpeed){
	//P*Kp + I*Ki + D*Kd

	float fHeading = 0;
	float P=0;
	float PrevP=0;
	float D=0;
	float Kp=1,Kd=1;
	setTouchLEDColor(Led,colorGreen);
	setMotorSpeed(rightMotor,iSpeed);
	setMotorSpeed(leftMotor,iSpeed);

	do {
		clearTimer(T1);
		sleep(10);

		fHeading = getGyroHeading(horizontalGyro);
		PrevP=P;

		P=iWantedHeading - fHeading;

		D=(PrevP-P)/time1[T1];
		if(abs(P) <.5){ P=0;} //Deadzone
		setMotorSpeed(rightMotor,(int) (iSpeed+( (P*Kp) + (D*Kd) )) );

	} while( abs(P)!=0 );

	setTouchLEDColor(Led,colorYellow);

	return 0;
}
task main()
{
	//This is calibrating the gyros.
	CalibrateGYROS(horizontalGyro,gyroCalibrateSamples4096);
	setGyroSensitivity(horizontalGyro,gyroHighSensitivity);

	setTouchLEDColor(Led,colorRed);



	//waitUntil(getTouchLEDValue(Led)== 1);
	setTouchLEDColor(Led,colorGreen);
	resetGyro(horizontalGyro);


	HoldHeading(0,45);//Test using PDI loop with out the I
stopAllTasks();

	float fHeading = 0;
	float fTarget=.5;
	int iSpeedDiffLeft=5;
	int iSpeedDiffRight=5;
	const int iMaxDiff=15;
	const int iMinDiff=1;


	//	setDistanceMaxRange(distance,100);
	repeatUntil(getDistanceValue(distance) <= 70)
	{
		displaySensorValues(line2, distance);
		fHeading = getGyroHeading(horizontalGyro);
		if (fHeading < fTarget && fHeading >(fTarget*-1))  ///This like its going straight
		{
			//setMotor(leftMotor, 45);
			//sleep(1);
			//setMotor(rightMotor, 45);
			//sleep(1);
			StopTurn();

			iSpeedDiffLeft=iSpeedDiffRight=iMinDiff;
			setMotorSpeed(rightMotor,45);
			setMotorSpeed(leftMotor,45);

			setTouchLEDColor(Led,colorGreen);

		}
		else if (fHeading >=fTarget) { //ITs veering off to the right

			//stopMotor(rightMotor);
			//sleep(1);
			////setMotor(rightMotor, 20);
			//setMotor(leftMotor, 20);
			//sleep(1);
			/*iSpeedDiffRight=iMinDiff;
			setMotorSpeed(rightMotor,45-iSpeedDiffLeft);
			setMotorSpeed(leftMotor,45);
			setTouchLEDColor(Led,colorBlueGreen);

			if(iSpeedDiffLeft <iMaxDiff) { iSpeedDiffLeft++;}
			*/
			StartTurn(true);

		}
		else{ //its veering off to left
			////	stopMotor(rightMotor);
			//stopMotor(leftMotor);

			//sleep(1);
			////	setMotor(leftMotor, 20);
			//setMotor(rightMotor, 20);
			//sleep(1);
			/*iSpeedDiffLeft=iMinDiff;
			setMotorSpeed(leftMotor,45-iSpeedDiffRight);
			setMotorSpeed(rightMotor,45);
			setTouchLEDColor(Led,colorYellow);
			if(iSpeedDiffRight <iMaxDiff) { iSpeedDiffRight++;}
			*/

			StartTurn(false);
		}

	}

	backward(1,rotations, 50);
	fHeading = getGyroHeading(horizontalGyro);
	while(fHeading >fTarget || fHeading <(fTarget*-1)){
		if (fHeading >=fTarget) { //ITs veering off to the right
			iSpeedDiffRight=iMinDiff;
			setMotorSpeed(leftMotor,25-iSpeedDiffLeft);
			setMotorSpeed(rightMotor,0);
			setTouchLEDColor(Led,colorBlueGreen);

			if(iSpeedDiffLeft <iMaxDiff) { iSpeedDiffLeft++;}


		}
		else{ //its veering off to left
			iSpeedDiffLeft=iMinDiff;
			setMotorSpeed(rightMotor,25-iSpeedDiffRight);
			setMotorSpeed(leftMotor,0);
			setTouchLEDColor(Led,colorYellow);
			if(iSpeedDiffRight <iMaxDiff) { iSpeedDiffRight++;}
		}

		fHeading = getGyroHeading(horizontalGyro);
	}

	setMotor(slideMotor, -50);
	sleep(1550);
	stopAllMotors();
	forward(1.25, rotations, 100);
	stopAllMotors();
	backward(2,rotations, 50);
	setTouchLEDColor(Led,colorBlue);

}
