#pragma config(Sensor, dgtl1,  button1,        sensorTouch)
#pragma config(Sensor, dgtl2,  button2,        sensorTouch)
#pragma config(Sensor, dgtl3,  button3,        sensorTouch)
#pragma config(Sensor, dgtl4,  sonar,          sensorSONAR_cm)
#pragma config(Sensor, dgtl9,  killSwitch,     sensorTouch)
#pragma config(Motor,  port3,  elevatorMotor, tmotorVex393_MC29, openLoop)

/*
 * Because RobotC does not contain malloc functions, queues cannot have a dynamic size.
 */
#define MAX 6

/**
 * Creates a Queue struct.
 */
typedef struct Queue {
	int intArray[MAX];
	int front;
	int rear;
	int itemCount;
}Queue;

Queue queue;
int currentFloor;

//Headers for Queue.
int peek(Queue queue);
bool isEmpty(Queue queue);
bool isFull(Queue queue);
int size(Queue queue);
void insert(Queue queue, int data);
int poll(Queue queue);

//Headers for the main program.
void moveElevator(int floor, int speed);
void run();

task buttonTask() {
	while (true) {
		if (SensorValue(killSwitch) == 1) {
			stopAllTasks();
			break;
		}

		if (SensorValue(button1) == 1) {
			insert(queue, 0);
		} else if (SensorValue(button2) == 1) {
			insert(queue, 1);
		} else if (SensorValue(button3) == 1) {
			insert(queue, 2);
		}

	}

}



task main() {
	//initialize queue data.
	queue.front = 0;
	queue.rear = -1;
	queue.itemCount = 0;

	currentFloor = 0;

	//start listening for button pushes asynchronously.
	startTask(buttonTask);

	while (true) {
		if (isEmpty(queue)) continue;
		moveElevator(poll(queue), 20);
	}

}

void moveElevator(int floor, int speed) {
	if (currentFloor == floor || floor < 0 || floor > 2) return;							//checks to see if the elevator is already on the current floor and to make sure the floor is valid.
	while (SensorValue(sonar) != (40 - (15 * floor))) {													//move elevator while the elevator is not on the correct floor.
 		startMotor(elevatorMotor, (floor < currentFloor ? speed : -1 * speed)); //make sure the motor turns the correct direction.
 	}
 	currentFloor = floor;																											//when finished, set the current floor.
 	stopMotor(elevatorMotor);
 }

/**
 * Returns the front value of the queue without removal.
 */
int peek(Queue queue) {
   return queue.intArray[queue.front];
}

/**
 * Returns true if the queue is empty.
 */
bool isEmpty(Queue queue) {
   return queue.itemCount == 0;
}

/**
 * Returns true if the queue is full.
 */
bool isFull(Queue queue) {
   return queue.itemCount == MAX;
}

/**
 * Function to get the size of a queue.
 */
int size(Queue queue) {
   return queue.itemCount;
}

/**
 * Adds data to the end of the queue.
 */
void insert(Queue queue, int data) {

   if(!isFull(queue)) {

      if(queue.rear == MAX-1) {
         queue.rear = -1;
      }

      queue.intArray[++queue.rear] = data;
      queue.itemCount++;
   }
}

/**
 * Gets and removes a value from the front of a queue.
 */
int poll(Queue queue) {
   int data = queue.intArray[queue.front++];

   if(queue.front == MAX) {
      queue.front = 0;
   }

   queue.itemCount--;
   return data;
}
