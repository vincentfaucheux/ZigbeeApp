#ifndef ZIGBEESO_H
#define ZIGBEESO_H

#include <mosquitto.h>
#include <string>
#include <iostream>
#include "ZigbeeSo_interface.h"

class t_Zigbee : public ZigbeeInterface {
public:
    t_Zigbee();
    ~t_Zigbee();
    void Switch(std::string module, 
            std::string state) override;

private:
    mosquitto *mosq = nullptr;

};

#endif //ZIGBEESO_H
