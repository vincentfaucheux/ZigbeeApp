#include "ZigbeeSo.h"

t_Zigbee ::t_Zigbee(std::string MqttPath, bool* pbAllOk) {
    // Init config module
    config_Ptr = new tZigbeesoConfig();
    if( config_Ptr != nullptr) {
        // Load MQTT configuration
         *pbAllOk = config_Ptr->LoadConfig(MqttPath);

    //Check if item in the vModuleListe
    if(config_Ptr->GetDevicesNumber() == 0) {
        std::cerr << "No module found in configuration" << std::endl;
        *pbAllOk = false;
    } else {
        // Initialize Mosquitto
        mosquitto_lib_init();
        mosq = mosquitto_new(nullptr, true, nullptr);
        // if mosq is nullptr, handle the error
        if(!mosq) {
            std::cerr << "Failed to create Mosquitto instance" << std::endl;
            *pbAllOk = false;
        } else {
            if(mosquitto_connect(mosq, "localhost", 1883, 60) != MOSQ_ERR_SUCCESS) {
                std::cerr << "Failed to connect to Mosquitto broker" << std::endl;
                *pbAllOk = false;
            }
        }
    }
    } else {
        std::cerr << "not able to open the config class" << std::endl;
        *pbAllOk = false;
    }
}

t_Zigbee ::~t_Zigbee() {
    //delete mosquitto instance
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    //delete config instance
    if( config_Ptr != nullptr) {
        delete config_Ptr;
        config_Ptr = nullptr;
    }
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

