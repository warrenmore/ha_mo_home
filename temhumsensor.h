/*******************************************************************************
 * Copyright (c) 2021 warren.
 *
 * http://blog.wjcgame.cn/
 *
 * 此类是智能家居温湿传感器类
 *
 *******************************************************************************/

#ifndef HA_MO_DEVICE_WWLSENSOR_H
#define HA_MO_DEVICE_WWLSENSOR_H

#include "device.h"
#ifdef WIN32
#else
#include "dht11.h"
#endif

class CTemHumSensor : public CDevice {
public:
	CTemHumSensor(const char* uid, int type, int flag, int pin1, int pin2, void* param = NULL):CDevice(uid, type, flag, pin1, pin2){
	}
	virtual void loop(unsigned long count);
	virtual void setup();
	virtual void init();

	virtual void commond(const char* payload, unsigned int length);

	dht11 DHT11;
};

#endif
