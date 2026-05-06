#include "TFT_Task.h"

TFT_Lib myTFT;

TaskHandle_t taskTFT_handle;

void tab1_content();
void tab2_content();
void tab3_content();
void tab4_content();

void TFTTask_handle(void *param);

TFT_task::TFT_task() {
    // No Func in Here
};

// Begin Task
void TFT_task::beginTask() {
    myTFT.TFTBegin();

    pinMode(BT1, INPUT);
    pinMode(BT2, INPUT);

    xTaskCreatePinnedToCore(
        TFTTask_handle,
        "TFT_Handle_task",
        8192,
        this,
        1,
        &taskTFT_handle,
        1
    );
}

// Handle FreeRTOS Task
void TFTTask_handle(void *param) {
    TickType_t saveTick = xTaskGetTickCount();

    TickType_t lastTick = 0;

    // Color Tab
    uint16_t color_tab1 = 0;
    uint16_t color_tab2 = 0;
    uint16_t color_tab3 = 0;
    uint16_t color_tab4 = 0;

    // Button State
    bool lastbt1;
    bool lastbt2;

    uint8_t swipe = 1;
    uint8_t last_swipe = 1;

    // Tab Content
    enum TAB {
        TAB1 = 1,
        TAB2 = 2,
        TAB3 = 3,
        TAB4 = 4
    };

    while(1) {
        TickType_t nowTick = xTaskGetTickCount();
        uint16_t intervalTick = 10;

        // Timer 
        if(nowTick - lastTick >= pdMS_TO_TICKS(intervalTick)) {
            lastTick = nowTick;
            // =================== Input ======================
            // Button Logic
            bool bt1 = digitalRead(BT1);
            bool bt2 = digitalRead(BT2);

            // =================== Proccess ===================
            // Swipe Logic
            if(!lastbt1 && bt1)
                swipe++;
            else if(!lastbt2 && bt2)
                swipe--;

            // Update
            lastbt1 = bt1;
            lastbt2 = bt2;

            if(swipe > 4)
                swipe = 1;
            else if(swipe < 1)
                swipe = 4;

            if(last_swipe != swipe) {
                tft_lib.fillRect(0, 41, 128, 120, TFT_BLACK);

                last_swipe = swipe;
            }

            // ================== Content =====================
            // Tab
            switch(swipe) {
                case TAB1:
                // Title
                    color_tab1 = TFT_CYAN;
                    color_tab2 = tft_lib.color565(128, 128, 128);
                    color_tab3 = tft_lib.color565(128, 128, 128);
                    color_tab4 = tft_lib.color565(128, 128, 128);

                    tab1_content();
                break;

                case TAB2:
                // Title
                    color_tab1 = tft_lib.color565(128, 128, 128);
                    color_tab2 = TFT_CYAN;
                    color_tab3 = tft_lib.color565(128, 128, 128);
                    color_tab4 = tft_lib.color565(128, 128, 128);

                    tab2_content();
                break;

                case TAB3:
                // Title
                    color_tab1 = tft_lib.color565(128, 128, 128);
                    color_tab2 = tft_lib.color565(128, 128, 128);
                    color_tab3 = TFT_CYAN;
                    color_tab4 = tft_lib.color565(128, 128, 128);

                    tab3_content();
                break;

                case TAB4:
                // Title
                    color_tab1 = tft_lib.color565(128, 128, 128);
                    color_tab2 = tft_lib.color565(128, 128, 128);
                    color_tab3 = tft_lib.color565(128, 128, 128);
                    color_tab4 = TFT_CYAN;

                    tab4_content();
                break;
            }

            // =================== Output =====================
            // Title
            myTFT.printF(F("ALGORITHMA"), 5, 2, TFT_GREENYELLOW, 2);
            myTFT.printF(F("ROGY - Agriculture"), 10, 20, TFT_WHITE);

            // Barrier Line
            tft_lib.drawFastHLine(0, 30, 128, TFT_WHITE);
            tft_lib.drawFastHLine(0, 40, 128, TFT_WHITE);

            tft_lib.drawFastVLine(29, 30, 10, TFT_WHITE);
            tft_lib.drawFastVLine(64, 30, 10, TFT_WHITE);
            tft_lib.drawFastVLine(97, 30, 10, TFT_WHITE);

            // Tab Title
            myTFT.printF(F("TAB1"), 2, 32, color_tab1);
            myTFT.printF(F("TAB2"), 35, 32, color_tab2);
            myTFT.printF(F("TAB3"), 70, 32, color_tab3);
            myTFT.printF(F("TAB4"), 102, 32, color_tab4);
        }

        xQueueReceive(dataQueue, &data, pdMS_TO_TICKS(5));
        xQueueReceive(raw_dataQueue, &rawData, pdMS_TO_TICKS(5));

        xTaskDelayUntil(&saveTick, pdMS_TO_TICKS(25));
        taskYIELD();
    }
}

