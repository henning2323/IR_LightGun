#include "Arduino.h"
#include "Transformation.h"
#include "DFRobotIRPosition.h"
#include "AbsMouse.h"
#include <Keyboard.h>
#include "Wire.h"

#define triggerA 2
#define triggerB 3
#define mag 9
#define lid 8
#define fireLED 4
#define warningLED 15


DFRobotIRPosition camera;
int positionX[] = {1023, 1023, 1023, 1023};
int positionY[] = {1023, 1023, 1023, 1023};

int cornersX[] = {1023, 1023, 1023, 1023};
int cornersY[] = {1023, 1023, 1023, 1023};

boolean dataComplete = false;

int button_triggerA = 0;
int button_triggerB = 0;
int button_mag = 0;
int button_lid = 0;



int warningLEDState = 0;

int screenW = 1920;
int screenH = 1080;

//int gunCenterX = 530;
//int gunCenterY = 312;
int gunCenterX = 595;
int gunCenterY = 325;

void setup() {

  pinMode(fireLED, OUTPUT);
  pinMode(warningLED, OUTPUT);
  pinMode(triggerA, INPUT_PULLUP);
  pinMode(triggerB, INPUT_PULLUP);
  pinMode(mag, INPUT_PULLUP);
  pinMode(lid, INPUT_PULLUP);

  Serial.begin(19200);
  AbsMouse.init(screenW, screenH);
  camera.begin();
  Keyboard.begin();

  digitalWrite(fireLED, LOW);
  digitalWrite(warningLED, LOW);
}

void loop() {
  handleButtons();
  getCameraData();
  sortPoints();
  moveCursor();
}

void handleButtons() {
  int triggerA_now = digitalRead(triggerA);
  int triggerB_now = digitalRead(triggerB);
  int mag_now = digitalRead(mag);
  int lid_now = digitalRead(lid);

  if (triggerA_now != button_triggerA) {
    button_triggerA = triggerA_now;
    if (button_triggerA == 0) {
      AbsMouse.press(MOUSE_LEFT);
      digitalWrite(fireLED, HIGH);
    } else {
      AbsMouse.release(MOUSE_LEFT);
      digitalWrite(fireLED, LOW);
    }
  }

  if (triggerB_now != button_triggerB) {
    button_triggerB = triggerB_now;
    if (button_triggerB == 0) {
      AbsMouse.press(MOUSE_RIGHT);
    } else {
      AbsMouse.release(MOUSE_RIGHT);
    }
  }

  if (lid_now != button_lid) {
    button_lid = lid_now;
    if (button_lid == 0) {
      AbsMouse.press(MOUSE_MIDDLE);
    } else {
      AbsMouse.release(MOUSE_MIDDLE);
    }
  }

  if (mag_now != button_mag) {
    button_mag = mag_now;
    if (button_mag == 0) {
      Keyboard.press(KEY_RETURN);
    }
  } else {
    Keyboard.release(KEY_RETURN);
  }
}

void getCameraData() {
  camera.requestPosition();
  if (camera.available()) {
    for (int i = 0; i < 4; i++) {
      positionX[i] = camera.readX(i);
      positionY[i] = camera.readY(i);
    }
  }
  
  if (positionX[3] == 1023 && positionY[3] == 1023) {
    dataComplete = false;
    setWarningLED(1);
  } else {
    dataComplete = true;
    setWarningLED(0);
  }
}

void sortPoints() {
  if (!dataComplete)
    return;

  int orderedX[] = {0, 1, 2, 3};

  for (int i = 0; i < 3 ; i++) {
    for (int j = i + 1; j < 4; j++) {
      if (positionX[orderedX[i]] < positionX[orderedX[j]]) {
        int temp = orderedX[i];
        orderedX[i] = orderedX[j];
        orderedX[j] = temp;
      }
    }
  }

  if (positionY[orderedX[0]] < positionY[orderedX[1]]) {
    cornersX[0] = positionX[orderedX[0]];
    cornersY[0] = positionY[orderedX[0]];
    cornersX[2] = positionX[orderedX[1]];
    cornersY[2] = positionY[orderedX[1]];
  } else {

    cornersX[0] = positionX[orderedX[1]];
    cornersY[0] = positionY[orderedX[1]];
    cornersX[2] = positionX[orderedX[0]];
    cornersY[2] = positionY[orderedX[0]];
  }

  if (positionY[orderedX[2]] < positionY[orderedX[3]]) {
    cornersX[1] = positionX[orderedX[2]];
    cornersY[1] = positionY[orderedX[2]];
    cornersX[3] = positionX[orderedX[3]];
    cornersY[3] = positionY[orderedX[3]];
  } else {

    cornersX[1] = positionX[orderedX[3]];
    cornersY[1] = positionY[orderedX[3]];
    cornersX[3] = positionX[orderedX[2]];
    cornersY[3] = positionY[orderedX[2]];
  }
}
void moveCursor() {
  if (!dataComplete)
    return;

  Transformation trans(cornersX, cornersY, screenW, screenH, gunCenterX, gunCenterY);
  AbsMouse.move(trans.u(),trans.v());
}

void setWarningLED(int x) {
  if (x == 1) {
    digitalWrite(warningLED, HIGH);
  } else {
    digitalWrite(warningLED, LOW);
  }
}
