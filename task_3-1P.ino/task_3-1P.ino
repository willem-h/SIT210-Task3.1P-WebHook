#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SimpleDHT.h>

const char *WIFI_SSID = "<SSID>";
const char *WIFI_PASS = "<PASSWORD>";

String baseUrl = "https://api.thingspeak.com/update?api_key=<API_KEY>";

const int pinDHT22 = D1;
SimpleDHT22 dht22(pinDHT22);

BearSSL::WiFiClientSecure client;

void setup()
{
  Serial.begin(9600);
  connectToWifi();
}

void loop()
{
  delay(30000);
  
  byte tempRaw = 0;
  byte humdRaw = 0;
  dht22.read(&tempRaw, &humdRaw, NULL);
  String temperature = String(tempRaw);
  String humidity = String(humdRaw);
  String climateString = "Temperature: " + temperature + "°C. Humidity: " + humidity + "%.";
  Serial.println(climateString);
  sendData(temperature, humidity);
}

void connectToWifi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected!");
  Serial.printf("Network: %s\n", WIFI_SSID);
  Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
  client.setInsecure();
}

void sendData(String temperature, String humidity)
{
  HTTPClient http;
  String urlData = baseUrl + "&field1=" + temperature + "&field2=" + humidity;
  http.begin(client, urlData);
  http.GET();
  http.end();
}
