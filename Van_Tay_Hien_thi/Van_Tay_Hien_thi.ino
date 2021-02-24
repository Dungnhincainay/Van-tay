//


#include <Adafruit_Fingerprint.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 
int button = 5;                   // Nối chân "touch" cảm biến

SoftwareSerial mySerial(2, 3);    // Day tim chan D2, Day xanh chan D3
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//int button = 5;
void setup()  
{
  Serial.begin(9600);
  pinMode(5, INPUT); 

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
  lcd.init();              // Xoá lcd     
  lcd.backlight();
  while (!Serial);        // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\n Please Wait....\n");

  finger.begin(57600);
  delay(15);
  if (finger.verifyPassword()) {
    Serial.println("Find done");
  } else {
    Serial.println("Sensor not Connected :(");
    lcd.setCursor(0,0);
    lcd.print("Sensor Not Connected...");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Cam bien chua "); Serial.print(finger.templateCount); 
  Serial.println(" mau van tay.");
  Serial.println("Dang doi mau van tay hop le ...");
  
  lcd.setCursor(0,0);
  lcd.print("Da ket noi...");
  lcd.setCursor(0,1);
  lcd.print(".....");
  delay(700);
  lcd.init(); // Xoa toan bo LCD
  lcd.setCursor(0,0);
  lcd.print("Cham tay vao");
  lcd.setCursor(0,1);
  lcd.print("Cam bien......");
}

void xanh_on(){
  digitalWrite(11, HIGH);
  delay(200);
}
void xanh_off(){
  digitalWrite(11, LOW);
  delay(200);
}
void do_on(){
  digitalWrite(9,HIGH);
  delay(200);
}
void do_off(){
  digitalWrite(9,LOW);
  delay(200);
}
 
void nhap_nhay()
{
    for (byte i = 0; i < 3; i++) {
        digitalWrite(12, HIGH);
        delay(200);
        digitalWrite(12 , LOW);
        delay(200);
    }
}

void loop()            // run over and over again
{
  int buttonStatus = digitalRead(5);    //Đọc trạng thái button
  if (buttonStatus == HIGH) {         // Nếu mà button bị nhấn
    digitalWrite(12, HIGH);
                     // Đèn led sáng
    }
  getFingerprintIDez();
  delay(50);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Hình ảnh đã chụp");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("Không phát hiện ngón tay");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Lỗi giao tiếp");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Lỗi hình ảnh");
      return p;
    default:
      Serial.println("Lỗi không xác định");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Dấu vân tay được chuyển đổi");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Dấu vân tay hỏng");
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
      Serial.println("Lỗi không xác định :( ");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Tìm thấy một dấu vân tay phù hợp!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Lỗi giao tiếp");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Không tìm thấy dấu vân tay phù hợp");
    return p;
  } else {
    Serial.println("Lỗi không xác định :( ");
    return p;
  }   
  
  // found a match!
  Serial.print("ID Tìm thấy #"); Serial.print(finger.fingerID); 
  Serial.print(" với độ tin cậy là "); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("ID Tìm thấy #"); Serial.print(finger.fingerID); 
  Serial.print(" với độ tin cậy là "); Serial.println(finger.confidence);

  lcd.init();
  lcd.setCursor(0,0);
  lcd.print("Tim Thay Van tay");
  lcd.setCursor(0,1);
  lcd.print(" ID:          ");
  lcd.setCursor(5,1);
  lcd.print(finger.fingerID);

  int id= finger.fingerID;
  if((id/10)==1){
    digitalWrite(11, HIGH);
    delay(500);
    digitalWrite(11,LOW);
  }else{
    if((id/10)==2){
    digitalWrite(9,HIGH);
    delay(500);
    digitalWrite(9,LOW);
    }else{
        digitalWrite(10,HIGH);
        delay(500);
        digitalWrite(10,LOW);
      }
      
  }
  return finger.fingerID; 
  delay(200);
  digitalWrite(12 , LOW);
  delay(200);
}
