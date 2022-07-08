#include <Servo.h>
#include <Wire.h>

#define MPU_addr 0x68
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

Servo EDF;
Servo Top;
Servo Bottom;
Servo Left;
Servo Right;

typedef struct axis {
  double kP;
  double kI;
  double kD;

  double target = 0;
  double current = 0;
  double last = 0;
  double I = 0;
  double D = 0;
  double output = 0;
} axis;

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
  delay(100);
}
void rocketInit() {
  EDF.attach(A0);
  Top.attach(A1);
  Bottom.attach(A2);
  Left.attach(A3);
  Right.attach(A6);

  Serial.begin(9600);

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}
void setGain(axis *sys) {
  sys->kP = 1;
  sys->kD = 0.1;
  sys->kI = 0.01;
}
void readIMU() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}
void calcIMU() {
  double A = atan2(AcX, AcZ) * 180 / PI ;  //rad to deg
  double B = GyY / 131. ;  //16-bit data to 250 deg/sec
  pitch.current = (0.90 * (pitch.current + (B * 0.001))) + (0.10 * A) ; //complementary filter

  A = atan2(AcY, AcZ) * 180 / PI ;  //rad to deg
  B = GyX / 131. ;  //16-bit data to 250 deg/sec
  roll.current = (0.90 * (roll.current + (B * 0.001))) + (0.10 * A) ; //complementary filter

  //Serial.println(pitch.current);
  //Serial.println(roll.current);
}
void PID(axis *sys) {
  double error = sys->target - sys->current;
  sys->D = error - sys->last;
  sys->I += error;
  sys->output = sys->kP * error + sys->kD * sys->D + sys->kI * sys->I;
  sys->last = error;
}
void writeMotors() {
  Top.write(roll.output);
  Bottom.write(roll.output);
  Left.write(pitch.output);
  Right.write(pitch.output);
}
