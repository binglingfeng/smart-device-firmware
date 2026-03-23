/**
 * MQTT Implementation
 */

#include "mqtt.h"
#include "config.h"
#include <ArduinoJson.h>

MQTTMgr::MQTTMgr() {
    initialized = false;
}

void MQTTMgr::init(PubSubClient* mqttClient) {
    DEBUG_PRINTLN("Initializing MQTT...");
    
    client = mqttClient;
    client->setServer(MQTT_SERVER, MQTT_PORT);
    
    initialized = true;
    DEBUG_PRINTLN("MQTT Initialized");
}

void MQTTMgr::setCallback(MQTT_CALLBACK_SIGNATURE) {
    if (client) {
        client->setCallback(callback);
    }
}

void MQTTMgr::loop() {
    if (!initialized || !client) return;
    
    if (!client->loop()) {
        reconnect();
    }
}

void MQTTMgr::reconnect() {
    if (!initialized) return;
    
    // Loop until we're reconnected
    while (!client->connected()) {
        DEBUG_PRINT("Attempting MQTT connection...");
        
        // Attempt to connect
        if (client->connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
            DEBUG_PRINTLN("connected");
            
            // Subscribe to command topic
            client->subscribe(MQTT_SET_TOPIC);
            DEBUG_PRINT("Subscribed to: ");
            DEBUG_PRINTLN(MQTT_SET_TOPIC);
            
            // Publish HA discovery
            publishHAConfig();
            
        } else {
            DEBUG_PRINT("failed, rc=");
            DEBUG_PRINT(client->state());
            DEBUG_PRINTLN(" try again in 5 seconds");
            delay(5000);
        }
    }
}

bool MQTTMgr::connected() {
    return client && client->connected();
}

void MQTTMgr::publish(const char* topic, const char* payload) {
    if (client && client->connected()) {
        client->publish(topic, payload);
    }
}

void MQTTMgr::publishSensorData(float voltage, float current, float capacity) {
    StaticJsonDocument<256> doc;
    doc["voltage"] = voltage;
    doc["current"] = current;
    doc["capacity"] = capacity;
    doc["timestamp"] = millis();
    
    char buffer[256];
    serializeJson(doc, buffer);
    
    publish(MQTT_SENSOR_TOPIC, buffer);
}

void MQTTMgr::publishRelayStatus(int relayNum, bool state) {
    char topic[64];
    char payload[32];
    
    snprintf(topic, sizeof(topic), "smart_device/relay/%d", relayNum);
    snprintf(payload, sizeof(payload), "%s", state ? "ON" : "OFF");
    
    publish(topic, payload);
}

void MQTTMgr::publishHAConfig() {
    if (!client->connected()) return;
    
    // Discover switches (relays)
    discoverSwitches();
    
    // Discover sensors
    discoverSensors();
}

void MQTTMgr::discoverSwitches() {
    String names[] = RELAY_NAMES;
    char topic[128];
    char payload[512];
    
    for (int i = 0; i < RELAY_COUNT; i++) {
        // Switch config
        StaticJsonDocument<512> doc;
        doc["name"] = names[i];
        doc["command_topic"] = String(MQTT_SET_TOPIC);
        doc["payload_off"] = "OFF";
        doc["payload_on"] = "ON";
        doc["state_topic"] = String(MQTT_ROOT) + "/relay/" + String(i);
        doc["value_template"] = "{{ value_json.state }}";
        doc["unique_id"] = String(DEVICE_NAME) + "_relay_" + String(i);
        doc["device"]["identifiers"] = String(DEVICE_NAME);
        doc["device"]["name"] = DEVICE_NAME;
        doc["device"]["model"] = DEVICE_MODEL;
        doc["device"]["manufacturer"] = "Custom";
        
        String baseTopic = String(HA_DISCOVERY_PREFIX) + "/switch/" + String(DEVICE_NAME) + "_relay" + String(i) + "/config";
        
        serializeJson(doc, payload);
        
        client->publish(baseTopic.c_str(), payload, true);
    }
}

void MQTTMgr::discoverSensors() {
    char topic[128];
    char payload[512];
    
    // Voltage sensor
    StaticJsonDocument<512> docV;
    docV["name"] = "Battery Voltage";
    docV["state_topic"] = String(MQTT_SENSOR_TOPIC);
    docV["value_template"] = "{{ value_json.voltage }}";
    docV["unit_of_measurement"] = "V";
    docV["device_class"] = "voltage";
    docV["unique_id"] = String(DEVICE_NAME) + "_voltage";
    docV["device"]["identifiers"] = String(DEVICE_NAME);
    docV["device"]["name"] = DEVICE_NAME;
    
    String baseTopicV = String(HA_DISCOVERY_PREFIX) + "/sensor/" + String(DEVICE_NAME) + "_voltage/config";
    serializeJson(docV, payload);
    client->publish(baseTopicV.c_str(), payload, true);
    
    // Current sensor
    StaticJsonDocument<512> docI;
    docI["name"] = "Battery Current";
    docI["state_topic"] = String(MQTT_SENSOR_TOPIC);
    docI["value_template"] = "{{ value_json.current }}";
    docI["unit_of_measurement"] = "A";
    docI["device_class"] = "current";
    docI["unique_id"] = String(DEVICE_NAME) + "_current";
    docI["device"]["identifiers"] = String(DEVICE_NAME);
    docI["device"]["name"] = DEVICE_NAME;
    
    String baseTopicI = String(HA_DISCOVERY_PREFIX) + "/sensor/" + String(DEVICE_NAME) + "_current/config";
    serializeJson(docI, payload);
    client->publish(baseTopicI.c_str(), payload, true);
    
    // Battery sensor
    StaticJsonDocument<512> docB;
    docB["name"] = "Battery Level";
    docB["state_topic"] = String(MQTT_SENSOR_TOPIC);
    docB["value_template"] = "{{ value_json.capacity }}";
    docB["unit_of_measurement"] = "%";
    docB["device_class"] = "battery";
    docB["unique_id"] = String(DEVICE_NAME) + "_battery";
    docB["device"]["identifiers"] = String(DEVICE_NAME);
    docB["device"]["name"] = DEVICE_NAME;
    
    String baseTopicB = String(HA_DISCOVERY_PREFIX) + "/sensor/" + String(DEVICE_NAME) + "_battery/config";
    serializeJson(docB, payload);
    client->publish(baseTopicB.c_str(), payload, true);
}
