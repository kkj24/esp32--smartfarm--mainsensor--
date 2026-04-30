#include "EMQX_lib.h"

WiFiClient espClient;
PubSubClient mqtt_lib(espClient);
JsonDocument doc;

const int MQTT_Port = 1883;                 // Port MQTT
const char *MQTT_server = "broker.emqx.io"; // Server Domain
const char *MQTT_Username = "";             // MQTT username [optional]
const char *MQTT_Pass = "";                 // MQTT password [optional]

EMQX::EMQX() {
    //                              No Once Function
};



// ————————————————————————————————————————————————————————————————————————————————————————————
//                                          Run EMQX Func
// ————————————————————————————————————————————————————————————————————————————————————————————
void EMQX::run() {
    mqtt_lib.loop();
}



// ————————————————————————————————————————————————————————————————————————————————————————————
//                                      Setup MQTT Automate Func
// ————————————————————————————————————————————————————————————————————————————————————————————
void EMQX::MQTTsetup() {
    mqtt_lib.setServer(MQTT_server, MQTT_Port);
}



// ————————————————————————————————————————————————————————————————————————————————————————————
//                                          MQTT Auto Reconnect
// ————————————————————————————————————————————————————————————————————————————————————————————
void EMQX::MQTTRec() {
    bool stateMqtt = MQTTState();

    static bool lastState = true;

    static String ID = "";
    if (ID == "") {
        ID = "ESP--smartFarm" + String(WiFi.macAddress());
    }

    //                                  Reconnect MQTT

    if(stateMqtt) {
        mqtt_lib.connect(ID.c_str());
    }
}



// ————————————————————————————————————————————————————————————————————————————————————————————
//                                  Save Data with JSON Form
// ————————————————————————————————————————————————————————————————————————————————————————————
char dataSend[200];
void EMQX::saveData(int16_t data, String data_key) {
    if(!mqtt_lib.connected())
    return;

    doc[data_key] = data;
    
    serializeJson(doc, dataSend, sizeof(dataSend));
}



// ————————————————————————————————————————————————————————————————————————————————————————————
//                          Clear JSON Data to Prevent Stack Over Flow
// ————————————————————————————————————————————————————————————————————————————————————————————
void EMQX::clearData() {
    doc.clear();
}



// ————————————————————————————————————————————————————————————————————————————————————————————
//                              Publish All Data into MQTT Server
// ————————————————————————————————————————————————————————————————————————————————————————————
void EMQX::sendData(String topic) {
    unsigned long now = millis();
    static unsigned long last = 0;
    int interval = 1000;

    if(now - last >= interval) {
        mqtt_lib.publish(topic.c_str(), dataSend);
        
        last = now;
    }
}



// ————————————————————————————————————————————————————————————————————————————————————————————
//                      MQTT Status [False == Connected -- True == Disconnect]
// ————————————————————————————————————————————————————————————————————————————————————————————
bool EMQX::MQTTState() {
    static bool state = false;

    if(mqtt_lib.connected())
        state = false;
    else
        state = true;

    return state;
}