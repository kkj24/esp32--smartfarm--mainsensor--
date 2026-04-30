#ifndef ESP_NOW_LIBRARIES_H
#define ESP_NOW_LIBRARIES_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "WiFi_lib.h"

extern uint8_t macAddress_receiver[];

class ESPNow_Lib {
    public:
        ESPNow_Lib();

        void begin_espNow();

        void autoGet_Mac();
        uint8_t get_deviceConnected();

    private:

};

void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);

#endif
