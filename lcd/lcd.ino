#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Ubidots.h>

#define I2C_ADDR 0x27
#define BUTTON_PIN 15
LiquidCrystal_I2C lcd(I2C_ADDR, 16, 2);

const char* UBIDOTS_TOKEN = "BBFF-HBVruVizsOTaazKbHRo1VTxpMZxOWZ";  // Put here your Ubidots TOKEN

const char* WIFI_SSID = "SAVY";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "sathwik2627";      // Put here your Wi-Fi password

const char* DEVICE_LABEL = "nodemcu";  // Replace with your device label

const char* ubidots_host = "industrial.api.ubidots.com";
Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);
//function to read value from ubidots
int readFromUbidots(String variable_label) {
    int value = false;
    WiFiClient client;
    if (client.connect(ubidots_host, 80)) {
        client.print(String("GET /api/v1.6/devices/") + DEVICE_LABEL + "/" + variable_label + "/values/?token=" + UBIDOTS_TOKEN);
        client.println(" HTTP/1.1");
        client.println("Host: industrial.api.ubidots.com");
        client.println("Connection: close");
        client.println();

        while (client.connected()) {
            String line = client.readStringUntil('\n');
            if (line.indexOf("value") != -1) {
                DynamicJsonDocument doc(1024);
                deserializeJson(doc, line);
                value = doc["results"][0]["value"];
                break;
            }
        }
    }

    return value;
}
void setup() {
    Serial.begin(9600);
    lcd.init();
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("WiFi ....");

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi!");
    lcd.setCursor(0,0);
    lcd.print("Wifi Connected");
     pinMode(15, INPUT_PULLUP);
}



void loop() { 
  // Serial.println("loop");

  
    int distance = readFromUbidots("dist"); // 1 if parking slot1 is occupied else 0
    int distance2 = readFromUbidots("dist2");// 1 if parking slot2 is occupied else 0
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(distance);
    Serial.println(distance);
    lcd.print("   ");   
    lcd.print(distance2);
    Serial.println(distance2);
    delay(200);
  // put your main code here, to run repeatedly:

}
