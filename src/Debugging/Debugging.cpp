#include "Debugging/Debugging.h"

TaskHandle_t debugging_handle;

void debuggingHandle(void *param);

Debugging::Debugging() {
    // No once Func in Here
}

void Debugging::beginDebug() {
    xTaskCreatePinnedToCore(
        debuggingHandle,
        "DebuggingTask",
        8192,
        this,
        1,
        &debugging_handle,
        1
    );
}

void debuggingHandle(void *param) {
    TickType_t saveTick = xTaskGetTickCount();

    while(1) {
        taskYIELD();
        xTaskDelayUntil(&saveTick, pdMS_TO_TICKS(1000));

        UBaseType_t wifi_stack = uxTaskGetStackHighWaterMark(WifiTask_handle);
        UBaseType_t mqtt_stack = uxTaskGetStackHighWaterMark(MqttTask);
        UBaseType_t esp_now_stack = uxTaskGetStackHighWaterMark(ESPNow_handle);
        UBaseType_t tft_stack = uxTaskGetStackHighWaterMark(MqttTask);
        UBaseType_t sensor_stack = uxTaskGetStackHighWaterMark(taskSensor_handle);
        UBaseType_t debugging_stack = uxTaskGetStackHighWaterMark(debugging_handle);

        // Debugging Stack Size
        #if 0
            Serial.printf(
                "\n————————————————————————————————\n"
                "\n=========Stack Remains==========\n"
                "\n————————————————————————————————\n"
                "|  WiFi-Stack:     %02d Remains |\n"
                "|  MQTT-Stack:     %02d Remains |\n"
                "|  ESPNow-Stack:   %02d Remains |\n"
                "|  TFT-Stack:      %02d Remains |\n"
                "|  Sensor-Stack:   %02d Remains |\n"
                "|  Debugging-Stack:%02d Remains |\n"
                "—————————————————————————————————\n",
                wifi_stack,
                mqtt_stack,
                esp_now_stack,
                tft_stack,
                sensor_stack,
                debugging_stack
            );
        #endif

        // Debugging Sensor
        #if 0
            Serial.printf(
                "\nMois1: %d\nMois2: %d\nMois3: %d\nMois4: %d\nMoisAv: %d\nHum: %d\nTemp: %d\n",
                data.vMois1,
                data.vMois2,
                data.vMois3,
                data.vMois4,
                data.vMoisAv,
                data.vHum,
                data.vTemp
            );
        #endif

        #if 0
            Serial.printf("Internal CPU Temp: %s °C\n", String(temperatureRead()).substring(0, 4));
        #endif

    }
}