
#include <Keypad.h>
#include <ESP32Servo.h>


const String PASSWORD1 = String("0000");
const byte PASSWORD_MAX_LEN = 10;

const byte SERVO_CLOSE_ANGLE = 170;
const byte SERVO_OPEN_ANGLE = 70;
const byte BUZZER_PIN = 21;
static const int servoPin = 23;


String inputPassword = String("");



const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '4', '7', '*' },
  { '2', '5', '8', '0' },
  { '3', '6', '9', '#' },
  { 'A', 'B', 'C', 'D' }
};
byte rowPins[4] = { 13, 12, 14, 27 };
byte colPins[4] = { 26, 25, 33, 32 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

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

    if (key == 'A') {
      inputPassword = String("");
      Serial.println("Password reset");
    } else if (inputPassword.length() < PASSWORD_MAX_LEN) {
      inputPassword += key;
      Serial.println(inputPassword);
      if (inputPassword == PASSWORD1) {
        Serial.println("Password correct");
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
