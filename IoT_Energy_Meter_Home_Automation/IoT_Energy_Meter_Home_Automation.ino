#define BLYNK_TEMPLATE_ID "TMPL61H8OgBLb"
#define BLYNK_TEMPLATE_NAME "IOT Based Energy Meter"
#define BLYNK_AUTH_TOKEN "qBqikGa1ElRY-a8uYevpRkDsAv29jcBw"

char ssid[] = "maruf";
char pass[] = "12345678";
bool fetch_blynk_state = true; // true or false

#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 14, 27, 26, 25);

#define BLYNK_PRINT Serial
#include "EmonLib.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <AceButton.h>
using namespace ace_button;

// Define GPIO pins
#define RelayPin1 19       // D19
#define RelayPin2 18       // D18
#define SwitchPin1 33      // D33
#define SwitchPin2 32      // D32
#define wifiLed 2          // D2

#define VPIN_BUTTON_1 V6
#define VPIN_BUTTON_2 V7

bool toggleState_1 = LOW;
bool toggleState_2 = LOW;

EnergyMonitor emon;
#define vCalibration 83.3
#define currCalibration 0.50

BlynkTimer timer;
int wifiFlag = 0;
char auth[] = BLYNK_AUTH_TOKEN;

ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);

void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, !toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, !toggleState_2);
}

void checkBlynkStatus() {
  bool isconnected = Blynk.connected();
  if (!isconnected) {
    wifiFlag = 1;
    Serial.println("Blynk Not Connected");
    digitalWrite(wifiLed, LOW);
  } else {
    wifiFlag = 0;
    if (!fetch_blynk_state) {
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    }
    digitalWrite(wifiLed, HIGH);
    Serial.println("Blynk Connected");
  }
}

BLYNK_CONNECTED() {
  if (fetch_blynk_state) {
    Blynk.syncVirtual(VPIN_BUTTON_1);
    Blynk.syncVirtual(VPIN_BUTTON_2);
  }
}

float bills = 0;
float kWh = 0;
unsigned long lastmillis = millis();

void myTimerEvent() {
  emon.calcVI(20, 2000);
  kWh += emon.apparentPower * (millis() - lastmillis) / 3600000000.0;
  bills = kWh * 7.0;

  Serial.print("Vrms: ");
  Serial.print(emon.Vrms, 2);
  Serial.print("V\tIrms: ");
  Serial.print(emon.Irms, 4);
  Serial.print("A\tPower: ");
  Serial.print(emon.apparentPower, 4);
  Serial.print("W\tUnit: ");
  Serial.print(kWh, 5);
  Serial.println("kWh\tBills: ");
  Serial.print(bills, 2);
  Serial.println("BDT");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vrms:");
  lcd.print(emon.Vrms, 2);
  lcd.print("V");
  lcd.setCursor(0, 1);
  lcd.print("Irms:");
  lcd.print(emon.Irms, 4);
  lcd.print("A");
  delay(2500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Power:");
  lcd.print(emon.apparentPower, 4);
  lcd.print("W");
  lcd.setCursor(0, 1);
  lcd.print("Unit:");
  lcd.print(kWh, 6);
  lcd.print("kWh");
  delay(2500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bills:");
  lcd.print(bills, 2);
  lcd.print("BDT");
  lcd.setCursor(0, 1);
  lcd.print("MARUF,");
  lcd.print("RUET");
  delay(2500);

  lastmillis = millis();

  Blynk.virtualWrite(V0, emon.Vrms);
  Blynk.virtualWrite(V1, emon.Irms);
  Blynk.virtualWrite(V2, emon.apparentPower);
  Blynk.virtualWrite(V3, kWh);
  Blynk.virtualWrite(V4, bills);
}

void setup() {
  Serial.begin(9600);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(wifiLed, OUTPUT);
  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);

  digitalWrite(RelayPin1, !toggleState_1);
  digitalWrite(RelayPin2, !toggleState_2);
  digitalWrite(wifiLed, LOW);

  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);
  button1.init(SwitchPin1);
  button2.init(SwitchPin2);

  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus);
  Blynk.config(auth);
  delay(1000);

  if (!fetch_blynk_state) {
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
  }

  lcd.begin(16, 2);
  emon.voltage(35, vCalibration, 1.7);
  emon.current(34, currCalibration);

  timer.setInterval(5000L, myTimerEvent);

  lcd.setCursor(3, 0);
  lcd.print("IoT Energy");
  lcd.setCursor(5, 1);
  lcd.print("Meter");
  delay(3000);
  lcd.clear();
}

void loop() {
  Blynk.run();
  timer.run();
  button1.check();
  button2.check();
}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  switch (eventType) {
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      digitalWrite(RelayPin1, toggleState_1);
      toggleState_1 = !toggleState_1;
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
      break;
  }
}

void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT2");
  switch (eventType) {
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      digitalWrite(RelayPin2, toggleState_2);
      toggleState_2 = !toggleState_2;
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
      break;
  }
}
