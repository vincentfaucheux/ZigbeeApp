
#include "ZigbeeSo_interface.h"
#include "ZigbeeSo.h"

extern "C" ZigbeeInterface* create_zigbee() {
    ZigbeeInterface* Zigbee_P = new t_Zigbee();

    return( Zigbee_P);
}

extern "C" void destroy_zigbee(ZigbeeInterface* p) {
    delete p;
}
