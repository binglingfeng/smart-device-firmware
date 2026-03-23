/**
 * Power Monitor Implementation
 */

#include "power.h"
#include "config.h"

PowerMonitor::PowerMonitor() {
    voltage = 0.0;
    current = 0.0;
    capacity = 0.0;
    charging = false;
    lastUpdate = 0;
    voltageOffset = 0.0;
    currentScale = 1.0;
}

void PowerMonitor::init() {
    DEBUG_PRINTLN("Initializing Power Monitor...");
    
    // Initialize ADC
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);  // 0-3.3V range
    
    // Initial reading
    update();
    
    DEBUG_PRINT("Voltage: ");
    DEBUG_PRINT(voltage);
    DEBUG_PRINTLN(" V");
}

void PowerMonitor::update() {
    unsigned long now = millis();
    if (now - lastUpdate < 1000) return;  // Update every second
    
    voltage = readVoltage();
    current = readCurrent();
    capacity = calculateCapacity(voltage);
    charging = detectCharging();
    
    lastUpdate = now;
}

float PowerMonitor::readVoltage() {
    // Read multiple samples and average
    const int samples = 10;
    long adcSum = 0;
    
    for (int i = 0; i < samples; i++) {
        adcSum += analogRead(POWER_PIN_VBAT);
        delay(10);
    }
    
    float adcAvg = adcSum / (float)samples;
    
    // Convert ADC to voltage using voltage divider
    // Vout = Vin * R2 / (R1 + R2)
    // Vin = Vout * (R1 + R2) / R2
    float vout = (adcAvg / POWER_ADC_MAX) * POWER_VREF;
    float vin = vout * (POWER_R1 + POWER_R2) / POWER_R2;
    
    // Apply calibration offset
    vin += voltageOffset;
    
    return vin;
}

float PowerMonitor::readCurrent() {
    // For current measurement with INA219 or similar
    // This is a placeholder - implement based on your hardware
    
    #ifdef USE_INA219
        return ina219.getCurrent_mA() / 1000.0;  // Convert to A
    #else
        // Simple ADC-based current sensing
        const int samples = 10;
        long adcSum = 0;
        
        for (int i = 0; i < samples; i++) {
            adcSum += analogRead(POWER_PIN_CURRENT);
            delay(10);
        }
        
        float adcAvg = adcSum / (float)samples;
        float voltage = (adcAvg / POWER_ADC_MAX) * POWER_VREF;
        
        // Current = V / R_sense (assuming 0.1 ohm sense resistor)
        float current = (voltage / 0.1) * currentScale;
        
        return current;
    #endif
}

float PowerMonitor::calculateCapacity(float v) {
    // LiPo battery percentage calculation
    // Fully charged: 4.2V = 100%
    // Empty: 3.0V = 0%
    
    if (v >= BATTERY_MAX_VOLTAGE) return 100.0;
    if (v <= BATTERY_MIN_VOLTAGE) return 0.0;
    
    float percent = ((v - BATTERY_MIN_VOLTAGE) / (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE)) * 100.0;
    
    return percent;
}

bool PowerMonitor::detectCharging() {
    // Detect charging state
    // This depends on your charging circuit
    
    #ifdef CHARGING_PIN
        return digitalRead(CHARGING_PIN) == HIGH;
    #else
        // Estimate based on voltage and current
        return (voltage > 4.0 && current > 0.1);
    #endif
}

float PowerMonitor::getVoltage() {
    return voltage;
}

float PowerMonitor::getCurrent() {
    return current;
}

float PowerMonitor::getCapacity() {
    return capacity;
}

bool PowerMonitor::isCharging() {
    return charging;
}

void PowerMonitor::calibrateVoltage(float offset) {
    voltageOffset = offset;
}

void PowerMonitor::calibrateCurrent(float scale) {
    currentScale = scale;
}
