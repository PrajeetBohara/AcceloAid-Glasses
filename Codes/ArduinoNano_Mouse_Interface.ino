// Date: 09/28/2024
// Name: ***Project Name Not Decided Yet***
// Author: Institute of Electrical and Electronics Engineers (IEEE) at McNeese
// Source Code and Libraries: Arduino Official Documentation, GitHub repositories
// Modified by: Prajeet Bohara
// Description: This code controls mouse cursor using Arduino Nano IoT 33 using it's HID feature and Inertial Mass Unit sensor (Accelerometer)
// Notes: This code was adapted and modified for educational purpose based on existing examples in the Arduino documentation and other open-source resources.


#include <Mouse.h>
#include <Arduino_LSM6DS3.h>

float accelX=0.0f, accelY=0.0f, accelZ=1.01f, sensitivity=0.01f, liftThreshold=0.1f, decay=0.99f, multiplier=2000000;
unsigned long prevTime=0, lastLiftTime=0, liftDelay=100000;

void setup() {
  prevTime=micros(); lastLiftTime=prevTime;
  if (!IMU.begin()) {
      while (1);
  }
  Mouse.begin();
}
void loop() {
  float readX, readY, readZ; float sampleRate; 
  float diffX, diffY, diffZ, deltaT, deltaT2; 
  int moveX, moveY; bool moveMouse = false; 
  bool liftMouse = false; unsigned long currentTime = micros();

  if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(readY, readX, readZ); 

      sampleRate = IMU.accelerationSampleRate(); 
      deltaT=1.0f/sampleRate; deltaT2=deltaT*deltaT;
      accelX=decay*accelX+(1.0f-decay)*readX; 
      accelY=decay*accelY+(1.0f-decay)*readY; 
      accelZ=decay*accelZ+(1.0f-decay)*readZ; diffX=readX-accelX; 
      diffY=readY-accelY; diffZ=readZ-accelZ;

      if (abs(diffX)>sensitivity) { moveX=multiplier*0.5*diffX*deltaT2; moveMouse = true; }
      if (abs(diffY)>sensitivity) { moveY=multiplier*0.5*diffY*deltaT2; moveMouse = true; }
      if (abs(diffZ)>liftThreshold) { liftMouse = true; lastLiftTime = micros(); }
      
        if (moveMouse) {
          Mouse.move(moveX, moveY, 0);
        }
      }
  }
}

