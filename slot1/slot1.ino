#include <Ubidots.h>
#include <ArduinoJson.h> 
const int trigPin = 2;  //D4
const int echoPin = 0;  //D3

long duration;
int distance;

const char* ubidots_host = "industrial.api.ubidots.com"; 
const char* UBIDOTS_TOKEN = "BBFF-HBVruVizsOTaazKbHRo1VTxpMZxOWZ";  // Put here your Ubidots TOKEN

const char* WIFI_SSID = "SAVY";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "sathwik2627";      // Put here your Wi-Fi password

const char* DEVICE_LABEL = "nodemcu";  // Replace with your device label

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);

void setup() {
     WiFi.begin(WIFI_SSID, WIFI_PASS);             // Connect to the network
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  Serial.println("Lets start");
}

void loop() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  // Serial.print("Distance: ");
  // Serial.println(distance);
  if(distance<10||distance>1800)
  {
    ubidots.add("dist", 4);
    Serial.println("Error..!! Please clean the sensor"); // when an object is placed very close to sensor it gives very large value as distance, hence it is assumed
    //dust like leaves are covering the sensor.
  }
  else if(distance<=50)
  { 
    //If the distance is less than 50 cm it means there exists a vechile in the parking slot
    ubidots.add("dist", 1);
    Serial.println(1);
    Serial.println(distance);
    
  }
  else
  {
    //If distance is greater than 50cm that means there isn't any vechile in the parking slot.
    ubidots.add("dist", 0);
    Serial.println(distance);
  }


  bool sent = false;

  sent = ubidots.send("nodemcu");

  if(sent){
    Serial.println("OK");
  }

  delay(500);

}