// get WiFi + MQTT Icon
void getIcon() {
    TickType_t nowTick = xTaskGetTickCount();
    static TickType_t lastTick = 0;

    static bool stateMqttIcon = false;
    static uint8_t countWiFi = 0;

    if(nowTick - lastTick >= pdMS_TO_TICKS(500)) {
        lastTick = nowTick;

        if(myMqtt.MQTTState() == 1) {
            stateMqttIcon = !stateMqttIcon;
        } else 
            stateMqttIcon = true;

        if(myWifi.stateWiFi() == 1) {
            countWiFi++;

            if(countWiFi > 5)
                countWiFi = 1;

        } else {
            countWiFi = myWifi.WiFiSignal();
        }
    }
        
    myTFT.WiFiLevel_Icon(116, 42, countWiFi);
    myTFT.MQTTstate_Icon(2, 42, stateMqttIcon);

}

// TAB1 Content || Data Dashboard
void tab1_content() {
    getIcon();

    static uint16_t color1 = 0;
    static uint16_t color2 = 0;
    static uint16_t color3 = 0;
    static uint16_t color4 = 0;
    static uint16_t color5 = 0;
    static uint16_t color6 = 0;
    static uint16_t color7 = 0;
    
    if(data.vMois1 < 50 || data.vMois1 >= 75)
        color1 = TFT_YELLOW;
    else if(data.vMois1 < 35 || data.vMois1 >= 80)
        color1 = TFT_ORANGE;
    else if(data.vMois1 < 15 || data.vMois1 >= 95)
        color1 = TFT_RED;
    else if(data.vMois1 >= 50)
        color1 = TFT_GREEN;
    
    if(data.vMois2 < 50 || data.vMois2 >= 75)
        color2 = TFT_YELLOW;
    else if(data.vMois2 < 35 || data.vMois2 >= 80)
        color2 = TFT_ORANGE;
    else if(data.vMois2 < 15 || data.vMois2 >= 95)
        color2 = TFT_RED;
    else if(data.vMois2 >= 50)
        color2 = TFT_GREEN;
    
    if(data.vMois3 < 50 || data.vMois3 >= 75)
        color3 = TFT_YELLOW;
    else if(data.vMois3 < 35 || data.vMois3 >= 80)
        color3 = TFT_ORANGE;
    else if(data.vMois3 < 15 || data.vMois3 >= 95)
        color3 = TFT_RED;
    else if(data.vMois3 >= 50)
        color3 = TFT_GREEN;
    
    if(data.vMois4 < 50 || data.vMois4 >= 75)
        color4 = TFT_YELLOW;
    else if(data.vMois4 < 35 || data.vMois4 >= 80)
        color4 = TFT_ORANGE;
    else if(data.vMois4 < 15 || data.vMois4 >= 95)
        color4 = TFT_RED;
    else if(data.vMois4 >= 50)
        color4 = TFT_GREEN;
    
    if(data.vMoisAv < 50 || data.vMoisAv >= 75)
        color5 = TFT_YELLOW;
    else if(data.vMoisAv < 35 || data.vMoisAv >= 80)
        color5 = TFT_ORANGE;
    else if(data.vMoisAv < 15 || data.vMoisAv >= 95)
        color5 = TFT_RED;
    else if(data.vMoisAv >= 50)
        color5 = TFT_GREEN;
    
    if(data.vHum < 50 || data.vHum >= 75)
        color6 = TFT_YELLOW;
    else if(data.vHum < 35 || data.vHum >= 80)
        color6 = TFT_ORANGE;
    else if(data.vHum < 15 || data.vHum >= 95)
        color6 = TFT_RED;
    else if(data.vHum >= 50)
        color6 = TFT_GREEN;

    if(data.vTemp < 50 || data.vTemp >= 75)
        color7 = TFT_YELLOW;
    else if(data.vTemp < 35 || data.vTemp >= 80)
        color7 = TFT_ORANGE;
    else if(data.vTemp < 15 || data.vTemp >= 95)
        color7 = TFT_RED;
    else if(data.vTemp >= 50)
            color7 = TFT_GREEN;

    // Left
    // TFT Bar Mois1
    myTFT.printF(F("Mois1"), 2, 60, TFT_CYAN);
    myTFT.soft_pBar(0, data.vMois1, 2, 68, 60, 5, TFT_WHITE, color1);
    
    // TFT Bar Mois2
    myTFT.printF(F("Mois2"), 2, 60 + (8 * 2), TFT_CYAN);
    myTFT.soft_pBar(1, data.vMois2, 2, 68 + (8 * 2), 60, 5, TFT_WHITE, color2);
    
    // TFT Bar Mois3
    myTFT.printF(F("Mois3"), 2, 60 + (8 * 4), TFT_CYAN);
    myTFT.soft_pBar(2, data.vMois3, 2, 68 + (8 * 4), 60, 5, TFT_WHITE, color3);

    // Right
    // TFT Bar Mois4
    myTFT.printF(F("Mois4"), 64, 60, TFT_CYAN);
    myTFT.soft_pBar(3, data.vMois4, 64, 68, 64, 5, TFT_WHITE, color4);
    
    // TFT Bar Hum
    myTFT.printF(F("Hum"), 64, 60 + (8 * 2), TFT_CYAN);
    myTFT.soft_pBar(5, data.vHum, 64, 68 + (8 * 2), 64, 5, TFT_WHITE, color6);
    
    // TFT Bar Temp
    myTFT.printF(F("Temp"), 64, 60 + (8 * 4), TFT_CYAN);
    myTFT.soft_pBar(6, data.vTemp, 64, 68 + (8 * 4), 64, 5, TFT_WHITE, color7);
    
    // Center
    // TFT Bar Temp
    myTFT.printF(F("Mois Av"), 2, 60 + (8 * 6), TFT_CYAN);
    myTFT.soft_pBar(7, data.vMoisAv, 2, 68 + (8 * 6), 126, 5, TFT_WHITE, color7);
}

