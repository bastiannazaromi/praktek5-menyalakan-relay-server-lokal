// Wifi
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
// Buat object Wifi
ESP8266WiFiMulti WiFiMulti;
// Buat object http
HTTPClient http;
#define USE_SERIAL Serial

String urlGetRelay = "http://http://192.168.21.186/praktek5-menyalakan-relay-server-lokal/data/getRelay";

String statusRelay = "";

#define WIFI_SSID "Bas"
#define WIFI_PASSWORD "12345678"

#define relay_off HIGH
#define relay_on LOW

#define kipas D5

void setup() {
  Serial.begin(115200);

  USE_SERIAL.begin(115200);
  USE_SERIAL.setDebugOutput(false);

  for(uint8_t t = 4; t > 0; t--) {
      USE_SERIAL.printf("[SETUP] Tunggu %d...\n", t);
      USE_SERIAL.flush();
      delay(1000);
  } 

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  for (int u = 1; u <= 5; u++)
  {
    if ((WiFiMulti.run() == WL_CONNECTED))
    {
      USE_SERIAL.println("Terhubung ke wifi");
      USE_SERIAL.flush();
      delay(1000);
    }
    else
    {
      Serial.println("Wifi belum terhubung");
      delay(1000);
    }
  }
  
  pinMode(kipas, OUTPUT);
  digitalWrite(kipas, relay_off);
}

void loop() {
  ambil_data();

  if (statusRelay == "ON") {
    Serial.println("Kipas Menyala");
    digitalWrite(kipas, relay_on);
  } else {
    Serial.println("Kipas Mati");
    digitalWrite(kipas, relay_off);
  }

  delay(1000);
}

void ambil_data() {
  if ((WiFiMulti.run() == WL_CONNECTED))
  {
    USE_SERIAL.print("[HTTP] Memulai...\n");
    
    http.begin( urlGetRelay );
    
    USE_SERIAL.print("[HTTP] Ambil data dari database ...\n");
    int httpCode = http.GET();

    if(httpCode > 0)
    {
      USE_SERIAL.printf("[HTTP] kode response GET : %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) // kode 200 
      {
        statusRelay = http.getString();
        USE_SERIAL.println("Status Relay : " + statusRelay);
        delay(200);
      }
    }
    else
    {
      USE_SERIAL.printf("[HTTP] GET data gagal, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
}
