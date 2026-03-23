/**
 * Smart Device Firmware - Main Entry
 * ESP32 Based Smart Power Controller
 */

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "power.h"
#include "control.h"
#include "display.h"
#include "mqtt.h"
#include "wifi_mgr.h"

// Global instances
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
PowerMonitor power;
RelayControl relays;
DisplayMgr display;
MQTTMgr mqtt;
WiFiMgr wifiMgr;

// Task handles
TaskHandle_t TaskHandle_Sensor = NULL;
TaskHandle_t TaskHandle_Display = NULL;
TaskHandle_t TaskHandle_MQTT = NULL;

// LED indicator
const int LED_PIN = 2;

// Forward declarations
void sensorTask(void* parameter);
void displayTask(void* parameter);
void mqttTask(void* parameter);
void mqttCallback(char* topic, byte* message, unsigned int length);

void setup() {
    Serial.begin(115200);
    Serial.println("\n=== Smart Device Firmware Starting ===");
    
    // Initialize LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    
    // Initialize components
    display.init();
    display.showMessage("Initializing...", 0);
    
    // Initialize power monitor
    power.init();
    
    // Initialize relay control
    relays.init();
    
    // Initialize WiFi
    wifiMgr.init();
    wifiMgr.connect();
    
    // Initialize MQTT
    mqtt.init(&mqttClient);
    mqtt.setCallback(mqttCallback);
    
    // Create sensor monitoring task (Core 0)
    xTaskCreatePinnedToCore(
        sensorTask,           // Task function
        "Sensor Task",        // Task name
        4096,                 // Stack size
        NULL,                 // Parameters
        1,                    // Priority
        &TaskHandle_Sensor,   // Task handle
        0                     // Core ID
    );
    
    // Create display update task (Core 0)
    xTaskCreatePinnedToCore(
        displayTask,
        "Display Task",
        4096,
        NULL,
        1,
        &TaskHandle_Display,
        0
    );
    
    // Create MQTT task (Core 1)
    xTaskCreatePinnedToCore(
        mqttTask,
        "MQTT Task",
        4096,
        NULL,
        1,
        &TaskHandle_MQTT,
        1
    );
    
    Serial.println("=== System Initialized ===");
    display.showMessage("Ready!", 0);
}

void loop() {
    // Check WiFi status
    if (WiFi.status() == WL_CONNECTED) {
        digitalWrite(LED_PIN, LOW);  // LED on when connected
    } else {
        digitalWrite(LED_PIN, HIGH);
        wifiMgr.reconnect();
    }
    
    // MQTT loop
    mqtt.loop();
    
    // Power monitoring
    power.update();
    
    delay(10);
}

// ==================== Tasks ====================

void sensorTask(void* parameter) {
    Serial.println("Sensor Task started on Core 0");
    
    while (true) {
        // Update power readings
        power.update();
        
        // Publish sensor data to MQTT
        if (mqttClient.connected()) {
            mqtt.publishSensorData(power.getVoltage(), 
                                   power.getCurrent(), 
                                   power.getCapacity());
        }
        
        vTaskDelay(pdMS_TO_TICKS(SENSOR_UPDATE_INTERVAL));
    }
}

void displayTask(void* parameter) {
    Serial.println("Display Task started on Core 0");
    uint32_t displayIndex = 0;
    
    while (true) {
        // Update display based on current page
        switch (displayIndex % 3) {
            case 0:
                // Power info page
                display.showPowerInfo(
                    power.getVoltage(),
                    power.getCurrent(),
                    power.getCapacity(),
                    power.isCharging()
                );
                break;
                
            case 1:
                // Relay status page
                display.showRelayStatus(relays.getStatus());
                break;
                
            case 2:
                // System info page
                display.showSystemInfo(
                    WiFi.RSSI(),
                    WiFi.localIP().toString().c_str(),
                    mqttClient.connected() ? "Connected" : "Disconnected"
                );
                break;
        }
        
        displayIndex++;
        vTaskDelay(pdMS_TO_TICKS(DISPLAY_UPDATE_INTERVAL));
    }
}

void mqttTask(void* parameter) {
    Serial.println("MQTT Task started on Core 1");
    
    while (true) {
        if (WiFi.status() == WL_CONNECTED) {
            if (!mqttClient.connected()) {
                mqtt.reconnect();
            }
        }
        
        mqttClient.loop();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// ==================== MQTT Callback ====================

void mqttCallback(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.println(topic);
    
    // Parse JSON message
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, message, length);
    
    if (error) {
        Serial.print("JSON parse failed: ");
        Serial.println(error.c_str());
        return;
    }
    
    // Handle commands
    String cmdTopic = String(topic);
    
    if (cmdTopic.endsWith("/set")) {
        // Relay control command
        if (doc.containsKey("relay")) {
            int relayNum = doc["relay"];
            bool state = doc["state"];
            relays.setRelay(relayNum, state);
        }
        
        // All relays command
        if (doc.containsKey("all")) {
            bool state = doc["all"];
            relays.setAll(state);
        }
        
        // Scene command
        if (doc.containsKey("scene")) {
            String scene = doc["scene"];
            relays.applyScene(scene);
        }
    }
}
