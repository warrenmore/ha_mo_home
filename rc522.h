/*******************************************************************************
   Copyright (c) 2021 warren.

   http://blog.wjcgame.cn/

   此类是智能RC522类

 *******************************************************************************/

#ifndef HA_MO_DEVICE_RC522_H
#define HA_MO_DEVICE_RC522_H

#include "device.h"
#include <SPI.h>
#include <MFRC522.h>
#include <map>
#include "door_lock.h"

class CRc522 : public CDevice {
  public:
    CRc522(const char* uid, int type, int flag, int pin1, int pin2, void* param = NULL): CDevice(uid, type, flag, pin1, pin2)
    , mfrc522(_pin1, _pin2) 
    , _hasCard(false)
    , _lastCount(0){
      if (param != NULL)
      {
        _lock = (CDoorlock*)param;
      }
      else
      {
        _lock = NULL;
      }
    }
    virtual void loop(unsigned long count);
    virtual void loopWithoutMqtt(unsigned long count);
    virtual void setup();
    virtual void init();

    virtual void commond(const char* payload, unsigned int length);
  private:
    String dump_byte_array(byte *buffer, byte bufferSize);
    void p(String car_uid, bool isNow = false);
    void checkRc522(bool isNow);
    MFRC522 mfrc522;

    std::map<String, String> mapName;
    CDoorlock* _lock;

    bool _hasCard;
    unsigned long _lastCount;
};

#endif
