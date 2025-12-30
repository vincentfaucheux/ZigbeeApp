#ifndef ZIGBEESO_H
#define ZIGBEESO_H

#include <mosquitto.h>
#include <yaml-cpp/yaml.h>
#include <string>
#include <iostream>
#include "ZigbeeSo_interface.h"
#include <vector>

class t_Zigbee : public ZigbeeInterface {
public:
    t_Zigbee(std::string MqttPath, bool* pbAllOk);
    ~t_Zigbee();
    void Switch(std::string module, 
            std::string state) override;

private:
    mosquitto *mosq = nullptr;
    YAML::Node MqttConfig = YAML::Node(YAML::NodeType::Null);
    std::vector<std::string> vModuleListe ;
};

#endif //ZIGBEESO_H
