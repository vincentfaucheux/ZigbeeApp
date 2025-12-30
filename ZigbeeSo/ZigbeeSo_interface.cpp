
#include "ZigbeeSo_interface.h"
#include "ZigbeeSo.h"

extern "C" ZigbeeInterface* create_zigbee(std::string MqttPath) {
    bool bAllOk = true;
    ZigbeeInterface* Zigbee_P = new t_Zigbee( MqttPath, &bAllOk);
    if( bAllOk == false ) {
        delete Zigbee_P;
        Zigbee_P = nullptr;
    }

    return( Zigbee_P);
}

extern "C" void destroy_zigbee(ZigbeeInterface* p) {
    delete p;
}
