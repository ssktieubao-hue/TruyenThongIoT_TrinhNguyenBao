#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

// ====== LCD + RTC ======
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;

// ====== CHÂN NÚT ======
#define NUT_SETTING 8
#define NUT_UP      9
#define NUT_DOWN    10
#define NUT_SAVE    11
#define BUZZER      4

// ====== BIẾN BÁO THỨC ======
int gioBaoThuc = 6;
int phutBaoThuc = 0;

// ====== TRẠNG THÁI ======
bool cheDoSet = false;
bool setGio = true;     // true: set giờ, false: set phút
bool baoThucDaLuu = false;

// ====== CHỐNG DỘI NÚT ======
bool docNut(int chan) {
  if (digitalRead(chan) == LOW) {
    delay(200);
    return true;
  }
  return false;
}

void setup() {
  pinMode(NUT_SETTING, INPUT_PULLUP);
  pinMode(NUT_UP, INPUT_PULLUP);
  pinMode(NUT_DOWN, INPUT_PULLUP);
  pinMode(NUT_SAVE, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();

  Wire.begin();
  rtc.begin();

  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {
  DateTime now = rtc.now();

  // ====== NÚT SETTING ======
  if (docNut(NUT_SETTING)) {
    cheDoSet = true;
    setGio = true;
    baoThucDaLuu = false;
  }

  // ====== CHẾ ĐỘ SET BÁO THỨC ======
  if (cheDoSet) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SET BAO THUC");

    lcd.setCursor(0, 1);
    lcd.print("GIO:");
    if (setGio) lcd.print(">");
    lcd.print(gioBaoThuc);

    lcd.print(" PHUT:");
    if (!setGio) lcd.print(">");
    lcd.print(phutBaoThuc);

    // UP
    if (docNut(NUT_UP)) {
      if (setGio) {
        gioBaoThuc = (gioBaoThuc + 1) % 24;
      } else {
        phutBaoThuc = (phutBaoThuc + 1) % 60;
      }
    }

    // DOWN
    if (docNut(NUT_DOWN)) {
      if (setGio) {
        gioBaoThuc--;
        if (gioBaoThuc < 0) gioBaoThuc = 23;
      } else {
        phutBaoThuc--;
        if (phutBaoThuc < 0) phutBaoThuc = 59;
      }
    }

    // SAVE
    if (docNut(NUT_SAVE)) {
      if (setGio) {
        setGio = false;   // lần 1
      } else {
        cheDoSet = false; // lần 2 → lưu
        baoThucDaLuu = true;
        lcd.clear();
        lcd.print("DA LUU BAO THUC");
        delay(1000);
      }
    }

    return;
  }

  // ====== HIỂN THỊ GIỜ ======
  lcd.setCursor(0, 0);
  lcd.print("TIME ");
  if (now.hour() < 10) lcd.print("0");
  lcd.print(now.hour());
  lcd.print(":");
  if (now.minute() < 10) lcd.print("0");
  lcd.print(now.minute());
  lcd.print(":");
  if (now.second() < 10) lcd.print("0");
  lcd.print(now.second());

  lcd.setCursor(0, 1);
  lcd.print("AL ");
  if (gioBaoThuc < 10) lcd.print("0");
  lcd.print(gioBaoThuc);
  lcd.print(":");
  if (phutBaoThuc < 10) lcd.print("0");
  lcd.print(phutBaoThuc);

  // ====== KIỂM TRA BÁO THỨC ======
  if (baoThucDaLuu &&
      now.hour() == gioBaoThuc &&
      now.minute() == phutBaoThuc &&
      now.second() == 0) {

    for (int i = 0; i < 10; i++) {
      digitalWrite(BUZZER, HIGH);
      delay(200);
      digitalWrite(BUZZER, LOW);
      delay(200);
    }
  }
}
