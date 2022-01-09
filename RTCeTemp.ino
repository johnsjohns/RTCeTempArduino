#include <LiquidCrystal.h>
#include <DHT.h>
#include <Wire.h>
#include "RTClib.h"

//pinos utilizados:
#define LCD_BOTAO A0
// pinos A4 e A5 são usados para o modulo RTC
#define DHTPIN 3 // pino de DATA do sensor de temperatura
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LCD_RS 8
#define LCD_ENABLE 9
#define LCD_BLT 10
/********************/
#define DHTTYPE DHT22


LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7); // 
int analogPin = A0; // pino de leitura dos botoes
DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc; //Os pinos SDA e SCL do modulo são conectados nos pinos nalógicos A4 e A5,
boolean verifica = false;
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sabado"};
boolean luz = true;

void setup() {
  pinMode(LCD_BLT, OUTPUT);
  digitalWrite(LCD_BLT, HIGH);
  lcd.begin(16, 2);
  dht.begin();
  delay(2000);
  Wire.begin();
  rtc.begin();
  if (! rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime tstamp = rtc.now();
  int x = analogRead(LCD_BOTAO);
  if(x < 700 && x > 600){
    verifica = !verifica;
  }
  
  if(x<100 && x > 0){
    if(luz){
      digitalWrite(LCD_BLT, LOW);
      luz = false;
      delay(500);
    } else { 
      digitalWrite(LCD_BLT, HIGH);
      luz = true;
      delay(500);
    }
  }
  
  if(verifica){
    menuTemp(tstamp);
  } else {
    menuData(tstamp);
  }
  delay(100);
  lcd.clear();
}

void printFormat(int digito){
  String digitos = String(digito);
  if (digitos.length() == 1) {
    digitos = "0" + digitos;
  }
  lcd.print(digitos);
}

void menuData(DateTime t){
  lcd.setCursor(0, 0);
  printFormat(t.hour());
  lcd.print(":");
  printFormat(t.minute());
  lcd.print(":");
  printFormat(t.second());
  lcd.print(" ");
  lcd.print(daysOfTheWeek[t.dayOfTheWeek()]);
  lcd.setCursor(0, 1);
  printFormat(t.day());
  lcd.print("/");
  printFormat(t.month());
  lcd.print("/");
  printFormat(t.year());
}

void menuTemp(DateTime t){
  lcd.setCursor(0, 0);
  lcd.print("U:");
  lcd.print(dht.readHumidity());
  lcd.print("% ");
  printFormat(t.hour());
  lcd.print(":");
  printFormat(t.minute());
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(dht.readTemperature()); 
  lcd.print("C ");
  printFormat(t.day());
  lcd.print("/");
  printFormat(t.month());
}
