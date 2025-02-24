#include <ESP8266WiFi.h>           // Include the WiFi library for ESP8266
#include <Adafruit_GFX.h>          // Include the Adafruit GFX library
#include <Adafruit_SSD1306.h>      // Include the Adafruit SSD1306 library

// Wi-Fi credentials
const char* ssid = "";           // Replace with your Wi-Fi SSID
const char* password = ""; // Replace with your Wi-Fi password

// ThingSpeak credentials
const char* apiKey = "OBBBXZSCFQXEWDN0";  // Replace with your ThingSpeak API Key
const char* server = "api.thingspeak.com";

const int analogPin = A0;  // Define the analog pin to which the sensor is connected
const int buzzerPin = D5;  // Define the pin for the buzzer
int sensorValue = 0;
int threshold = 60;        // The threshold value for comparison

WiFiClient client;

// OLED display configuration
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Variables for heart animation
int heartSize = 10;
bool grow = true;

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Set up the buzzer pin
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connecting to Wi-Fi...");
  display.display();

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
    display.println(".");
    display.display();
  }

  Serial.println("Wi-Fi connected");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Wi-Fi connected");
  display.display();
}

void loop() {
  // Read the analog value from the sensor
  sensorValue = analogRead(analogPin);

  // Check against the threshold
  if (sensorValue < threshold) {
    sensorValue = 0;
  }

  // Trigger buzzer if sensor value exceeds 150
  if (sensorValue > 150) {
    triggerBuzzer();
  } else {
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
  }

  // Display the beating heart and BPM on the OLED
  display.clearDisplay();
  showBeatingHeart();
  displayBPM();

  // Attempt to send data to ThingSpeak if Wi-Fi is connected
  if (WiFi.status() == WL_CONNECTED) {
    sendToThingSpeak(sensorValue);
  } else {
    Serial.println("Wi-Fi not connected, retrying...");
  }

  delay(200);  // Delay for heart beat effect
}

// Function to trigger the buzzer
void triggerBuzzer() {
  digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
  delay(100);                    // Short beep
  digitalWrite(buzzerPin, LOW);  // Turn off the buzzer
  delay(100);
}

// Function to send data to ThingSpeak
void sendToThingSpeak(int value) {
  if (client.connect(server, 80)) {
    String getStr = "/update?api_key=" + String(apiKey) + "&field1=" + String(value);
    client.print(String("GET ") + getStr + " HTTP/1.1\r\n" +
                 "Host: " + server + "\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.print("Sending data to ThingSpeak: ");
    Serial.println(value);

    delay(100);  // Wait for server response

    while (client.available()) {
      String response = client.readStringUntil('\r');
      Serial.print(response);
    }

    client.stop();
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Connection to ThingSpeak failed");
  }
}

// Function to show a beating heart on the OLED display
void showBeatingHeart() {
  int leftCircleX = 32;
  int rightCircleX = 64;
  int centerY = 32;
  int triangleY = 54;

  if (grow) {
    heartSize++;
    if (heartSize >= 14) grow = false;
  } else {
    heartSize--;
    if (heartSize <= 10) grow = true;
  }

  display.fillCircle(leftCircleX, centerY, heartSize, SSD1306_WHITE);
  display.fillCircle(rightCircleX, centerY, heartSize, SSD1306_WHITE);
  display.fillTriangle(leftCircleX - heartSize, centerY,
                       rightCircleX + heartSize, centerY,
                       (leftCircleX + rightCircleX) / 2, triangleY, SSD1306_WHITE);
  display.display();
}

// Function to display BPM reading
void displayBPM() {
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("BPM: ");
  display.println(sensorValue);
  display.display();
}
