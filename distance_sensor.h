/*******************************************************************************
 * Copyright (c) 2021 warren.
 *
 * http://blog.wjcgame.cn/
 *
 * 此类是智能家居距离测量类
 *
 *******************************************************************************/

#ifndef HA_MO_DEVICE_DISTANCE_SENSOR_H
#define HA_MO_DEVICE_DISTANCE_SENSOR_H

#include "device.h"

class CDistanceSensor : public CDevice {
public:
  CDistanceSensor(const char* uid, int type, int flag, int pin1, int pin2, void* param = NULL):CDevice(uid, type, flag, pin1, pin2),cm(0),temp(0){
  }
  virtual void loop(unsigned long count);
  virtual void setup();
  virtual void init();

  virtual void commond(const char* payload, unsigned int length);

  float cm;
  float temp;
};

#endif
