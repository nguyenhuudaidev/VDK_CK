#include <IRremote.hpp>

const int receiverPin = 2;

// ===== Motor =====
int IN1 = 7;
int IN2 = 8;
int ENA = 9; // Chân ENA cắm vào chân có hỗ trợ PWM (chân 9 là chuẩn)

// ===== CONFIG =====
const int MY_ADDRESS = 0x03;

// Đổi tên các lệnh cho đúng mục đích
const int CMD_WHITE = 0x01;  // Lệnh quay thuận
const int CMD_BLACK = 0x02; // Lệnh quay ngược

// Tốc độ mặc định của Motor (0 - 255)
unsigned long lastReceiveTime = 0;
const  unsigned long TIMEOUT = 600;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  Serial.begin(9600);
  stopMotor(); 

  // Khởi động bộ thu hồng ngoại
  IrReceiver.begin(receiverPin, ENABLE_LED_FEEDBACK);
  Serial.println("IR Receiver Ready!");
}

void loop() {

  bool hasSignal = false;

  // ===== NHẬN IR =====
  if (IrReceiver.decode()) {

    lastReceiveTime = millis();

    hasSignal = true;

    Serial.println("IR received");

    // đúng địa chỉ
    if (IrReceiver.decodedIRData.address == MY_ADDRESS) {

      // =====================
      // SÁNG -> QUAY THUẬN
      // =====================
      if (IrReceiver.decodedIRData.command == CMD_WHITE) {

        motorForward(200);

        Serial.println("FORWARD");
      }

      // =====================
      // TỐI -> QUAY NGƯỢC
      // =====================
      else if (IrReceiver.decodedIRData.command == CMD_BLACK) {

        motorBackward(200);

        Serial.println("BACKWARD");
      }
    }

    IrReceiver.resume();
  }

  // ===== MẤT TÍN HIỆU =====
  if (millis() - lastReceiveTime > TIMEOUT) {

    Serial.println("NO SIGNAL -> STOP");

    stopMotor();

    return;
  }
}

// ================= MOTOR FUNCTIONS =================

void motorForward(int speedValue) {

  analogWrite(ENA, speedValue);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void motorBackward(int speedValue) {

  analogWrite(ENA, speedValue);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void stopMotor() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  analogWrite(ENA, 0);
}
/*
Cách lắp:

- IR:
    + (-): GND
    + pin giữa: 5V
    + pin còn lại: D2

- L298N:
    + IN1 -> D7
    + IN2 -> D8
    + ENA -> D9

- Motor DC:
    + nối OUT1 OUT2 của L298N

- Nguồn:
    + motor dùng nguồn ngoài 5V-12V
    + GND nguồn ngoài nối chung GND Arduino
*/