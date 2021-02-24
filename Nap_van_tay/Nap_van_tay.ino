/*************************************************** 
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_Fingerprint.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

void setup()  
{
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nĐăng ký cảm biến vân tay Adafruit");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Đã tìm thấy cảm biến vân tay!");
  } else {
    Serial.println("Không tìm thấy cảm biến vân tay :(");
    while (1) { delay(1); }
  }
}

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop()                     // run over and over again
{
  Serial.println("Sẵn sàng đăng ký dấu vân tay!");
  Serial.println("Vui lòng nhập số ID (từ 1 đến 127) mà bạn muốn lưu ngón tay này dưới mã ID = ");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Đăng ký ID #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Đang đợi ngón tay hợp lệ để đăng ký là #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("\nĐã chụp mẫu vân tay");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Lỗi giao tiếp");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Lỗi hình ảnh");
      break;
    default:
      Serial.println("Lỗi không xác định ");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Hình ảnh được chuyển đổi");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Hình ảnh lỗi");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Lỗi giao tiếp");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Không thể tìm thấy các tính năng vân tay");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Không thể tìm thấy các tính năng vân tay");
      return p;
    default:
      Serial.println("Lỗi không xác định");
      return p;
  }
  
  Serial.println("Xoá dấu vân tay");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Đặt lại cùng một ngón tay");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Đã chụp mẫu vân tay");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Lỗi giao tiếp");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Hình ảnh lỗi");
      break;
    default:
      Serial.println("Lỗi không xác định");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Hình ảnh đã được chuyển đổi");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Hình ảnh lỗi");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Lỗi giao tiếp");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Không thể tìm thấy các tính năng vân tay");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Không thể tìm thấy các tính năng vân tay");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Dấu vân tay khớp (*_')");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Lỗi giao tiếp");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Dấu vân tay không khớp");
    return p;
  } else {
    Serial.println("Lỗi không xác định");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Đã lưu trữ !");
    delay(500);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Lỗi giao tiếp");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Không thể lưu trữ ở vị trí đó");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Lỗi ghi vào flash");
    return p;
  } else {
    Serial.println("Lỗi không rõ");
    return p;
  }   
}
