#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "device_manager.h"

#define wifi_ssid1 "***"
#define wifi_password1 "***"

#define wifi_ssid2 "***"
#define wifi_password2 "***"

#define waiting_wifi 20

WiFiClient espClient;
PubSubClient client(espClient);

#define mqtt_server1 "***" // MQTT服务器的IP地址
#define mqtt_server2 "***" // MQTT服务器的IP地址
#define mqtt_user "***" // MQTT 用户名
#define mqtt_password "***" //MQTT 密码

void setup() {

  CDeviceManager::instance()->setup();
  Serial.begin(9600);
  delay(500);
  Serial.print("ready to setup!");
  setup_wifi();

  Serial.print("ready to connect mqtt!");
  client.setServer(mqtt_server1, 1883);
  client.setCallback(callback) ; //设定回掉方式，当8266收到订阅消息时调用此方法

  Serial.print("setup over!");

}

bool setup_wifi() { //链接wifi
  unsigned int c = 1;
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid1);
  WiFi.begin(wifi_ssid1, wifi_password1);
  while (WiFi.status() != WL_CONNECTED) {
    CDeviceManager::instance()->loopWithoutMqtt(millis());
    delay(500);
    Serial.print("- ");
    Serial.print(c);
    unsigned int dt = CDeviceManager::getMod(c, waiting_wifi);
    if (0 == dt)
    {
      if (c >= waiting_wifi)
      {
        Serial.print("Connecting to ");
        Serial.println(wifi_ssid2);
        WiFi.begin(wifi_ssid2, wifi_password2);
      }
      else
      {
        Serial.print("Connecting to ");
        Serial.println(wifi_ssid1);
        WiFi.begin(wifi_ssid1, wifi_password1);
      }
    }
    c++;
    if (c >= 2 * waiting_wifi)
    {
      c = 0;
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}
int serverIndex = 0;

bool reconnect() {
  int waitTimes = 0;
  while (!client.connected()) {
    
    CDeviceManager::instance()->loopWithoutMqtt(millis());
    Serial.print("Attempting MQTT connection…");
    //ESP8266Client_child_room
    //ESP8266Client_keting
    //ESP8266Client_child_curtain
    //ESP8266Client_shufang_curtain
    //ESP8266Client_keting_curtain
    //ESP8266Client_woshi_curtain
    //ESP8266Client_woshi1

    if (client.connect("ESP8266Client_test407", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    }
    else {
      Serial.print("failed, rc=”");
      Serial.print(client.state());
      Serial.print(", waitTimes=”");
      Serial.print(waitTimes);
      Serial.println(" try again in 1 seconds");
      delay(1000);
      waitTimes++;

      if (waitTimes > 3)
      {
        if (serverIndex == 0)
        {
          client.setServer(mqtt_server2, 1883);
          serverIndex = 1;
        }
        else
        {
          client.setServer(mqtt_server1, 1883);
          serverIndex = 0;
        }
        waitTimes = 0;
      }
    }
  }

  Serial.println("reconnect over!!");
  return true;
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic); // 打印主题信息
  Serial.print("] ");
  char buf[1024]={0};
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]); // 打印主题内容
    buf[i]=(char)payload[i];
  }
  Serial.println();
  CDeviceManager::instance()->router(topic, (char*)buf, length);
}

int status = 0;
void loop() {
  if (!client.connected()) {
    if (reconnect())
      status = 1;
  }
  if (status == 1) //这几句不能写在setup
  {

    Serial.print("mqttInit");
    CDeviceManager::instance()->mqttInit(&client);
    status = 2;
  }
  //Serial.print(count);
  CDeviceManager::instance()->loop(millis());
  client.loop();
}
