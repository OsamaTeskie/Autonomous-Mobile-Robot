#include "DeviceDriverSet_xxx0.h"
Servo myservo; // create servo object to control a servo
void DeviceDriverSet_Servo::DeviceDriverSet_Servo_Init(unsigned int Position_angle)
{
  myservo.attach(PIN_Servo_z, 500, 2400); //500: 0 degree  2400: 180 degree
  myservo.attach(PIN_Servo_z);
  myservo.write(Position_angle); //sets the servo position according to the 90（middle）
  delay(500);

}
#if _Test_DeviceDriverSet
void DeviceDriverSet_Servo::DeviceDriverSet_Servo_Test(void)
{
  for (;;)
  {
    myservo.attach(PIN_Servo_z);
    myservo.write(180);
    delay(500);
    myservo.write(0);
    delay(500);
  }
}
#endif

/*0.17sec/60degree(4.8v)*/
void DeviceDriverSet_Servo::DeviceDriverSet_Servo_control(unsigned int Position_angle)
{
  myservo.attach(PIN_Servo_z);
  myservo.write(Position_angle);
  delay(450);
  myservo.detach();
}




/*Motor control*/
void DeviceDriverSet_Motor::DeviceDriverSet_Motor_Init(void)
{
  pinMode(PIN_Motor_PWMA, OUTPUT);
  pinMode(PIN_Motor_PWMB, OUTPUT);
  pinMode(PIN_Motor_AIN_1, OUTPUT);
  pinMode(PIN_Motor_BIN_1, OUTPUT);
  pinMode(PIN_Motor_STBY, OUTPUT);
}


void DeviceDriverSet_Motor::DeviceDriverSet_Motor_control(boolean direction_A, uint8_t speed_A, //A组电机参数
    boolean direction_B, uint8_t speed_B, //B组电机参数
    boolean controlED                     //AB使能允许 true
                                                         )                                     //电机控制
{

  if (controlED == control_enable) //使能允许？
  {
    digitalWrite(PIN_Motor_STBY, HIGH);
    { //A...Right

      switch (direction_A) //方向控制
      {
        case direction_just:
          digitalWrite(PIN_Motor_AIN_1, HIGH);
          analogWrite(PIN_Motor_PWMA, speed_A);
          break;
        case direction_back:

          digitalWrite(PIN_Motor_AIN_1, LOW);
          analogWrite(PIN_Motor_PWMA, speed_A);
          break;
        case direction_void:
          analogWrite(PIN_Motor_PWMA, 0);
          digitalWrite(PIN_Motor_STBY, LOW);
          break;
        default:
          analogWrite(PIN_Motor_PWMA, 0);
          digitalWrite(PIN_Motor_STBY, LOW);
          break;
      }
    }

    { //B...Left
      switch (direction_B)
      {
        case direction_just:
          digitalWrite(PIN_Motor_BIN_1, HIGH);

          analogWrite(PIN_Motor_PWMB, speed_B);
          break;
        case direction_back:
          digitalWrite(PIN_Motor_BIN_1, LOW);
          analogWrite(PIN_Motor_PWMB, speed_B);
          break;
        case direction_void:
          analogWrite(PIN_Motor_PWMB, 0);
          digitalWrite(PIN_Motor_STBY, LOW);
          break;
        default:
          analogWrite(PIN_Motor_PWMB, 0);
          digitalWrite(PIN_Motor_STBY, LOW);
          break;
      }
    }
  }
  else
  {
    digitalWrite(PIN_Motor_STBY, LOW);
    return;
  }
}

