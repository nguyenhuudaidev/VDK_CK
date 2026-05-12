#include <IRremote.hpp>

// ================= CONFIG =================
const int IR_SEND_PIN = 3;

// ================= LINE SENSOR =================
const int LINE_SENSOR_PIN = 2;

// ================= IR =================
const int MY_ADDRESS = 0x03;

const int CMD_WHITE = 0x01;
const int CMD_BLACK  = 0x02;

// ================= SETUP =================
void setup() {

  Serial.begin(9600);

  pinMode(LINE_SENSOR_PIN, INPUT);

  IrSender.begin(IR_SEND_PIN);

  Serial.println("IR Transmitter Ready");
}

// ================= LOOP =================
void loop() {

  int lineState = digitalRead(LINE_SENSOR_PIN);

  Serial.print("Line Sensor: ");
  Serial.println(lineState);

  // ==================================
  // GẶP VẠCH ĐEN -> QUAY THUẬN
  // ==================================
  if (lineState == LOW) {

    IrSender.sendNEC(MY_ADDRESS, CMD_WHITE, 0);

    Serial.println("SEND WHITE");
  }

  // ==================================
  // KHÔNG GẶP VẠCH -> QUAY NGƯỢC
  // ==================================
  else {

    IrSender.sendNEC(MY_ADDRESS, CMD_BLACK, 0);

    Serial.println("SEND BLACK");
  }

  delay(200);
}

/*
Cách lắp:

- Cảm biến dò line:
    VCC  -> 5V
    GND  -> GND
    OUT  -> D2

- IR:
    (-)       -> GND
    chân giữa -> 5V
    chân còn lại -> D3

Lưu ý:
- Phần lớn cảm biến dò line:
    LOW  = gặp vạch đen
    HIGH = nền trắng

Nếu hoạt động ngược thì đổi:
    if (lineState == LOW)

thành:
    if (lineState == HIGH)
*/