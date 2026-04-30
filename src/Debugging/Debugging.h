#ifndef DEBUGGING_FILE_H
#define DEBUGGING_FILE_H

#include "task/Network/ESPNow_Task/ESPNow_task.h"
#include "task/Network/Mqtt_Task/Mqtt_Task.h"
#include "task/Network/WiFi_Task/WiFi_Task.h"

#include "task/Sensor/TaskSensor.h"

#include "task/TFT_Task/TFT_Task.h"

extern TaskHandle_t debugging_handle;

class Debugging {
    public:
        Debugging();

        void beginDebug();

    private:

};

#endif