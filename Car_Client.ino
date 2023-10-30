#include<ESP8266WiFi.h>
#include<ESP8266WiFiMulti.h>
#include<ESP8266mDNS.h>
#include<ESP8266HTTPClient.h>

#include<WiFiClient.h>

ESP8266WiFiMulti wifiMulti;

const char* ssid="ESP8266-Access-Point";
const char* password="123456789";

const char* serverNameLeft="http://192.168.4.1/vl";
const char* serverNameRight="http://192.168.4.1/vr";

const int enA=5;
const int in1=4;
const int in2=0;
const int in3=2;
const int in4=14;
const int enB=12;

int vMult=254;
int vl=0;
int vr=0;
int lIn=500;
int rIn=500;

String lInStr;
String rInStr;

const long interval=20;
unsigned long prevMillis=0;

void setup() {
  Serial.begin(9600);
  pinMode(enA,OUTPUT); //set pins to mode and initialize
  pinMode(enB,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);

  digitalWrite(enA,LOW);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  digitalWrite(enB,LOW);
  
  delay(250);

  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected to WiFi Access Point");
}



void loop() {
  unsigned long currentMillis=millis();
  if(currentMillis-prevMillis>=interval){
  if(WiFi.status()==WL_CONNECTED){
    lInStr=httpGETRequest(serverNameLeft);
    rInStr=httpGETRequest(serverNameRight);
    
    lIn=lInStr.toInt();
    rIn=rInStr.toInt();
  }

  if(lIn>750){
    vl=(lIn-750)*vMult/145;
    analogWrite(enA,vl);
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  else if(lIn<650){
    vl=(((-1)*lIn)+650)*vMult/100;
    analogWrite(enA,vl);
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
  else{
    vl=0;
    analogWrite(enA,0);
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
  }
  if(rIn<700){
    vr=((-1)*(rIn-700))*vMult/86;
    analogWrite(enB,vr);
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
  }
  else if(rIn>800){
    vr=(rIn-800)*vMult/124;
    analogWrite(enB,vr);
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
  }
  else{
    vr=0;
    analogWrite(enB,0);
    digitalWrite(in3,LOW);
    digitalWrite(in4,LOW);
  }
  Serial.println("Left: " + String(vl) + "    Right: " + String(vr));
}
}

String httpGETRequest(const char* serverName){
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);

  int httpResponseCode=http.GET();

  String payload="--";

  if(httpResponseCode>0){
    
    //Serial.print("HTTP Resonse code: ");
    //Serial.println(httpResponseCode);
    payload=http.getString();
  }
  else{
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return payload;
}
