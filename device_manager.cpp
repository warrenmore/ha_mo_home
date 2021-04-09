#include "device_manager.h"

#include "switch.h"
#include "temhumsensor.h"
#include "distance_sensor.h"
#include "duplex_switch.h"
#include "ip_check_sensor.h"
#include "curtain_switch.h"
#include "curtain_stop_switch.h"
#include "rc522.h"
#include "door_lock.h"
#include "door.h"
#include <ESP8266httpUpdate.h>

CDeviceManager* ins = NULL;

CDeviceManager* CDeviceManager::instance() {
  if (ins == NULL)
    ins = new CDeviceManager();
  return ins;
}

String CDeviceManager::getStateTopic(const char* uid) {
  String topic = state_topic;
  topic += uid;
  return topic;
}
String CDeviceManager::getCommandTopic(const char* uid) {
  String topic = command_topic;
  topic += uid;
  return topic;
}

String CDeviceManager::getUid(const char* topic) {
  String uid = &(topic[13]);
  return uid;
}
int CDeviceManager::getIndex(String uid) {
  for (int i = 0; i < m_nSize; i++) {
    if (uid == m_devList[i]->_uid) {
      return i;
    }
  }
  return -1;
}
int CDeviceManager::getMod(unsigned long n, unsigned int m)
{
  if (0 == n || 0 == m)
  {
    return 0;
  }
  unsigned long dt = n - (unsigned long)(n / m) * m;
  return dt;
}
void CDeviceManager::router(const char* topicName, const char* payload, unsigned int length) {
  if (getUpdateTopic() == String(topicName))
  {
    haprintln(String("payload:")+payload);
    t_httpUpdate_return ret = ESPhttpUpdate.update(payload);//"http://server/file.bin"); // 编译好的固件文件

    switch (ret) {
      case HTTP_UPDATE_FAILED:
      {
        char buf[1024]={0};
        snprintf(buf, 1024, "HTTP_UPDATE_FAILD Error=%d=%s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        haprintln(buf);
        break;
      }
      case HTTP_UPDATE_NO_UPDATES:
        haprintln("HTTP_UPDATE_NO_UPDATES");
        break;

      case HTTP_UPDATE_OK:
        haprintln("HTTP_UPDATE_OK");
        break;
    }
    return;
  }
  String uid = getUid(topicName);
  int index = getIndex(uid);
  if (index == -1) {
    haprintln("not found the topic!");
    return;
  }
  m_devList[index]->commond(payload, length);
  return;
}

CDevice* CDeviceManager::addDevice(const char* uid, int type, int flag, int pin1, int pin2, void* param)
{
  if (m_nSize >= max_uid_size)
  {
    return NULL;
  }
  CDevice* device = createDevice(uid, type, flag, pin1, pin2, param);
  m_devList[m_nSize] = device;
  m_nSize += 1;
  return device;
}
CDevice* CDeviceManager::createDevice(const char* uid, int type, int flag, int pin1, int pin2, void* param)
{
  switch (type)
  {
    case HA_MO_SWITCH:
      return new CSwitch(uid, type, flag, pin1, pin2, param);
    case HA_MO_WWLSENSOR:
      return new CTemHumSensor(uid, type, flag, pin1, pin2, param);
    case HA_MO_DIS_SENSOR:
      return new CDistanceSensor(uid, type, flag, pin1, pin2, param);
    case HA_MO_DUPLEX_SWITCH:
      return new CDUplexSwitch(uid, type, flag, pin1, pin2, param);
    case HA_MO_IP_CHECK_SENSOR:
      return new CIpCheckSensor(uid, type, flag, pin1, pin2, param);
    case HA_MO_CURTAIN_SWITCH:
      return new CCurtainSwitch(uid, type, flag, pin1, pin2, param);
    case HA_MO_CURTAIN_STOP_SWITCH:
      return new CCurtainStopSwitch(uid, type, flag, pin1, pin2, param);
    case HA_MO_RC522:
      return new CRc522(uid, type, flag, pin1, pin2, param);
    case HA_MO_DOOR_LOCK:
      return new CDoorlock(uid, type, flag, pin1, pin2, param);
    case HA_MO_DOOR:
      return new CDoor(uid, type, flag, pin1, pin2, param);

  }
  return new CSwitch(uid, type, flag, pin1, pin2);
}
void CDeviceManager::loop(unsigned long count)
{
  m_count = count;
  for (int i = 0; i < m_nSize; i++) {
    m_devList[i]->loop(count);
  }
}
void CDeviceManager::loopWithoutMqtt(unsigned long count)
{
  m_count = count;
  for (int i = 0; i < m_nSize; i++) {
    m_devList[i]->loopWithoutMqtt(count);
  }
}

void CDeviceManager::setup()
{
  //*test
    m_uid = "dev00001";
    addDevice("00001", HA_MO_SWITCH, 0, D5, 0);			//test switch
    //addDevice("00002", HA_MO_WWLSENSOR, 0, D1, 0);		//test hum tem
    //*/

  /*child room
    m_uid = "dev00002";
    CDianliu* dianliu = new CDianliu2(A0, D0, D3, D7, D8);
    addDevice("00004", HA_MO_DUPLEX_SWITCH, 0, D1, 0, (void*)dianliu);	//child room
    addDevice("00005", HA_MO_DUPLEX_SWITCH, 0, D2, 1, (void*)dianliu);	//wc
    addDevice("00006", HA_MO_DUPLEX_SWITCH, 0, D5, 2, (void*)dianliu);	//pc
    addDevice("00008", HA_MO_DUPLEX_SWITCH, 0, D6, 3, (void*)dianliu);  //wc cha
    //addDevice("00007", HA_MO_WWLSENSOR, 0, D1, 0);		//
    //*/

  /*live room
    m_uid = "dev00003";
    CDianliu* dianliu = new CDianliu2(A0, D0, D3, D7, D8);
    addDevice("00009", HA_MO_DUPLEX_SWITCH, 0, D1, 0, (void*)dianliu);
    addDevice("00010", HA_MO_DUPLEX_SWITCH, 0, D2, 1, (void*)dianliu);
    addDevice("00011", HA_MO_DUPLEX_SWITCH, 0, D4, 2, (void*)dianliu);
    addDevice("00012", HA_MO_DUPLEX_SWITCH, 0, D5, 3, (void*)dianliu);
    addDevice("00013", HA_MO_DUPLEX_SWITCH, 0, D6, 4, (void*)dianliu);
    //*/

  /*child curtain
    m_uid = "dev00004";
    CCurtainDianyuan* dianyuan = new CCurtainDianyuan(D0);
    addDevice("00014", HA_MO_CURTAIN_SWITCH, 0, D5, D1, (void*)dianyuan); //
    addDevice("00015", HA_MO_CURTAIN_STOP_SWITCH, 0, D2, D0, (void*)dianyuan); //
    //addDevice("00016", HA_MO_CURTAIN_SWITCH, 0, D5, D0, (void*)dianyuan); //
    addDevice("00017", HA_MO_CURTAIN_SWITCH, 0, D6, D8, (void*)dianyuan); //
    addDevice("00018", HA_MO_CURTAIN_STOP_SWITCH, 0, D7, D0, (void*)dianyuan); //
    //addDevice("00019", HA_MO_CURTAIN_SWITCH, 0, D8, D0, (void*)dianyuan); //
    //*/
  /*shufang curtain
  m_uid = "dev00005";
  CCurtainDianyuan* dianyuan = new CCurtainDianyuan(D0);
  addDevice("00020", HA_MO_CURTAIN_SWITCH, 0, D1, D5, (void*)dianyuan); //
  addDevice("00021", HA_MO_CURTAIN_STOP_SWITCH, 0, D2, D0, (void*)dianyuan); //
  //addDevice("00022", HA_MO_CURTAIN_SWITCH, 0, D5, D0, (void*)dianyuan); //
  addDevice("00023", HA_MO_CURTAIN_SWITCH, 0, D7, D8, (void*)dianyuan); //
  //addDevice("00024", HA_MO_CURTAIN_STOP_SWITCH, 0, D6, D0, (void*)dianyuan); //
  //addDevice("00025", HA_MO_CURTAIN_STOP_SWITCH, 0, D8, D0, (void*)dianyuan); //
  //*/
  /*keting curtain
    m_uid = "dev00006";
    CCurtainDianyuan* dianyuan = new CCurtainDianyuan(D0);
    addDevice("00026", HA_MO_CURTAIN_SWITCH, 0, D1, D5, (void*)dianyuan); //
    addDevice("00027", HA_MO_CURTAIN_STOP_SWITCH, 0, D2, D0, (void*)dianyuan); //
    //addDevice("00028", HA_MO_CURTAIN_SWITCH, 0, D5, D0, (void*)dianyuan); //
    // */
  /*woshi curtain
    m_uid = "dev00007";
    CCurtainDianyuan* dianyuan = new CCurtainDianyuan(D0);
    addDevice("00029", HA_MO_CURTAIN_SWITCH, 0, D1, D5, (void*)dianyuan); //
    addDevice("00030", HA_MO_CURTAIN_STOP_SWITCH, 0, D2, D0, (void*)dianyuan); //
    //addDevice("00031", HA_MO_CURTAIN_STOP_SWITCH, 0, D5, D0, (void*)dianyuan); //
    addDevice("00032", HA_MO_CURTAIN_SWITCH, 0, D6, D8, (void*)dianyuan); //
    addDevice("00033", HA_MO_CURTAIN_STOP_SWITCH, 0, D7, D0, (void*)dianyuan); //
    //addDevice("00034", HA_MO_CURTAIN_SWITCH, 0, D8, D0, (void*)dianyuan); //
    //*/

  /*门禁卡读取
    m_uid = "dev00008";
    CDevice* lock = NULL;//addDevice("00038", HA_MO_DOOR_LOCK, 0, D0, D3);
    addDevice("00035", HA_MO_RC522, 0, D2, D1, (void*)lock); //
    //*/
  /*大门锁
    m_uid = "dev00009";
    addDevice("00036", HA_MO_DOOR, 0, D2, D1);
    //*/

  /*woshi
    m_uid = "dev00010";
    CDianliu* dianliu = new CDianliu3(A0);
    addDevice("00037", HA_MO_DUPLEX_SWITCH, 0, D1, 0, (void*)dianliu);
    //*/


  for (int i = 0; i < m_nSize; i++) {
    m_devList[i]->setup();
  }
}

void CDeviceManager::mqttInit(PubSubClient* client)
{
  m_client = client;
  m_client->subscribe(getUpdateTopic().c_str());
  int rc = 0;
  for (int i = 0; i < m_nSize; i++) {
    String topic = getCommandTopic(m_devList[i]->_uid.c_str());
    m_client->subscribe(topic.c_str());
    m_devList[i]->init();
    haprintln(String(m_devList[i]->_uid.c_str())+":初始化完毕！");
  }
}
void CDeviceManager::clear()
{
  int rc = 0;
  for (int i = 0; i < m_nSize; i++) {
    String topic = getCommandTopic(m_devList[i]->_uid.c_str());
    m_client->unsubscribe(topic.c_str());
  }
}
PubSubClient* CDeviceManager::getPubSubClient()
{
  return m_client;
}

void CDeviceManager::pushlog(const char* a)
{
  if (m_client)
  {
    m_client->publish(log_topic, a, true);
  }
}
String CDeviceManager::getUpdateTopic()
{
  String topic = update_topic;
  //topic += m_uid;
  return topic;
}
