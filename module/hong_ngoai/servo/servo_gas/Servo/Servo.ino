#include <IRremote.hpp>
#include <Servo.h>

// ================= CONFIG =================
const int IR_RECEIVE_PIN = 2;
const int SERVO_PIN      = 9;

Servo myServo;

// ================= IR =================
const int MY_ADDRESS = 0x03;

const int CMD_GAS_DETECTED = 0x01;
const int CMD_NO_GAS       = 0x02;


// ================= BIẾN =================
unsigned long lastReceiveTime = 0;
unsigned long lastMoveTime    = 0;

// Timeout mất tín hiệu
const unsigned long TIMEOUT = 600;

// Trạng thái hiện tại
bool isGasDetected = false;
bool servoHigh     = false;

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
  // NHẬN TÍN HIỆU IR
  // =====================================
  if (IrReceiver.decode()) {

    int address = IrReceiver.decodedIRData.address;
    int command = IrReceiver.decodedIRData.command;

    bool isRepeat =
      (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT);

    // Chỉ xử lý đúng địa chỉ hoặc tín hiệu giữ nút
    if (address == MY_ADDRESS || isRepeat) {

      lastReceiveTime = millis();

      // Nếu đang dừng mà có tín hiệu lại
      if (isStopped) {
        isStopped = false;
        lastMoveTime = millis();
      }

      // Chỉ đổi mode khi KHÔNG phải mã repeat
      if (!isRepeat && address == MY_ADDRESS) {

        // =====================================
        // MODE GAS DETECTED
        // =====================================
        if (command == CMD_GAS_DETECTED) {

          // Reset trạng thái trước khi đổi mode
          if (!isGasDetected) {
            servoHigh = false;
            myServo.write(0);
            lastMoveTime = millis();
          }

          isGasDetected = true;

          Serial.println("MODE -> GAS DETECTED");
        }

        // =====================================
        // MODE NO GAS
        // =====================================
        else if (command == CMD_NO_GAS) {

          // Reset trạng thái trước khi đổi mode
          if (isGasDetected) {
            servoHigh = false;
            myServo.write(0);
            lastMoveTime = millis();
          }

          isGasDetected = false;

          Serial.println("MODE -> NO GAS");
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

  // Nếu đang dừng thì bỏ qua
  if (isStopped) {
    return;
  }

  // =====================================
  // LOGIC SERVO
  // =====================================

  // -------------------------------------
  // CHẾ ĐỘ GAS: 0 <-> 180
  // -------------------------------------
  if (isGasDetected) {

    if (millis() - lastMoveTime >= 700) {

      lastMoveTime = millis();

      servoHigh = !servoHigh;

      if (servoHigh) {
        myServo.write(180);
        Serial.println("GAS -> 180");
      }
      else {
        myServo.write(0);
        Serial.println("GAS -> 0");
      }
    }
  }

  // -------------------------------------
  // CHẾ ĐỘ NO GAS: 0 <-> 30
  // -------------------------------------
  else {

    if (millis() - lastMoveTime >= 200) {

      lastMoveTime = millis();

      servoHigh = !servoHigh;

      if (servoHigh) {
        myServo.write(30);
        Serial.println("NO GAS -> 30");
      }
      else {
        myServo.write(0);
        Serial.println("NO GAS -> 0");
      }
    }
  }
}