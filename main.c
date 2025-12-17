#include <DHT.h>

#define DHTPIN 3        // DHT11 data pin updated
#define DHTTYPE DHT11
#define PRESSURE_PIN 5  // Analog input for pressure sensor
#define RAIN_PIN 6      // Analog input for rain sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);
  dht.begin();

  // Stabilize rain sensor analog input
  pinMode(RAIN_PIN, INPUT_PULLUP);

  Serial.println("🌡️ DHT11 + Pressure + Rain Sensor (ESP32-C6)");
}

void loop() {
  // --- DHT11: Temperature & Humidity ---
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("❌ Failed to read DHT11");
  } else {
    Serial.print("🌡️ Temp: "); Serial.print(temp); Serial.println(" °C");
    Serial.print("💧 Humidity: "); Serial.print(hum); Serial.println(" %");
  }

  // --- Pressure Sensor ---
  int rawPressure = analogRead(PRESSURE_PIN);
  float voltage = (rawPressure / 4095.0) * 3.3;  // 12-bit ADC, 3.3V reference

  // For MPX10DP: 0.2V → 0 kPa, 4.7V → 10 kPa
  float pressure_kPa = ((voltage - 0.2) * (10.0 / 4.5));
  if (pressure_kPa < 0) pressure_kPa = 0; // avoid negative values

  Serial.print("🧭 Pressure: ");
  Serial.print(pressure_kPa, 2);
  Serial.println(" kPa");

  // --- Rain Sensor (2-pin) ---
  int rainValue = analogRead(RAIN_PIN);
  Serial.print("🌧️ Rain Value: ");
  Serial.println(rainValue);

  if (rainValue < 2000) {  // lower value = rain detected
    Serial.println("☔ Rain Detected");
  } else {
    Serial.println("🌤️ No Rain");
  }

  Serial.println("-----------------------------");
  delay(2000);
}
