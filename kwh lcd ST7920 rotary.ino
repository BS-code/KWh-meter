#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PZEM004T.h>
#include <LCD12864_ST7920.h>
#include <Rotary.h>

LCD12864_ST7920 lcd(14, 15, 16, 17, 18, 19);
Rotary encoder(2, 3);

#define RX_PIN 4
#define TX_PIN 5

SoftwareSerial pzemSerial(RX_PIN, TX_PIN);
PZEM004T pzem(&pzemSerial);

float voltage = 0;
float current = 0;
float powerFactor = 0;
float frequency = 0;
float energy = 0;
float cost = 0;
float tariff = 0.15;

int rotaryValue = 0;
int prevRotaryValue = 0;

void setup() {
  Serial.begin(9600);
  pzemSerial.begin(9600);
  lcd.init();
  lcd.clear();
}

void loop() {
  voltage = pzem.voltage();
  current = pzem.current();
  powerFactor = pzem.powerFactor();
  frequency = pzem.frequency();
  energy = pzem.energy();
  cost = energy * tariff;

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Tegangan (V): ");
  lcd.setCursor(0, 1);
  lcd.print(voltage);

  lcd.setCursor(0,3);
  lcd.print("Arus (A): ");
  lcd.setCursor(0, 4);
  lcd.print(current);

  checkRotaryEncoder();

  delay(2000);

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Faktor Daya: ");
  lcd.setCursor(0, 1);
  lcd.print(powerFactor);

  lcd.setCursor(0,3);
  lcd.print("Frekuensi (Hz): ");
  lcd.setCursor(0, 4);
  lcd.print(frequency);

  checkRotaryEncoder();

  delay(2000);

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("KWh: ");
  lcd.setCursor(0, 1);
  lcd.print(energy);

  lcd.setCursor(0,3);
  lcd.print("Biaya Listrik (Rp): ");
  lcd.setCursor(0, 4);
  lcd.print(cost);

  checkRotaryEncoder();

  delay(2000);
}

void checkRotaryEncoder() {
  rotaryValue = encoder.read();
  if (rotaryValue != prevRotaryValue) {
    tariff = map(rotaryValue, 0, 1023, 5, 25) / 1000.0;
    prevRotaryValue = rotaryValue;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tarif: Rp ");
    lcd.print(tariff * 1000);
    lcd.print("/kWh");
    delay(500);
  }
}
