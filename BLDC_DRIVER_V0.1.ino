//THIS CODE IS NOT TESTED YET.
#include <Wire.h>

#define I2C_MB 0x08  //I2c
int spd = 128;       // speed
int dir = 1;         // direction

// hall & phase
const int hA = 13, hB = 14, hC = 15;
const int pAH = 23, pAL = 25, pBH = 26, pBL = 27, pCH = 32, pCL = 33;

void setup() {
  Wire.begin();
  pinMode(hA, INPUT);
  pinMode(hB, INPUT);
  pinMode(hC, INPUT);
  pinMode(pAH, OUTPUT);
  pinMode(pAL, OUTPUT);
  pinMode(pBH, OUTPUT);
  pinMode(pBL, OUTPUT);
  pinMode(pCH, OUTPUT);
  pinMode(pCL, OUTPUT);
  ledcAttachPin(pAH, 0);
  ledcAttachPin(pBH, 1);
  ledcAttachPin(pCH, 2);
  ledcSetup(0, 20000, 8);
  ledcSetup(1, 20000, 8);
  ledcSetup(2, 20000, 8);
}

void loop() {
  Wire.requestFrom(I2C_MB, 2);
  if (Wire.available() >= 2) {
    spd = Wire.read();
    if (Wire.read() == 1) {
      dir = 1;
    } else {
      dir = -1;
    }

    int sA = digitalRead(hA);
    int sB = digitalRead(hB);
    int sC = digitalRead(hC);

    if (sA == 1 && sB == 0 && sC == 1) {
      setPhase(spd, 0, -1);
    } else if (sA == 1 && sB == 1 && sC == 0) {
      setPhase(0, spd, -1);
    } else if (sA == 0 && sB == 1 && sC == 1) {
      setPhase(-1, spd, 0);
    } else if (sA == 0 && sB == 1 && sC == 0) {
      setPhase(-1, 0, spd);
    } else if (sA == 0 && sB == 0 && sC == 1) {
      setPhase(spd, -1, 0);
    } else if (sA == 1 && sB == 0 && sC == 0) {
      setPhase(0, -1, spd);
    } else {
      setPhase(0, 0, 0);
    }
  }

  void setPhase(int A, int B, int C) {
    if (dir == -1) {
      int x = A;
      A = C;
      C = x;
    }
    if (A >= 0) {
      ledcWrite(0, A);
      digitalWrite(pAL, LOW);
    } else {
      ledcWrite(0, 0);
      digitalWrite(pAL, HIGH);
    }

    if (B >= 0) {
      ledcWrite(1, B);
      digitalWrite(pBL, LOW);
    } else {
      ledcWrite(1, 0);
      digitalWrite(pBL, HIGH);
    }

    if (C >= 0) {
      ledcWrite(2, C);
      digitalWrite(pCL, LOW);
    } else {
      ledcWrite(2, 0);
      digitalWrite(pCL, HIGH);
    }
  }
