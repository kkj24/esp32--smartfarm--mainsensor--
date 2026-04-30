#ifndef WIFI_TASK_HANDLE_H
#define WIFI_TASK_HANDLE_H

#include "WiFi_lib.h"

extern WiFi_lib myWifi;

extern bool xOnlineMode;
extern bool xOfflineMode;

extern uint8_t xWifiSignal;
extern bool xWifiState;

extern TaskHandle_t WifiTask_handle;

class WiFi_Task {
    public:
        WiFi_Task();

        void beginTask();

    private:

};

#endif