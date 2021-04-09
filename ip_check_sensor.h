/*******************************************************************************
 * Copyright (c) 2021 warren.
 *
 * http://blog.wjcgame.cn/
 *
 * 此类是智能家居IP检测类，局域网内是否存在此IP
 * 如手机的IP，若存在则代表手机在家。且些状态邮不存在变为存在时，会发一条开门指令到门锁
 *
 *******************************************************************************/

#ifndef HA_MO_IP_CHECK_SENSOR_H
#define HA_MO_IP_CHECK_SENSOR_H

#include "device.h"

class CIpCheckSensor : public CDevice {
public:
	CIpCheckSensor(const char* uid, int type, int flag, int pin1, int pin2, void* param = NULL):CDevice(uid, type, flag, pin1, pin2){
		if (param)
		{
			_ip = (char *)param;
		}
	}
	virtual void loop(unsigned long count);
	virtual void setup();
	virtual void init();

	virtual void commond(const char* payload, unsigned int length);
private:
	int checkIp(const char *szDestIP);
	int getWaitSeconds();
	void setState(int flag);
	String _ip;
	bool _lastState;
};

#endif
