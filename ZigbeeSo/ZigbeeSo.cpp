#include "ZigbeeSo.h"

t_Zigbee ::t_Zigbee() {
    mosquitto_lib_init();
    mosq = mosquitto_new(nullptr, true, nullptr);
    if(!mosq) {
        std::cerr << "Failed to create Mosquitto instance" << std::endl;
    } else {
        if(mosquitto_connect(mosq, "localhost", 1883, 60) != MOSQ_ERR_SUCCESS) {
            std::cerr << "Failed to connect to Mosquitto broker" << std::endl;
        }
    }
}

t_Zigbee ::~t_Zigbee() {
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

void t_Zigbee ::Switch(std::string module, 
        std::string state) {
    std::string state_l1 = "";
    std::string state_l2 = "";
    // Simple parsing logic for state
    if(state == "CONFORT") {
        state_l1 = "OFF";
        state_l2 = "OFF";
    } else if(state == "ECO") {
        state_l1 = "ON";
        state_l2 = "ON";
    } else if(state == "HORS GEL") {
        state_l1 = "ON";
        state_l2 = "OFF";
    } else {
        state_l1 = "OFF";
        state_l2 = "ON";
    }

    // Request to Mosquitto
    std::string topic = "zigbee2mqtt/" + module + "/set";
    std::string payload = "{\"state_l1\":\"" + state_l1 + "\"" 
        +", " 
        +"\"state_l2\":\"" + state_l2  + "\"" 
        +"}";
    mosquitto_publish(
        mosq,
        nullptr,
        topic.c_str(),
        payload.length(),
        payload.c_str(),
    0,
    false
    );
}

