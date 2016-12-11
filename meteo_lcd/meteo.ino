#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 8
#define DHTTYPE DHT11
#define PRINT_TIME 5000

LiquidCrystal lcd(7,6,5,4,3,2);

DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(9600);

  initDHT();
  
  lcd.begin(16,2);
}

void loop() {

  printTemperature();
  delay(PRINT_TIME);
  lcd.clear();
  printHumidity();
  delay(PRINT_TIME);
  lcd.clear();
}

void printTemperature(){

  lcd.setCursor(0, 0);
  lcd.print("Temperature: ");
  lcd.setCursor(0, 1);
  lcd.print(getTemperature());
  printDegree();
}

void printHumidity(){

  lcd.print("Humidity: ");
  lcd.setCursor(0, 1);
  lcd.print(getHumidity());
  lcd.setCursor(6, 1);
  lcd.write("%");
}

void printDegree(){

  byte degree[8] = {
  0b11100,
  0b10100,
  0b11100,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
  };

  lcd.createChar(0, degree);
  lcd.setCursor(6, 1);
  lcd.write((uint8_t)0);
  lcd.write("C");
}

void initDHT(){

  dht.begin();
}

float getTemperature(){

  float t = dht.readTemperature();

  if (isnan(t)) {
    
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");

  return t;
}

float getHumidity(){

  float h = dht.readHumidity();

  if (isnan(h)) {
    
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %\t");

  return h;
}