// TAB2 Content || Raw Data Dashboard
void tab2_content() {
    getIcon();

    static TickType_t lastTick = 0;
    TickType_t nowTick = xTaskGetTickCount();

    if(nowTick - lastTick >= pdMS_TO_TICKS(1000)) {
        lastTick = nowTick;

        // Static Title
        myTFT.printF(F("MoisRaw1 : "), 10, 60, TFT_WHITE);
        myTFT.printF(F("MoisRaw2 : "), 10, 60 + (8 * 1), TFT_WHITE);
        myTFT.printF(F("MoisRaw3 : "), 10, 60 + (8 * 2), TFT_WHITE);
        myTFT.printF(F("MoisRaw4 : "), 10, 60 + (8 * 3), TFT_WHITE);
        myTFT.printF(F("DHT Temp : "), 10, 60 + (8 * 4), TFT_WHITE);
        myTFT.printF(F("DHT Hum  : "), 10, 60 + (8 * 5), TFT_WHITE);

        // Data Show
        tft_lib.fillRect(68, 58, 40, 50, TFT_BLACK);
        myTFT.printF((String)rawData.vRaw_Mois1, 68, 60, TFT_YELLOW);
        myTFT.printF((String)rawData.vRaw_Mois2, 68, 60 + (8 * 1), TFT_YELLOW);
        myTFT.printF((String)rawData.vRaw_Mois3, 68, 60 + (8 * 2), TFT_YELLOW);
        myTFT.printF((String)rawData.vRaw_Mois4, 68, 60 + (8 * 3), TFT_YELLOW);
        myTFT.printF((String)data.vTemp, 68, 60 + (8 * 4), TFT_YELLOW);
        myTFT.printF((String)data.vHum, 68, 60 + (8 * 5), TFT_YELLOW);
    }
}

