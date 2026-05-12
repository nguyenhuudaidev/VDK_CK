#include <IRremote.hpp>

// ================= CONFIG =================
const int IR_SEND_PIN = 3;

const int LDR_PIN = A0;

const int MY_ADDRESS = 0x03;

const int CMD_BRIGHT = 0x01;
const int CMD_DARK   = 0x02;

// Ngưỡng ánh sáng
int lightThreshold = 500;

// ================= SETUP =================
void setup() {

  Serial.begin(9600);

  IrSender.begin(IR_SEND_PIN);

  Serial.println("IR Transmitter Ready");
}

// ================= LOOP =================
void loop() {

  int lightValue = analogRead(LDR_PIN);

  Serial.print("Light: ");
  Serial.println(lightValue);

  // =========================
  // SÁNG -> GỬI QUAY THUẬN
  // =========================
  if (lightValue < lightThreshold) {

    IrSender.sendNEC(MY_ADDRESS, CMD_BRIGHT, 0);

    Serial.println("SEND FORWARD");
  }

  // =========================
  // TỐI -> GỬI QUAY NGƯỢC
  // =========================
  else {

    IrSender.sendNEC(MY_ADDRESS, CMD_DARK, 0);

    Serial.println("SEND BACKWARD");
  }

  delay(200);
}

// /*
// Cách lắp:
// - Cảm biến ánh sáng:
//     + 1 chân nối A0
//     + 2 chân còn lại nối 5V và GND

// - IR:
//     + (-): GND
//     + pin giữa: 5V
//     + pin còn lại: pin D3
// */
