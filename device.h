/*******************************************************************************
 * Copyright (c) 2021 warren.
 *
 * http://blog.wjcgame.cn/
 *
 * 此类是智能家居设备基类
 *
 *******************************************************************************/

#ifndef HA_MO_DEVICE_H
#define HA_MO_DEVICE_H

#ifndef WIN32
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#define haprint Serial.print
//#define haprintln Serial.println
#else
#include "esp8266.h"
#endif

#define state_topic "ha-mo/sta/uid"			//状态Topic 由HA发给设备的
#define command_topic "ha-mo/com/uid"		//命令Topic 由设备发给HA的
#define log_topic "ha-mo/log/topic"    //日志Topic 由设备发给HA的
#define update_topic "ha-mo/update/topic"    //更新Topic 由HA发给设备的

#define max_uid_size 50						//单个MCU最大的设备数

//以下为设备类型定义
#define HA_MO_SWITCH 1				//开关
#define HA_MO_WWLSENSOR 2			//温度、湿度、光度传感器
#define HA_MO_DIS_SENSOR 3			//距离传感器
#define HA_MO_DUPLEX_SWITCH 4		//双联开关
#define HA_MO_IP_CHECK_SENSOR 5		//
#define HA_MO_CURTAIN_SWITCH 6    //窗帘开关
#define HA_MO_CURTAIN_STOP_SWITCH 7    //窗帘开关
#define HA_MO_RC522 8    //RC522
#define HA_MO_DOOR_LOCK 9    //门锁
#define HA_MO_DOOR 10    //门锁


//NoceMcu的针脚与arduino对照
#define A0 17   //ADC  只能作为ADC输入脚,没有输出功能
#define D0 16  
#define D1 5    // SoftServo
#define D2 4    // SoftServo
#define D3 0    // SoftServo    <>    EX: Key
#define D4 2    // SoftServo    <>    UART1: RX     <>    EX: Led
#define D5 14   // SoftServo    <>    HSPI: SCLK
#define D6 12   // SoftServo    <>    HSPI: MISO
#define D7 13   // SoftServo    <>    HSPI: MOSI
#define D8 15   // SoftServo    <>    HSPI: CS
#define D9 3    // SoftServo    <>    UART0: RX
#define D10 1   // SoftServo    <>    UART0: TX
#define SD3 10  // SD_Card: SDD3
#define SD2 9   // SD_Card: SDD2
#define SD1 8   // SD_Card: SDD1    <>  SPI: MOSI   <>    UART1: RX
#define SD0 7   // SD_Card: SDD0    <>  SPI: MISO
#define CMD 11  // SD_Card: SDCMD   <>  SPI: CS
#define CLK 6   // SD_Card: SDCLK   <>  SPI: SCLK
#define RX D9   // SoftServo
#define TX D10  // SoftServo

//设备状态定义
#define STATE_FLAG_NULL 0	//无状态
#define STATE_FLAG_ON 1		//开
#define STATE_FLAG_OFF 2	//关

class CDevice {
public:
	CDevice(const char* uid, int type, int flag, int pin1, int pin2, void* param = NULL) {
		_uid = uid;
		_type = type;
		_flag = flag;
		_pin1 = pin1;
		_pin2 = pin2;
	}
	virtual void loop(unsigned long count) = 0;	//循环调用, count为开机到现在的计数器
	virtual void setup() = 0;	//设备初始化
	virtual void init() = 0;	//网络完成后的初始化

	virtual void commond(const char* payload, unsigned int length) = 0;
  virtual void loopWithoutMqtt(unsigned long count){}

	String _uid;
	int _type;
	int _pin1;
	int _pin2;
	int _flag;
};
#endif
