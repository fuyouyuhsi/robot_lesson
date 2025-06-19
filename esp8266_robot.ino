
#include <ArduinoMqttClient.h>
#include <DFRobot_DHT11.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "Redmi";    // your network SSID (name)
char pass[] = "abc123456";    // your network password (use for WPA, or use as key for WEP)
Servo myServo;  // 创建舵机对象

DFRobot_DHT11 DHT;
int angle = 0;
#define DHT11_PIN 5
#define servoPin 4

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate 
//    flashed in the WiFi module.

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[]    = "iot-06z00cxx1p87j6t.mqtt.iothub.aliyuncs.com";
int        port        = 1883;

const char inTopic[]   = "/sys/k0t5suseNtK/esp8266_dev/thing/service/property/set";
const char outTopic[]  = "/sys/k0t5suseNtK/esp8266_dev/thing/event/property/post";

const long interval = 10000;
unsigned long previousMillis = 0;

int count = 0;
String inputString = "";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // != WL_CONNECTED wait for serial port to connect. Needed for native USB port only
  }
  pinMode(4,OUTPUT);
  myServo.attach(servoPin); // 绑定舵机到指定引脚
  myServo.write(90); // 初始化舵机到 90 度
  Serial.println("Servo initialized at 90 degrees");

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid,pass);
  while (!WiFi.isConnected()) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();
  String willPayload = "oh no!";
  bool willRetain = true;
  int willQos = 1;

  mqttClient.setId("k0t5suseNtK.esp8266_dev|securemode=2,signmethod=hmacsha256,timestamp=1740661520959|");
  mqttClient.setUsernamePassword("esp8266_dev&k0t5suseNtK","52fd0e66b4e62198dae622c867189ee85c2b549fb69915b0a44db117ad39a50a");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(inTopic);
  Serial.println();
  int subscribeQos = 1;

  mqttClient.subscribe(inTopic, subscribeQos);


  Serial.print("Waiting for messages on topic: ");
  Serial.println(inTopic);
  Serial.println();
}

void loop() {

  mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;
    

    delay(1000);
    String payload;
    
    DHT.read(DHT11_PIN);//DHT.temperature DHT.humidity 
    DynamicJsonDocument json_msg(512);
    DynamicJsonDocument json_data(512);

    json_data["temp"] = DHT.temperature;
    json_data["humi"] = DHT.humidity;

    json_msg["params"] = json_data;

    json_msg["vision"] = "1.0.0";

    serializeJson(json_msg,payload);

    //payload ="{ \"params\": {\"temp\": 30,\"huml\": 66},\"version\": \"1.0.0\"}";

    Serial.print("Sending message to topic: ");
    Serial.println(outTopic);
    Serial.println(payload);

    bool retained = false;
    int qos = 1;
    bool dup = false;

    mqttClient.beginMessage(outTopic, payload.length(), retained, qos, dup);
    mqttClient.print(payload);
    mqttClient.endMessage();

    Serial.println();

    count++;
  }
}

//{"deviceType":"CustomCategory","iotId":"fwJnX0wS4z4MbrzkyRlt000000","requestId":"1689847083906","checkFailedData":{},"productKey":"a1gfbOdKycU","gmtCreate":1689847087015,"deviceName":"app_dev","items":{"led":{"value":0,"time":1689847086996}}}

void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', duplicate = ");
  Serial.print(mqttClient.messageDup() ? "true" : "false");
  Serial.print(", QoS = ");
  Serial.print(mqttClient.messageQoS());
  Serial.print(", retained = ");
  Serial.print(mqttClient.messageRetain() ? "true" : "false");
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    char inChar =(char)mqttClient.read();
    inputString +=inChar;
    if(inputString.length()==messageSize) {

      DynamicJsonDocument json_msg(1024);
      DynamicJsonDocument json_item(1024);
      DynamicJsonDocument json_value(1024);

      deserializeJson(json_msg,inputString);

      String items = json_msg["items"];

      deserializeJson(json_item,items);
      String led = json_item["led"];

      deserializeJson(json_value,led);
      bool value = json_value["value"];

      if(value ==0) {
        //关
        Serial.println("off");
        //digitalWrite(4,HIGH);
        angle = 0;
        if (angle >= 0 && angle <= 180) { // 限制角度范围
            myServo.write(angle); // 设置舵机角度
            Serial.print("Servo moved to: ");
            Serial.println(angle);
        }
      } else {
        //开
        Serial.println("on");
        //digitalWrite(4,LOW);
        angle = 90;
        if (angle >= 0 && angle <= 180) { // 限制角度范围
            myServo.write(angle); // 设置舵机角度
            Serial.print("Servo moved to: ");
            Serial.println(angle);
        }
      }
      inputString="";
    }


  }
  Serial.println();

  Serial.println();
}
