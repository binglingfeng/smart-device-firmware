/**
 * Configuration Header
 * Smart Device Firmware
 */

#ifndef CONFIG_H
#define CONFIG_H

// ==================== WiFi Configuration ====================
#define WIFI_SSID           "Your_WiFi_SSID"
#define WIFI_PASSWORD       "Your_WiFi_Password"
#define WIFI_TIMEOUT        30000  // 30 seconds

// ==================== MQTT / Home Assistant Configuration ====================
#define MQTT_SERVER         "192.168.1.100"
#define MQTT_PORT           1883
#define MQTT_USERNAME       "mqtt_user"
#define MQTT_PASSWORD       "mqtt_password"
#define MQTT_CLIENT_ID      "smart_device_esp32"

// HA Discovery Prefix
#define HA_DISCOVERY_PREFIX "homeassistant"

// MQTT Topics
#define MQTT_ROOT           "smart_device"
#define MQTT_STATUS_TOPIC   MQTT_ROOT "/status"
#define MQTT_SET_TOPIC      MQTT_ROOT "/set"
#define MQTT_SENSOR_TOPIC   MQTT_ROOT "/sensor"

// ==================== Device Configuration ====================
#define DEVICE_NAME         "Smart Power Controller"
#define DEVICE_VERSION      "1.0.0"
#define DEVICE_MODEL        "ESP32-Controller"

// ==================== Power Monitor Configuration ====================
#define POWER_PIN_VBAT      34      // ADC pin for battery voltage
#define POWER_PIN_CURRENT   35      // ADC pin for current (optional)
#define POWER_R1            100000  // Voltage divider R1 (100kΩ)
#define POWER_R2            10000   // Voltage divider R2 (10kΩ)
#define POWER_ADC_MAX       4095    // 12-bit ADC max value
#define POWER_VREF          3.3     // Reference voltage
#define BATTERY_MAX_VOLTAGE 4.2     // LiPo max voltage
#define BATTERY_MIN_VOLTAGE 3.0     // LiPo cutoff voltage
#define BATTERY_CAPACITY     2000   // mAh

// ==================== Relay Control Configuration ====================
#define RELAY_COUNT         4       // Number of relays
#define RELAY_PINS          {26, 27, 14, 12}  // GPIO pins

// Relay names for HA
#define RELAY_NAMES         {"Relay 1", "Relay 2", "Relay 3", "Relay 4"}

// Default relay states on boot (all off)
#define RELAY_DEFAULT_STATE {false, false, false, false}

// ==================== Display Configuration ====================
#define DISPLAY_TYPE        OLED_SSD1306   // OLED_SSD1306, OLED_SH1106, LCD_16x2
#define DISPLAY_I2C_SDA     4
#define DISPLAY_I2C_SCL     15
#define DISPLAY_ADDRESS     0x3C

// Display update interval (ms)
#define DISPLAY_UPDATE_INTERVAL  3000
#define SENSOR_UPDATE_INTERVAL   5000

// ==================== System Configuration ====================
#define LED_PIN             2
#define BUTTON_PIN          0       // Boot button

// Deep sleep configuration
#define ENABLE_DEEP_SLEEP   false
#define SLEEP_DURATION      300000000  // 5 minutes in microseconds

// ==================== Debug Configuration ====================
#define DEBUG_ENABLED       true
#define DEBUG_BAUD_RATE     115200

// Serial debug macros
#if DEBUG_ENABLED
    #define DEBUG_PRINT(x)    Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
#endif

// ==================== Pin Definitions ====================
/*
 * ESP32 Pin Layout:
 * 
 * GPIO0  - Boot Button / IR Receiver
 * GPIO2  - LED (Builtin)
 * GPIO4   - I2C SDA (Display)
 * GPIO5   - I2C SCL (Display)
 * GPIO12  - Relay 4
 * GPIO14  - Relay 3
 * GPIO15  - I2C SCL (Alternative)
 * GPIO18  - SPI SCK
 * GPIO19  - SPI MISO
 * GPIO21  - I2C SDA (Alternative)
 * GPIO22  - I2C SCL
 * GPIO23  - SPI MOSI
 * GPIO25  - DAC1 / ADC
 * GPIO26  - Relay 1 / DAC2
 * GPIO27  - Relay 2
 * GPIO32  - ADC1_CH4 / Battery Voltage
 * GPIO33  - ADC1_CH5 / Current Sensor
 * GPIO34  - ADC1_CH6 (Input only)
 * GPIO35  - ADC1_CH7 (Input only)
 * GPIO36  - ADC1_CH0 (Input only) - VP
 */

#endif // CONFIG_H
