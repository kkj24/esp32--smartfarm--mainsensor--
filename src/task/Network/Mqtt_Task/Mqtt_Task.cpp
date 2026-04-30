#include "Mqtt_Task.h"

EMQX myMqtt;

// Func Definition
void MQTTHandle_Task(void *param);

bool xStateMQTT;

TaskHandle_t MqttTask;

MQTT_Task::MQTT_Task() {
    // No once Func in Here
};

// Setup MQTT
void MQTT_Task::beginTask() {
    myMqtt.MQTTsetup();

    xTaskCreatePinnedToCore(
        MQTTHandle_Task,
        "Mqtt_HandleTask",
        8192,
        this,
        5,
        &MqttTask,
        0
    );
}

// MQTT_Handle
void MQTTHandle_Task(void *param) {
    TickType_t saveTick = xTaskGetTickCount();

    TickType_t lastTick1 = 0;
    TickType_t lastTick2 = 0;

    while(1) {
        TickType_t nowTick = xTaskGetTickCount();
        uint16_t interval1 = 1000;
        uint16_t interval2 = 5000;

        xQueueReceive(dataQueue, &data, pdMS_TO_TICKS(1));
        
        xStateMQTT = myMqtt.MQTTState();
         
        if(!xStateMQTT && !xWifiState) {
            if(nowTick - lastTick1 >= pdMS_TO_TICKS(interval1)) {
                lastTick1 = nowTick;
    
                myMqtt.saveData(data.vMois1, "Mois1");
                myMqtt.saveData(data.vMois2, "Mois2");
                myMqtt.saveData(data.vMois3, "Mois3");
                myMqtt.saveData(data.vMois4, "Mois4");
                myMqtt.saveData(data.vMoisAv, "MoisAv");
                myMqtt.saveData(data.vHum, "Air-Hum");
                myMqtt.saveData(data.vTemp, "Air-Temp");
                
                myMqtt.sendData();
    
                myMqtt.clearData();
            }
        }
        
        if(nowTick - lastTick2 >= pdMS_TO_TICKS(interval2)) {
            lastTick2 = nowTick;
 
            if(myWifi.stateWiFi() == 0)
                myMqtt.MQTTRec();
        }

        if(!xStateMQTT && !xWifiState) 
            myMqtt.run();

        taskYIELD();
        xTaskDelayUntil(&saveTick, pdMS_TO_TICKS(50));
    }
}