#include "curtain_stop_switch.h"
#include "device_manager.h"

#define WAIT_COUNT 10
#define DELAY_COUNT 50
#define STOP_COUNT 20000

void CCurtainStopSwitch::loop(unsigned long count)
{
  if (_flag == STATE_FLAG_ON)
  {
    if ( (count < lastCount_ && count > (WAIT_COUNT+DELAY_COUNT)) || (count -lastCount_) > (WAIT_COUNT+DELAY_COUNT))
    {
      digitalWrite(_pin1, LOW); //
      _flag = STATE_FLAG_OFF;
      haprintln("set state OFF");
      GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), "OFF", true);
    }
  }
  else if (lastCount_>0)
  {
    if (state_ == 0 && ((count < lastCount_ && count > DELAY_COUNT) || (count -lastCount_) > WAIT_COUNT))
    {
      digitalWrite(_pin1, HIGH); // 亮灯
      _flag = STATE_FLAG_ON;
      haprintln("set state ON");
      
      GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), "ON", true);
      state_ = 1;
    }
    else if (state_ == 1 &&( (count < lastCount_ && count > (WAIT_COUNT+DELAY_COUNT+STOP_COUNT)) || (count -lastCount_) > (WAIT_COUNT+DELAY_COUNT+STOP_COUNT)))
    {
      if (dianyuan_)
        dianyuan_->close();
      lastCount_ = 0;
    }
  }
}
void CCurtainStopSwitch::setup()
{	
	pinMode(_pin1, OUTPUT);
	digitalWrite(_pin1, LOW);
}
void CCurtainStopSwitch::init()
{

}

void CCurtainStopSwitch::commond(const char* payload, unsigned int length)
{
	if ((char)payload[1] == 'n' || (char)payload[1] == 'N') {
  if (dianyuan_)
   {
    if (lastCount_ > 0)
      dianyuan_->close(true);
    dianyuan_->open();
   }
   state_ = 0;
    lastCount_=CDeviceManager::instance()->getCount();
      haprintln("set state START");
		
	}
}
