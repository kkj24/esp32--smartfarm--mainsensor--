#ifndef ESP_NOW_TASK_H
#define ESP_NOW_TASK_H

#include "ESPNow_lib.h"

#include "task/Sensor/TaskSensor.h"

extern ESPNow_Lib espnow_task;

extern TaskHandle_t ESPNow_handle;

class ESP_nowTask {
    public:
        ESP_nowTask();

        void beginTask();

    private:

};

#endif