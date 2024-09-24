/*
Light Touch, a FSR based LED project

This program uses a FSR sensor to sense pressure excerted on it and use that data to operate three leds, using those leds
like an indicator of how much force in put on the sensor.

Made by Harry zhao
Date: 2024/9/24

*/

int sensorValue0;
int sensorValue1;

int ledLevel;
int led1;
int led2;
int led3;
const int ledPin1 = 9;
const int ledPin2 = 10;
const int ledPin3 = 11;

int forceHistory[50];
int forceHistoryIndex = 0;

bool isLate = true;
void updateLEDs()
{
  ledLevel = 0;

}
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  led1 = 0;
  led2 = 0;
  led3 = 0;
  ledLevel = 0;
  for (int i = 0; i < 50; i++) {
    forceHistory[i] = -1;
  }
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
}

//get smallest number in an array
int getSmallest(int myarray[], int count)
{
  int smallest = 1000;
  for (int i = 0; i < count; i++)
  {
    if (myarray[i] < smallest && myarray[i] > 0)
    {
      smallest = myarray[i];
    }
  }
  return smallest;

}

//get largest number in an array
int getBiggest(int myarray[], int count)
{
  int biggest = 0;
  for (int i = 0; i < count; i++)
  {
    if (myarray[i] > biggest)
    {
      biggest = myarray[i];
    }
  }
  return biggest;

}

//delete array[0] and move everything forward one index then add new value to end of array
void push(int myarray[], int count, int newValue) {
  
  for (int i = 0; i < count - 1; i++) {
    myarray[i] = myarray[i + 1];
  }
  
  myarray[count - 1] = newValue;
}

// the loop routine runs over and over again forever:
void loop() {

  //Reading data from FSR sensor
  sensorValue0 = analogRead(A0); 
  sensorValue1 = analogRead(A1);

  //update force array with new sensor value
  push(forceHistory, 50, sensorValue0);

  //legacy
  int largestForce = getBiggest(forceHistory, 50);
  int smallestForce = getSmallest(forceHistory, 50);

  int numOfZero = 0;

  //toggle for smooth input, if false then the program will use raw FSR sensing data
  if(isLate == true)
    {
    for (int i = 0; i <= 49; i++) {
    sensorValue0 += forceHistory[i];
    if (forceHistory[i] == -1)
    {
      numOfZero++;
    }
    }
    sensorValue0 = sensorValue0 / 50;
    }

  //map sensor value to pre-set range
  led1 = map(sensorValue0, 80, 233, 0, 255);
  led2 = map(sensorValue0, 233, 386, 0, 255);
  led3 = map(sensorValue0, 386, 540, 0, 255);

  //Hard limit on Led brightness
  if(led1 > 255)
  {
    led1 = 255;
  }
  if(led1 < 0)
  {
    led1 = 0;
  }
  if(led2 > 255)
  {
    led2 = 255;
  }
  if(led2 < 0)
  {
    led2 = 0;
  }
  if(led3 > 255)
  {
    led3 = 255;
  }
  if(led3 < 0)
  {
    led3 = 0;
  }

  //operate LEDs
  analogWrite(ledPin1, led1);
  analogWrite(ledPin2, led2);
  analogWrite(ledPin3, led3);

  //report value to Serial port for further usage
  Serial.println(sensorValue0);

  //legacy, not used for now
  forceHistoryIndex ++;

  //delay, change for lower or higher latency
  delay(25); 

  //legacy, not used
  if(forceHistoryIndex >= 49)
  {
    forceHistoryIndex = 0;
  }
  
  
}
