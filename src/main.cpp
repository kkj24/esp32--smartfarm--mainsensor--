#include "task/Sensor/TaskSensor.h"
#include "task/Network/WiFi_Task/WiFi_Task.h"
#include "task/Network/Mqtt_Task/Mqtt_Task.h"
#include "task/Network/ESPNow_Task/ESPNow_task.h"
#include "task/TFT_Task/TFT_Task.h"

#include "Debugging/Debugging.h"

taskSensor_read task_sensor;
WiFi_Task wifi_task;
MQTT_Task mqtt_task;
TFT_task tft_task;
ESP_nowTask esp_nowtask;

Debugging debug;

void setup() {
    Serial.begin(115200);

    task_sensor.beginTask();
    wifi_task.beginTask();
    mqtt_task.beginTask();
    tft_task.beginTask();
    debug.beginDebug();
    esp_nowtask.beginTask();
}

void loop() {


}