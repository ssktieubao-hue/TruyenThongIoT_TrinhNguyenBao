const int buttonPin = 2;
unsigned long pressStart = 0;
bool setupMode = false;
int baudIndex = 0;
long baudRates[] = {9600, 19200, 38400, 57600};
bool buttonState = HIGH;
bool lastButtonState = HIGH;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // dùng điện trở kéo lên
  Serial.begin(baudRates[baudIndex]);
  Serial.println("Khoi dong o toc do 9600");
}

void loop() {
  buttonState = digitalRead(buttonPin);

  // Khi nhấn xuống
  if (buttonState == LOW && lastButtonState == HIGH) {
    pressStart = millis();
    delay(50); // chống dội
  }

  // Khi thả ra
  if (buttonState == HIGH && lastButtonState == LOW) {
    unsigned long pressTime = millis() - pressStart;

    if (pressTime > 1000) { // tạm test 1 giây cho dễ
      setupMode = !setupMode;
      if (setupMode)
        Serial.println("Da vao che do cai dat");
      else
        Serial.println("Da thoat che do cai dat");
    } else if (setupMode) {
      baudIndex = (baudIndex + 1) % 4;
      Serial.end();
      Serial.begin(baudRates[baudIndex]);
      Serial.print("Toc do moi: ");
      Serial.println(baudRates[baudIndex]);
    }
  }

  lastButtonState = buttonState;
  if (!setupMode) {

    Serial.println("Hello IoT!");

    delay(1000);

  }
}
