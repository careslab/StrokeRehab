#include <Servo.h>
//note nano esp32 uses a different library and pin layout; A1 is not the same in nano32 vs uno
//#include <ESP32Servo.h>

#define NUM_MOTORS 7
#define BUZZ_DURATION 500  // divide by 1,000 to go from ms to s

int motor_flag[NUM_MOTORS] = { 0 };                    // Array to store received values
//
//int motor_pin[NUM_MOTORS] = { 2, 3, 4, 5, 6, 7, 8 };  // Assign motor control pins: index buzzer, x+/-, y+/-, z+/-
//this pin arrangement works for arduino nanoESP32 (see pingames file and sticky note for proper mapping; A1!=A1)
//
int motor_pin[NUM_MOTORS] = { A1, 2, 3, 4, 5, 6, 7 };  // Assign motor control pins: index buzzer, x+/-, y+/-, z+/-
//this pin arrangement works for arduino uno
//
unsigned long last_buzz_time[NUM_MOTORS] = { 0 };  //refractory timer for each motor
unsigned long last_serial_time = 0;
uint8_t incomingByte;

//updated circa 3/19/25
//Servo myServo;  // Servo object
//int button0Pin = A1;  // Pin connected to  button0
//int indexServoPin = A1;
// int XPosPin = 2;
// int XNegPin = 3;
// int YPosPin = 4;
// int YNegPin = 5;
// int ZPosPin = 6;
// int ZNegPin = 7;
int led13 = 13;
int holder = 0;
Servo indexServo;

int buttonState;
int xState = 0, yState = 0, zState = 0;  // Default to neutral position
void setup() {
  Serial.begin(9600);  // Initialize Serial

  for (int i = 0; i < NUM_MOTORS; i++) {
    pinMode(motor_pin[i], OUTPUT);
  }

  pinMode(led13, OUTPUT);
  indexServo.attach(A0);
  Serial.println("Arduino ready, waiting for command...");
}

void loop() {
  unsigned long current_time = millis();

  if (Serial.available()) {
    incomingByte = Serial.read();
    Serial.println(incomingByte);
    last_serial_time = current_time;

    for (int i = 0; i < NUM_MOTORS; i++) {
      int new_flag = (incomingByte >> i) & 0x01;

      if (i == 0) {
        motor_flag[i] = new_flag;  // button0 flag is stateful
      } else if (new_flag == 1) {
        motor_flag[i] = 1;
        last_buzz_time[i] = current_time;  // Record activation time
      }
    }
  }


  // Activate buzzers based on state
  //button0 buzzer
  if (motor_flag[0] == 1) {
    analogWrite(motor_pin[0], 255);
    indexServo.write(180);
    digitalWrite(led13, HIGH);
  } else {
    analogWrite(motor_pin[0], 0);
    indexServo.write(0);
    digitalWrite(led13, LOW);
  }

  for (int i = 1; i < NUM_MOTORS; i++) {
    if (motor_flag[i] == 1 && (current_time - last_buzz_time[i] <= BUZZ_DURATION)) {
      analogWrite(motor_pin[i], 255);  // Vibrate
    } else {
      analogWrite(motor_pin[i], 0);  // Stop
      motor_flag[i] = 0;             // Reset flag
    }
  }
}
