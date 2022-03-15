#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <ESP8266WebServer.h>
ESP8266WebServer webserver(80);
LiquidCrystal_I2C lcd(0x27,16,2);

#define Led 13
#define transistor 12
#define DHTPIN 2 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define buzzer 15
#define FIREBASE_HOST "temperature-reader-8a671-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "mrdaMWZliziCywyyuh6MG9folgOuj1qYzGnjPehY"
float t;
float h;
String str;
bool buzzerState =LOW;

unsigned long lasTime = 0;
unsigned long lasTime_1 = 0;
const long interval = 5000;
const long interval_1 = 1000;
unsigned long currenTime;
unsigned long currenTime_1;


FirebaseData firebaseData;
FirebaseData loadData;
void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.init();
  lcd.backlight();
  dht.begin();
  lcd.init();  
  Serial.begin(115200);
  pinMode (Led,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(transistor, OUTPUT);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin("8pxX2", "coldmineral1");
  while(WiFi.status() !=WL_CONNECTED){
    digitalWrite(Led,LOW);
    Serial.print(".");
    lcd.clear();
    lcd.print("Connecting to");
    lcd.setCursor(0,1);
    lcd.print("Mobile Hotspot");
    //delay(500);
}
Serial.println();
Serial.println("Connected");
digitalWrite(Led,HIGH);
lcd.clear();
lcd.print("Connected to");
lcd.setCursor(0,1);
lcd.print("8pxX2");
//delay(1000);
}
void loop() {
  t = dht.readTemperature();
  h = dht.readHumidity();
  lcd.clear();
  lcdprint();
  str = String("Temp= ")+String(t)+String(" Humidity= ")+String(h);
  Serial.println(str);
 if(t>=45){
  digitalWrite(transistor, HIGH);
  currenTime_1 = millis();
  if (currenTime_1 - lasTime_1 >= interval_1){
      lasTime_1 = currenTime_1;
        
      if(buzzerState==LOW){
          buzzerState=HIGH;
         }
      else{
          buzzerState = LOW;
          }
      digitalWrite(buzzer, buzzerState);
    }
  }
  else{
  digitalWrite(transistor, LOW);
  digitalWrite(buzzer, LOW);
 }
 currenTime = millis();
 if(currenTime - lasTime >= interval){
    lasTime = currenTime;
    lcdprint();  
    Firebase.setString(firebaseData,"Environment",str);
 } 
}   
void lcdprint(){
   
  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.setCursor(5,0);
  lcd.print(t);
  lcd.setCursor(0,1);
  lcd.print("Humidity:");
  lcd.setCursor(9,1);
  lcd.print(h);
}

