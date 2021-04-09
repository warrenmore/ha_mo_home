#include "ip_check_sensor.h"
#include "device_manager.h"

#ifdef WIN32
#include "ping.h"
#else

#define DEF_PACKET_SIZE 32
#define inet_addr(x) 0

struct PingReply
{
	int m_usSeq;
	int m_dwRoundTripTime;
	int m_dwBytes;
	int m_dwTTL;
};
class CPing
{
public:
	bool Ping(int dwDestIP, PingReply *pPingReply, int dwTimeout=100)
	{
		return true;
	}
};
#endif // WIN32



void CIpCheckSensor::loop(unsigned long count)
{
	unsigned int dt = CDeviceManager::getMod(count, getWaitSeconds());
	if (0 == dt)
	{
		int ret = checkIp(_ip.c_str());
		setState(ret);
	}
}
void CIpCheckSensor::setup()
{	
	
}
void CIpCheckSensor::init()
{

}

void CIpCheckSensor::commond(const char* payload, unsigned int length)
{
}

int CIpCheckSensor::checkIp(const char *szDestIP)
{
	CPing objPing;
	PingReply reply;

	printf("Pinging %s with %d bytes of data:", szDestIP, DEF_PACKET_SIZE);
	bool ret = objPing.Ping(inet_addr(szDestIP), &reply);
	printf("Reply from %s: bytes=%d time=%ldms TTL=%ld\n", szDestIP, reply.m_dwBytes, reply.m_dwRoundTripTime, reply.m_dwTTL);
	if (ret == false)
	{
		return STATE_FLAG_OFF;
	}
	else
	{
		return STATE_FLAG_ON;
	}
}
int CIpCheckSensor::getWaitSeconds()
{
	if (_flag == STATE_FLAG_ON)
	{
		return 10000;
	}
	else
	{
		return 1000;
	}
}
void CIpCheckSensor::setState(int flag)
{
	if (flag == _flag)
	{
		return;
	}
	_flag = flag;
	char buf[64] = { 0 };
	if (_flag == STATE_FLAG_ON)
	{
		haprint("set state ON\n");
		snprintf(buf, 64, "{\"sta\":%s}", "1");

	}
	else
	{
		haprint("set state OFF\n");
		snprintf(buf, 64, "{\"sta\":%s}", "2");
	}
	GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), buf, true);
}
