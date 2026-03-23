/**
 * Relay Control Module
 * Multi-channel relay control with scenes
 */

#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include <vector>

class RelayControl {
private:
    std::vector<int> pins;
    std::vector<bool> states;
    std::vector<String> names;
    
public:
    RelayControl();
    void init();
    
    void setRelay(int num, bool state);
    void toggleRelay(int num);
    void setAll(bool state);
    bool getRelay(int num);
    std::vector<bool> getStatus();
    
    // Scene control
    void applyScene(String sceneName);
    void saveScene(String name);
    
private:
    void writePins();
};

#endif
