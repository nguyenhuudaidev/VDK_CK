#include <IRremote.hpp>
#include <Stepper.h>

// ================= CONFIG =================
const int IR_RECEIVE_PIN = 2;

const int stepsPerRevolution = 2038;

// Stepper 28BYJ-48 + ULN2003
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

// ================= IR =================
const int MY_ADDRESS = 0x03;

const int CMD_HUMID = 0x01;
const int CMD_DRY = 0x02;

// ================= BIẾN =================
unsigned long lastReceiveTime = 0;

const unsigned long TIMEOUT = 600;

// ================= SETUP =================
void setup() {

  Serial.begin(9600);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  myStepper.setSpeed(15);

  lastReceiveTime = millis();

  Serial.println("IR Receiver Ready");
}

// ================= LOOP =================
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
      if (IrReceiver.decodedIRData.command == CMD_HUMID) {

        myStepper.setSpeed(15);

        myStepper.step(stepsPerRevolution / 4);

        Serial.println("FORWARD");
      }

      // =====================
      // TỐI -> QUAY NGƯỢC
      // =====================
      else if (IrReceiver.decodedIRData.command == CMD_DRY) {

        myStepper.setSpeed(15);

        myStepper.step(-stepsPerRevolution / 4);
        Serial.println("BACKWARD");
      }
    }

    IrReceiver.resume();
  }

  // ===== MẤT TÍN HIỆU =====
  if (millis() - lastReceiveTime > TIMEOUT) {

    Serial.println("NO SIGNAL -> STOP");

    return;
  }
}

// /*
// Cách lắp:
// - IR:
//     + (-): GND
//     + pin giữa: 5V
//     + pin còn lại: pin D2

// - Buoc:
//     + IN1 -> IN4: D8-D11
//     + VCC → 5V
//     + GND → GND
// */
