#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PZEM004T.h>
#include <LCD12864_ST7920.h>

LCD12864_ST7920 lcd(14, 15, 16, 17, 18, 19);

#define RX_PIN 2
#define TX_PIN 3

SoftwareSerial pzemSerial(RX_PIN, TX_PIN);
PZEM004T pzem(&pzemSerial);

float voltage = 0;
float current = 0;
float powerFactor = 0;
float frequency = 0;
float energy = 0;
float cost = 0;
float tariff = 0.15;

int setButtonState = 0;
int prevSetButtonState = 0;

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

  checkSetButton();

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

  checkSetButton();

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

  checkSetButton();

  delay(2000);
}

void checkSetButton() {
  setButtonState = digitalRead(7);
  if (setButtonState == HIGH && prevSetButtonState == LOW) {
    tariff = inputTariff();
  }
  prevSetButtonState = setButtonState;
}

float inputTariff() {
  float newTariff = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Masukkan Tarif Baru: ");
  while (1) {
    if (Serial.available() > 0) {
      char inputChar = Serial.read();
      if (inputChar == '\n') {
        break;
      } else if (isdigit(inputChar)) {
        newTariff = newTariff * 10 + (inputChar - '0');
        lcd.print(inputChar);
      }
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tarif Baru: ");
  lcd.setCursor(0, 1);
  lcd.print(newTariff);
  delay(2000);
  return newTariff / 1000;
}
