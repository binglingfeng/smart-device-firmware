/**
 * Display Module - OLED SSD1306
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <vector>

// Display types
enum DisplayType {
    OLED_SSD1306,
    OLED_SH1106,
    LCD_16x2
};

class DisplayMgr {
private:
    DisplayType type;
    bool initialized;
    int currentPage;
    
public:
    DisplayMgr();
    void init();
    void showMessage(const char* message, int line = 0);
    void showPowerInfo(float voltage, float current, float capacity, bool charging);
    void showRelayStatus(std::vector<bool> relayStates);
    void showSystemInfo(int rssi, const char* ip, const char* mqttStatus);
    void clear();
    void update();
};

#endif
