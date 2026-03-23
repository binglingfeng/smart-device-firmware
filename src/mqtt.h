/**
 * MQTT Module
 * Home Assistant Integration
 */

#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <PubSubClient.h>

class MQTTMgr {
private:
    PubSubClient* client;
    bool initialized;
    
public:
    MQTTMgr();
    void init(PubSubClient* mqttClient);
    void setCallback(MQTT_CALLBACK_SIGNATURE);
    void loop();
    void reconnect();
    bool connected();
    
    // Publishing
    void publish(const char* topic, const char* payload);
    void publishSensorData(float voltage, float current, float capacity);
    void publishRelayStatus(int relayNum, bool state);
    void publishHAConfig();
    
    // Discovery
    void discoverSwitches();
    void discoverSensors();
};

#endif
