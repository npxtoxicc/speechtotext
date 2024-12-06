#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Bluetooth Module Configuration
#define BT_RX 10  // Connect HC-05 TX to Arduino pin 10
#define BT_TX 11  // Connect HC-05 RX to Arduino pin 11
SoftwareSerial bluetooth(BT_RX, BT_TX);

// Scrolling text variables
String currentMessage = "";
int scrollPosition = 0;
unsigned long lastScrollTime = 0;
const int scrollDelay = 300; // Scroll delay in milliseconds

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);
  
  // Initialize Bluetooth Serial
  bluetooth.begin(9600);
  
  // Initialize OLED Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("Bluetooth OLED Ready"));
  display.display();
  delay(2000);
}

void loop() {
  // Check for Bluetooth messages
  if (bluetooth.available()) {
    // Clear previous message
    currentMessage = "";
    
    // Read incoming Bluetooth message
    while (bluetooth.available()) {
      char inChar = (char)bluetooth.read();
      currentMessage += inChar;
      delay(5); // Small delay to allow buffer to fill
    }
    
    // Reset scroll position for new message
    scrollPosition = 0;
  }
  
  // Scroll and display message
  if (currentMessage.length() > 0) { // Replace isEmpty() with length() > 0
    scrollText(currentMessage);
  }
}

void scrollText(String text) {
  // Check if it's time to scroll
  unsigned long currentMillis = millis();
  if (currentMillis - lastScrollTime >= scrollDelay) {
    // Clear the display
    display.clearDisplay();
    
    // Set cursor and print scrolling text
    display.setCursor(0, 0);
    
    // Calculate substring to display based on scroll position
    int displayLength = min(text.length() - scrollPosition, 8); // 8 characters per line
    String displayText = text.substring(scrollPosition, scrollPosition + displayLength);
    
    display.println(displayText);
    display.display();
    
    // Update scroll position
    scrollPosition++;
    
    // Reset scroll position if we've reached the end
    if (scrollPosition >= text.length()) {
      scrollPosition = 0;
    }
    
    // Update last scroll time
    lastScrollTime = currentMillis;
  }
}