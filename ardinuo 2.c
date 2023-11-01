#include<wifi.h>
#include <Stepper.h>
#include <NewPing.h>
#include <TimeLib.h>
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
const int stepsPerRevolution = 2048; // Adjust this value based on your stepper motor
Stepper myStepper(stepsPerRevolution, 16, 17, 18, 19);
const int motorSpeed = 5; // Adjust this value based on your motor's speed requirements
const int waterPumpPin = 23;
const int ultrasonicTriggerPin = 24;
const int ultrasonicEchoPin = 25;
const int ledPin = 26;
NewPing sonar(ultrasonicTriggerPin, ultrasonicEchoPin, 200); // Adjust the max distance (200cm) as needed
const int morningHour = 7; // Set the hour for the morning activation
const  int eveningHour = 19; // Set the hour for the evening activation
void setup() {
Serial.begin(115200);
// Initialize components
pinMode(waterPumpPin, OUTPUT);
pinMode(ledPin, OUTPUT);
// Connect to Wi-Fi
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.println("Connecting to WiFi...");
 }
 Serial.println("Connected to WiFi");
 // Initialize Time library
 configTime(0, 0, "pool.ntp.org", "time.nist.gov"); // Configure NTP servers
}
void loop() {
// Update time
struct tm timeinfo;
if(!getLocalTime(&timeinfo));
 Serial.println("Failed to obtain time");
 return;
 }
int currentHour = timeinfo.tm_hour;
if ((currentHour == morningHour || currentHour == eveningHour) && timeinfo.tm_min == 0 && timeinfo.tm_sec == 0) {
// It's the specified activation hour and minute
 // Check water level
 int distance = sonar.ping_cm();
Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");
 if (distance < 10) {
 // Water level is low, turn on the LED and water pump
 digitalWrite(ledPin, HIGH);
 digitalWrite(waterPumpPin, HIGH);
 // Dispense water using the stepper motor
  myStepper.setSpeed(motorSpeed);
 myStepper.step(stepsPerRevolution);
 // Wait for dispensing to complete
 delay(5000); // Adjust as needed
 // Turn off water pump and LED
digitalWrite(waterPumpPin, LOW);
digitalWrite(ledPin, LOW);
 }
  }
 delay(1000); // Adjust delay as needed for your application
}
