#include <Servo.h>
 
Servo test_motor;
Servo Top;
int val=50;
int cnt = 0;
void setup() {
  Serial.begin(9600);
  test_motor.attach(9);      
  test_motor.write(50);
  Top.attach(10);
  delay(3000);                        
}
 
void loop() {
  if(Serial.available()){
    char temp = Serial.read();
    if(temp=='u'){
      val += 2;
    }
    else if(temp=='d'){
      val -= 2;
    }
    test_motor.write(val);
  }
  Serial.print("current value : ");
  Serial.println(val); 
  delay(100);
  if(cnt == 0){
    Top.write(100);
    cnt = 1;
  }
  else {
    Top.write(50);
    cnt = 0;
  }
}
