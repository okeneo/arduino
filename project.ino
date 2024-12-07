#include <Servo.h>  //include the servo library

Servo myservo;  //create a servo object

const int trigPin = 11;  //connects to the trigger pin on the distance sensor
const int echoPin = 12;  //connects to the echo pin on the distance sensor

const int servoPin = 9;

const int frontGreenPin = 7;
const int frontRedPin = 5;

const int backBluePin = 4;
const int backRedBin = 6;
const int backYellowPin = 8;
const int backGreenPin = 10;

const int buzzerPin = 3;

const int redButtonPin = 2;

int distThreshold = 3;  // in inches

float distance = 0;  //stores the distance measured by the distance sensor
int servoPos = 1;    //the servo will move to this position

bool serviceRunning = true;

float temp = 0;

void setup() {
  Serial.begin(9600);  //set up a serial connection with the computer

  pinMode(trigPin, OUTPUT);  //the trigger pin will output pulses of electricity
  pinMode(echoPin, INPUT);   //the echo pin will measure the duration of pulses coming back from the distance sensor

  myservo.attach(servoPin);  //tell the servo object that its servo is plugged into pin 9

  pinMode(frontGreenPin, OUTPUT);
  pinMode(frontRedPin, OUTPUT);

  pinMode(backBluePin, OUTPUT);
  pinMode(backRedBin, OUTPUT);
  pinMode(backYellowPin, OUTPUT);
  pinMode(backGreenPin, OUTPUT);

  pinMode(buzzerPin, OUTPUT);

  pinMode(redButtonPin, INPUT_PULLUP);
}

void loop() {
  digitalWrite(frontRedPin, HIGH);

  digitalWrite(backBluePin, HIGH);
  digitalWrite(backRedBin, HIGH);
  digitalWrite(backYellowPin, HIGH);
  digitalWrite(backGreenPin, HIGH);

  temp = getTemp();
  Serial.println(temp);

  if (digitalRead(redButtonPin) == LOW) {
    if (serviceRunning == true) {
      serviceRunning = false;
      criticalAlarm();
    } else {
      serviceRunning = true;
    }
    // Button debouce.
    delay(500);
  }

  if (serviceRunning == true) {
    distance = getDistance();  //variable to store the distance measured by the sensor

    if (distance < distThreshold) {
      // Turn off the red light and keep the green on for
      // a short amount of time.
      digitalWrite(frontRedPin, LOW);
      digitalWrite(frontGreenPin, HIGH);
      moveTollGate();
      digitalWrite(frontGreenPin, LOW);
      digitalWrite(frontRedPin, HIGH);
      delay(5000);  //add delay how much you want
    } else {
      digitalWrite(frontRedPin, HIGH);
      if (servoPos == 1) {
        moveTollDown();
      }
    }

    delay(50);  //delay 50ms between each distance reading
  }
}


//------------------FUNCTIONS-------------------------------
//RETURNS THE DISTANCE MEASURED BY THE HC-SR04 DISTANCE SENSOR
float getDistance() {
  float echoTime;            //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance;  //variable to store the distance calculated from the echo time

  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  echoTime = pulseIn(echoPin, HIGH);  //use the pulsein command to see how long it takes for the
                                      //pulse to bounce back to the sensor

  calculatedDistance = echoTime / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)

  return calculatedDistance;  //send back the distance that was calculated
}

float getTemp() {
  float voltage;   //the voltage measured from the TMP36
  float degreesC;  //the temperature in Celsius, calculated from the voltage
  // float degreesF;  //the temperature in Fahrenheit, calculated from the voltage

  voltage = analogRead(A0) * 0.004882813;  //convert the analog reading, which varies from 0 to 1023, back to a voltage value from 0-5 volts
  degreesC = (voltage - 0.5) * 100.0;      //convert the voltage to a temperature in degrees Celsius
  // degreesF = degreesC * (9.0 / 5.0) + 32.0;  //convert the voltage to a temperature in degrees Fahrenheit
  return degreesC;
}

void criticalAlarm() {
  for (int i = 0; i < 5; i++) {  // Repeat 5 times for a strong alarm effect
    tone(buzzerPin, 880, 200);   // A5
    delay(250);                  // Short pause between tones
    tone(buzzerPin, 698, 200);   // F5
    delay(250);
  }
}

void moveTollGate() {
  moveTollUp();
  delay(500);
  moveTollDown();
}

void moveTollUp() {
  for (servoPos = 1; servoPos <= 120; servoPos += 1) {
    myservo.write(servoPos);
    delay(15);
  }
}

void moveTollDown() {
  for (servoPos = 120; servoPos >= 1; servoPos -= 1) {
    myservo.write(servoPos);
    delay(15);
  }
}
