class Transformation{

  private:
  int* x;
  int* y;

  int pointX;
  int pointY;

  int screenWidth;
  int screenHeight;
 
  double x1(double varX, double varY);
  double x2(double varX, double varY);
  double x3(double varX, double varY);
  double x4(double varX, double varY);
  
  double y1(double varX, double varY);
  double y2(double varX, double varY);
  double y3(double varX, double varY);
  double y4(double varX, double varY);

  public:
  Transformation(int* pX, int* pY, int pScreenWidth, int pScreenHeight, int pPointX, int pPointY);
  int u();
  int v();
  
};
