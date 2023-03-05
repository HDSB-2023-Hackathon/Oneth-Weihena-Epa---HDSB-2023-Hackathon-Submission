#include <Servo.h>
#include <LiquidCrystal.h>
#define POTENTIOMETER_PIN A0
Servo phoneLockServo;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Sets pin numbers
const int triggerPin = 7;
const int echoPin = 8; 
const int servoPin = 9;
const int buzzerPin = 13;

// Distance calculation variables
double timeToReceive;
double distance;

// Time calculation variables
double potentiometerData;
double startTime;
double currentTimeInMins;
double timeRemainingMins;

// Time display variables
int hours;
int minutes;
int seconds;
int timeSetDuration;

bool isFinished;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  phoneLockServo.attach(servoPin);
  lcd.begin(16, 2);
  startTime = millis();
  lockPhone();
  isFinished = false;
}

void loop() {
  checkPosture();
  // Time calculations
  currentTimeInMins = (millis() - startTime) / 60000;
  potentiometerData = analogRead(POTENTIOMETER_PIN);
  timeSetDuration = map(potentiometerData, 0, 1023, 0, 60); // turns input from potentiometer into time(m)
  timeRemainingMins = timeSetDuration - currentTimeInMins;
  
  // Time display calculations
  hours = timeRemainingMins / 60;
  minutes = timeRemainingMins - (hours * 60);
  seconds = (timeRemainingMins - (hours * 60) - minutes) * 60;
    
  lcd.setCursor(0, 1);
  
  // Displays remaining time on LCD screen
  if (!isFinished) {
 	  lcd.print(minutes);
  	lcd.print(":");
  	lcd.print(seconds);
  	lcd.print(" Remaining");  
  }
  
  // Unlocks phone after timer runs out 
  if (timeRemainingMins <= 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time Complete!");
    unlockPhone();
    isFinished = true;
  }
}

void checkPosture() {
  // Checks distance between user and the ultrasonic distance sensor
  digitalWrite(triggerPin,HIGH);
  delay(1000);
  digitalWrite(triggerPin,LOW);
  timeToReceive = pulseIn(echoPin,HIGH); 
  distance = timeToReceive*0.034/2; //Gets distance of the person away from the sensor in cm
  
  // Alerts the user if their distance from the sensor is above 20 cm
  if (distance >= 20 && !isFinished) {
	lcd.setCursor(0, 0);
    lcd.print("Fix Posture!");
    lcd.print("                ");
    tone(buzzerPin, 523, 100);
  }
  if (distance < 20 && !isFinished) {
  	lcd.setCursor(0, 0);
    lcd.print("Keep On Working!");
  }
}

void lockPhone() {
  // Locks the phone lock by turning the servo 90 degrees
  phoneLockServo.write(90);
}

void unlockPhone() {
  // Unlocks the phone lock by turning the servo back to original position
  phoneLockServo.write(0);
}