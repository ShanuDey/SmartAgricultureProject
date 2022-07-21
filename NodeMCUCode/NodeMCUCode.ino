/*
 * author : Shanu Dey
 * project : Smart Agriculture
 * Date : 5/10/18
 * 
*/
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include<SoftwareSerial.h>

// Set these to run example.
#define FIREBASE_HOST "firebase host name" //enter firebase host
#define FIREBASE_AUTH "firebase database secreat" // enter firebase database auth
#define WIFI_SSID "SSID" // enter your wifi ssid
#define WIFI_PASSWORD "password" //enter your wifi password

String uid = "user id "; // enter firebase auth logged in one user id
SoftwareSerial mySerial(13, 15); //Rx,Tx

const int pumpPin = 16;//gpio16 -> D0
int humi, temp,smoke,soil,sun;
String str,s;
char c;

void setup() {
  Serial.begin(9600);
  mySerial.begin(115200);

  pinMode(pumpPin, OUTPUT);


  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  Firebase.set(uid + "/sensor/humidity", 0);
  Firebase.set(uid + "/sensor/temperature", 0);
  Firebase.set(uid + "/sensor/smoke", 0);
  Firebase.set(uid + "/sensor/soilmoisture", 0);
  Firebase.set(uid + "/sensor/sunlightintensity", 0);
  
}



void loop() {
  //serial code  
  str="";
  s="";
  while (mySerial.available()) {
    c = mySerial.read();
    str.concat(c);
  }
  Serial.println(str);
  for(int i=0;str[i]!='\0';i++){
    if(str[i]==';'){
      /*
        * special note
        * h=humidity
        * t=temperature
        * m=moisture
        * s=smoke
        * i=intensity
        * pin 3 of ArduinoUNO connect to D7(GPIO 13) of NodeMCU
        */
      switch(str[i-1]){
        case 's':
          smoke = s.toInt();
          break;
        case 'h':
          humi = s.toInt();
          break;
        case 'm':
           soil= s.toInt();
           break;
        case 't':
           temp = s.toInt();
           break;
        case 'i':
           sun = s.toInt();
           break;
        default:
          Serial.print("Invalid char");
      }
      s="";
    }
    else{
       s.concat(str[i]);
    }
  }
  //s = "soil="+String(soil)+"temp="+String(temp)+"humi="+String(humi)+"sun="+String(sun)+"Smoke="+String(smoke);
  //Serial.println(s);
  //delay(1000);
  
  //set value
  digitalWrite(pumpPin, Firebase.getInt(uid + "/sensor/pump"));
  Firebase.setInt(uid + "/sensor/humidity", humi);
  Firebase.setInt(uid + "/sensor/temperature", temp);
  Firebase.setInt(uid + "/sensor/smoke", smoke);
  Firebase.setInt(uid + "/sensor/soilmoisture", soil);
  Firebase.setInt(uid + "/sensor/sunlightintensity",sun);

 /* 
  *  uncomment it with calibration of your sensors
  *  if(soil<50 || humi<60 || temp>40){
    Firebase.setInt(uid + "/sensor/pump", 1);
  }
  else{
    Firebase.setInt(uid + "/sensor/pump",0);
  }
 */

}


