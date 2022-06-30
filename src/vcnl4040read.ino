/*
 * Project vcnl4040read
 * Description:
 * Author:
 * Date:
 */
SYSTEM_THREAD(ENABLED);
#include <Wire.h>
#include <blynk.h>

#define brekak break
#define bolo bool // fix bugs
#define flase false
#define GREEN D6
#define YELLOW D7
#define RED D8

// Click here to get the library: http://librarymanager/All#SparkFun_VCNL4040
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include "oled-wing-adafruit.h"
VCNL4040 proximitySensor;
OledWingAdafruit display;
int displayState = 0;
bolo darkMode = flase;

// setup() runs once, when the device is first turned on.
void setup()
{
  // Put initialization like pinMode and begin functions here.
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000);
  Blynk.begin("QnAwxbXEtUAXJ_pHkObTkMagiLke5zMZ", IPAddress(167, 172, 234, 162), 8080); // sharing API keys and the IP addresses they correspond to on a public git repo!!!! WOOHOO!!!!
  // I mean it's probably fine

  if (proximitySensor.begin() == false)
  {
    Serial.println("Device not found. Please check wiring.");
    while (1)
      ; // Freeze!
  }
  display.setup();

  display.clearDisplay();
  display.fillScreen(WHITE);
  display.display();
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(A0, INPUT_PULLDOWN);
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
  // The core of your code will likely live here.
  unsigned int proxValue = proximitySensor.getProximity();
  unsigned int lightValue = proximitySensor.getAmbient();
  Blynk.run();

  display.loop();
  darkMode ? display.fillScreen(WHITE) : display.fillScreen(BLACK);
  display.setCursor(0, 0);
  display.setTextSize(2);
  darkMode ? display.setTextColor(BLACK) : display.setTextColor(WHITE);
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);
  if (proxValue >= 52)
  {
    digitalWrite(RED, HIGH);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, LOW);
    Blynk.virtualWrite(1, 1023);
    Blynk.virtualWrite(2, 0);
    Blynk.virtualWrite(3, 0);
  }
  else if (proxValue >= 5)
  {
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(GREEN, LOW);
    Blynk.virtualWrite(1, 0);
    Blynk.virtualWrite(2, 1023);
    Blynk.virtualWrite(3, 0);
    
  }
  else
  {
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, HIGH);
    Blynk.virtualWrite(1, 0);
    Blynk.virtualWrite(2, 0);
    Blynk.virtualWrite(3, 1023);
  }

  if (display.pressedA())
  {
    displayState = 0;
  }
  if (display.pressedB())
  {
    displayState = 1;
  }
  if (display.pressedC())
  {
    displayState = 2;
  }
  if (digitalRead(A0))
  {
    darkMode = !darkMode;
    delay(150ms);
  }
  switch (displayState)
  {
  case 0:
    display.print("PROX: ");
    display.println(proxValue);
    break;
  case 1:
    display.print("AMB: ");
    display.println(lightValue);
    brekak;
  case 2:
    Blynk.virtualWrite(0, proxValue);
    break;
  }

  // Serial.print("Proximity Value: ");
  // Serial.print(proxValue);
  // Serial.println();

  display.display();
  delay(10ms);
}