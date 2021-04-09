/*******************************************************************************
 * Copyright (c) 2021 warren.
 *
 * http://blog.wjcgame.cn/
 *
 * 此类是智能家居双联开关类
 *
 *******************************************************************************/

#ifndef HA_MO_DEVICE_DUPLEX_SWITCH_H
#define HA_MO_DEVICE_DUPLEX_SWITCH_H

#include "device.h"

class CDianliu
{
public:
CDianliu(){}
  CDianliu(int Z, int E, int S0, int S1, int S2):Z_(Z), E_(E), S0_(S0), S1_(S1), S2_(S2)
  {
    pinMode(Z_, INPUT);
    pinMode(E_, OUTPUT);
    pinMode(S0_, OUTPUT);
    pinMode(S1_, OUTPUT);
    pinMode(S2_, OUTPUT);
  }
  int getDianPin(int n, int flag);
  virtual int getDianliu(int n);
  int Z_;
  int E_;
  int S0_;
  int S1_;
  int S2_;
};

class CDianliu2 : public CDianliu
{
public:
  CDianliu2(int Z, int E, int S0, int S1, int S2):CDianliu(Z, E, S0, S1, S2)
  {
  }
  virtual int getDianliu(int n);
};
class CDianliu3 : public CDianliu
{
public:
  CDianliu3(int pin):CDianliu(), _pin(pin)
  {
    pinMode(_pin, INPUT);
  }
  virtual int getDianliu(int n);
  int _pin;
};
class CDUplexSwitch : public CDevice {
public:
	CDUplexSwitch(const char* uid, int type, int flag, int pin1, int pin2, void* param = NULL):CDevice(uid, type, flag, pin1, pin2),_lastDianpin(HIGH){
    if (param != NULL)
    {
      _dianliu = (CDianliu*)param;
    }
    else
    {
      _dianliu = NULL;
    }
	}
	virtual void loop(unsigned long count);
	virtual void setup();
	virtual void init();

	virtual void commond(const char* payload, unsigned int length);
private:
  void setState(int flag);
  int getDianpin(int flag);
  int getDx();
  int _lastDianpin;

  CDianliu* _dianliu;
};

#endif
