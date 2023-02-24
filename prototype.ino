#include <LiquidCrystal.h>
#include <Keypad.h>
#include <OneWire.h>
#include <DallasTemperature.h>

bool coffee_heat_ele = false;
bool water_heat_ele = false;
bool milk_heat_ele = false;

#include "SR04.h"
#define TRIG_PIN_c 30
#define ECHO_PIN_c 31
SR04 sr04_coffee = SR04(ECHO_PIN_c,TRIG_PIN_c);

#define TRIG_PIN_m 14
#define ECHO_PIN_m 15
SR04 sr04_milk = SR04(ECHO_PIN_m,TRIG_PIN_m);

#define TRIG_PIN_w 16
#define ECHO_PIN_w 17
SR04 sr04_water = SR04(ECHO_PIN_w,TRIG_PIN_w);

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#define TEMPERATURE_PRECISION 10

bool milk_tank_heat = false;
bool water_tank_heat = false;
bool coffee_tank_heat = false;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {29, 28, 27, 26}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {25, 24, 23, 22}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int heat_mode_index = 0;
float coffee_tank_temp;
float coffee_tank_vol;
float water_tank_temp;
float water_tank_vol;
float milk_tank_temp;
float milk_tank_vol;

float milk_target_temp = 65;
float coffee_target_temp = 70;
int input_time;

int page_index = 0;
bool newpage;
bool not_writing = true;
int temp_input = 0;
int heat_time = 0;

int keypad_input;
char Keypad_press;

void page_director () {
  if (page_index == 0) {
    LCD_mainpage();
  }
  else if (page_index == 1000) {
    LCD_info_subpage();
  }
  else if (page_index == 2000) {
    LCD_mode_subpage();
  }
  else if (page_index == 3000) {
    LCD_start_subpage();
  }
  else if (page_index == 4000) {
    LCD_time_subpage();
  }
  else if (page_index == 1100) {
    water_info_subpage();
  }
  else if (page_index == 1200) {
    coffee_info_subpage();
  }
  else if (page_index == 1300) {
    milk_info_subpage();
  }
  else if (page_index == 1400) {
    auto_info_subpage();
  }

  else if (page_index == 2300) {
    mode_man_subpage();
  }
  else if (page_index == 2310) {
    mode_man_TEMP_subpage();
  }
  else if (page_index == 2311) {
    mode_man_TEMP_Milk_subpage();
  }
  else if (page_index == 2312) {
    mode_man_TEMP_Coffee_subpage();
  }
}

void LCD_mainpage() {
  if (newpage) {
    lcd.clear();
    newpage = false;
  }
  lcd.setCursor(0, 0);
  lcd.print("1.Info");
  lcd.setCursor(8, 0);
  lcd.print("2.Mode");
  lcd.setCursor(0, 1);
  lcd.print("3.Start");
  lcd.setCursor(8, 1);
  lcd.print("4.Time");
}

void LCD_info_subpage() {
  if (newpage) {
    lcd.clear();
    newpage = false;
  }
  lcd.setCursor(0, 0);
  lcd.setCursor(0, 0);
  lcd.print("1.Water");
  lcd.setCursor(8, 0);
  lcd.print("2.Coffee");
  lcd.setCursor(0, 1);
  lcd.print("3.Milk");
  lcd.setCursor(8, 1);
  lcd.print("4.Auto");
}

void water_info_subpage() {
  if (newpage) {
    lcd.clear();
    newpage = false;
  }
  lcd.setCursor(0, 0);
  lcd.print("Water Tank");
  lcd.setCursor(0, 1);
  lcd.print(water_tank_temp, 1);
  lcd.print("C");
  lcd.setCursor(8, 1);
  lcd.print(water_tank_vol, 1);
  lcd.print("%");
}

void coffee_info_subpage() {
  if (newpage) {
    lcd.clear();
    newpage = false;
  }
  lcd.setCursor(0, 0);
  lcd.print("Coffee Tank");
  lcd.setCursor(0, 1);
  lcd.print(coffee_tank_temp, 1);
  lcd.print("C");
  lcd.setCursor(8, 1);
  lcd.print(coffee_tank_vol, 1);
  lcd.print("%");
}

