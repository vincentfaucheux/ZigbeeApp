#include "ZigbeeSo_interface.h"

class t_Zigbee : public ZigbeeInterface {
public:
    t_Zigbee() {
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

    ~t_Zigbee() {
        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
    }

    void Switch(std::string module, 
            std::string state) override {
        // Implementation of switching logic
        std::string topic = "zigbee2mqtt/" + module + "/set";
        std::string payload = "{\"state_l1\":\"" + state + "\"" 
            +", " 
            +"\"state_l2\":\"" + state  + "\"" 
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

private:
    mosquitto *mosq = nullptr;

};

extern "C" ZigbeeInterface* create_zigbee() {
    ZigbeeInterface* Zigbee_P = new t_Zigbee();

    return( Zigbee_P);
}

extern "C" void destroy_zigbee(ZigbeeInterface* p) {
    delete p;
}
