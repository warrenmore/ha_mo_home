#include "rc522.h"
#include "device_manager.h"
MFRC522::MIFARE_Key key;

void CRc522::checkRc522(bool isNow)
{
  // 寻找新卡
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    //Serial.println("没有找到卡");
    return;
  }

  // 选择一张卡
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    Serial.println("没有卡可选");
    return;
  }


  // 显示卡片的详细信息
  Serial.print(F("卡片 UID:"));
  String car_uid = dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  Serial.print(F("卡片类型: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
  /*
    // 检查兼容性
    if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
          &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
          &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("仅仅适合Mifare Classic卡的读写"));
    return;
    }

    MFRC522::StatusCode status;
    if (status != MFRC522::STATUS_OK) {
    Serial.print(F("身份验证失败？或者是卡链接失败"));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
    }*/

  //停止 PICC
  mfrc522.PICC_HaltA();
  //停止加密PCD
  mfrc522.PCD_StopCrypto1();


  p(car_uid, isNow);
}
#define rc522Delay 3000
void CRc522::loopWithoutMqtt(unsigned long count)
{
  checkRc522(true);
}
void CRc522::loop(unsigned long count)
{
  if (_hasCard)
  {
    if ((count < _lastCount && count > (rc522Delay)) || (count - _lastCount) > (rc522Delay))
     {
        p("0");
        _hasCard = false;
     }
  }
  else
  {
    checkRc522(false);
  }
}
void CRc522::p(String car_uid, bool isNow)
{
  _hasCard = true;
  _lastCount = CDeviceManager::instance()->getCount();
  String name = "未知用户";
/*
  std::map<String, String>::iterator it = mapName.find(car_uid);
  if (it != mapName.end() )
  {
    name = it->second;

    if (_lock)
    {
      if (isNow)
      {
        _lock->commond("now", 3);
      }
      else
      {
        _lock->commond("on", 3);
      }
    }

  }//*/
  if (!isNow)
  {
    char buf[128] = { 0 };
    snprintf(buf, 128, "{\"card_uid\":\"%s\",\"name\":\"%s\"}", car_uid.c_str(), name.c_str());
    haprintln(buf);
    GET_PUBSUBCLIENT.publish(GET_STATE_TOPIC(_uid), buf, true);
  }
}
void CRc522::setup()
{
  SPI.begin();        // SPI开始
  mfrc522.PCD_Init(); // Init MFRC522 card
  Serial.println("rc522 start");
  String u1 = "82247196200";
  String n1 = "勤奋树树";
 /* 
  mapName[u1] = n1;
  u1 = "768882246";
  n1 = "勤奋树树";
  mapName[u1] = n1;
  u1 = "1624616970";
  n1 = "勤奋树树";
  mapName[u1] = n1;
  u1 = "16321620224";
  n1 = "白卡1";
  mapName[u1] = n1;
  u1 = "55141150214";
  n1 = "挂卡1";
  mapName[u1] = n1;
  u1 = "1701139660";
  n1 = "莫妈咪";
  mapName[u1] = n1;
  u1 = "163947176";
  n1 = "听花开花乐";
  mapName[u1] = n1;
  u1 = "612154515";
  n1 = "听花开花乐";
  mapName[u1] = n1;
  u1 = "624225247";
  n1 = "莫可萱";
  mapName[u1] = n1;//*/
}
void CRc522::init()
{

}

void CRc522::commond(const char* payload, unsigned int length)
{

}
/**
   将字节数组转储为串行的十六进制值
*/
String CRc522::dump_byte_array(byte *buffer, byte bufferSize) {
  String temp;
  for (byte i = 0; i < bufferSize; i++) {
    //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    //Serial.print(buffer[i]);
    char ch[2] = {0};
    snprintf(ch, 128, "%d", buffer[i]);
    temp += ch;
  }
  haprintln(temp);
  return temp;
}