void milk_info_subpage() {
  if (newpage) {
    lcd.clear();
    newpage = false;
  }

  lcd.setCursor(0, 0);
  lcd.print("Milk Tank");
  lcd.setCursor(0, 1);
  lcd.print(milk_tank_temp, 1);
  lcd.print("C");
  lcd.setCursor(8, 1);
  lcd.print(milk_tank_vol, 1);
  lcd.print("%");
}

void auto_info_subpage() {
  if (newpage) {
    lcd.clear();
    newpage = false;
  }
  lcd.setCursor(0, 0);
  lcd.print("AutoDisplay");
  lcd.setCursor(0, 1);
  lcd.print("Placeholder");
}

void LCD_mode_subpage() {
  if (newpage) {
    lcd.clear();
    newpage = false;
  }
  lcd.setCursor(0, 0);
  lcd.print("1.Espresso");
  lcd.setCursor(0, 1);
  lcd.print("2.Mixture");
  lcd.setCursor(10, 1);
  lcd.print("3.Man");
}

void mode_man_subpage() {
  if (newpage) {
    lcd.clear();
    newpage = false;
  }
  lcd.setCursor(0, 0);
  lcd.print("1.Target Temp");
  lcd.setCursor(0, 1);
  lcd.print("2.Mixture Ratio");
}



void mode_man_TEMP_subpage() {
  if (newpage) {
    lcd.clear();
    newpage = false;
  }
  lcd.setCursor(0, 0);
  lcd.print("1.Milk Temp");
  lcd.setCursor(0, 1);
  lcd.print("2.Coffee TEMP");
}

void mode_man_TEMP_Milk_subpage() {
  if (newpage) {
    lcd.clear();
    newpage = false;
  }
  input_time = millis();
  lcd.setCursor(0, 0);
  lcd.print("Old TEMP:");
  lcd.setCursor(10, 0);
  lcd.print(milk_target_temp, 1);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("New TEMP:");
  lcd.blink();
  float a = 0;
  char t = 0;
  while ((millis() < input_time + 15000) && temp_input < 3  ) {

    if (temp_input == 0) {
      lcd.setCursor(10,1);
      lcd.blink();
      while (!t && millis() < input_time + 15000) {
        t = customKeypad.getKey();
      }
      keypad_input = keypad_translate (t);
      a = a + keypad_input * 10;
      lcd.setCursor(10, 1);
      lcd.print(keypad_input);
      temp_input++;
      t = 0;
    }
    else if (temp_input == 1 ) {
      lcd.setCursor(11,1);
      lcd.blink();
      while (!t && millis() < input_time + 15000) {
        t = customKeypad.getKey();
      }
      keypad_input = keypad_translate (t);
      a = a + keypad_input;
      lcd.print(keypad_input);
      temp_input++;
      keypad_input = 10;
      t = 0;
      lcd.setCursor(12, 1);
      lcd.print(".");
      lcd.setCursor(13, 1);
      lcd.blink();
    }
    else if (temp_input == 2 ) {
      while (!t && millis() < input_time + 15000) {
        t = customKeypad.getKey();
      }

      keypad_input = keypad_translate (t);
      a = a + keypad_input * 0.1;
      lcd.print(keypad_input);
      temp_input++;
    }
  }
  lcd.noBlink();
  temp_input = 0;
  not_writing = true;
  milk_target_temp = a;
  a = 0;
  page_index = 2300;
  newpage = true;
}

void mode_man_TEMP_Coffee_subpage() {
  if (newpage) {
    lcd.clear();
    newpage = false;
  }
  //
}

void LCD_start_subpage() {
  if (newpage) {
    lcd.clear();
    newpage = false;
  }
  lcd.setCursor(0, 0);
  lcd.print("1.Cook Coffee");
  lcd.setCursor(0, 2);
  lcd.print("2.Warm Milk");
  digitalWrite(7,HIGH);
  Serial.println("Pump On");
}

void LCD_start_coffee_subpage() {
  if (newpage) {
    lcd.clear();
    newpage = false;
  }
  lcd.setCursor(0, 0);
  lcd.print("Start Cooking");
  water_heat_ele = true;
  milk_heat_ele = true;
}

