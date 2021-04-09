/*******************************************************************************
 * Copyright (c) 2021 warren.
 *
 * http://blog.wjcgame.cn/
 *
 * 此类是智能家居门锁开关类
 *
 *******************************************************************************/

#ifndef HA_MO_DEVICE_DOOR_LOCK_H
#define HA_MO_DEVICE_DOOR_LOCK_H

#include "device.h"

class CDoorlock : public CDevice {
public:
	CDoorlock(const char* uid, int type, int flag, int pin1, int pin2, void* param = NULL):CDevice(uid, type, flag, pin1, pin2),_status(0), _count(0){
	}
	virtual void loop(unsigned long count);
	virtual void setup();
	virtual void init();

	virtual void commond(const char* payload, unsigned int length);

 private:
 void openDoor();
 void closeDoor();
 void stopDoor();
 int _status;
 unsigned long _count;
};

#endif
