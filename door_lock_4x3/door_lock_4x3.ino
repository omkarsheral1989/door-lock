
#include <Keypad.h>
#include <ESP32Servo.h>

// install library: Keypad by Mark Stanley, Alexander Brevig.
// 
//   1       2       3 
// 
//   4       5       6 
// 
//   7       8       9
// 
//   *       0       #
// 
// 1   2   3   4   5   6   7
// ☉   ☉   ☉   ☉   ☉   ☉   ☉ 
// C2, R1, C1, R4, C3, R3, R2
// 25  13  26  27  33  14  12   - uC pin no
// 14  2   13  12  15  5   4    - uC pin no


const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'},
};
// byte rowPins[ROWS] = {13, 12, 14, 27};   // R1, R2, R3, R4
// byte colPins[COLS] = {26, 25, 33};       // C1, C2, C3
byte rowPins[ROWS] = {2, 4, 5, 12};   // R1, R2, R3, R4
byte colPins[COLS] = {13, 14, 15};       // C1, C2, C3
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


const String PASSWORD1 = String("0000");
const byte PASSWORD_MAX_LEN = 10;

const byte SERVO_CLOSE_ANGLE = 70;
const byte SERVO_OPEN_ANGLE = 160;
const byte BUZZER_PIN = 21;
static const int servoPin = 23;
const byte LED_PIN = 19;


String inputPassword = String("");


Servo servo1;

void buzzerOn() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void buzzerOff(){
  pinMode(BUZZER_PIN, INPUT);
  digitalWrite(BUZZER_PIN, HIGH);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  buzzerOff();
  servo1.attach(servoPin);
  servo1.write(SERVO_CLOSE_ANGLE);

  Serial.begin(9600);
  while (!Serial) {
    delay(20);  // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("\n\nInitializing...");
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
    buzzerOn();
    delay(80);
    buzzerOff();

    if (key == '*') {
      inputPassword = String("");
      Serial.println("Password reset");
    } else if (inputPassword.length() < PASSWORD_MAX_LEN) {
      inputPassword += key;
      // Serial.println(inputPassword);
      if (inputPassword == PASSWORD1) {
        Serial.println("\nPassword correct");
        inputPassword = String("");
        servo1.write(SERVO_OPEN_ANGLE);
        buzzerOn();
        delay(2000);
        buzzerOff();
        servo1.write(SERVO_CLOSE_ANGLE);
      }
    }
  }
}
