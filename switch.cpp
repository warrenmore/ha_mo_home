#include "switch.h"
#include "device_manager.h"


void CSwitch::loop(unsigned long count)
{

}
void CSwitch::setup()
{	
	pinMode(_pin1, OUTPUT);
	digitalWrite(_pin1, HIGH);
}
void CSwitch::init()
{

}

void CSwitch::commond(const char* payload, unsigned int length)
{
  haprint("第一版\n");
	if ((char)payload[1] == 'n' || (char)payload[1] == 'N') {
		digitalWrite(_pin1, LOW); // 亮灯
		_flag = STATE_FLAG_ON;
		haprint("set state ON\n");
		GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), "ON", true);
	}
	else {
		digitalWrite(_pin1, HIGH); // 熄灯
		_flag = STATE_FLAG_OFF;
		haprint("set state OFF\n");
		GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), "OFF", true);
	}
}
