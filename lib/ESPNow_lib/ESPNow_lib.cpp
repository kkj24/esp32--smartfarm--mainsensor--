#include "ESPNow_lib.h"

uint8_t macAddress_receiver[] = {0xFF};

ESPNow_Lib::ESPNow_Lib() {
    //                  No Once Func in Here
};



// ——————————————————————————————————————————————————————————————————————————————————
//                  Read Mac Receiver with WiFi Event [Un-Polling]
// ——————————————————————————————————————————————————————————————————————————————————
void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
    if(event == SYSTEM_EVENT_AP_STACONNECTED) {
        memcpy(macAddress_receiver, info.wifi_ap_staconnected.mac, sizeof(6));  // Copy Mac Event to macAddress_receiver
    }
}



// ——————————————————————————————————————————————————————————————————————————————————
//                              Begin ESp-NOW Func
// ——————————————————————————————————————————————————————————————————————————————————
void ESPNow_Lib::begin_espNow() {
    //                      Initialize ESP-Now
    if(esp_now_init() != ESP_OK) {
        while(1) {
            Serial.println(F("[E][2] ESP_ERR at Initialize ESP_NOW"));
            esp_now_init();

            // Delay every 1000ms
            delay(1000);
        }
    }

    Serial.println(F("Initialize Success"));

    //                         Callback Event
    WiFi.onEvent(WiFiEvent);

    //                         ESP-Now Setup
    esp_now_peer_info_t peer;                                                   // Init Peer
    memcpy(peer.peer_addr, macAddress_receiver, sizeof(macAddress_receiver));   // Copy Address to Peer Func
    peer.channel = 1;                                                           // Set ESP-Now Channel
    peer.encrypt = false;                                                       // ESP-Now Encrypt Mode [False]

    //                      Add Peer Connection
    esp_now_add_peer(&peer);
}

void ESPNow_Lib::autoGet_Mac() {
    // Var
    wifi_sta_list_t connected_list;
    static uint8_t last_deviceConnected = 0;

    // Get WiFi Event
    esp_wifi_ap_get_sta_list(&connected_list);
    WiFiEventInfo_t info;

    // Update Mac
    if(last_deviceConnected != connected_list.num) {
        last_deviceConnected = connected_list.num;

        memcpy(macAddress_receiver, info.wifi_ap_staconnected.mac, sizeof(6));
    }
}

uint8_t ESPNow_Lib::get_deviceConnected() {
    wifi_sta_list_t deviceList;

    esp_wifi_ap_get_sta_list(&deviceList);

    return deviceList.num;
}