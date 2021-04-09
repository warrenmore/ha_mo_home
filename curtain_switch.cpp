#include "curtain_switch.h"
#include "device_manager.h"

#define WAIT_COUNT 3000
#define DELAY_COUNT 50
#define STOP_COUNT 20000
void CCurtainDianyuan::setup(){
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, HIGH);
  }
void CCurtainDianyuan::open(){
    _index++;
    haprintln("dianyuan OPEN");
    digitalWrite(_pin, LOW);
}
void CCurtainDianyuan::close(bool notClose){
    _index--;
    haprintln(String("dianyuan index:")+tostring(_index));
    if (_index <= 0 && notClose == false)
    {
      haprintln("dianyuan CLOSE");
      digitalWrite(_pin, HIGH);
    }
}
  
void CCurtainSwitch::loop(unsigned long count)
{
  if (_flag == STATE_FLAG_ON)
  {
    if( (count < lastCount_ && count > (WAIT_COUNT+DELAY_COUNT)) || (count -lastCount_) > (WAIT_COUNT+DELAY_COUNT))
    {
          digitalWrite(curPin_, LOW); 
          _flag = STATE_FLAG_OFF;
          if (curPin_ == _pin1)
         {
          haprintln("当前执行完毕，开");
         }
         else
         {
          haprintln("当前执行完毕，关");
         }
    }
  }
  else if (lastCount_>0)
  {
    if (state_ == 0 && ((count < lastCount_ && count > DELAY_COUNT) || (count -lastCount_) > WAIT_COUNT))
    {
          digitalWrite(curPin_, HIGH);
          _flag = STATE_FLAG_ON;
          state_ = 1;
            if (curPin_ == _pin1)
           {
            haprintln("开始执行，开");
           }
           else
           {
            haprintln("开始当前执行，关");
           }
    }
    else if (state_ == 1 &&( (count < lastCount_ && count > (WAIT_COUNT+DELAY_COUNT+STOP_COUNT)) || (count -lastCount_) > (WAIT_COUNT+DELAY_COUNT+STOP_COUNT)))
    {
      if (dianyuan_)
        dianyuan_->close();
      lastCount_ = 0;
    }
  }
}
void CCurtainSwitch::setup()
{	
    	pinMode(_pin1, OUTPUT);
    	digitalWrite(_pin1, LOW);
      pinMode(_pin2, OUTPUT);
      digitalWrite(_pin2, LOW);
}
void CCurtainSwitch::init()
{

}

void CCurtainSwitch::commond(const char* payload, unsigned int length)
{
  /*
  if (state_ == 0 || _flag == STATE_FLAG_ON)
  {
    if (curPin_ == _pin1)
     {
        GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), "ON", true);
     }
    else
     {
      GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), "OFF", true);
     }
    return;
  }//*/
  if (dianyuan_)
   {
      if (lastCount_ > 0)
          dianyuan_->close(true);
      dianyuan_->open();
   }
  state_ = 0;
  lastCount_=CDeviceManager::instance()->getCount();
  haprintln("准备执行。。。。");
  digitalWrite(_pin1, LOW); 
  digitalWrite(_pin2, LOW); 
	if ((char)payload[1] == 'n' || (char)payload[1] == 'N') {
      curPin_ = _pin1;
      
      GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), "ON", true);
	}
 else
 {
      curPin_ = _pin2;
  
      GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), "OFF", true);
 }
}
