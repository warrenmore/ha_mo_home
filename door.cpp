#include "door.h"
#include "device_manager.h"

#define UnlockingTime1 500
#define RecoveryTime1  10000

#define STATUS_0 0
#define STATUS_1 1 //开锁
#define STATUS_2 2 //开锁完毕
#define STATUS_3 3 //关锁
#define STATUS_4 4 //关锁完毕

void CDoor::loop(unsigned long count)
{
  //Serial.print("-----------------count:");Serial.println(count);
  switch (_status)
  {
    case STATUS_1:
      {
        if ((count < _count && count > (UnlockingTime1)) || (count - _count) > (UnlockingTime1))
        {

          Serial.println("now count:"); Serial.println(count);
          stopOpenDoor();
        }
        break;
      }
    case STATUS_2:
      {
        if ((count < _count && count > (RecoveryTime1)) || (count - _count) > (RecoveryTime1))
        {
          Serial.print("now count:"); Serial.println(count);
          closeDoor();
        }
        break;
      }
    case STATUS_3:
      {
        if ((count < _count && count > (UnlockingTime1)) || (count - _count) > (UnlockingTime1))
        {
          Serial.print("now count:"); Serial.println(_count);
          stopCloseDoor();
        }
        break;
      }
  }

}
void CDoor::setup()
{
  pinMode(_pin1, OUTPUT);
  pinMode(_pin2, OUTPUT);
  digitalWrite(_pin1, LOW);
  digitalWrite(_pin2, LOW);
}
void CDoor::init()
{

}

void CDoor::commond(const char* payload, unsigned int length)
{
  /*if (_status != STATUS_0 && _status != STATUS_4)
  {
    if ( _flag = STATE_FLAG_ON)
      GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), "ON", true);
    else
      GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), "OFF", true);
    return;
  }*/ 
  if ((char)payload[1] == 'n' || (char)payload[1] == 'N') {
    openDoor();
  }
  else {
    closeDoor();
  }
}

void CDoor::openDoor() {
  Serial.println("openDoor");
  digitalWrite(_pin1, HIGH);
  digitalWrite(_pin2, LOW);
  _status = STATUS_1;
  _count = CDeviceManager::instance()->getCount();
  Serial.print("count:"); Serial.println(_count);

  _flag = STATE_FLAG_ON;
  haprint("set state ON\n");
  GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), "ON", true);
}

void CDoor::stopOpenDoor()
{
  Serial.println("stopDoor");
  digitalWrite(_pin1, LOW);
  _status++;
  _count = CDeviceManager::instance()->getCount();
  Serial.print("count:"); Serial.println(_count);
}
void CDoor::closeDoor() {
  Serial.println("closeDoor");
  digitalWrite(_pin2, HIGH);
  digitalWrite(_pin1, LOW);
  _status = STATUS_3;
  _count = CDeviceManager::instance()->getCount();
  Serial.print("count:"); Serial.println(_count);

  _flag = STATE_FLAG_OFF;
  haprint("set state OFF\n");
  GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), "OFF", true);
}

void CDoor::stopCloseDoor()
{
  Serial.println("stopDoor");
  digitalWrite(_pin2, LOW);
  _status++;
  _count = CDeviceManager::instance()->getCount();
  Serial.print("count:"); Serial.println(_count);
}
