#include "ESPNow_task.h"

ESPNow_Lib espnow_task;

TaskHandle_t ESPNow_handle;

void ESPNow_taskHandle(void *param);

ESP_nowTask::ESP_nowTask() {
    // No once Func at Here
};

void ESP_nowTask::beginTask() {
    espnow_task.begin_espNow();
    
    xTaskCreatePinnedToCore(
        ESPNow_taskHandle,
        "esp-now_taskhandle",
        8192,
        this,
        3,
        &ESPNow_handle,
        0
    );
}

void ESPNow_taskHandle(void *param) {
    TickType_t saveTick = xTaskGetTickCount();

    TickType_t lastTick = 0;

    while(1) {
        TickType_t nowTick = xTaskGetTickCount();
        uint16_t interval = 1000;

        if(nowTick - lastTick >= pdMS_TO_TICKS(interval)) {
            lastTick = nowTick;

            xQueueReceive(dataQueue, &data, pdMS_TO_TICKS(10));

            esp_now_send(macAddress_receiver, (uint8_t*)&data, sizeof(data));
        }

        xTaskDelayUntil(&saveTick, pdMS_TO_TICKS(25));
    }
}