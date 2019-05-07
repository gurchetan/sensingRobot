#pragma config(Sensor, S1,     Colour,         sensorEV3_Color, modeEV3Color_Reflected)
#pragma config(Sensor, S4,     Colour2,         sensorEV3_Color, modeEV3Color_Reflected)
#pragma config(Motor,  motorD,          motorLeft,     tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorA,          motorRight,    tmotorEV3_Large, PIDControl, encoder)
#pragma config(Sensor, S3,     IRDistance1,    sensorEV3_IRSensor, modeEV3IR_Proximity)
#pragma DebuggerWindows("debugStream")

/*
    TCSS 437 Mobile Robotic
    Challenge 1
    Arjun Prajapati, Allen Tran, Chris Jin Kim
		Design, construct, program, test, and demonstrate a mobile robot with the following behaviors:
			Wandering: As was done in the previous challenge.
 			Trail following: When the robot detects a trail (a black line on the floor) it will follow the
			trail to its end, make a sound, and then resume wandering.
			Object Detection and Approach: When the robot detects an object nearby (within 3 feet)
			with its ultrasonic sensor, it will approach as close as possible without actually touching the
			object (try to approach within 1 inch), stop for a few seconds, back up and turn away from
			the object, and then resume wandering. The speed at which the robot approaches the object
			should be proportional to the distance to the object. The robot's approach should initially be
			faster than the normal wander speed, but the robot should slow down as it gets close to the
			object such that its final approach is slower than the normal wander speed.
*/

//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
int leftmotor = 0;
int rightmotor = 0;

// for wandering
int totalleft= 0;
int totalright = 0;
int difference = 0;


// speed for object detect
int currentDistance = 0;

int randomDirection;


int left = 20;
int right = 20;
int max = 20;
bool beep = false;

// Method for going forward
void moveForward() {


	leftmotor = random(90);
	rightmotor = random(90);

	totalleft += leftmotor;
	totalright += rightmotor;
	difference = totalleft - totalright;
	if(difference <= 10000 && difference >= -10000){
		setMotorSpeed(motorLeft, leftmotor);
	  setMotorSpeed(motorRight, rightmotor);
	}
	else if(difference > 10000){
        setMotorSpeed(motorLeft, 0);
        setMotorSpeed(motorRight, 100);
        totalleft -= 100;
	} else if(difference < -10000)  {
        setMotorSpeed(motorLeft, 100);
        setMotorSpeed(motorRight, 0);
        totalright -= 100;

 } else{
		setMotorSpeed(motorLeft, 100);
		setMotorSpeed(motorRight, 100);


	}


}

//if object is near
void objectDetected(){
			beep = false;
			motor[motorLeft] = 0;
			motor[motorRight] = 0;
			sleep(2000);
			randomDirection = random(1);
			if(randomDirection==0){
				motor[motorLeft] = -30;
				motor[motorRight] = 0;
			} else {
				motor[motorLeft] = 0;
				motor[motorRight] = -30;
			}
			sleep(1000);
}


//for color
short rightcurrentColour;
short leftcurrentColour;



task main()
{


	while (true)
	{
		// read distance object censor
		currentDistance = SensorValue[IRDistance1];

		displayCenteredBigTextLine(1, "Dist: %d", currentDistance);

		// color detect
		rightcurrentColour = SensorValue[Colour];
		leftcurrentColour = SensorValue[Colour2];

		displayCenteredBigTextLine(7, " %d", rightcurrentColour);

		if((SensorValue[Colour] < 17) && ( SensorValue[Colour2] < 17))
		{
			beep = true;
		}

		if((currentDistance < 900) && currentDistance > 35)
		{
			setMotorSpeed(motorLeft, (currentDistance/10));
			setMotorSpeed(motorRight, (currentDistance/10));
			beep = false;

		}else if(currentDistance <= 35){
			objectDetected();
		} else if((SensorValue[Colour] < 17) || ( SensorValue[Colour2] < 17)) {
			clearTimer(T1);
			displayBigTextLine(9 ,"inside statement");
			//playTone(100, true);
			if(SensorValue[Colour] < SensorValue[Colour2])
			{
				int difference = (SensorValue[Colour2] - SensorValue[Colour]) * 2;
				if(difference > max) {
					difference = max;
				}
				setMotorSpeed(motorLeft, left + difference + 10);
				setMotorSpeed(motorRight, right-difference - 15);
				// displayBigTextLine(
			}
			else
			{
				int difference = (SensorValue[Colour] - SensorValue[Colour2]) * 2;
				if(difference > max) {
						difference = max;
				}
				//float speedChange = base * ((SensorValue[Colour2] + 1) / (SensorValue[Colour] + 1 + SensorValue[Colour2]));
				setMotorSpeed(motorLeft, left-difference - 15);
				setMotorSpeed(motorRight, right + difference + 10);
			}
		} else {
			if(beep == true && (SensorValue[Colour] > 35) && ( SensorValue[Colour2] > 35) )
			{
				playTone(100, 100);
				beep = false;
			}
			moveForward();

		}

	} // while- ends

}// Main ends