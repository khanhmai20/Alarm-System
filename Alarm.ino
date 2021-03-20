#include <dht.h>
#include <LiquidCrystal.h>
#include <IRremote.h>  //including infrared remote header file

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

dht DHT;

#define DHT11_PIN 7
#define buzzer 13
int RECV_PIN = 10;

IRrecv irrecv(RECV_PIN);
decode_results results;  

int hour = 0;
int minute = 0; 
int sec = 0;
bool freeze = false;

void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(buzzer,OUTPUT); 
  irrecv.enableIRIn(); 
}

void loop(){
  /*At first clock is not freeze so freeze = false*/
  if (!freeze) {
    sec += 2;
    if (sec >= 60) { 
      minute++; 
      sec = 0;
    }
    if (minute >= 60) { 
      hour++; 
      minute = 0; 
    }
    if (hour >= 24) { 
      hour = 0; 
    } 
   
    if (irrecv.decode(&results)){
      int value = results.value;
      Serial.println(value);
      if (value == -16833) {
        hour++; 
      }else if(value == -7177) {
        minute++; 
      }else if(value == 539) { 
        sec++; 
      }
      irrecv.resume();
    }
  }
  
  /*Display the time on the lcd screen*/
  lcd.setCursor(0,0); 
  if (hour < 10) {
    lcd.print("0");
  }
  lcd.print(hour); 
  lcd.print(":");
  if(minute < 10) { 
    lcd.print("0");
  }
  lcd.print(minute); 
  lcd.print(":"); 
  if(sec < 10) { 
    lcd.print("0"); 
  }
  lcd.print(sec);

  /*Display the temperature infomation on the second line of the lcd*/
  int chk = DHT.read11(DHT11_PIN);
  lcd.setCursor(0,1); 
  lcd.print("T=");
  lcd.print(DHT.temperature);
  lcd.print((char)223);
  lcd.print(" H= ");
  lcd.print(DHT.humidity);

  /*buzzer emits sounds and freeze the clock*/
  if (DHT.humidity > 85) { 
    digitalWrite(buzzer,HIGH);
    freeze = true;
  }
  
  delay(2000);
}
