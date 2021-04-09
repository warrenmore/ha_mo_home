#include "temhumsensor.h"
#include "device_manager.h"


void CTemHumSensor::loop(unsigned long count)
{
  unsigned int dt = CDeviceManager::getMod(count, 5000);
  //haprintln(dt);
	if (0==dt)
	{
		int chk = DHT11.read(_pin1);     //将读取到的值赋给chk
		switch (chk)
		{
      		case DHTLIB_OK:
          {
      			haprintln("OK");
      
      			int hum = DHT11.humidity;      //将湿度值赋给hum
      			int tem = DHT11.temperature;   //将湿度值赋给tem		
            if (hum != 0 && tem != 0)
            {  
              char buf[64] = { 0 };
              snprintf(buf, 64, "{\"tem\":%d,\"hum\":%d}", tem, hum);
              haprintln(buf);
              GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), buf, true);
            }
      
      			break;
      		}
      		case DHTLIB_ERROR_CHECKSUM:
      			haprintln("Checksum error");
      			break;
      		case DHTLIB_ERROR_TIMEOUT:
      			haprintln("Time out error");
      			break;
      		default:
      			haprintln("Unknown error");
      			break;
		}
   //delay(500);
	}
 
}
void CTemHumSensor::setup()
{
	pinMode(_pin1, OUTPUT);
}
void CTemHumSensor::init()
{

}

void CTemHumSensor::commond(const char* payload, unsigned int length)
{	
}
