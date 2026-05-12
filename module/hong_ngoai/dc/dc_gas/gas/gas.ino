#include <IRremote.hpp>

// ================= CONFIG =================
const int IR_SEND_PIN = 3;

// ================= GAS SENSOR =================
const int GAS_SENSOR_PIN = A0;

// ================= IR =================
const int MY_ADDRESS = 0x03;

const int CMD_GAS_DETECTED = 0x01;
const int CMD_NO_GAS   = 0x02;

// ================= GAS THRESHOLD =================
int gasThreshold = 500;

// ================= SETUP =================
void setup() {

  Serial.begin(9600);

  IrSender.begin(IR_SEND_PIN);

  Serial.println("IR Transmitter Ready");
}

// ================= LOOP =================
void loop() {

  int gasValue = analogRead(GAS_SENSOR_PIN);

  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  // ==================================
  // PHÁT HIỆN KHÍ GAS
  // ==================================
  if (gasValue > gasThreshold) {

    IrSender.sendNEC(MY_ADDRESS, CMD_GAS_DETECTED, 0);

    Serial.println("GAS DETECTED -> SEND FORWARD");
  }

  // ==================================
  // KHÔNG CÓ KHÍ GAS
  // ==================================
  else {

    IrSender.sendNEC(MY_ADDRESS, CMD_NO_GAS, 0);

    Serial.println("NO GAS -> SEND BACKWARD");
  }

  delay(200);
}

/*
Cách lắp:

- Cảm biến khí gas MQ-2 / MQ-5:
    VCC -> 5V
    GND -> GND
    AO  -> A0

- IR:
    (-) -> GND
    chân giữa -> 5V
    chân còn lại -> D3

Lưu ý:
- gasThreshold cần chỉnh thực tế.
- Thường giá trị:
    0 -> 1023

- Có thể mở Serial Monitor để xem giá trị gas
  rồi chỉnh ngưỡng phù hợp.

Ví dụ:
    gasThreshold = 300;
    gasThreshold = 500;
*/