#include "ESPNow_lib.h"

uint8_t macAddress_receiver[] = {0xEC, 0xE3, 0x34, 0x1C, 0x32, 0x0C};

ESPNow_Lib::ESPNow_Lib() {
    //                  No Once Func in Here
};


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
            vTaskDelay(1000);
        }
    }

    Serial.println(F("Initialize Success"));

    //                         ESP-Now Setup
    esp_now_peer_info_t peer = {};                                              // Init Peer
    memcpy(peer.peer_addr, macAddress_receiver, sizeof(macAddress_receiver));   // Copy Address to Peer Func
    peer.channel = WiFi.channel();                                              // Set ESP-Now Channel
    peer.encrypt = false;                                                       // ESP-Now Encrypt Mode [False]

    //                      Add Peer Connection
    esp_now_add_peer(&peer);
}