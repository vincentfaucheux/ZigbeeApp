#ifndef ZIGBEESO_H
#define ZIGBEESO_H

#include <mosquitto.h>
#include <string>
#include <iostream>
#include "ZigbeeSo_interface.h"
#include "ZigbeeSo_config.h"

class t_Zigbee : public ZigbeeInterface {
public:
    t_Zigbee(std::string MqttPath, bool* pbAllOk);
    ~t_Zigbee();
    void Switch(std::string module, 
            std::string state) override;

private:
    mosquitto *mosq = nullptr;
    tZigbeesoConfig* config_Ptr = nullptr;
};

#endif //ZIGBEESO_H
