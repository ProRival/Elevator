#pragma config(Sensor, dgtl1,  button1,        sensorTouch)
#pragma config(Sensor, dgtl2,  button2,        sensorTouch)
#pragma config(Sensor, dgtl3,  button3,        sensorTouch)
#pragma config(Sensor, dgtl4,  sonar,          sensorSONAR_cm)
#pragma config(Sensor, dgtl9,  killSwitch,     sensorTouch)
#pragma config(Motor,  port3,  elevatorMotor, tmotorVex393_MC29, openLoop)

/*
 * Because RobotC does not contain malloc functions, queues cannot have a dynamic size.
 */
#define MAX 10                                 //The max size of a queue is 10.

/**
 * Creates a Queue struct.
 */
typedef struct Queue {                             //Create Queue struct.
								int intArray[MAX];                       //Define an int array that acts as the queue.
								int front;                           //Define front node.
								int rear;                            //Define rear node.
								int itemCount;                         //Define a variable to keep track of the amount of items in the queue.
}Queue;

Queue queue;                                  //Define our queue "instance".
int currentFloor;                                //Keep track of the current floor of the elevator.

int peek(Queue queue);                             //Define peek header.
bool isEmpty(Queue queue);                            //Define isEmpty header.
bool isFull(Queue queue);                            //Define isFull header.
int size(Queue queue);                             //Define size header.
void insert(Queue queue, int data);                       //Define insert header.
int poll(Queue queue);                             //Define poll header.

void moveElevator(int floor, int speed);                    //Define moveElevator header.

task buttonTask() {                               //Define buttonTask task.
								while (true) {                         //Start an infinite loop that only ends when the program is terminated.
																if (SensorValue(killSwitch) == 1) {       //Check to see if the killswitch has been set off.
																								stopAllTasks();         //End program.
																								break;             //End loop.
																}

																if (isFull(queue)) continue;          //If the queue is full, don't worry about what buttons are pressed.

																if (SensorValue(button1) == 1) {        //Check the value of button1.
																								insert(queue, 0);        //If button1 is pressed, add floor 1 to the queue.
																} else if (SensorValue(button2) == 1) {     //Check the value of button2.
																								insert(queue, 1);        //If button2 is pressed, add floor 2 to the queue.
																} else if (SensorValue(button3) == 1) {     //Check the value of button 3.
																								insert(queue, 2);        //If button3 is pressed, add floor 3 to the queue.
																}
																waitInMilliseconds(10);             //Prevents the current task from hogging the cpu.
								}

}



task main() {                                  //Main task defined.
								queue.front = 0;                        //The front value of the queue is 0 at default.
								queue.rear = -1;                        //Set the value of the rear to undefined.
								queue.itemCount = 0;                      //The item count of the queue is 0 at default since there are no items in the queue.

								currentFloor = 3 + (SensorValue(sonar) / -20);         //Using integer division to determine the starting floor of the elevator.

								startTask(buttonTask);                     //Start listening for button pushes asynchronously.

								while (true) {                         //Infinite loop, will never terminate.
																if (isEmpty(queue)) continue;          //If the queue is empty, keep looping.
																moveElevator(poll(queue), 20);         //At this point, we know there is something in the queue, lets set the elevator to that floor, then remove it from the queue.
																waitInMilliseconds(10);             //Prevents the current task from hogging the cpu.
								}

}

void moveElevator(int floor, int speed) {                    //Define moveElevator function.
								if (currentFloor == floor || floor < 0 || floor > 2) return;  //Checks to see if the elevator is already on the current floor and to make sure the floor is valid.
								while (SensorValue(sonar) != (40 - (20 * floor))) {       //Move elevator while the elevator is not on the correct floor.
																startMotor(elevatorMotor, (floor < currentFloor ? speed : -speed)); //Make sure the motor turns the correct direction.
								}
								currentFloor = floor;                       //When finished, set the current floor.
								stopMotor(elevatorMotor);                     //Stop the motor.
}

/**
 * Returns the front value of the queue without removal.
 */
int peek(Queue queue) {
								return queue.intArray[queue.front];                //Return, but do not delete the front of the queue.
}

/**
 * Returns true if the queue is empty.
 */
bool isEmpty(Queue queue) {
								return queue.itemCount == 0;                    //If itemCount is 0, return true.
}

/**
 * Returns true if the queue is full.
 */
bool isFull(Queue queue) {
								return queue.itemCount == MAX;                   //If itemCount is Max, return true.
}

/**
 * Function to get the size of a queue.
 */
int size(Queue queue) {
								return queue.itemCount;                      //Return the itemCount.
}

/**
 * Adds data to the end of the queue.
 */
void insert(Queue queue, int data) {                      //Define insert function.

								if(!isFull(queue)) {                        //As long as the queue is not finished, lets continue.
																if(queue.rear == MAX-1) {               //If the queue is full, purge the last item in the queue.
																								queue.rear = -1;             //Purge the last item.
																}

																queue.intArray[++queue.rear] = data;         //Add the new value to the end of the of the queue.
																queue.itemCount++;                  //Increase the item count.
								}
}

/**
 * Gets and removes a value from the front of a queue.
 */
int poll(Queue queue) {                             //Define poll function.
								int data = queue.intArray[queue.front++];             //Get the first item in the queue.

								if(queue.front == MAX) {                      //If the item at the front of the queue is the max position, reset it to 0.
																queue.front = 0;                   //Set it to 0.
								}

								queue.itemCount--;                         //Decrement the itemCount.
								return data;                            //Return the old value.
}
