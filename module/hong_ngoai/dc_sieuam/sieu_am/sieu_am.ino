#include <IRremote.hpp>

// ================= CONFIG =================
const int IR_SEND_PIN = 3;

// ================= ULTRASONIC =================
const int trigPin = 5;
const int echoPin = 6;

// ================= IR =================
const int MY_ADDRESS = 0x03;

const int CMD_OBJECT_NEAR = 0x01;
const int CMD_OBJECT_FAR  = 0x02;

// ================= DISTANCE THRESHOLD =================
int distanceThreshold = 20; // cm

long duration;
int distance;

// ================= SETUP =================
void setup() {

  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  IrSender.begin(IR_SEND_PIN);

  Serial.println("IR Transmitter Ready");
}

// ================= LOOP =================
void loop() {

  // Gửi xung siêu âm
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // Đọc tín hiệu phản hồi
  duration = pulseIn(echoPin, HIGH);

  // Tính khoảng cách (cm)
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // ==================================
  // CÓ VẬT Ở GẦN
  // ==================================
  if (distance <= distanceThreshold) {

    IrSender.sendNEC(MY_ADDRESS, CMD_OBJECT_NEAR, 0);

    Serial.println("OBJECT NEAR -> SEND FORWARD");
  }

  // ==================================
  // KHÔNG CÓ VẬT GẦN
  // ==================================
  else {

    IrSender.sendNEC(MY_ADDRESS, CMD_OBJECT_FAR, 0);

    Serial.println("OBJECT FAR -> SEND BACKWARD");
  }

  delay(200);
}

/*
Cách lắp HC-SR04:

VCC  -> 5V
GND  -> GND
TRIG -> D5
ECHO -> D6

IR:
(-) -> GND
chân giữa -> 5V
chân còn lại -> D3

Lưu ý:
- distanceThreshold có thể chỉnh:
    10 cm
    20 cm
    30 cm

Ví dụ:
    int distanceThreshold = 20;
*/