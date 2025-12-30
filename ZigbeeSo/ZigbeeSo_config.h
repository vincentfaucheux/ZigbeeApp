#ifndef ZIGBEESO_CONFIG_H
#define ZIGBEESO_CONFIG_H

#include <cstring>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <vector>

class tZigbeesoConfig{
public:
    tZigbeesoConfig();
    ~tZigbeesoConfig();
    bool LoadConfig(std::string ConfigPath);
    int GetDevicesNumber();
    std::string GetDeviceID(int index);

private:
    std::vector<std::string> vModuleListe ;
};

#endif //ZIGBEESO_CONFIG_H