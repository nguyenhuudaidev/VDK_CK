#include <IRremote.hpp>

// ================= CONFIG =================
const int IR_RECEIVE_PIN = 2;

// ================= MOTOR DC =================
const int IN1 = 7;
const int IN2 = 8;
const int ENA = 9;

// ================= IR =================
const int MY_ADDRESS = 0x03;

const int CMD_HUMID = 0x01;
const int CMD_DRY   = 0x02;

// ================= BIẾN =================
unsigned long lastReceiveTime = 0;

const unsigned long TIMEOUT = 1500;

// ================= SETUP =================
void setup() {

  Serial.begin(9600);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  stopMotor();

  lastReceiveTime = millis();

  Serial.println("IR Receiver Ready");
}

// ================= LOOP =================
void loop() {

  // ===== NHẬN IR =====
  if (IrReceiver.decode()) {

    lastReceiveTime = millis();

    Serial.println("IR received");

    // Đúng địa chỉ
    if (IrReceiver.decodedIRData.address == MY_ADDRESS) {

      // =====================
      // ĐỘ ẨM CAO -> QUAY THUẬN
      // =====================
      if (IrReceiver.decodedIRData.command == CMD_HUMID) {

        motorForward(200);

        Serial.println("HUMID -> FORWARD");
      }

      // =====================
      // ĐỘ ẨM THẤP -> QUAY NGƯỢC
      // =====================
      else if (IrReceiver.decodedIRData.command == CMD_DRY) {

        motorBackward(200);

        Serial.println("DRY -> BACKWARD");
      }
    }

    IrReceiver.resume();
  }

  // ===== MẤT TÍN HIỆU =====
  if (millis() - lastReceiveTime > TIMEOUT) {

    Serial.println("NO SIGNAL -> STOP");

    stopMotor();
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

- IR Receiver:
    (-) -> GND
    giữa -> 5V
    OUT -> D2

- L298N:
    IN1 -> D7
    IN2 -> D8
    ENA -> D9

- Motor DC:
    nối OUT1 OUT2 của L298N

- Nguồn:
    motor dùng nguồn ngoài
    nhớ nối chung GND với Arduino
*/