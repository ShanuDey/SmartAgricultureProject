/*
 * Assembled by Shanu Dey
 * Date : 5/10/18
 * Project Name : Smart Agriculture 
 */
#include<Servo.h>
#include<dht.h>
#include<LiquidCrystal.h>
#include<SoftwareSerial.h>
// function declaration
void servo(void);
void soilmoisture(void);
void pirsensor(void);
void smokesensor(void);
void humidityAndTemperature(void);
// initialize
LiquidCrystal lcd(12,11,4,5,6,7);
Servo servo1;
dht DHT;
SoftwareSerial mySerial(2,3);//Rx,Tx

//pin initialize
int soilpin=A0;
int smokepin=A1;
int LDR2=A2;
int LDR1=A3;
int humiditytemp_pin=A4;
const int buzzerpin=8;
const int pirpin =10;
int servopin=9;

//important vavriables
int initial_position=90;
int error=5;
int pirState = LOW;
int val =LOW;
int soil_data;
int smoke;
int temperature1;
int humidity1 ;
String str,str1;


void setup()
{
  pinMode(buzzerpin,OUTPUT);
  pinMode(pirpin,INPUT);
  pinMode(smokepin,INPUT);
  servo1.attach(servopin);
  pinMode(LDR1,INPUT);
  pinMode(LDR2,INPUT); 
  servo1.write(initial_position);
  pinMode(humidityAndTemperature,INPUT);
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  Serial.begin(9600);
  mySerial.begin(115200);
}
void loop()
{
  str1="";
  lcd.setCursor(0,0);
  lcd.scrollDisplayLeft();
  smokesensor();
  soilmoisture();
  servo();
  humidityAndTemperature();
  pirsensor(); 
  mySerial.println(str1);
  delay(3000);
}

void pirsensor(void) {
  val = digitalRead(pirpin); 
  if (val == HIGH) {            
    digitalWrite(buzzerpin, HIGH);  
    if (pirState == LOW) {
      Serial.print("Motion detected!");    
      pirState = HIGH;
    }
  } else {
    digitalWrite(buzzerpin, LOW); 
    if (pirState == HIGH) {
      Serial.print("Motion ended!");
      pirState = LOW;
    }
  }
}

void smokesensor(void)
{
  smoke=analogRead(smokepin);
  String str = " Smoke = "+String(smoke);//+" ppm ";
  String str1= String(smoke)+"s;";
  mySerial.print(str1);
  Serial.println(str);
  lcd.print(str);
}

void soilmoisture(void) {
   soil_data= analogRead(soilpin);
   soil_data = map(soil_data,0,1023,100,0);
   str= " Moisture = "+String(soil_data)+" % ";
   str1+= String(soil_data)+"m;";
   mySerial.print(str1);
   Serial.println(str);
   lcd.print(str);
  // delay(1000);
}

void servo(void) 
{ 
  int R1 = analogRead(LDR1);
  int R2 = analogRead(LDR2);
  int diff1= abs(R1 - R2);  
  int diff2= abs(R2 - R1);
  if((diff1 <= error) || (diff2 <= error)) {
   servo1.write(initial_position);
  } else {    
    if(R1 > R2)
    {
      initial_position = initial_position-3;
    }
    if(R1 < R2) 
    {
      initial_position = initial_position+3;
    }
  }
  servo1.write(initial_position); 
  lcd.setCursor(0,0);
  int tempo=1023-min(R1,R2);
  str =" Sun Intensity = "+String(tempo)+" cd ";
  str1+=String(tem6po)+"i;";
  mySerial.print(str1);
  Serial.println(str);
  lcd.print(str);
  Serial.println(R1);
  Serial.println(R2);
}

void humidityAndTemperature(void)
{
  int chk = DHT.read11(humiditytemp_pin);
  temperature1 = DHT.temperature;
  humidity1 = DHT.humidity;
  str = " Humidity = "+String(humidity1)+" % "+" Temperature = "+String(temperature1)+"degC";
  str1+= String(humidity1)+"h;"+String(temperature1)+"t;";
  mySerial.print(str1);
  lcd.setCursor(0,1);
  Serial.println(str);
  lcd.print(str);
}
