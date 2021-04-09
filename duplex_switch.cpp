#include "duplex_switch.h"
#include "device_manager.h"
#include "map"

#define MAX_DX__ 15

int CDianliu::getDianPin(int n, int flag)
{
  if (n & flag)
  {
    return HIGH;
  }
  else 
  {
    return LOW;
  }
}

int CDianliu::getDianliu(int n)
{
  digitalWrite(E_, LOW);
  digitalWrite(S0_, getDianPin(n, 1));
  digitalWrite(S1_, getDianPin(n, 2));
  digitalWrite(S2_, getDianPin(n, 4));
  std::map<int, int> mark;
  for(int i = 0; i < 20; i++)
  {
    int sensorValue = analogRead(Z_);
    mark[sensorValue]++;
    delayMicroseconds(500);    //等待 2微妙
  }
  
  digitalWrite(E_, HIGH);
  
  if (mark.size() <= 2)
    return 0;
  else
    return (++(mark.rbegin()))->first - (++(mark.begin()))->first;
}
int CDianliu2::getDianliu(int n)
{
  digitalWrite(E_, LOW);
  digitalWrite(S0_, getDianPin(n, 1));
  digitalWrite(S1_, getDianPin(n, 2));
  digitalWrite(S2_, getDianPin(n, 4));
  std::map<int, int> mark;
  for(int i = 0; i < 32; i++)
  {
    int sensorValue = analogRead(Z_);
    mark[sensorValue]++;
    delayMicroseconds(500);    //等待 2微妙
  }
  
  digitalWrite(E_, HIGH);
  if (mark.size() <= 1)
    return 0;
  else
    return ((mark.rbegin()))->first - ((mark.begin()))->first;
}
int CDianliu3::getDianliu(int n)
{
  std::map<int, int> mark;
  for(int i = 0; i < 32; i++)
  {
    int sensorValue = analogRead(_pin);
    mark[sensorValue]++;
    delayMicroseconds(500);    //等待 2微妙
  }
  
  if (mark.size() <= 1)
    return 0;
  else
    return ((mark.rbegin()))->first - ((mark.begin()))->first;
}
int CDUplexSwitch::getDx()
{
  if (_dianliu != NULL)
  {
    return _dianliu->getDianliu(_pin2);
  }
  return 0;
}

void CDUplexSwitch::loop(unsigned long count)
{
   unsigned int dt = CDeviceManager::getMod(count, 100+_pin2*100);
  //haprintln(dt);
  if (0!=dt)
  {
    return;
  }
  int dx = getDx();
  //if (dx > MAX_DX__)
  {
        haprint(_uid+" dx = "+tostring(dx));
  }
  if (_flag == STATE_FLAG_OFF)
    {
      if (dx <= MAX_DX__)
      {
        return;
      }
      else
      {
        haprint(" now ON\n");
        setState(STATE_FLAG_ON);
      }
    }
    else
    {
      if (dx > MAX_DX__)
      {
        return;
      }
      else
      {
        haprint(" now OFF\n");
        setState(STATE_FLAG_OFF);
      }
    }
}
void CDUplexSwitch::setup()
{	
	pinMode(_pin1, OUTPUT);
  _lastDianpin = HIGH;
  digitalWrite(_pin1, _lastDianpin);
}
void CDUplexSwitch::init()
{

}
int CDUplexSwitch::getDianpin(int flag)
{
  if (_flag == flag)
  {
    return _lastDianpin;
  }
  else
  {
    if (_lastDianpin == HIGH)
    {
      return LOW;
    }
  }
  return HIGH;
}

void CDUplexSwitch::commond(const char* payload, unsigned int length)
{
	if ((char)payload[1] == 'n' || (char)payload[1] == 'N') {
   _lastDianpin = getDianpin(STATE_FLAG_ON);
	}
	else {
    _lastDianpin = getDianpin(STATE_FLAG_OFF);
	}
 digitalWrite(_pin1, _lastDianpin); 
 
 haprint(String("cur dianpin:")+tostring(_lastDianpin));
}
void CDUplexSwitch::setState(int flag)
{
  if (flag == _flag)
  {
    haprintln(_uid+String(" state is ")+tostring(_flag));
    return;
  }
  _flag = flag;
  if (_flag == STATE_FLAG_ON)
  {
    haprint(" set state ON\n");
    GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), "ON", true);
  }
  else
   {
    haprint(" set state OFF\n");
    GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), "OFF", true);
   }
}
