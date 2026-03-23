/**
 * Relay Control Implementation
 */

#include "control.h"
#include "config.h"

RelayControl::RelayControl() {}

void RelayControl::init() {
    DEBUG_PRINTLN("Initializing Relay Control...");
    
    // Initialize relay pins
    int relayPins[] = RELAY_PINS;
    String relayNames[] = RELAY_NAMES;
    bool defaultStates[] = RELAY_DEFAULT_STATE;
    
    for (int i = 0; i < RELAY_COUNT; i++) {
        pins.push_back(relayPins[i]);
        states.push_back(defaultStates[i]);
        names.push_back(relayNames[i]);
        
        pinMode(pins[i], OUTPUT);
        DEBUG_PRINT("Relay ");
        DEBUG_PRINT(i);
        DEBUG_PRINT(" on GPIO");
        DEBUG_PRINTLN(pins[i]);
    }
    
    writePins();
    DEBUG_PRINTLN("Relay Control Initialized");
}

void RelayControl::setRelay(int num, bool state) {
    if (num >= 0 && num < pins.size()) {
        states[num] = state;
        writePins();
        
        DEBUG_PRINT("Relay ");
        DEBUG_PRINT(num);
        DEBUG_PRINT(" set to ");
        DEBUG_PRINTLN(state ? "ON" : "OFF");
    }
}

void RelayControl::toggleRelay(int num) {
    if (num >= 0 && num < pins.size()) {
        states[num] = !states[num];
        writePins();
    }
}

void RelayControl::setAll(bool state) {
    for (int i = 0; i < states.size(); i++) {
        states[i] = state;
    }
    writePins();
    
    DEBUG_PRINTLN(state ? "All relays ON" : "All relays OFF");
}

bool RelayControl::getRelay(int num) {
    if (num >= 0 && num < states.size()) {
        return states[num];
    }
    return false;
}

std::vector<bool> RelayControl::getStatus() {
    return states;
}

void RelayControl::applyScene(String sceneName) {
    DEBUG_PRINT("Applying scene: ");
    DEBUG_PRINTLN(sceneName);
    
    if (sceneName == "off") {
        setAll(false);
    } else if (sceneName == "on") {
        setAll(true);
    } else if (sceneName == "night") {
        // Night mode: only relay 1 on
        setAll(false);
        setRelay(0, true);
    } else if (sceneName == "away") {
        // Away mode: all off
        setAll(false);
    }
    // Add more scenes as needed
}

void RelayControl::writePins() {
    for (int i =0; i < pins.size(); i++) {
        digitalWrite(pins[i], states[i] ? HIGH : LOW);
    }
}
