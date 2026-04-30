#include "TaskSensor.h"

Mois mois_libSensor;
DHT_Lib dht_libSensor;

dataSensor data;
raw_dataSensor rawData;

QueueHandle_t dataQueue;
QueueHandle_t raw_dataQueue;

TaskHandle_t taskSensor_handle;

//          Definition of Private Func
void readSensor_task(void *param);

taskSensor_read::taskSensor_read() {
    //          No Once Func at Here
};

//                      Setup Func
void taskSensor_read::beginTask() {
    // Set Sensor
    dht_libSensor.DHTAutoset();
    mois_libSensor.begin();

    // Set
    dataQueue = xQueueCreate(7, sizeof(data));  // Create Queue
    raw_dataQueue = xQueueCreate(4, sizeof(rawData));  // Create Queue

    xTaskCreatePinnedToCore(                    // Create Task
        readSensor_task,    // Task Func
        "readSensorTask",   // Name Task
        4096,               // Stack Size [bits]
        this,               // Parameter
        1,                  // Priority
        &taskSensor_handle, // Task Handle
        1                   // Core ID
    );
}

//                  Read Sensor Task Func
void readSensor_task(void *param) {
    TickType_t saveTick = xTaskGetTickCount();

    TickType_t lastTick = 0;

    while(1) {
        // Timer Var
        TickType_t nowTick = xTaskGetTickCount();
        int intervalTick = 1000;

        // Timer with FreeRTOS Tick
        if(nowTick - lastTick >= pdMS_TO_TICKS(intervalTick)) {
            lastTick = nowTick; // Update Tick

            // DHT Read
            dht_libSensor.readHum(&data.vHum);    // DHT Humidity
            dht_libSensor.readTemp(&data.vTemp);  // DHT Temperature

            // Mois Read
            data.vMois1 = mois_libSensor.liveGetRead(0);  // Mois1
            data.vMois2 = mois_libSensor.liveGetRead(1);  // Mois2
            data.vMois3 = mois_libSensor.liveGetRead(2);  // Mois3
            data.vMois4 = mois_libSensor.liveGetRead(3);  // Mois4
            // Get Average
            mois_libSensor.getAv(
                &data.vMoisAv, // Get Average
                data.vMois1,   // Mois1
                data.vMois2,   // Mois2
                data.vMois3,   // Mois3
                data.vMois4    // Mois4
            );

            // Mois raw data
            rawData.vRaw_Mois1 = mois_libSensor.getRawData(0);
            rawData.vRaw_Mois2 = mois_libSensor.getRawData(1);
            rawData.vRaw_Mois3 = mois_libSensor.getRawData(2);
            rawData.vRaw_Mois4 = mois_libSensor.getRawData(3);

        }
        xQueueSend(raw_dataQueue, &rawData, pdMS_TO_TICKS(100));    // Save Raw Data to Queue Task
        xQueueSend(dataQueue, &data, pdMS_TO_TICKS(100));           // Save Data to Queue Task
        
        xTaskDelayUntil(&saveTick, pdMS_TO_TICKS(100));
    }
}