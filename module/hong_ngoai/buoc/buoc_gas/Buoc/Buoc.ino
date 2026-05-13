#include <IRremote.hpp>
#include <Stepper.h>

#define BUOC 2048
const int receiverPin = 3;

Stepper stepper(BUOC, 8, 10, 9, 11);
double goc_quay = 0;

void setup() {
  stepper.setSpeed(10);
  Serial.begin(9600);

  // IRremote 4.x
  IrReceiver.begin(receiverPin, ENABLE_LED_FEEDBACK);
}

void translateIR(uint8_t cmd) {
  switch (cmd) {
    case 0x10:
      goc_quay = 90.0;
      break;
    case 0x11:
      goc_quay = -180.0;
      break;
    default:
      goc_quay = 0;  // không khớp thì khỏi quay
      break;
  }
}

void loop() {
  if (IrReceiver.decode()) {
    // Lấy command (thường cái bạn cần để switch)
    uint8_t cmd = IrReceiver.decodedIRData.command;

    Serial.print("Protocol: ");
    Serial.print(IrReceiver.decodedIRData.protocol);
    Serial.print("  Command: 0x");
    Serial.println(cmd, HEX);

    translateIR(cmd);

    if (goc_quay != 0) {
      stepper.step((goc_quay / 360.0) * BUOC);
      delay(200);
    }

    IrReceiver.resume();
  }
}
