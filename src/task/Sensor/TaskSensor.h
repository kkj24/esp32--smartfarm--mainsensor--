#ifndef READ_SENSOR_TASK_H
#define READ_SENSOR_TASK_H

#include <Arduino.h>
#include "Mois_Lib.h"
#include "DHT_lib.h"

extern Mois mois_libSensor;
extern DHT_Lib dht_libSensor;

extern QueueHandle_t dataQueue;
extern QueueHandle_t raw_dataQueue;

extern TaskHandle_t taskSensor_handle;

class taskSensor_read {
    public:
        taskSensor_read();

        void beginTask();   // Setup Task

    private:
};

typedef struct {
    uint8_t vMois1;     // Soil Mois1
    uint8_t vMois2;     // Soil Mois2
    uint8_t vMois3;     // Soil Mois3
    uint8_t vMois4;     // Soil Mois4
    uint8_t vMoisAv;    // Soil Mois Average
    
    uint8_t vTemp;      // DHT Air Temperature
    uint8_t vHum;       // DHT Air Humidity
} dataSensor;

typedef struct {
    uint8_t vRaw_Mois1;     // Soil Mois1
    uint8_t vRaw_Mois2;     // Soil Mois2
    uint8_t vRaw_Mois3;     // Soil Mois3
    uint8_t vRaw_Mois4;     // Soil Mois4
} raw_dataSensor;

extern raw_dataSensor rawData;
extern dataSensor data;

#endif