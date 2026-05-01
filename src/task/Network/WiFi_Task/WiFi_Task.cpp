#include "WiFi_Task.h"

// Local Func Def
void WiFiTask_handle(void *param);

bool xOnlineMode = true;
bool xOfflineMode = false;

bool xWifiState;

TaskHandle_t WifiTask_handle;

uint8_t xWifiSignal;

WiFi_lib myWifi;

WiFi_Task::WiFi_Task() {
    // No Once Func in Here
};

void WiFi_Task::beginTask() {
    myWifi.setupWiFi();

    xTaskCreatePinnedToCore(
        WiFiTask_handle,
        "WiFiTask_handle",
        8192,
        this,
        5,
        &WifiTask_handle,
        0
    );
}

void WiFiTask_handle(void *param) {
    TickType_t saveTick = xTaskGetTickCount();

    TickType_t lastTick = 0;

    while(1) {
        TickType_t nowTick = xTaskGetTickCount();

        int interval1 = 3000;

        if(nowTick - lastTick >= pdMS_TO_TICKS(interval1)) {
            lastTick = nowTick;

            myWifi.autoRec(xOnlineMode, xOfflineMode);
            xWifiSignal = myWifi.WiFiSignal();

            xWifiState = myWifi.stateWiFi();
        }

        myWifi.Login();

        taskYIELD();
        xTaskDelayUntil(&saveTick, pdMS_TO_TICKS(50));
    };
}