#include <IRremote.hpp>
#include <Servo.h>

// ================= CONFIG =================
const int IR_RECEIVE_PIN = 2;
const int SERVO_PIN      = 9;

Servo myServo;

// ================= IR =================
const int MY_ADDRESS = 0x03;

const int CMD_LEFT  = 0x01;
const int CMD_RIGHT = 0x02;
const int CMD_STOP  = 0x04;

// ================= BIẾN =================
unsigned long lastReceiveTime = 0;
unsigned long lastMoveTime    = 0;

// Timeout mất tín hiệu
const unsigned long TIMEOUT = 600;

// Trạng thái hiện tại
bool isRightMode = false;
bool servoHigh   = false;

// Cờ dừng servo
bool isStopped = true;

// ================= SETUP =================
void setup() {

  Serial.begin(9600);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  myServo.attach(SERVO_PIN);
  myServo.write(0);

  lastReceiveTime = millis();

  Serial.println("IR Receiver & Servo Ready");
}

// ================= LOOP =================
void loop() {

  // =====================================
  // NHẬN IR
  // =====================================
  if (IrReceiver.decode()) {

    int address = IrReceiver.decodedIRData.address;
    int command = IrReceiver.decodedIRData.command;

    bool isRepeat =
      (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT);

    // Chỉ xử lý đúng địa chỉ hoặc repeat
    if (address == MY_ADDRESS || isRepeat) {

      lastReceiveTime = millis();

      // Nếu đang stop mà có tín hiệu lại
      if (isStopped) {
        isStopped = false;
        lastMoveTime = millis();
      }

      // Chỉ đổi mode khi KHÔNG phải repeat
      if (!isRepeat && address == MY_ADDRESS) {

        // =====================================
        // LEFT MODE
        // =====================================
        if (command == CMD_LEFT) {

          if (isRightMode == true) {
            servoHigh = false;
            myServo.write(0);
            lastMoveTime = millis();
          }

          isRightMode = false;

          Serial.println("MODE -> LEFT");
        }

        // =====================================
        // RIGHT MODE
        // =====================================
        else if (command == CMD_RIGHT) {

          if (isRightMode == false) {
            servoHigh = false;
            myServo.write(0);
            lastMoveTime = millis();
          }

          isRightMode = true;

          Serial.println("MODE -> RIGHT");
        }

        // =====================================
        // STOP
        // =====================================
        else if (command == CMD_STOP) {

          myServo.write(0);

          servoHigh = false;
          isStopped = true;

          Serial.println("MODE -> STOP");
        }
      }
    }

    IrReceiver.resume();
  }

  // =====================================
  // MẤT TÍN HIỆU
  // =====================================
  if (!isStopped &&
      (millis() - lastReceiveTime > TIMEOUT)) {

    Serial.println("NO SIGNAL -> STOP");

    myServo.write(0);

    servoHigh = false;
    isStopped = true;
  }

  // Nếu đang stop thì bỏ qua
  if (isStopped) {
    return;
  }

  // =====================================
  // LOGIC SERVO
  // =====================================

  // -------------------------------------
  // RIGHT: 0 <-> 180
  // -------------------------------------
  if (isRightMode) {

    if (millis() - lastMoveTime >= 700) {

      lastMoveTime = millis();

      servoHigh = !servoHigh;

      if (servoHigh) {
        myServo.write(180);
        Serial.println("RIGHT -> 180");
      }
      else {
        myServo.write(0);
        Serial.println("RIGHT -> 0");
      }
    }
  }

  // -------------------------------------
  // LEFT: 0 <-> 30
  // -------------------------------------
  else {

    if (millis() - lastMoveTime >= 200) {

      lastMoveTime = millis();

      servoHigh = !servoHigh;

      if (servoHigh) {
        myServo.write(30);
        Serial.println("LEFT -> 30");
      }
      else {
        myServo.write(0);
        Serial.println("LEFT -> 0");
      }
    }
  }
}