/*******************************************************************************
 * Copyright (c) 2021 warren.
 *
 * http://blog.wjcgame.cn/
 *
 * 此类是智能家居设备管理类
 *
 *******************************************************************************/

#ifndef HA_MO_DEVICE_MANAGER_H
#define HA_MO_DEVICE_MANAGER_H
#include "device.h"
class CDeviceManager{
private:
	CDeviceManager():m_nSize(0), m_client(NULL),m_count(0){}
private:
	int getIndex(String uid);  
	CDevice* createDevice(const char* uid, int type, int flag, int pin1, int pin2, void* param = NULL);
public:
	static CDeviceManager* instance();
	static String getStateTopic(const char* uid);
	static String getCommandTopic(const char* uid);  
	static String getUid(const char* topic);
  static int getMod(unsigned long n, unsigned int m);

	void router(const char* topicName, const char* payload, unsigned int length);
	CDevice* addDevice(const char* uid, int type, int flag, int pin1, int pin2, void* param = NULL);
	void loop(unsigned long count);	//循环调用, count为开机到现在的计数器
  void loopWithoutMqtt(unsigned long count);
	void setup();	//设备初始化
	void mqttInit(PubSubClient* client); //mqtt网络初始化
	void clear();
	PubSubClient* getPubSubClient();
  int getCount(){return m_count;}
  
void haprint(int a) {
  Serial.print(a);
  char buf[64] = {0};
  snprintf(buf, 64, "%d", a);
  pushlog(buf);
}
void haprintln(int a){
  Serial.println(a);
  char buf[64] = {0};
  snprintf(buf, 64, "%d", a);
  pushlog(buf);
}


void haprint(const char* a) {
  Serial.print(a);
  pushlog(a);
}
void haprintln(const char* a){
  Serial.println(a);
  pushlog(a);
}

void haprint(String a) {
  Serial.print(a);
  pushlog(a.c_str());
}
void haprintln(String a){
  Serial.println(a);
  pushlog(a.c_str());
}
String tostring(int a)
{
  char buf[64] = {0};
  snprintf(buf, 64, "%d", a);
  return String(buf);
}
  String getUpdateTopic();
private:
 void pushlog(const char* a);
	int m_nSize;
	CDevice* m_devList[max_uid_size];
	PubSubClient* m_client;
 unsigned long m_count;
 String m_uid;
};
#define GET_PUBSUBCLIENT (*(CDeviceManager::instance()->getPubSubClient()))
#define GET_STATE_TOPIC(x) String(CDeviceManager::getStateTopic(x.c_str())).c_str()
#define GET_COMMAND_TOPIC(x) String(CDeviceManager::getCommandTopic(x.c_str())).c_str()

#define haprint(a) CDeviceManager::instance()->haprint(a)
#define haprintln(a) CDeviceManager::instance()->haprintln(a)
#define tostring(a) CDeviceManager::instance()->tostring(a)
#endif
