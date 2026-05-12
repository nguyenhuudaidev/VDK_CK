#include <IRremote.hpp>
#include <DHT.h>

// ================= CONFIG =================
const int IR_SEND_PIN = 3;

// ================= DHT SENSOR =================
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ================= IR =================
const int MY_ADDRESS = 0x03;

const int CMD_HUMID = 0x01;
const int CMD_DRY   = 0x02;

// ================= HUMIDITY =================
float humidityThreshold = 70.0;

// ================= SETUP =================
void setup() {

  Serial.begin(9600);

  dht.begin();

  IrSender.begin(IR_SEND_PIN);

  Serial.println("IR Transmitter Ready");
}

// ================= LOOP =================
void loop() {

  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();

  // Kiểm tra lỗi đọc cảm biến
  if (isnan(temperature) || isnan(humidity)) {

    Serial.println("Failed to read from DHT sensor!");

    delay(1000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // ==================================
  // ĐỘ ẨM CAO
  // ==================================
  if (humidity > humidityThreshold) {

    IrSender.sendNEC(MY_ADDRESS, CMD_HUMID, 0);

    Serial.println("HUMID");
  }

  // ==================================
  // ĐỘ ẨM THẤP
  // ==================================
  else {

    IrSender.sendNEC(MY_ADDRESS, CMD_DRY, 0);

    Serial.println("DRY");
  }

  delay(1000);
}

/*
Cách lắp DHT11 / DHT22:

VCC  -> 5V
GND  -> GND
DATA -> D2

IR:
(-) -> GND
giữa -> 5V
OUT -> D3

Lưu ý:
- Cài thư viện:
    DHT sensor library
    Adafruit Unified Sensor

- Có thể chỉnh:
    humidityThreshold = 70.0;
*/