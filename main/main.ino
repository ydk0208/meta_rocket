#include <Servo.h>
 
Servo EDF;
Servo Top;
Servo Bottom;
Servo Left;
Servo Right;

typedef struct axis{
  double kP;
  double kI;
  double kD;

  double target = 0;
  double current = 0;
  double last = 0;
  double I = 0;
  double D = 0;
  double output = 0;
  
}axis;

axis roll;  //motor top/bottom
axis pitch; //motor left/right

void rocketInit();
void setGain();
void readIMU();
void calcIMU();
void PID(axis *sys);
void writeMotors();

void setup() {                     
  rocketInit();
  setGain(&pitch);
  setGain(&roll);
}
 
void loop() {
  readIMU();
  calcIMU();
  PID(&pitch);
  PID(&roll);
  writeMotors();
  delay(10);
}

void rocketInit(){
  EDF.attach(A0);
  Top.attach(A1);
  Bottom.attach(A2);
  Left.attach(A3);
  Right.attach(A4);

  Serial.begin(9600);
}

void setGain(axis *sys){
  sys->kP = 1;
  sys->kD = 0.1;
  sys->kI = 0.01;
}
void PID(axis *sys){
  double error = sys->target-sys->current;
  sys->D = error-sys->last;
  sys->I += error;
  sys->output = sys->kP*error + sys->kD*sys->D + sys->kI*sys->I;
  sys->last = error;
}
