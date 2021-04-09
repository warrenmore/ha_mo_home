#include "distance_sensor.h"
#include "device_manager.h"


void CDistanceSensor::loop(unsigned long count)
{
   unsigned int dt = CDeviceManager::getMod(count, 5000);
  //haprintln(dt);
  if (0==dt)
  {
    //给Trig发送一个低高低的短时间脉冲,触发测距
  digitalWrite(_pin1, LOW); //给Trig发送一个低电平
  delayMicroseconds(2);    //等待 2微妙
  digitalWrite(_pin1,HIGH); //给Trig发送一个高电平
  delayMicroseconds(10);    //等待 10微妙
  digitalWrite(_pin1, LOW); //给Trig发送一个低电平
  
  temp = float(pulseIn(_pin2, HIGH)); //存储回波等待时间,
  //pulseIn函数会等待引脚变为HIGH,开始计算时间,再等待变为LOW并停止计时
  //返回脉冲的长度
  
  //声速是:340m/1s 换算成 34000cm / 1000000μs => 34 / 1000
  //因为发送到接收,实际是相同距离走了2回,所以要除以2
  //距离(厘米)  =  (回波时间 * (34 / 1000)) / 2
  //简化后的计算公式为 (回波时间 * 17)/ 1000
  
  cm = (temp * 17 )/1000; //把回波时间换算成cm
 
  /*haprint("Echo =");
  haprint(temp);//串口输出等待时间的原始数据
  haprint(" | | Distance = ");
  haprint(cm);//串口输出距离换算成cm的结果
  haprint("cm\n");*/

  }
}
void CDistanceSensor::setup()
{  
  pinMode(_pin1, OUTPUT);
  pinMode(_pin2, INPUT);
}
void CDistanceSensor::init()
{

}

void CDistanceSensor::commond(const char* payload, unsigned int length)
{
  
}
