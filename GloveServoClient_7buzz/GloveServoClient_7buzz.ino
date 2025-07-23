#include <Servo.h>
#define NUM_MOTORS 6


int motor_flag[NUM_MOTORS] = {0}; // Array to store received values
int motor_pins[NUM_MOTORS] = {2, 3, 4, 5, 6, 7}; // Assign motor control pins x+/-, y+/-, z+/-

//updated circa 3/19/25
//Servo myServo;  // Servo object
int button0Pin = A1;  // Pin connected to  button0
//int indexServoPin = A1;
// int XPosPin = 2;
// int XNegPin = 3;
// int YPosPin = 4;
// int YNegPin = 5;
// int ZPosPin = 6;
// int ZNegPin = 7;
int led13=13;
int holder=0;
Servo indexServo;

int buttonState;
int xState = 0, yState = 0, zState = 0;  // Default to neutral position
void setup() {
    Serial.begin(9600);  // Initialize Serial

    pinMode(button0Pin, OUTPUT);
    // pinMode(XPosPin, OUTPUT);
    // pinMode(XNegPin, OUTPUT);
    // pinMode(YPosPin, OUTPUT);
    // pinMode(YNegPin, OUTPUT);
    // pinMode(ZPosPin, OUTPUT);
    // pinMode(ZNegPin, OUTPUT);
    for (int i = 0; i < NUM_MOTORS; i++) {
        pinMode(motor_pins[i], OUTPUT);
    }

    pinMode(led13, OUTPUT);


    indexServo.attach(A0);

    Serial.println("Arduino ready, waiting for command...");

}

void loop() {
    if (Serial.available() > 0) {
        int flag = Serial.parseInt();
        holder = flag; //send flag into holder
        Serial.println(holder);

        // Decode button state
        buttonState = holder & 1;

        // Decode movement directions
        xState = ((holder >> 1) & 3) - 1;  // Extract bits and remap
        yState = ((holder >> 3) & 3) - 1;
        zState = ((holder >> 5) & 3) - 1;

        //Serial.print("Button: "); Serial.print(buttonState);
        Serial.print(" | X: "); Serial.print(xState);
        Serial.print(" | Y: "); Serial.print(yState);
        Serial.print(" | Z: "); Serial.println(zState);
    }

    // Activate buzzers based on state
    if (buttonState == 1) {
        analogWrite(button0Pin, 255);
        indexServo.write(180);
        digitalWrite(led13, HIGH);
    } else {
        analogWrite(button0Pin, 0);
        indexServo.write(0);
        digitalWrite(led13, LOW);
    }
    if (xState == 1){ digitalWrite(XPosPin, HIGH);}
    if (xState == -1){ digitalWrite(XNegPin, HIGH);}
    
    // Map movement states to buzzers (example pin numbers)
    //digitalWrite(2, (xState == -1));  // X negative
    //digitalWrite(3, (xState == 1));   // X positive
    //digitalWrite(4, (yState == -1));  // Y negative
    //digitalWrite(5, (yState == 1));   // Y positive
    //digitalWrite(6, (zState == -1));  // Z negative
    //digitalWrite(7, (zState == 1));   // Z positive
    digitalWrite(XPosPin, LOW);
    digitalWrite(XPosPin, LOW);
}
