
#ifndef ZIGBEESO_INTERFACE_H
#define ZIGBEESO_INTERFACE_H

#include <cstring>
#include <iostream>

class ZigbeeInterface {
public:
    virtual ~ZigbeeInterface() = default;
    virtual int GetDevicesNumber() = 0;
    virtual std::string GetDeviceID(int index) = 0;
    virtual void Switch(std::string module, 
        std::string state) = 0;
};

extern "C" {
    ZigbeeInterface* create_zigbee( std::string MqttPath);
    void destroy_zigbee(ZigbeeInterface*);
}

#endif //ZIGBEESO_INTERFACE_H