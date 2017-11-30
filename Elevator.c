#pragma config(Sensor, dgtl1,  button1, sensorTouch)
#pragma config(Sensor, dgtl2,  button2, sensorTouch)
#pragma config(Sensor, dgtl3,  button3, sensorTouch)
#pragma config(Sensor, dgtl4,  sonar, sensorSONAR_cm)
#pragma config(Motor,  port2,  elevatorMotorLeft, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,  elevatorMotorRight, tmotorVex393_MC29, openLoop)

																																						//floor 1: 6.5cm, 19.5cm, 32.5cm
																																						//function headers
void moveElevator(int floor, int speed);
void run();
int getFloor();

int currentFloor = 0;																												//keeps count of the current floor

/**
 * Elevator
 * Author: Kyle Posluns
 *
 */
task main() {
	run();																																		//call the run function.
}

/**
 * Function to run the program.
 *
 */
void run() {
	int floor = getFloor();																									 	//Check for a floor, then move the elevator.
	moveElevator(floor, 20);
}

/**
 * Function to get the corresponding floor to the button that was pushed.
 */
int getFloor() {
	while (true) { 																														//Test for button pushes.
		if (SensorValue(button1) == 1) {																				//Tldr; if the button is pressed, return the corresponding floor.
			return 0;
		} else if (SensorValue(button2) == 1) {
			return 1;
		} else if (SensorValue(button3) == 1) {
			return 2;
		}
	}
	return -1;
}

/**
 * Function to move elevator.
 * @param floor
 * @param speed
 */
void moveElevator(int floor, int speed) {
	if (currentFloor == floor || floor < 0 || floor > 2) return;							//checks to see if the elevator is already on the current floor and to make sure the floor is valid.
	while (SensorValue(sonar) != (32.5 - (13 * floor))) {													//move elevator while the elevator is not on the correct floor.
		startMotor(elevatorMotorLeft, (floor > currentFloor ? speed : -1 * speed)); //make sure the motor turns the correct direction.
		startMotor(elevatorMotorRight, (floor > currentFloor ? speed : -1 * speed));
	}
	currentFloor = floor;																											//when finished, set the current floor.
}
