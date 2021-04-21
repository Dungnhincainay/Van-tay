//


#include <Adafruit_Fingerprint.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2); 
SoftwareSerial mySerial(2, 3);    // Day tim chan D2, Day xanh chan D3
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//int button = 5;                   // Nối chân "touch" cảm biến
int Van_tay;
#define INTERRUPT_PIN 5

int led = 6;
int flag= 0;
int ledState = LOW;
int touchStatus = LOW;
byte Status = LOW;
unsigned long Ago = 0;
unsigned long Now = 0;
const long Set_time = 300; // Thời gian đèn led nhấp nháy

void ICACHE_RAM_ATTR Tim_Van_Tay(){
  
  digitalWrite(12, HIGH);   // Đèn led sáng
  touchStatus = HIGH;
}

void setup()  
{
  Serial.begin(9600);
  pinMode(5, INPUT);      // Touch

  pinMode(9, OUTPUT);     // Cop
  pinMode(10, OUTPUT); 
  pinMode(11, OUTPUT);    // Khoa
  pinMode(12, OUTPUT);    // real time
  pinMode(13, OUTPUT);    // Led
  
  
  //pinMode(INTERRUPT_PIN, INPUT_PULLUP);                   // CHÚ Ý 2 DÒNG NÀY_____Khai báo Ngắt
  //attachInterrupt(INTERRUPT_PIN, Tim_Van_Tay, FALLING);  // CHÚ Ý 2 DÒNG NÀY_____RISING- có 5 chế độ ngắt

  lcd.init();              // Xoá lcd     
  lcd.backlight();
  while (!Serial);        // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\n Please Wait....\n");
  finger.begin(57600);
  delay(15);

  if (finger.verifyPassword()) {
    lcd.setCursor(3,0);
    lcd.print("Find done !");
    Serial.println("Find done");
  } else {
    Serial.println("Sensor not Connected :(");
    lcd.setCursor(0,0);
    lcd.print("Sensor Not Connected...");
    while (1) { delay(1); }
  }
  Start_Up();
}

void do_on(){
  digitalWrite(9,HIGH);
  delay(200);
}
void do_off(){
  digitalWrite(9,LOW);
  delay(200);
}
void xanh_on(){
  digitalWrite(11, HIGH);
  delay(200);
}
void xanh_off(){
  digitalWrite(11, LOW);
  delay(200);
}
 
void nhap_nhay()
{
    for (byte i = 1; i < 3; i++) {  // Nhap nhay 2 lan
        digitalWrite(13, HIGH);
        delay(200);
        digitalWrite(13, LOW);
        delay(200);
    }
}

void loop()            // run over and over again
{
  //int buttonStatus = digitalRead(5);    //Đọc trạng thái button
  if (touchStatus){  // Phát hiện chạm
    nhap_nhay();
    touchStatus = Low;
    digitalWrite(12, LOW);
  }
  digitalWrite(11, Status);

  Now = millis();                 // Nhấp nháy led
  if (Now - Ago >= Set_time) {    // Nhấp nháy led
    Ago = Now;                    // Nhấp nháy led
    if (ledState == LOW) {        // Nhấp nháy led
      ledState = HIGH;            // Nhấp nháy led
    } else {                      // Nhấp nháy led
      ledState = LOW;             // Nhấp nháy led
    }                             // Nhấp nháy led
    digitalWrite(led, ledState);      
  }

  getFingerprintIDez();
  delay(50);            //don't ned to run this at full speed.
  
  if (Van_tay< 0){      //  Tắt tất cả
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);

    digitalWrite(12, HIGH);
    delay(500);
    digitalWrite(12,LOW);
  }
  if (Van_tay>=5){
    if((Van_tay/10)==1){  // Mở khoá
      Status = !Status;
      digitalWrite(11, Status);
      delay(2000);
    }else{
      if((Van_tay/10)==2){  // Mở Cốp
      digitalWrite(9,HIGH);
      delay(500);
      digitalWrite(9,LOW);
      }else{              // Bat den
        digitalWrite(10,HIGH);
        delay(500);
        digitalWrite(10,LOW);
      }
    }
  }
  
}


void Start_Up(){

  finger.getTemplateCount();
  Serial.print("Cam bien chua "); 
  Serial.print(finger.templateCount); 
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
  }  // OK success!

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
  }  // OK converted!

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Tìm thấy một dấu vân tay phù hợp!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Lỗi giao tiếp");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Van_tay = -1;
    Serial.println("Không tìm thấy dấu vân tay phù hợp");
    return p;
  } else {
    Serial.println("Lỗi không xác định :( ");
    return p;
  }    // found a match!
  
  Van_tay = finger.fingerID;
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
  Van_tay = finger.fingerID;
  Serial.print(" ID Tìm thấy #"); Serial.print(finger.fingerID); 
  Serial.print(" với độ tin cậy là "); Serial.println(finger.confidence);

  lcd.init();
  lcd.setCursor(0,0);
  lcd.print("Tim Thay Van tay");
  lcd.setCursor(0,1);
  lcd.print(" ID:          ");
  lcd.setCursor(5,1);
  lcd.print(finger.fingerID);

  return finger.fingerID; 
  delay(200);
  digitalWrite(12 , LOW);
}
