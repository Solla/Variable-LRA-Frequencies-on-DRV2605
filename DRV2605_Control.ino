#include <Wire.h>
#include "Adafruit_DRV2605.h"

const int Motor_PWM_Pins = 16;
const uint32_t Default_PWM_Frequency = 100;
Adafruit_DRV2605 drv;

void SetMotorVoltage(uint8_t Fraction = 0x89)
{
  drv.writeRegister8(0x16, Fraction);
  drv.writeRegister8(0x17, Fraction);
}

void SetFreq(uint32_t Freq = Default_PWM_Frequency)
{
  //DRV2605 Input PWM Frequency: 10-250kHz
  assert (Freq <= 1950); //DRV2605 cannot support > 250kHz PWM signals
  assert (Freq != 0);   //DRV2605 cannot support < 10Hz PWM signals
  Freq *= 128;
  ledcSetup(0, Freq, 8);  //Clear PWM Signal
  ledcAttachPin(Motor_PWM_Pins, 0);
  ledcWrite(0, 1);
}

void setup() 
{
  Serial.begin(115200);
  Wire.begin();
  
  /*Setup DRV2605*/
  drv.begin();
  drv.useLRA();
  drv.setMode(DRV2605_MODE_PWMANALOG);
  
  //LRA_OPEN_LOOP
  drv.writeRegister8(DRV2605_REG_CONTROL3, drv.readRegister8(DRV2605_REG_CONTROL3) | 0x01);
  /*Setup DRV2605*/

  SetFreq(Default_PWM_Frequency);
}

void loop() 
{
  while (Serial.available())
  {
    int Frequency = Serial.parseInt();
    uint8_t Power = Serial.parseInt();
    SetFreq(Frequency);
    SetMotorVoltage(Power);
    
    while (Serial.available())
      Serial.read();  //Read redundant data in buffer
  }
}
