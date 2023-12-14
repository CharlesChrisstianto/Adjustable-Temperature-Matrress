//Define Libary
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "CountDown.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

//Index CountDown timer
CountDown CD(CountDown::MINUTES); // jangan lupa ubah ke MINUTES dengan variable

//Index untuk Timer server
const long  utcOffsetInSeconds = 25200;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// Define Relay PIN
int relay_1 = 18;//Peltier
int relay_2 = 19; //Hot Pump
int relay_3 = 23; //Cold Pump
int relay_4 = 5;  //??

// Temperature Sensor PIN
#define ONE_WIRE_BUS 2 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensor(&oneWire);

float temp_0;
float temp_1;

//Define Push Button PIN
int pb_1 = 13; // 1
int pb_2 = 12; // 2
int pb_3 = 14; // 3
int pb_4 = 27; // 4

// untuk value pb interface
int ok = 0;
int up = 0;
int down = 0;
int back = 0;

int bt1 = 0;
int bt2 = 0;
int bt3 = 0;
int bt4 = 0;

//Starting Temperature Val for setting
int  settingTime_default = 15; //minutes
int  settingTime_user = 15; //minutes

//Starting Wakeup Timer
int wake_up_time_jam_default = 22;
int wake_up_time_min_default = 0;
int wake_up_time_jam_user = wake_up_time_jam_default;
int wake_up_time_min_user = wake_up_time_min_default;

const char* ssid = "BSQ"; //Password WIFI
const char* password = "Admin456";

char* level = "none"; //Level Temperature
int limit_temp;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 (SDA and SCL pin) for a 16 chars and 2 line display


