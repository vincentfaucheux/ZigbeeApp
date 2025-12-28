#include "mainwindow.h" 
#include <QDebug>

MainWindow::MainWindow() {

    //try to open the shared library
    bool bZigbeeSoOk = openZigbeeSo();

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


MainWindow::~MainWindow() {
    closeZigbeeSo();
}

bool MainWindow::openZigbeeSo(){
    bool bRet = true;
   handle = dlopen( "../ZigbeeSo/libZigbeeSo.so", RTLD_LAZY);
    if (!handle) {
        qDebug() << dlerror();
        bRet = false;
    }

    if( bRet == true) {
        auto create = reinterpret_cast<ZigbeeInterface*(*)()>(
            dlsym(handle, "create_zigbee")
        );

        if(!create) {
            qDebug() << dlerror();
            bRet = false;
        }
        else {
            plugin = create();
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