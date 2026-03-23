/**
 * Display Implementation
 */

#include "display.h"
#include "config.h"

// Include display library (SSD1306)
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Display object
Adafruit_SSD1306 display(128, 64, &Wire, -1);

DisplayMgr::DisplayMgr() {
    type = DISPLAY_TYPE;
    initialized = false;
    currentPage = 0;
}

void DisplayMgr::init() {
    DEBUG_PRINTLN("Initializing Display...");
    
    // Initialize I2C
    Wire.begin(DISPLAY_I2C_SDA, DISPLAY_I2C_SCL);
    
    // Initialize SSD1306 display
    if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS)) {
        DEBUG_PRINTLN("SSD1306 allocation failed");
        initialized = false;
        return;
    }
    
    // Display settings
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    
    // Show startup message
    display.println("Smart Device");
    display.println("Firmware v1.0");
    display.display();
    
    initialized = true;
    DEBUG_PRINTLN("Display Initialized");
}

void DisplayMgr::showMessage(const char* message, int line) {
    if (!initialized) return;
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, line * 10);
    display.println(message);
    display.display();
}

void DisplayMgr::showPowerInfo(float voltage, float current, float capacity, bool charging) {
    if (!initialized) return;
    
    display.clearDisplay();
    display.setTextSize(1);
    
    // Title
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("=== Power ===");
    
    // Voltage
    display.setCursor(0, 12);
    display.print("V: ");
    display.print(voltage, 2);
    display.println(" V");
    
    // Current
    display.print("I: ");
    display.print(current, 3);
    display.println(" A");
    
    // Capacity bar
    display.print("Bat: ");
    int barWidth = (capacity / 100.0) * 70;
    for (int i = 0; i < 70; i++) {
        display.print(i < barWidth ? "#" : "-");
    }
    display.println();
    display.print("    ");
    display.print(capacity, 0);
    display.println("%");
    
    // Charging indicator
    if (charging) {
        display.setTextColor(SSD1306_WHITE);
        display.println("⚡ Charging");
    }
    
    display.display();
}

void DisplayMgr::showRelayStatus(std::vector<bool> relayStates) {
    if (!initialized) return;
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // Title
    display.setCursor(0, 0);
    display.println("=== Relays ===");
    
    // Relay states
    for (int i = 0; i < relayStates.size(); i++) {
        display.setCursor(0, 12 + i * 10);
        display.print("R");
        display.print(i + 1);
        display.print(": ");
        if (relayStates[i]) {
            display.setTextColor(SSD1306_WHITE);
            display.print("ON ");
        } else {
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
            display.print("OFF");
        }
    }
    
    display.display();
}

void DisplayMgr::showSystemInfo(int rssi, const char* ip, const char* mqttStatus) {
    if (!initialized) return;
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // Title
    display.setCursor(0, 0);
    display.println("=== System ===");
    
    // IP Address
    display.setCursor(0, 12);
    display.print("IP: ");
    display.println(ip);
    
    // RSSI
    display.print("WiFi: ");
    display.print(rssi);
    display.println(" dBm");
    
    // MQTT Status
    display.print("MQTT: ");
    if (strcmp(mqttStatus, "Connected") == 0) {
        display.setTextColor(SSD1306_WHITE);
        display.println("OK");
    } else {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        display.println("OFF");
    }
    
    display.display();
}

void DisplayMgr::clear() {
    if (initialized) {
        display.clearDisplay();
        display.display();
    }
}

void DisplayMgr::update() {
    // Called from task
}
