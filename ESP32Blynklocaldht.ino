#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "";      //ชื่อ SSID ที่เราต้องการเชื่อมต่อ
char pass[] = "";  //รหัสผ่าน WI-FI
char auth[] = "";            //Auth token from blynk app

#include <SimpleTimer.h>
SimpleTimer timer;

//dht11,dht22
#include <DHT.h>  //https://github.com/adafruit/DHT-sensor-library
#define DHTPIN 18
#define DHTTYPE DHT11
//#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10);

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

  dht.begin();

  //Connect to Blynk Server
  //Blynk.begin(auth, ssid, pass, "192.168.1.135", 8080);
  Blynk.begin(auth, ssid, pass, "ip blynk server", hw-port);
  timer.setInterval(1000L, dht);
}

void loop() {
  Blynk.run();
  timer.run();
}

//DHT
void dht() {
 float h = dht.readHumidity();
  float t = dht.readTemperature();      // Read temperature as Celsius (the default)
  float f = dht.readTemperature(true);  // Read temperature as Fahrenheit (isFahrenheit = true)
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);         // Compute heat index in Fahrenheit (the default)
  float hic = dht.computeHeatIndex(t, h, false);  // Compute heat index in Celsius (isFahreheit = false)
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
}
