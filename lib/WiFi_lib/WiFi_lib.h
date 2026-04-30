#ifndef WIFI_LIBRARIES_H
#define WIFI_LIBRARIES_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>

#include "HTML_Page/Login_FAST.h"

class WiFi_lib {
    public:
        WiFi_lib();

        void setupWiFi();

        void autoRec(bool OnlineMode, bool OfflineMode);

        bool stateWiFi();
        uint8_t WiFiSignal();

        void Login();

    private:
        //                      STA Config [NetWork]

        const char *HostName = "ROGY--SmartFarm_MainSensor";    // ConfigureWiFi Hostname
        
        //                      AP Config [Login]
        
        const char *AP_ssid = "ROGY--MainSensor";       // WiFi AP Name
        const char *AP_pass = "&ROGY the SmartFarm&";   // WiFi AP Password
};
    
extern String STA_ssid;    // Set STA WiFi Name
extern String STA_pass;    // Set STA WiFi Password

#endif