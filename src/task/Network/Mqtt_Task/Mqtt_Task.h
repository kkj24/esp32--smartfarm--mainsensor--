#ifndef MQTT_WIFI_TASK_H
#define MQTT_WIFI_TASK_H

#include "task/Sensor/TaskSensor.h"
#include "task/Network/WiFi_Task/WiFi_Task.h"
#include "EMQX_lib.h"

extern EMQX myMqtt;

extern bool xStateMQTT;

extern TaskHandle_t MqttTask;

class MQTT_Task {
    public:
        MQTT_Task();

        void beginTask();

    private:

};

#endif