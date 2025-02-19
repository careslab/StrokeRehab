#include <Servo.h>

Servo myServo;  // Servo object
int servoPin = 7;  // Pin connected to the servo

void setup() {
    Serial.begin(9600);  // Initialize Serial
    myServo.attach(servoPin);  // Attach servo to pin
    myServo.write(0);  // Set initial position to 0°
    Serial.println("Arduino ready, waiting for command...");
}

void loop() {
     Serial.println("Arduino loop entered"); 
    if (Serial.available()) {  // Check if data is available
        int flag = Serial.parseInt();  // Read integer (0 or 1)

        if (flag == 1) {
            Serial.println("Rotating to 180°...");
            myServo.write(180);
        } else if (flag == 0) {
            Serial.println("Returning to 0°...");
            myServo.write(0);
        } else {
            Serial.println("Invalid command received.");
        }

        delay(1000);  // Give the servo time to move
    }
}
