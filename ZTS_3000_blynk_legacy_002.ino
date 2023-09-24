/*
Libraries
1. Blynk : https://downloads.arduino.cc/libraries/github.com/blynkkk/Blynk-1.0.1.zip
ใช้กับ Blynk libraries ต่ำกว่า 1.0.1 เท่านั้น
2. Simple Timer : https://github.com/jfturcot/SimpleTimer/archive/refs/heads/master.zip
3. ModbusMaster : https://github.com/4-20ma/ModbusMaster/archive/refs/heads/master.zip
*/
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
bool fetch_blynk_state = true;  //true or false

char ssid[] = "";                            //ชื่อ SSID ที่เราต้องการเชื่อมต่อ
char pass[] = "";                        //รหัสผ่าน WI-FI
char auth[] = "";  //Auth token from blynk app

//=============Simple Timer=============//
#include <SimpleTimer.h>
SimpleTimer timer;
//=============Simple Timer=============//
//============Modbus Master=============//
#include <ModbusMaster.h>
#define RX2 16  //RX
#define TX2 17  //TX
//============Modbus Master=============//
//=============Modbus Object============//
ModbusMaster node1;  //ZTS-3000 Slave ID1
//=============Modbus Object============//

//=========IO connect to relay==========//
#define relay1 25
#define relay2 33
#define relay3 32
#define relay4 23
#define ledpin  2
//=========IO connect to relay==========//

//===========Setup Function=============//
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);  //RX2=16 ,TX2=17
  node1.begin(1, Serial2);                    //XY-MD02 Slave ID1
  node2.begin(2, Serial2);                    //XY-MD02 Slave ID2

  //Set Mode
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(ledpin, OUTPUT);
  //Set default status
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
  digitalWrite(ledpin, LOW);

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  delay(500);

  Blynk.begin(auth, ssid, pass, "iotservices.thddns.net", 5536);

  timer.setInterval(1000, zts01);
}
//===========Setup Function=============//

//===============ZTS-3000===============//
void zts01() {
  uint8_t result1;
  float humi1 = (node1.getResponseBuffer(0) / 10.0f);
  float temp1 = (node1.getResponseBuffer(1) / 10.0f);

  Serial.println("Get ZTS-3000 Data1");
  result1 = node1.readInputRegisters(0x0000, 2);  // Read 2 registers starting at 1)
  if (result1 == node1.ku8MBSuccess) {
    Serial.print("Humi1: ");
    Serial.println(node1.getResponseBuffer(0) / 10.0f);
    Serial.print("Temp1: ");
    Serial.println(node1.getResponseBuffer(1) / 10.0f);
  }
  delay(1000);
  Blynk.virtualWrite(V5, humi1);
  Blynk.virtualWrite(V6, temp1);
}
//===============ZTS-3000===============//

//=================Blynk Connected==================//
BLYNK_CONNECTED() {
  // Request the latest state from the server
  if (fetch_blynk_state) {
    digitalWrite(ledpin, HIGH);
    Blynk.syncAll();
  }
}
//=================Blynk Connected==================//

//=============Loop Function============//
void loop() {
  timer.run();
}
//=============Loop Function============//

//==========BTN Blynk ON-OFF Relay1=============//
BLYNK_WRITE(V1) {
  int valuebtn1 = param.asInt();
  if (valuebtn1 == 1) {
    digitalWrite(relay1, HIGH);
  } else {
    digitalWrite(relay1, LOW);
  }
}
//==========BTN Blynk ON-OFF Relay1============//

//==========BTN Blynk ON-OFF Relay2============//
BLYNK_WRITE(V2) {
  int valuebtn2 = param.asInt();
  if (valuebtn2 == 1) {
    digitalWrite(relay2, HIGH);
  } else {
    digitalWrite(relay2, LOW);
  }
}
//==========BTN Blynk ON-OFF Relay2============//

//==========BTN Blynk ON-OFF Relay3============//
BLYNK_WRITE(V3) {
  int valuebtn3 = param.asInt();
  if (valuebtn3 == 1) {
    digitalWrite(relay3, HIGH);
  } else {
    digitalWrite(relay3, LOW);
  }
}
//==========BTN Blynk ON-OFF Relay3============//

//==========BTN Blynk ON-OFF Relay4============//
BLYNK_WRITE(V4) {
  int valuebtn4 = param.asInt();
  if (valuebtn4 == 1) {
    digitalWrite(relay4, HIGH);
  } else {
    digitalWrite(relay4, LOW);
  }
}
//==========BTN Blynk ON-OFF Relay4===========//