// TAB3 Content || Diagnostics
void tab3_content() {
    getIcon();

    static TickType_t lastTick = 0;
    TickType_t nowTick = xTaskGetTickCount();

    // Position
    uint8_t shift0 = 1;
    uint8_t shift1 = 2;
    uint8_t shift2 = 4;
    uint8_t shift3 = 10;
    uint8_t shift4 = 5;
    uint8_t shift5 = 3;
    uint8_t shift6 = 6;
    uint8_t shift7 = 7;
    uint8_t shift8 = 8;
    uint8_t shift9 = 9;
    uint8_t shift10 = 0;

    // Loop Timing
    if(nowTick - lastTick >= pdMS_TO_TICKS(1000)) {
        lastTick = nowTick;
        
        // Refresh
        tft_lib.fillRect(46, 59, 100 , 90, TFT_BLACK);


        // WiFi Status
        myTFT.printF(F("WiFi :"), 2, 60 + (8 * shift0), TFT_YELLOW);
        if(myWifi.stateWiFi() == 1)
            myTFT.printF(F("Disconnect"), 46, 60 + (8 * shift0), TFT_RED);
        else 
            myTFT.printF(F("Connected"), 46, 60 + ( 8 * shift0), TFT_GREEN);
        

        // Mqtt Status
        myTFT.printF(F("MQTT :"), 2, 60 + (8 * shift1), TFT_YELLOW);
        if(myMqtt.MQTTState() == 1)
            myTFT.printF(F("Offline"), 46, 60 + (8 * shift1), TFT_RED);
        else 
            myTFT.printF(F("Online"), 46, 60 + (8 * shift1), TFT_GREEN);

        // Task
        myTFT.printF(F("Task :"), 2, 60 + (8 * shift2), TFT_YELLOW);
        myTFT.printF((String)uxTaskGetNumberOfTasks() + (" [Running]"), 46, 60 + (8 * shift2), TFT_YELLOW);


        // Uptime
        TickType_t nowSec = xTaskGetTickCount() / configTICK_RATE_HZ;
        static uint8_t sec = 0;
        static uint8_t min = 0;
        static uint8_t hour = 0;

        if(sec >= 60)
            sec = 0;
        else
            sec = nowSec % 60;

        if(sec >= 59)
            min++;
        else if(min >= 60)
            min = 0;

        if(min >= 59 && sec >= 59)
            hour++;
        else if(hour >= 60)
            hour = 0;

        static char times[32];

        sprintf(times, "%02d:%02d:%02d", hour, min, sec);

        myTFT.printF(F("Time :"), 2, 60 + (8 * shift3), TFT_YELLOW);
        myTFT.printF(times, 46, 60 + (8 * shift3), TFT_GREEN);


        // CPU temp
        static uint8_t i = 0;
        i++;
        if(i > 2)
            i = 0;
        myTFT.printF(F("CPU  :"), 2, 60 + (8 * shift4), TFT_YELLOW);
        if(temperatureRead() < 60.0)
            myTFT.printF(String(temperatureRead()).substring(0, 4) + " C", 46, 60 + (8 * shift4), TFT_GREEN);
        else if(temperatureRead() > 60.0)    
            myTFT.printF(String(temperatureRead()).substring(0, 4) + " C", 46, 60 + (8 * shift4), TFT_YELLOW);
        else if(temperatureRead() > 80.0)    
            myTFT.printF(String(temperatureRead()).substring(0, 4) + " C", 46, 60 + (8 * shift4), TFT_ORANGE);
        else if(temperatureRead() > 90.0)    
            myTFT.printF(String(temperatureRead()).substring(0, 4) + " C", 46, 60 + (8 * shift4), TFT_RED);


        // RSSI WiFi
        myTFT.printF(F("RSSI :"), 2, 60 + (8 * shift5), TFT_YELLOW);

        if(myWifi.stateWiFi() == 1)
            myTFT.printF(F("- dBm"), 46, 60 + (8 * shift5), TFT_RED);

        else if(WiFi.RSSI() < -35)
            myTFT.printF((String)WiFi.RSSI() + " dBm", 46, 60 + (8 * shift5), TFT_YELLOW);
        
        else if(WiFi.RSSI() < -55)
            myTFT.printF((String)WiFi.RSSI() + " dBm", 46, 60 + (8 * shift5), TFT_ORANGE);
        
        else if(WiFi.RSSI() < -80)
            myTFT.printF((String)WiFi.RSSI() + " dBm", 46, 60 + (8 * shift5), TFT_RED);
    
        else if(WiFi.RSSI() >= -35)
            myTFT.printF((String)WiFi.RSSI() + " dBm", 46, 60 + (8 * shift5), TFT_GREEN);


        // Free Heap
        myTFT.printF(F("Heap :"), 2, 60 + (8 * shift6), TFT_YELLOW);
        if(ESP.getFreeHeap() / 1024 <= 10)
            myTFT.printF((String)(ESP.getFreeHeap() / 1024)+ "! KB", 46, 60 + (8 * shift6), TFT_RED);
        else if(ESP.getFreeHeap() / 1024 < 50)
            myTFT.printF((String)(ESP.getFreeHeap() / 1024) + " KB", 46, 60 + (8 * shift6), TFT_YELLOW);
        else if(ESP.getFreeHeap() / 1024 > 80)
            myTFT.printF((String)(ESP.getFreeHeap() / 1024)+ " KB", 46, 60 + (8 * shift6), TFT_GREEN);


        // Block Heap - MAX
        myTFT.printF(F("MAX  :"), 2, 60 + (8 * shift7), TFT_YELLOW);
        if(heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) > 100)
            myTFT.printF((String)(heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) / 1024) +" KB" , 46, 60 + (8 * shift7), TFT_GREEN);
        else if(heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) < 100)
            myTFT.printF((String)(heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) / 1024) +" KB" , 46, 60 + (8 * shift7), TFT_YELLOW);
        else if(heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) < 50)
            myTFT.printF((String)(heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) / 1024) +" KB" , 46, 60 + (8 * shift7), TFT_ORANGE);
        else if(heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) < 20)
            myTFT.printF((String)(heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) / 1024) +" KB" , 46, 60 + (8 * shift7), TFT_RED);


        // Block Heap - SIZE
        myTFT.printF(F("MEM  :"), 2, 60 + (8 * shift8), TFT_YELLOW);
        if(ESP.getHeapSize() / 1024 > 100)
            myTFT.printF((String)(ESP.getHeapSize() / 1024) + " KB", 46, 60 + (8 * shift8), TFT_GREEN);
        else if(ESP.getHeapSize() / 1024 < 100)
            myTFT.printF((String)(ESP.getHeapSize() / 1024) + " KB", 46, 60 + (8 * shift8), TFT_YELLOW);
        else if(ESP.getHeapSize() / 1024 < 50)
            myTFT.printF((String)(ESP.getHeapSize() / 1024) + " KB", 46, 60 + (8 * shift8), TFT_ORANGE);
        else if(ESP.getHeapSize() / 1024 < 10)
            myTFT.printF((String)(ESP.getHeapSize() / 1024) + " KB", 46, 60 + (8 * shift8), TFT_RED);


        // Block Heap - USED
        myTFT.printF(F("USED :"), 2, 60 + (8 * shift9), TFT_YELLOW);
        if((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024 > 300)
            myTFT.printF((String)((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + " KB", 46, 60 + (8 * shift9), TFT_RED);
        else if((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024 < 300)
            myTFT.printF((String)((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + " KB", 46, 60 + (8 * shift9), TFT_ORANGE);
        else if((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024 < 80)
            myTFT.printF((String)((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + " KB", 46, 60 + (8 * shift9), TFT_YELLOW);
        else if((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024 < 50)
            myTFT.printF((String)((ESP.getHeapSize() - ESP.getFreeHeap()) / 1024) + " KB", 46, 60 + (8 * shift9), TFT_GREEN);
    

        // Mode || Offline - Online
        myTFT.printF(F("Mode :"), 2, 60 + (8 * shift10), TFT_YELLOW);
        if(xOfflineMode && !xOnlineMode)
            myTFT.printF(F("Offline"), 46, 60 + (8 * shift10), TFT_RED);
        else
            myTFT.printF(F("Online"), 46, 60 + (8 * shift10), TFT_GREEN);
    }
}

// TAB4 Content || Settings
void tab4_content() {
     
}