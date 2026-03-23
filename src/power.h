/**
 * Power Monitor Module
 * Battery voltage, current, and capacity monitoring
 */

#ifndef POWER_H
#define POWER_H

#include <Arduino.h>

class PowerMonitor {
private:
    float voltage;
    float current;
    float capacity;  // percentage
    bool charging;
    unsigned long lastUpdate;
    
    // Calibration values
    float voltageOffset;
    float currentScale;
    
public:
    PowerMonitor();
    void init();
    void update();
    
    float getVoltage();
    float getCurrent();
    float getCapacity();
    bool isCharging();
    
    // Calibration
    void calibrateVoltage(float offset);
    void calibrateCurrent(float scale);
    
private:
    float readVoltage();
    float readCurrent();
    float calculateCapacity(float v);
    bool detectCharging();
};

#endif
