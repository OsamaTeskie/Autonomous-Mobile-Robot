#ifndef _DeviceDriverSet_xxx0_H_
#define _DeviceDriverSet_xxx0_H_
#include <arduino.h>
#include <Servo.h>

class DeviceDriverSet_Servo
{
public:
  void DeviceDriverSet_Servo_Init(unsigned int Position_angle);
#if _Test_DeviceDriverSet
  void DeviceDriverSet_Servo_Test(void);
#endif
  void DeviceDriverSet_Servo_control(unsigned int Position_angle);

private:
#define PIN_Servo_z 10
};

/*Motor*/
class DeviceDriverSet_Motor
{
  public:
    void DeviceDriverSet_Motor_Init(void);
#if _Test_DeviceDriverSet
    void DeviceDriverSet_Motor_Test(void);
#endif
    void DeviceDriverSet_Motor_control(boolean direction_A, uint8_t speed_A, //A组电机参数
                                       boolean direction_B, uint8_t speed_B, //B组电机参数
                                       boolean controlED                     //AB使能允许 true
                                      );                                                                       //电机控制
  private:

#define PIN_Motor_PWMA 5
#define PIN_Motor_PWMB 6
#define PIN_Motor_BIN_1 8
#define PIN_Motor_AIN_1 7
#define PIN_Motor_STBY 3

  public:
#define speed_Max 255
#define direction_just true
#define direction_back false
#define direction_void 3

#define Duration_enable true
#define Duration_disable false
#define control_enable true
#define control_disable false
};

/*ITR20001 Detection*/
class DeviceDriverSet_ITR20001
{
public:
  bool DeviceDriverSet_ITR20001_Init(void);
  float DeviceDriverSet_ITR20001_getAnaloguexxx_L(void);
  float DeviceDriverSet_ITR20001_getAnaloguexxx_M(void);
  float DeviceDriverSet_ITR20001_getAnaloguexxx_R(void);
#if _Test_DeviceDriverSet
  void DeviceDriverSet_ITR20001_Test(void);
#endif

private:
//03
// #define PIN_ITR20001xxxL A0
// #define PIN_ITR20001xxxM A1
// #define PIN_ITR20001xxxR A2
//04
#define PIN_ITR20001xxxL A2
#define PIN_ITR20001xxxM A1
#define PIN_ITR20001xxxR A0
};

#endif

