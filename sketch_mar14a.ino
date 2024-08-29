#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <ESP8266WiFi.h>

const char DEVICE_LOGIN_NAME[] = "051f9efa-7e1f-4698-bbba-1249bd182cd4";
const char SSID[] = "My_esp8622";  // Corrected SSID for ESP8266
const char PASS[] = "Rash@2002";
const char DEVICE_KEY[] = "eRuCM?t5CjM8veV?yHq6HQ4V#";

float voltage;
int bat_percentage;

void initProperties() {
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(voltage, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(bat_percentage, READ, ON_CHANGE, NULL);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

int analogInPin = A0;
int sensorValue;
float calibration = 0.36;

void setup() {
  Serial.begin(9600);
  delay(1500);

  initProperties();

  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  sensorValue = analogRead(analogInPin);
  voltage = (((sensorValue * 3.3) / 1024) * 2 + calibration);
  
  // Map voltage to percentage (assuming battery voltage ranges from 2.8V to 4.2V)
  bat_percentage = map(voltage, 2.8, 4.2, 0, 100);
  // Ensure battery percentage stays within bounds
  bat_percentage = constrain(bat_percentage, 0, 100);
  Serial.begin(9600);
  Serial.print("Analog Value = ");
  Serial.print(sensorValue);
  Serial.print("\t Output Voltage = ");
  Serial.print(voltage);
  Serial.print("\t Battery Percentage = ");
  Serial.println(bat_percentage);
  
  delay(1000);  // Consider using a non-blocking delay method
}