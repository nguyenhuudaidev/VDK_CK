#include <IRremote.hpp>
#include <Servo.h>

// ================= CONFIG =================
const int IR_RECEIVE_PIN = 2;
const int SERVO_PIN      = 9;

Servo myServo;

// ================= IR =================
const int MY_ADDRESS = 0x03;

const int CMD_OBJECT_NEAR = 0x01;
const int CMD_OBJECT_FAR   = 0x02;

// ================= BIẾN =================
unsigned long lastReceiveTime = 0;
unsigned long lastMoveTime = 0;

// Timeout mất tín hiệu (chỉnh thành 600ms giống code mới để phù hợp với việc nhấn giữ nút)
const unsigned long TIMEOUT = 600;

// Trạng thái hiện tại
bool isObjectFar = false;
bool servoHigh = false;

// Cờ theo dõi trạng thái dừng để tránh spam lệnh myServo.write(0)
bool isStopped = true; 

// ================= SETUP =================
void setup() {
  Serial.begin(9600);
  
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  
  myServo.attach(SERVO_PIN);
  myServo.write(0); // Trạng thái ban đầu

  lastReceiveTime = millis();
  Serial.println("IR Receiver & Servo Ready");
}

// ================= LOOP =================
void loop() {

  // ===== NHẬN IR =====
  if (IrReceiver.decode()) {
    
    int address = IrReceiver.decodedIRData.address;
    int command = IrReceiver.decodedIRData.command;
    bool isRepeat = (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT);

    // Chỉ gia hạn thời gian nếu đúng địa chỉ hoặc là tín hiệu đang giữ nút
    if (address == MY_ADDRESS || isRepeat) {
      
      lastReceiveTime = millis();

      // Nếu đang ở trạng thái dừng mà có tín hiệu lại -> Đánh thức servo
      if (isStopped) {
        isStopped = false;
        lastMoveTime = millis(); // Khôi phục mốc thời gian vẫy để không bị delay
      }

      // Chỉ xử lý đổi mode nếu không phải mã lặp (chỉ chạy lúc mới bấm)
      if (!isRepeat && address == MY_ADDRESS) {
        
        // =====================
        // CHUYỂN MODE GẦN
        // =====================
        if (command == CMD_OBJECT_NEAR) {
          // Reset trạng thái nếu đang từ Far chuyển sang
          if (isObjectFar == true) {
            servoHigh = false;
            myServo.write(0);
            lastMoveTime = millis();
          }
          isObjectFar = false;
          Serial.println("MODE -> OBJECT_NEAR");
        }
        
        // =====================
        // CHUYỂN MODE XA
        // =====================
        else if (command == CMD_OBJECT_FAR) {
          // Reset trạng thái nếu đang từ Near chuyển sang
          if (isObjectFar == false) {
            servoHigh = false;
            myServo.write(0);
            lastMoveTime = millis();
          }
          isObjectFar = true;
          Serial.println("MODE -> OBJECT_FAR");
        }
      }
    }
    
    IrReceiver.resume();
  }

  // ===== MẤT TÍN HIỆU =====
  // Chỉ kiểm tra timeout nếu servo đang KHÔNG ở trạng thái dừng
if (!isStopped && (millis() - lastReceiveTime > TIMEOUT)) {
    
    Serial.println("NO SIGNAL -> RETURN TO 0° AND STOP");
    
    myServo.write(0);  // Đưa servo về 0
    servoHigh = false; // Reset trạng thái góc
    isStopped = true;  // Bật cờ dừng để không lặp lại lệnh này ở các vòng lặp sau
  }

  // ===== KIỂM TRA DỪNG =====
  // Nếu cờ isStopped đang bật, bỏ qua khối code vẫy servo bên dưới
  if (isStopped) {
    return;
  }

  // =========================
  // LOGIC VẪY SERVO (Từ code cũ)
  // =========================

  // --- CHẾ ĐỘ GẦN: 0 <-> 180 ---
  if (isObjectNear) {
    if (millis() - lastMoveTime >= 700) {
      lastMoveTime = millis();
      servoHigh = !servoHigh;
      
      if (servoHigh) {
        myServo.write(180);
        Serial.println("OBJECT_NEAR -> 180");
      } else {
        myServo.write(0);
        Serial.println("OBJECT_FAR -> 0");
      }
    }
  }
  
  // --- CHẾ ĐỘ XA: 0 <-> 30 ---
  else {
    if (millis() - lastMoveTime >= 200) {
      lastMoveTime = millis();
      servoHigh = !servoHigh;
      
      if (servoHigh) {
        myServo.write(30);
        Serial.println("OBJECT_FAR -> 30");
      } else {
        myServo.write(0);
        Serial.println("OBJECT_FAR -> 0");
      }
    }
  }
}
