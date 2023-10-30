#include<ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

//#include<WiFiClient.h>

//ESP8266WiFiMulti wifiMulti;
const char* ssid="ESP8266-Access-Point";
const char* password="123456789";

AsyncWebServer server(80);




const int leftPot=5;
const int rightPot=4;

int lIn=0;
int rIn=0;


void setup() {
  Serial.begin(9600);
  pinMode(leftPot,OUTPUT);
  pinMode(rightPot,OUTPUT);

  digitalWrite(leftPot,LOW);
  digitalWrite(rightPot,LOW);
  
 WiFi.softAP(ssid, password);
 IPAddress IP=WiFi.softAPIP();
 Serial.print("SoftAP IP Address: ");
 Serial.println(IP);

 server.on("/vl",HTTP_GET,[](AsyncWebServerRequest *request){
  request->send_P(200,"text/plain",String(lIn).c_str());
 });
 server.on("/vr",HTTP_GET,[](AsyncWebServerRequest *request){
  request->send_P(200,"text/plain",String(rIn).c_str());
 });

 server.begin();
}



void loop() {
  
 digitalWrite(leftPot,HIGH);
  lIn=analogRead(A0);
  digitalWrite(leftPot,LOW);
  delay(10);
  digitalWrite(rightPot,HIGH);
  rIn=analogRead(A0);
  digitalWrite(rightPot,LOW);
  delay(10);
  Serial.print(lIn);
  Serial.print("    ");
  Serial.println(rIn);
  
}
