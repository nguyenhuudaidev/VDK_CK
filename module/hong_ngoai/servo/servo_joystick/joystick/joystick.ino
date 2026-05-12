#include <IRremote.hpp>

// ================= CONFIG =================
const int IR_SEND_PIN = 3;

// ================= JOYSTICK =================
const int JOYSTICK_X = A0;

// ================= IR =================
const int MY_ADDRESS = 0x03;

const int CMD_LEFT  = 0x01;
const int CMD_RIGHT = 0x02;
const int CMD_STOP  = 0x04;

// ================= JOYSTICK THRESHOLD =================
int centerValue = 512;

int leftThreshold  = 400;
int rightThreshold = 600;

// ================= SETUP =================
void setup() {

  Serial.begin(9600);

  IrSender.begin(IR_SEND_PIN);

  Serial.println("IR Transmitter Ready");
}

// ================= LOOP =================
void loop() {

  int xValue = analogRead(JOYSTICK_X);

  Serial.print("Joystick X: ");
  Serial.println(xValue);

  // ==================================
  // QUAY TRÁI
  // ==================================
  if (xValue < leftThreshold) {

    IrSender.sendNEC(MY_ADDRESS, CMD_LEFT, 0);

    Serial.println("LEFT");
  }

  // ==================================
  // QUAY PHẢI
  // ==================================
  else if (xValue > rightThreshold) {

    IrSender.sendNEC(MY_ADDRESS, CMD_RIGHT, 0);

    Serial.println("RIGHT");
  }

  // ==================================
  // THẢ JOYSTICK -> DỪNG
  // ==================================
  else {

    IrSender.sendNEC(MY_ADDRESS, CMD_STOP, 0);

    Serial.println("STOP");
  }

  delay(100);
}

/*
Cách lắp Joystick:

VCC -> 5V
GND -> GND
VRx -> A0

(Không dùng VRy)

IR:
(-) -> GND
chân giữa -> 5V
OUT -> D3

Lưu ý:
- Khi thả joystick:
    giá trị khoảng 512

- Có thể chỉnh:
    leftThreshold
    rightThreshold

Ví dụ:
    leftThreshold  = 400;
    rightThreshold = 600;
*/