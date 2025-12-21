#pragma once

class ZigbeeInterface {
public:
    virtual ~ZigbeeInterface() = default;
    virtual const char* name() const = 0;
    virtual int compute(int a, int b) = 0;
};

extern "C" {
    ZigbeeInterface* create_zigbee();
    void destroy_zigbee(ZigbeeInterface*);
}