void initWiFi() { //connect wifi fuction
  lcd.setCursor(0,0);
  lcd.print(" Connecting to Wifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  lcd.setCursor(0,1);
  lcd.print("     Succesful");
  Serial.println(WiFi.localIP());
}


void cold_mode(int settingTime_user){

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("      Cold Mode");
          
  lcd.setCursor(0,1);
  lcd.print("Time Left:");

  while (CD.remaining() > 0){
    digitalWrite(relay_1, HIGH);
    digitalWrite(relay_3, HIGH);
    lcd.setCursor(10,1);
    lcd.print(CD.remaining());
    lcd.print(" \'  ");
  }
    digitalWrite(relay_1, LOW);
    digitalWrite(relay_3, LOW);
}

void count_minutes(){ 
settingTime_user = settingTime_user - 1;
Serial.println("masuk");
//if(time_s<0){time_s=59; time_m = time_m-1;}
//if(time_m<0){time_m=59; time_h = time_h-1;}
}


// Menu Selection and Choice
void menu(){
menu: //Menu Awal Utama
          lcd.clear();
          while(1)
          {

          lcd.setCursor(0,0);
          lcd.print("      Main Menu");
          
          lcd.setCursor(0,1);
          lcd.print("1. Basic");
          lcd.setCursor(0,2);
          lcd.print("2. Timer");
          delay(100);
          
          bt1 = digitalRead(pb_1);
          bt2 = digitalRead(pb_2);
          bt3 = digitalRead(pb_3);
          bt4 = digitalRead(pb_4);
          
          if (bt1 == LOW) { delay(300); goto menu1; } // Ke Basic Mode
          if (bt2 == LOW) { delay(300); goto menu2; } // Ke Timer Mode
          if (bt3 == LOW) { delay(300);}
          if (bt4 == LOW) {  }
          }   


menu1: // Basic Mode
          lcd.clear();
          while(1)
          {
          
          lcd.setCursor(0,0);
          lcd.print("      Basic Mode");
          
          lcd.setCursor(0,1);
          lcd.print("1. Cold Mode");
          lcd.setCursor(0,2);
          lcd.print("2. Hot Mode");
          delay(100);

          bt1 = digitalRead(pb_1);
          bt2 = digitalRead(pb_2);
          bt3 = digitalRead(pb_3);
          bt4 = digitalRead(pb_4);

          if (bt1 == LOW) { delay(300); goto menu3; } // Ke Cold Mode
          if (bt2 == LOW) { delay(300); goto menu4; } // Ke Hot Mode
          if (bt3 == LOW) { } 
          if (bt4 == LOW) { delay(300); goto menu; } // KE Menu Utama
          }

menu2: // Timer Mode
          lcd.clear();
          
          while(1)
          {
          timeClient.update();
          lcd.setCursor(0,0);
          lcd.print("      Timer Mode");
          
          lcd.setCursor(0,1);
          lcd.print("Set Time:");
          lcd.print(wake_up_time_jam_user );
          lcd.print(":");
          lcd.print(wake_up_time_min_user);
          lcd.print("  ");
          lcd.setCursor(9,2);
           lcd.print(".");
          lcd.setCursor(0,3);
          lcd.print("Waktu:");
          lcd.print(timeClient.getHours());
          lcd.print(":");
          lcd.print(timeClient.getMinutes());
          lcd.print(":");
          lcd.print(timeClient.getSeconds()); //<-- ini utk masukin time nya
         
          ok = digitalRead(pb_1);
          up = digitalRead(pb_2);
          down = digitalRead(pb_3);
          back = digitalRead(pb_4);
          
          if (up == LOW) {//set jam
           delay(100); 
           
           if (up == LOW){
            wake_up_time_jam_user = wake_up_time_jam_user + 1;}
          }
          if (down == LOW) { 
             if (down == LOW &&  wake_up_time_jam_user > 0){
            wake_up_time_jam_user = wake_up_time_jam_user - 1; }
            }
          if (back == LOW) { delay(300); goto menu; }
          if (ok == LOW) { delay(300); goto menu8; } //KE Menu Utama
          }

menu3: // cold Mode

          lcd.clear();
          while(1)
          {

          lcd.setCursor(0,0);
          lcd.print("      Cold Mode");
          
          lcd.setCursor(0,1);
          lcd.print("Set Timer:");
          lcd.print(settingTime_user,0);
          lcd.print(" \'");
          lcd.print("<--");
          lcd.setCursor(0,2);
          lcd.print("Level:");
          lcd.print(level);
          delay(100);
          
          ok = digitalRead(pb_1);
          up = digitalRead(pb_2);
          down = digitalRead(pb_3);
          back = digitalRead(pb_4);
          
          if (ok == LOW) {delay(300); goto menu9; }
          if (up == LOW) { delay(50); settingTime_user = settingTime_user + 5;}
          if (down == LOW) { delay(50); if(settingTime_user > 5){ settingTime_user = settingTime_user - 5;} else{settingTime_user = 5;} }
          if (back == LOW) { delay(300); settingTime_user = settingTime_default; goto menu1;  }
          }



menu4: //Hot Mode
          lcd.clear();
          while(1)
          {

          lcd.setCursor(0,0);
          lcd.print("      Hot Mode");
          
          lcd.setCursor(0,1);
          lcd.print("Set Timer:");
          lcd.print(settingTime_user,0);
          lcd.print(" \'");
          lcd.print("<--");
          lcd.setCursor(0,2);
          lcd.print("Level:");
          lcd.print(level);
          delay(100);
          
          ok = digitalRead(pb_1);
          up = digitalRead(pb_2);
          down = digitalRead(pb_3);
          back = digitalRead(pb_4);

          if (ok == LOW) { delay(300); goto menu10; }
          if (up == LOW) { delay(50);   settingTime_user = settingTime_user + 5; }
          if (down == LOW) { delay(50); if(settingTime_user > 5){ settingTime_user = settingTime_user - 5;} else{settingTime_user = 5;} }
          if (back == LOW) { delay(300);  settingTime_user = settingTime_default; goto menu1;  }
          }


menu5: // Start Function Cold Mode
          
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("      Cold Mode");
          
          lcd.setCursor(0,1);
          lcd.print("Time Left:");
          lcd.setCursor(0,2);
          lcd.print("Temperature:");

          CD.start(settingTime_user);//CountDown Timer

          while (CD.remaining() >= 0){
            sensor.requestTemperaturesByIndex(0); // Send the command to get temperatures
            float temp_0 = sensor.getTempCByIndex(0);
            if ((temp_0 < (limit_temp + 2) && temp_0 > (limit_temp -2))|| (temp_0 < (limit_temp -2))){
              digitalWrite(relay_1, LOW);} //peltier tidak kerja
            else {
              digitalWrite(relay_1, HIGH);
            }
            digitalWrite(relay_3, HIGH);

            lcd.setCursor(10,1);
            lcd.print(CD.remaining());
            lcd.print(" \'  ");
            lcd.setCursor(12,2);
            lcd.print(temp_0, 1);
            Serial.println(temp_0, 1);

            back = digitalRead(pb_4);
            if (back == LOW) { 
              digitalWrite(relay_1, LOW);
              digitalWrite(relay_3, LOW);
              delay(300); goto menu; }
          }
            digitalWrite(relay_1, LOW);
            digitalWrite(relay_3, LOW);

          ok = digitalRead(pb_1);
          up = digitalRead(pb_2);
          down = digitalRead(pb_3);
          back = digitalRead(pb_4);
          if (ok == LOW) { delay(300);}
          if (up == LOW) { delay(50); lcd.setCursor(9,0);}
          if (down == LOW) { delay(50); lcd.setCursor(9,0);}
          if (back == LOW) { delay(300); goto menu4; }
          
      
menu6: // Start Function Hot Mode
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("      Hot Mode");
          
          lcd.setCursor(0,1);
          lcd.print("Time Left:");
          lcd.setCursor(0,2);
          lcd.print("Temperature:");

          CD.start(settingTime_user);//CountDown Timer

          while (CD.remaining() >= 0){
            sensor.requestTemperaturesByIndex(0); // Send the command to get temperatures
            float temp_0 = sensor.getTempCByIndex(0);
            if ((temp_0 < limit_temp + 2 && temp_0 > limit_temp -2)|| (temp_0 > limit_temp +2)){
              digitalWrite(relay_1, LOW);}
            else {
              digitalWrite(relay_1, HIGH);
            }
            digitalWrite(relay_2, HIGH);

            lcd.setCursor(10,1);
            lcd.print(CD.remaining());
            lcd.print(" \'  ");
            lcd.setCursor(12,2);
            lcd.print(temp_0, 1);
            Serial.println(temp_0, 1);
            
            back = digitalRead(pb_4);
            if (back == LOW) { 
              digitalWrite(relay_1, LOW);
              digitalWrite(relay_2, LOW);
              delay(300); goto menu; }
          }
            digitalWrite(relay_1, LOW);
            digitalWrite(relay_2, LOW);

          ok = digitalRead(pb_1);
          up = digitalRead(pb_2);
          down = digitalRead(pb_3);
          back = digitalRead(pb_4);
          if (ok == LOW) { delay(300);}
          if (up == LOW) { delay(50); lcd.setCursor(9,0);}
          if (down == LOW) { delay(50); lcd.setCursor(9,0);}
          if (back == LOW) { delay(300); goto menu4; }

menu7: // start timer mode
          
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("     WAKE UP!!!");
          
          lcd.setCursor(0,1);
          lcd.print("");
          digitalWrite(relay_1, LOW);
          digitalWrite(relay_3, LOW);
          


menu8: // Timer Mode
          lcd.clear();
          
          while(1)
          {
          timeClient.update();
          lcd.setCursor(0,0);
          lcd.print("      Timer Mode");
          
          lcd.setCursor(0,1);
          lcd.print("Set Time:");
          lcd.print(wake_up_time_jam_user );
          lcd.print(":");
          lcd.print(wake_up_time_min_user);
          lcd.print("  ");
          lcd.setCursor(11,2);
           lcd.print(".");
          lcd.setCursor(0,3);
          lcd.print("Clock:");
          lcd.print(timeClient.getHours());
          lcd.print(":");
          lcd.print(timeClient.getMinutes());
          lcd.print(":");
          lcd.print(timeClient.getSeconds()); //<-- ini utk masukin time nya
         
          ok = digitalRead(pb_1);
          up = digitalRead(pb_2);
          down = digitalRead(pb_3);
          back = digitalRead(pb_4);
          
          if (up == LOW) {//set jam
           delay(100); 
           
           if (up == LOW && wake_up_time_min_user < 60){
            wake_up_time_min_user = wake_up_time_min_user + 5;}
            else{
            wake_up_time_min_user = 0;
            wake_up_time_jam_user =  wake_up_time_jam_user + 1;
              
            }
          }
          if (down == LOW) { 
             if (down == LOW && wake_up_time_min_user >5 ){
            wake_up_time_min_user = wake_up_time_min_user - 5; }
            }
          if (back == LOW) { delay(300); goto menu2; }
          if (ok == LOW) { delay(300); 
            lcd.setCursor(14,3);
            lcd.print("Saved");
            delay(2000);
           goto menu; } //KE Menu Utama
          }

menu9: // cold Mode

          lcd.clear();
          while(1)
          {

          lcd.setCursor(0,0);
          lcd.print("      Cold Mode");
          
          lcd.setCursor(0,1);
          lcd.print("Set Timer:");
          lcd.print(settingTime_user,0);
          lcd.print(" \'  ");
          lcd.setCursor(0,2);
          lcd.print("Level:");
          lcd.print(level);
          lcd.print("  <--");
          delay(100);
          
          ok = digitalRead(pb_1);
          up = digitalRead(pb_2);
          down = digitalRead(pb_3);
          back = digitalRead(pb_4);
          
          if (ok == LOW) {delay(300); goto menu5; }
          if (up == LOW) { delay(50); level = "LOW"; limit_temp = 18; }
          if (down == LOW) { delay(50); level = "MID"; limit_temp = 20;}
          if (back == LOW) { delay(300); level = "HIGH"; limit_temp = 22; }
          }

menu10: // hot Mode

          lcd.clear();
          while(1)
          {

          lcd.setCursor(0,0);
          lcd.print("      Hot Mode");
          
          lcd.setCursor(0,1);
          lcd.print("Set Timer:");
          lcd.print(settingTime_user,0);
          lcd.print(" \'  ");
          lcd.setCursor(0,2);
          lcd.print("Level:");
          lcd.print(level);
          lcd.print("  <--");
          delay(100);
          
          ok = digitalRead(pb_1);
          up = digitalRead(pb_2);
          down = digitalRead(pb_3);
          back = digitalRead(pb_4);
          
          if (ok == LOW) {delay(300); goto menu6; }
          if (up == LOW) { delay(50); level = "LOW"; limit_temp = 26; }
          if (down == LOW) { delay(50); level = "MID"; limit_temp = 28;}
          if (back == LOW) { delay(300); level = "HIGH"; limit_temp = 30; }
          }


}

void setup() {

  Serial.begin(115200);

  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);

  // LCD Display
  pinMode(pb_1,INPUT_PULLUP);
  pinMode(pb_2,INPUT_PULLUP);
  pinMode(pb_3,INPUT_PULLUP);
  pinMode(pb_4,INPUT_PULLUP); 
  
  sensor.begin();
  lcd.init();                      // initialize the lcd 
  lcd.backlight(); 
  initWiFi();
  timeClient.begin();

  delay(1000);
  menu();

  
  

}


void loop() {
  // put your main code here, to run repeatedly:

}
