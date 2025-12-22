#include "mainwindow.h" 
#include <QDebug>

MainWindow::MainWindow() {

    // Window
    auto* central = new QWidget;

    // GroupBox choix des modules
    QGroupBox *groupBoxModules = new QGroupBox("Select module");
    // ComboBox choix des modules
    comboModules = new QComboBox();
    comboModules->addItem("Addition", "../ZigbeeSo/ZigbeeSoSub/libZigbeeSo.so");
    comboModules->addItem("Substraction", "../ZigbeeSo/ZigbeeSoSub/libZigbeeSo.so");
    // Layout GroupBox choix des modules
    auto* groupModulesLayout = new QVBoxLayout();
    groupModulesLayout->addWidget(comboModules);
    groupBoxModules->setLayout(groupModulesLayout);

    // GroupBox choix des etats
    QGroupBox *groupBoxStates= new QGroupBox("Select state");
    // ComboBox choix des modules
    comboStates = new QComboBox();
    comboStates->addItem("On");
    comboStates->addItem("Off");
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

    openZigbeeSo();

 }


MainWindow::~MainWindow() {
    closeZigbeeSo();
}

void MainWindow::openZigbeeSo(){
   handle = dlopen( "../ZigbeeSo/ZigbeeSoSub/libZigbeeSo.so", RTLD_LAZY);
    if (!handle) {
        qDebug() << dlerror();
        return;
    }

    auto create = reinterpret_cast<ZigbeeInterface*(*)()>(
        dlsym(handle, "create_zigbee")
    );

    plugin = create();
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
}

//void MainWindow::onPluginChanged(int) {
//    commandZigbeeObsolete(comboModules->currentData().toString());
//}