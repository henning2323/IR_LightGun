#include "Transformation.h"

Transformation::Transformation(int* pX, int* pY, int pScreenWidth, int pScreenHeight, int pPointX, int pPointY) {
  x = pX;
  y = pY;
  screenWidth = pScreenWidth;
  screenHeight = pScreenHeight;
  pointX = pPointX;
  pointY = pPointY;
}

double Transformation::x1(double varX, double varY) {
  return varX - x[0];
}

double Transformation::x2(double varX, double varY) {
  return x1(varX, varY);
}

double Transformation::x3(double varX, double varY) {
  return x2(varX, varY) - y2(varX, varY) * x2(x[2], y[2]) / y2(x[2], y[2]);
}

double Transformation::x4(double varX, double varY) {
  return (((x3(x[1], y[1]) / x3(x[3], y[3])) - 1) * (y3(varX, varY) / y3(x[3], y[3])) + 1) * x3(varX, varY);
}

int Transformation::u() {
  return (int) (x4(pointX, pointY) * screenWidth / x4(x[3], y[3]));
}


double Transformation::y1(double varX, double varY) {
  return varY - y[0];
}

double Transformation::y2(double varX, double varY) {
  return y1(varX, varY) - x1(varX, varY) * y1(x[1], y[1]) / x1(x[1], y[1]);
}

double Transformation::y3(double varX, double varY) {
  return y2(varX, varY);
}

double Transformation::y4(double varX, double varY) {
  return (((y3(x[2], y[2]) / y3(x[3], y[3])) - 1) * (x3(varX, varY) / x3(x[3], y[3])) + 1) * y3(varX, varY);
}

int Transformation::v() {
  return (int) (y4(pointX, pointY) * screenHeight / y4(x[3], y[3]));
}
