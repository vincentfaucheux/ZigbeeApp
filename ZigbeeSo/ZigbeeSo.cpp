#include "ZigbeeSo_interface.h"

class ZigbeeA : public ZigbeeInterface {
public:
    const char* name() const override {
        return "Addition";
    }

    int compute(int a, int b) override {
        return a + b;
    }
};

extern "C" ZigbeeInterface* create_zigbee() {
    return new ZigbeeA();
}

extern "C" void destroy_zigbee(ZigbeeInterface* p) {
    delete p;
}