void LCD_time_subpage() {
  if (newpage) {
    lcd.clear();
    newpage = false;
  }
  lcd.setCursor(0, 0);
  lcd.print("Time Placeholder");
}

void keypad_in () {
  char customKey = customKeypad.getKey();
  if (customKey) {
    Keypad_press = customKey;
  }
}


int keypad_translate (char a) {
  int b;

  if (a == '1') {
    b = 1;
  }
  else if (a == '2') {
    b = 2;
  }
  else if (a == '3') {
    b = 3;
  }
  else if (a == '4') {
    b = 4;
  }
  else if (a == '5') {
    b = 5;
  }
  else if (a == '6') {
    b = 6;
  }
  else if (a == '7') {
    b = 7;
  }
  else if (a == '8') {
    b = 8;
  }
  else if (a == '9') {
    b = 9;
  }
  else if (a == '0') {
    b = 0;
  }
  return b;
}

void page_redirector () {
  if (customKeypad.getState() == RELEASED && not_writing) {
    if (page_index == 0 && keypad_input < 5 && keypad_input > 0) {
      page_index = keypad_input * 1000;
    }
    else if (page_index == page_index / 1000 * 1000 && (keypad_input < 5 && keypad_input > 0)) {
      page_index = page_index + keypad_input * 100;
    }
    else if (page_index == page_index / 100 * 100 && (keypad_input < 5 && keypad_input > 0)) {
      page_index = page_index + keypad_input * 10;
    }
    else if (page_index == page_index / 10 * 10 && (keypad_input < 5 && keypad_input > 0)) {
      page_index = page_index + keypad_input;
    }
    else if (keypad_input == 0) {
      if (page_index == page_index / 1000 * 1000) {
        page_index = 0;
      }
      if (page_index == page_index / 100 * 100) {
        page_index = page_index / 1000 * 1000;
      }
      if (page_index == page_index / 10 * 10) {
        page_index = page_index / 100 * 100;
      }
      else {
        page_index = page_index / 10 * 10;
      }
    }
    newpage = true;
  }
  delay(50);
}

void user_interact() {
  page_director ();
  keypad_in ();
  keypad_input = keypad_translate (Keypad_press);
  page_redirector ();
}

bool water_temp_time = false;


void TempManagement() {
  sensors.requestTemperatures();
  coffee_tank_temp = sensors.getTempCByIndex(0);
//  water_tank_temp = sensors.getTempCByIndex(1);
//  milk_tank_temp = sensors.getTempCByIndex(2);

  if (coffee_tank_temp > coffee_target_temp + 3 ) {
    coffee_tank_heat = false;
  }
  if (milk_tank_temp > milk_target_temp + 3 ) {
    milk_tank_heat = false;
  }

  if (water_tank_temp > 95 && !water_temp_time){
    heat_time = millis();
    water_temp_time = false;
  }
  
  if (coffee_tank_temp > 95 && millis() > heat_time + 60000 ) {
    water_tank_heat = false;
    pinMode(7,HIGH);
    Serial.println("Pump On");
  }
  if (water_tank_temp < 75){
    water_temp_time = false;
  }
}

void cook () {
  TempManagement();
  if (coffee_tank_heat) {
    pinMode(coffee_heat_ele,HIGH);
  }
  if (milk_tank_heat) {
    pinMode(milk_heat_ele,HIGH);
  }
  if (coffee_tank_heat) {
    pinMode(coffee_heat_ele,HIGH);
  }
}

float vol_cal(float k) {
  float a = (14-k)/14*100;
  return a;
}

void vol() {
  coffee_tank_vol = vol_cal(sr04_coffee.Distance());
  milk_tank_vol = vol_cal(sr04_milk.Distance());
  water_tank_vol = vol_cal(sr04_water.Distance());
}

void liq_out() {
  pinMode(31,HIGH);
  pinMode(32,HIGH);
}

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  sensors.begin();
  
}

void loop() {
  user_interact();
//  TempManagement();
//  vol();
//Serial.println(coffee_tank_temp);
}
