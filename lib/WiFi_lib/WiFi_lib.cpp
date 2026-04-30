#include "WiFi_lib.h"

String STA_ssid = "";
String STA_pass = "";

WiFi_lib::WiFi_lib() {
    //                      No Once Func in Here
};



/* 
——————————————————————————————————————————————————————————————————————————————————————————————
                                        Status of WiFi
——————————————————————————————————————————————————————————————————————————————————————————————
*/
//                  Status of WiFi[Connect = false -- Disconnect = true]

bool WiFi_lib::stateWiFi() {
    static bool StateNow = false;       // Temporary Storage of WiFiState
    uint8_t Wifistatus = WiFi.status(); // Save WiFi status on 8byte Form
    
    if(Wifistatus != WL_CONNECTED)
        StateNow = true;  // True == Not connected
    else
        StateNow = false; // False == Connected
        
    return StateNow;
}
/* 
——————————————————————————————————————————————————————————————————————————————————————————————
                                        Status of WiFi
——————————————————————————————————————————————————————————————————————————————————————————————
*/



/* 
——————————————————————————————————————————————————————————————————————————————————————————————
                                WiFi Signal Strength Level
——————————————————————————————————————————————————————————————————————————————————————————————
*/
//                  WiFi Signal Strength Level[Range: 1-5 -- {low - strong}]

uint8_t WiFi_lib::WiFiSignal() {
    int signal = WiFi.RSSI();                               // Get Signal Strength Range
    signal = constrain(map(signal, -85, -55, 1, 5), 1, 5);  // Change Range from -85 | -55 to 1 | 5
    
    return signal;
}
/* 
——————————————————————————————————————————————————————————————————————————————————————————————
                                WiFi Signal Strength Level
——————————————————————————————————————————————————————————————————————————————————————————————
*/



/* 
——————————————————————————————————————————————————————————————————————————————————————————————
                                    WiFi Auto Reconnect Func
——————————————————————————————————————————————————————————————————————————————————————————————
*/
void WiFi_lib::autoRec(bool OnlineMode, bool OfflineMode) {
    bool nowState = stateWiFi();    // Get WiFi Status
    static bool lastState = false;  // Last Status Wifi
    
    // Last Mode Var [Boolean]
    static bool lastOnlineMode = false;
    static bool lastOfflineMode = false;

    // ——————————————————————————————————————————————————————————————————————————
    //                          While Online mode active
    if((!lastOnlineMode && OnlineMode) || (lastOfflineMode && !OfflineMode)) {
        WiFi.setAutoConnect(true);
        WiFi.setAutoReconnect(true);
        
        Serial.print(F("\nOnline Mode Active! || Offline Mode Deactive!\n"));
    } 
    
    //                          While Online Mode Deactive

    else if((lastOnlineMode && !OnlineMode) || (!lastOfflineMode && OfflineMode)) {
        WiFi.setAutoConnect(false);
        WiFi.setAutoReconnect(false);
        
        Serial.print(F("\nOnline Mode Deactive! || Offline Mode Active!\n"));
    }
    
    //                              Update Status

    lastOnlineMode = OnlineMode;
    lastOfflineMode = OfflineMode;
    // ——————————————————————————————————————————————————————————————————————————
    
    //                               Update Conf

    getConf();  // Get Config
    
    // ——————————————————————————————————————————————————————————————————————————
    //              WiFi Auto Reconnect while Online mode Active

    if((nowState) && (OnlineMode && !OfflineMode)) {
        WiFi.begin(STA_ssid.c_str(), STA_pass.c_str()); // Reconnect
        WiFi.reconnect();
    }
    // ——————————————————————————————————————————————————————————————————————————

    if(lastState && !nowState) {
        Serial.println(F("Connected..!"));
    } else if(!lastState && nowState) {
        Serial.println(F("Disconnected..!"));
    }

    lastState = nowState;
}
/* 
——————————————————————————————————————————————————————————————————————————————————————————————
                                    WiFi Auto Reconnect Func
——————————————————————————————————————————————————————————————————————————————————————————————
*/



/* 
——————————————————————————————————————————————————————————————————————————————————————————————
                                        WiFi Auto Set Func
——————————————————————————————————————————————————————————————————————————————————————————————
*/
void WiFi_lib::setupWiFi() {
    esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE); // Set All Channel to CH1
    WiFi.setHostname(HostName);                     // Configure HostName WiFi
    WiFi.mode(WIFI_AP_STA);                         // Set WiFi Mode [Dual Mode]
    WiFi.softAP(AP_ssid, AP_pass);                  // Set AP mode Config
    
    
    //                              Set Auto Connect
    
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    
    //                                 Web Page Handler
    
    web.on("/", handleURL);         // Main Page
    web.on("/save", handleSave);    // Endpoint
    
    web.begin();

    getConf();

    WiFi.begin(STA_ssid.c_str(), STA_pass.c_str()); // Set STA mode Config
    WiFi.reconnect();

    if(WiFi.status() == WL_CONNECTED)
        Serial.println(F("Connected..!"));
}
/* 
——————————————————————————————————————————————————————————————————————————————————————————————
                                        WiFi Auto Set Func
——————————————————————————————————————————————————————————————————————————————————————————————
*/



/* 
——————————————————————————————————————————————————————————————————————————————————————————————
                                        Handling Login Page 
——————————————————————————————————————————————————————————————————————————————————————————————
*/
void WiFi_lib::Login() {
    //                          Handling HTML Server Web
    
    web.handleClient();
}
/* 
——————————————————————————————————————————————————————————————————————————————————————————————
                                        Handling Login Page 
——————————————————————————————————————————————————————————————————————————————————————————————
*/