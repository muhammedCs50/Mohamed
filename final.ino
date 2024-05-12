#define BLYNK_TEMPLATE_ID "TMPL2uRMD5psq"
#define BLYNK_TEMPLATE_NAME "current"
#define BLYNK_AUTH_TOKEN "GVtbJwt9R-pHoFKIFxRps3tvgoT3R32S"

#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "Wire.h"
#include "Adafruit_INA219.h"

char auth[] = "GVtbJwt9R-pHoFKIFxRps3tvgoT3R32S";
const char* ssid = "realme 11";
const char* password = "adel0000";

#define DHTPIN D3  
#define DHTTYPE DHT11  
// DHT 11
DHT dht(DHTPIN, DHTTYPE); // إنشاء كائن DHT

#define MAX_TEMP 30  
// Maximum temperature threshold (°C)
#define MAX_CURRENT 250    
// Maximum current threshold (mA)

// INA219 sensor object
Adafruit_INA219 ina219;
// Blynk virtual pins
#define VOLTAGE_PIN V0
#define CURRENT_PIN V1
#define POWER_PIN V2

// Relay control pin
const int relayPin = D5; // Relay control pin connected to GPIO pin D5

// Motor control pins
const int enablePin = D8; // Motor enable pin connected to GPIO pin D8

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, password); // بدء الاتصال بمنصة Blynk
  dht.begin(); // بدء جهاز DHT

  ina219.begin();

  // Set pin mode
  pinMode(relayPin, OUTPUT);

  // Set pin modes
  pinMode(enablePin, OUTPUT);

  // Set initial motor state
  analogWrite(enablePin, 0); // Initially, motor is stopped
}

void loop() {
  // Run Blynk background tasks
  Blynk.run();

  float temperature = dht.readTemperature(); // قراءة درجة الحرارة
  Blynk.virtualWrite(V5, temperature); // إرسال درجة الحرارة إلى العنصر V5 في تطبيق Blynk

  // Read voltage, current, and power from INA219 sensor
  float voltage = ina219.getBusVoltage_V();
  float power = ina219.getPower_mW();
  float current = ina219.getCurrent_mA();
 
  
  // Print values to Serial Monitor
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print(" V\t");
  
  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" mA");
  
  Serial.print("Power: ");
  Serial.print(power);
  Serial.println(" mW");
  
  // Update Blynk virtual pins
  Blynk.virtualWrite(VOLTAGE_PIN, voltage);
  Blynk.virtualWrite(CURRENT_PIN, current);
  Blynk.virtualWrite(POWER_PIN, power);

  // Control relay and potentially activate pump based on temperature
    if (temperature > MAX_TEMP) {
      Serial.println("Temperature exceeded limit, turning off pump!");
      digitalWrite(relayPin, LOW);  // Turn off pump (assuming relay cuts power when LOW)
    } else {
      Serial.println("Temperature within limits, pump operation can continue.");
  }


 // Control relay and potentially activate pump based on clogging
    if (current > MAX_CURRENT) {
      Serial.println("clogging warning, turning off pump!");
      digitalWrite(relayPin, LOW);  // Turn off pump (assuming relay cuts power when LOW)
    } else {
      Serial.println("Normal water flow, pump operation can continue.");
    }
  
  // Delay between readings
  delay(500);
}

// Blynk virtual pin reading
BLYNK_WRITE(V3) {
  int relayState = param.asInt();
  digitalWrite(relayPin, relayState);
}

// Blynk virtual pin reading
BLYNK_WRITE(V4) {
  int motorSpeed = param.asInt();
  analogWrite(enablePin, motorSpeed);
}




