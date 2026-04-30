#ifndef TFT_TASK_H
#define TFT_TASK_H

#include "TFT_lib.h"
#include "task/Sensor/TaskSensor.h"

#include "task/Network/WiFi_Task/WiFi_Task.h"
#include "task/Network/Mqtt_Task/Mqtt_Task.h"

extern TFT_Lib myTFT;

extern TaskHandle_t taskTFT_handle;

#define BT1 27
#define BT2 26

class TFT_task {
    public:
        TFT_task();

        void beginTask();

    private:

};

#endif