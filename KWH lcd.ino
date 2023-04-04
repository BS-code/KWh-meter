#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PZEM004T.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

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

void setup() {
  Serial.begin(9600);
  pzemSerial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}

void loop() {
  voltage = pzem.voltage();
  current = pzem.current();
  powerFactor = pzem.powerFactor();
  frequency = pzem.frequency();
  energy = pzem.energy();
  cost = energy * 0.15; // Tarif listrik per kWh adalah Rp 1500

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Tegangan (V): ");
  display.setCursor(0, 8);
  display.print(voltage);

  display.setCursor(0,20);
  display.print("Arus (A): ");
  display.setCursor(0, 28);
  display.print(current);

  delay(2000);

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Faktor Daya: ");
  display.setCursor(0, 8);
  display.print(powerFactor);

  display.setCursor(0,20);
  display.print("Frekuensi (Hz): ");
  display.setCursor(0, 28);
  display.print(frequency);

  delay(2000);

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("KWh: ");
  display.setCursor(0, 8);
  display.print(energy);

  display.setCursor(0,20);
  display.print("Biaya Listrik (Rp): ");
  display.setCursor(0, 28);
  display.print(cost);

  delay(2000);
}
