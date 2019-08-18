/* Start of Code */

#include <MFRC522.h> // Include of the RC522 Library
#include "FastLED.h" // Include of the FastLED library
#include <SPI.h> // Used for communication via SPI with the Module

// WS2812 RGB Stick connection
#define led_pin 5 // Pin 5 connected to DIN of RGB Stick
#define NUM_LEDS 8 // # of WS2812 LEDs on stick
CRGB leds[NUM_LEDS]; // FastLED Library Init

#define SDAPIN 10 // RFID Module SDA Pin is connected to the UNO 10 Pin
#define RESETPIN 8 // RFID Module RST Pin is connected to the UNO 8 Pin

#define Buzzer 3 // Pin 3 connected to + pin of the Buzzer

byte FoundTag; // Variable used to check if Tag was found
byte ReadTag; // Variable used to store anti-collision value to read Tag information
byte TagData[MAX_LEN]; // Variable used to store Full Tag Data
byte TagSerialNumber[5]; // Variable used to store only Tag Serial Number
byte GoodTagSerialNumber[5] = {0x95, 0xEB, 0x17, 0x53}; // The Tag Serial number we are looking for

MFRC522 nfc(SDAPIN, RESETPIN); // Init of the library using the UNO pins declared above

void setup() {
pinMode(Buzzer, OUTPUT); // Set buzzer pin to an Output pin
digitalWrite(Buzzer, LOW); // Buzzer Off at startup
SPI.begin();
Serial.begin(115200);

FastLED.addLeds<NEOPIXEL,led_pin>(leds, NUM_LEDS); // Setup FastLED Library
FastLED.clear();

// Start to find an RFID Module
Serial.println("Looking for RFID Reader");
nfc.begin();
byte version = nfc.getFirmwareVersion(); // Variable to store Firmware version of the Module

// If can't find an RFID Module 
if (! version) { 
Serial.print("Didn't find RC522 board.");
while(1); //Wait until a RFID Module is found
}

// If found, print the information about the RFID Module
Serial.print("Found chip RC522 ");
Serial.print("Firmware version: 0x");
Serial.println(version, HEX);
Serial.println();
}

void loop() {

// Light up starting LED's
for (int x = 0; x < 8; x++) {
leds[x] = CRGB::Blue;
}
FastLED.setBrightness(50);
FastLED.show(); 

String GoodTag="False"; // Variable used to confirm good Tag Detected

// Check to see if a Tag was detected
// If yes, then the variable FoundTag will contain "MI_OK"
FoundTag = nfc.requestTag(MF1_REQIDL, TagData);

if (FoundTag == MI_OK) {
delay(200);

// Get anti-collision value to properly read information from the Tag
ReadTag = nfc.antiCollision(TagData);
memcpy(TagSerialNumber, TagData, 4); // Write the Tag information in the TagSerialNumber variable

Serial.println("Tag detected.");
Serial.print("Serial Number: ");
for (int i = 0; i < 4; i++) { // Loop to print serial number to serial monitor
Serial.print(TagSerialNumber[i], HEX);
Serial.print(", ");
}
Serial.println("");
Serial.println();


// Check if detected Tag has the right Serial number we are looking for 
for(int i=0; i < 4; i++){
if (GoodTagSerialNumber[i] != TagSerialNumber[i]) {
break; // if not equal, then break out of the "for" loop
}
if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
GoodTag="TRUE";
} 
}
if (GoodTag == "TRUE"){
Serial.println("Success!!!!!!!");
Serial.println();
for(int x = 0; x < 8; x++) {
leds[x] = CRGB::Green;
}
FastLED.show();
for (int y = 0; y < 3; y++){
digitalWrite (Buzzer, HIGH) ;// Buzzer On
delay (50) ;// Delay 1ms 
digitalWrite (Buzzer, LOW) ;// Buzzer Off
delay (50) ;// delay 1ms
}
delay(1500);
}
else {
Serial.println("TAG NOT ACCEPTED...... :(");
Serial.println();
for(int x = 0; x < 8; x++) {
leds[x] = CRGB::Red;
}
FastLED.show();
for (int y = 0; y < 3; y++){
digitalWrite (Buzzer, HIGH) ;// Buzzer On
delay (300) ;// Delay 1ms 
digitalWrite (Buzzer, LOW) ;// Buzzer Off
delay (400) ;// delay 1ms
}
delay(500); 
}
}
}

/* End of Code */
