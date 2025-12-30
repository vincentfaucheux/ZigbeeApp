#include "mainwindow.h" 
#include <QDebug>
#include <fstream>
#include <yaml-cpp/yaml.h>

MainWindow::MainWindow() {

    //Check if the configuration file exists
    std::ifstream ConfigFile("configuration.yaml");
    if (!ConfigFile.good()) {
        qDebug() << "Not able to open the configuration file";
    } else {
        //close the file
        ConfigFile.close();

        //read the yaml file to get the path of zigbee2mqtt
        YAML::Node YamlConfig = YAML::Node(YAML::NodeType::Null);
        std::string Zigbee2MqttPath = "";
        std::string ZigbeeLib = "";
        try {
            YamlConfig = YAML::LoadFile("configuration.yaml");
            if( YamlConfig["mqtt_config_path"] ) {
                Zigbee2MqttPath = YamlConfig["mqtt_config_path"].as<std::string>();
                qDebug() << "mqtt_config_path:" << QString::fromStdString(Zigbee2MqttPath);
            } else {
                qDebug() << "mqtt_config_path not found in configuration.yaml";
            }
            if( YamlConfig["zigbee_app_lib"] ) {
                ZigbeeLib = YamlConfig["zigbee_app_lib"].as<std::string>();
                qDebug() << "zigbee_app_lib:" << QString::fromStdString(ZigbeeLib);
            } else {
                qDebug() << "zigbee_app_lib not found in configuration.yaml";
            }
        } 
        catch (const YAML::BadFile& e) {
            qDebug() << "Error reading YAML file:" << e.what();
        }
        catch (const YAML::ParserException& e) {
            qDebug() << "Error parsing YAML file:" << e.what();
        }

        //if path exists, continue
        if(( Zigbee2MqttPath != "" ) && ( ZigbeeLib != "" )) {
            //try to open the shared library
            bool bZigbeeSoOk = openZigbeeSo( ZigbeeLib, Zigbee2MqttPath);

            //if nok quit the program
            if( bZigbeeSoOk == false) {
                    qDebug() << "Not able to open the shared library";
            }
            else {

                // create the Window
                auto* central = new QWidget;

                // GroupBox choix des modules
                QGroupBox *groupBoxModules = new QGroupBox("Select module");
                // ComboBox choix des modules
                comboModules = new QComboBox();
                comboModules->addItem("premier_switch");
                // Layout GroupBox choix des modules
                auto* groupModulesLayout = new QVBoxLayout();
                groupModulesLayout->addWidget(comboModules);
                groupBoxModules->setLayout(groupModulesLayout);

                // GroupBox choix des etats
                QGroupBox *groupBoxStates= new QGroupBox("Select state");
                // ComboBox choix des modules
                comboStates = new QComboBox();
                comboStates->addItem("CONFORT");
                comboStates->addItem("ECO");
                comboStates->addItem("HORS GEL");
                comboStates->addItem("OFF");
                // Layout GroupBox choix des modules
                auto* groupStatesLayout = new QVBoxLayout();
                groupStatesLayout->addWidget(comboStates);
                groupBoxStates->setLayout(groupStatesLayout);

                auto* groupsLayout = new QVBoxLayout();
                groupsLayout->addWidget(groupBoxModules);
                groupsLayout->addWidget(groupBoxStates);

                QPushButton *button = new QPushButton("Action Zigbee");

                auto* mainLayout = new QVBoxLayout();
                mainLayout->addLayout(groupsLayout);
                mainLayout->addWidget(button);
                central->setLayout(mainLayout);

                setCentralWidget(central);

                //connect(comboModules, QOverload<int>::of(&QComboBox::currentIndexChanged),
                //        this, &MainWindow::onPluginChanged);

                //onPluginChanged(0);

                QObject::connect(button, &QPushButton::clicked, [&]() {
                    MainWindow::commandZigbee(comboModules->currentText(),
                                comboStates->currentText());
                });
            }
        }
    }

 }


MainWindow::~MainWindow() {
    closeZigbeeSo();
}

bool MainWindow::openZigbeeSo( std::string ZigbeeLib, std::string Zigbee2MqttPath  ){
    bool bRet = true;
   handle = dlopen( ZigbeeLib.c_str(), RTLD_LAZY);
    if (!handle) {
        qDebug() << dlerror();
        bRet = false;
    }

    if( bRet == true) {
        auto create = reinterpret_cast<ZigbeeInterface*(*)(std::string)>(
            dlsym(handle, "create_zigbee")
        );

        if(!create) {
            qDebug() << dlerror();
            bRet = false;
        }
        else {
            plugin = create( Zigbee2MqttPath);
            if( plugin == nullptr ) {
                qDebug() << "Not able to create the zigbee interface";
                bRet = false;
            }
        }
    }

    return bRet;
}

void MainWindow::closeZigbeeSo() {
    if (plugin && handle) {
        auto destroy = reinterpret_cast<void(*)(ZigbeeInterface*)>(
            dlsym(handle, "destroy_zigbee")
        );
        destroy(plugin);
        dlclose(handle);
    }
    plugin = nullptr;
    handle = nullptr;
}

//void MainWindow::commandZigbeeObsolete(const QString& path) {
//
//    if( plugin != nullptr ) {
//        qDebug() << "Plugin chargé:" << plugin->name();
//        qDebug() << "Compute:" << plugin->compute(4, 5);
//    }
//}

void MainWindow::commandZigbee(const QString& ModuleSelected, const QString& StateSelected) {

    qDebug() << "Action zigbee";
    qDebug() << "module: " << ModuleSelected;
    qDebug() << "state: " << StateSelected;
    std::string ModuleSelected_std = ModuleSelected.toStdString();
    std::string StateSelected_std = StateSelected.toStdString();
    plugin->Switch(ModuleSelected_std, StateSelected_std);
}

//void MainWindow::onPluginChanged(int) {
//    commandZigbeeObsolete(comboModules->currentData().toString());
//}