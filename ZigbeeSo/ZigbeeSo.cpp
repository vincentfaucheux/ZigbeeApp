#include "ZigbeeSo.h"

t_Zigbee ::t_Zigbee(std::string MqttPath, bool* pbAllOk) {
    // Load MQTT configuration
    vModuleListe.clear();
    try {
        MqttConfig = YAML::LoadFile(MqttPath + "/configuration.yaml");
        if( !MqttConfig["devices"] ) {
            std::cerr << "No device in this configuration file" << std::endl;
            *pbAllOk = false;
        }
        else {
            YAML::Node devicesNode = MqttConfig["devices"];
            if(( devicesNode) &&( devicesNode.IsMap())) {
                for (const auto& device : devicesNode) {
                    std::string device_id = device.first.as<std::string>();
                    YAML::Node device_data = device.second;
                    if ((device_data["friendly_name"]) &&
                        (device_data["friendly_name"].IsScalar())) {
                        std::string ModuleName =device_data["friendly_name"].as<std::string>();
                        vModuleListe.push_back( ModuleName);
                    }
                    else {
                    }
                }
            }
        }
    }
    catch (const YAML::BadFile& e) {
        std::cerr << "Error reading MQTT configuration file: " << e.what() << std::endl;
        *pbAllOk = false;
    }
    catch (const YAML::ParserException& e) {
        std::cerr << "Error parsing MQTT configuration file: " << e.what() << std::endl;
        *pbAllOk = false;
    }

    //Check if item in the vModuleListe
    if(vModuleListe.empty()) {
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

