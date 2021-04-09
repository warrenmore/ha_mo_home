/*******************************************************************************
 * Copyright (c) 2021 warren.
 *
 * http://blog.wjcgame.cn/
 *
 * 此类是智能家居窗帘开关类
 *
 *******************************************************************************/

#ifndef HA_MO_DEVICE_CURTAIN_STOP_SWITCH_H
#define HA_MO_DEVICE_CURTAIN_STOP_SWITCH_H

#include "device.h"
#include "curtain_switch.h"

class CCurtainStopSwitch : public CDevice {
public:
	CCurtainStopSwitch(const char* uid, int type, int flag, int pin1, int pin2, void* param = NULL):CDevice(uid, type, flag, pin1, pin2),lastCount_(0),state_(0){
    if (param)
    {
      dianyuan_ = (CCurtainDianyuan*)param;
    }
    else
    {
      dianyuan_ = NULL;
    }
	}
	virtual void loop(unsigned long count);
	virtual void setup();
	virtual void init();

	virtual void commond(const char* payload, unsigned int length);

	unsigned long lastCount_;
	unsigned int state_;
	CCurtainDianyuan* dianyuan_;
};

#endif
