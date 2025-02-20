Automated Street Light Control System using LDR and Arduino

This project automatically turns street lights ON and OFF based on ambient light levels using an LDR (Light Dependent Resistor) and Arduino.

Hardware Required:
1.	Arduino Uno (or any compatible microcontroller)
2.	LDR (Light Dependent Resistor)
3.	10KΩ Resistor (for LDR voltage divider)
4.	Relay Module (5V) (to control high-power lights)
5.	Bulb/LED Street Light (230V AC or 12V DC, depending on the relay)
6.	BC547 Transistor (if needed for relay switching)
7.	Diode (1N4007 for back EMF protection)
8.	Connecting Wires
9.	Power Supply (5V for Arduino, external power for relay and light)

 Arduino Code:

#define LDR_PIN A0 // LDR connected to Analog pin A0
#define RELAY_PIN 7 // Relay connected to Digital pin 7

void setup() {
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH); // Turn OFF light initially
    Serial.begin(9600);
}

void loop() {
    int ldrValue = analogRead(LDR_PIN); // Read LDR value
    Serial.println(ldrValue); // Display value in Serial Monitor
    
    if (ldrValue < 300) { // Threshold for darkness
        digitalWrite(RELAY_PIN, LOW); // Turn ON light
    } else {
        digitalWrite(RELAY_PIN, HIGH); // Turn OFF light
    }
    
    delay(500); // Small delay to prevent flickering
}
Explanation:
•	The LDR reads ambient light levels.
•	If it's dark (LDR value < 300), the relay is activated, turning the light ON.
•	If it's bright (LDR value >= 300), the relay is deactivated, turning the light OFF.

Circuit Diagram:
Basic Connection Overview:
•	LDR: One leg connected to 5V, the other leg connected to A0 and a 10KΩ resistor to GND (voltage divider).
•	Relay: IN pin to Arduino pin 7, VCC to 5V, GND to GND.
•	Light (AC Bulb or LED) connected to relay output (NO & COM terminals).




 


