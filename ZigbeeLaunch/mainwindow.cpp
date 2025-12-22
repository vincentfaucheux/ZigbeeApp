#include "mainwindow.h" 
#include <QVBoxLayout>
#include <QDebug>

MainWindow::MainWindow() {
    combo = new QComboBox;
    combo->addItem("Addition", "../ZigbeeSo/ZigbeeSoSub/libZigbeeSo.so");

    auto* central = new QWidget;
    auto* layout = new QVBoxLayout(central);
    layout->addWidget(combo);
    setCentralWidget(central);

    connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onPluginChanged);

    onPluginChanged(0);
}


MainWindow::~MainWindow() {
    unloadPlugin();
}

void MainWindow::loadPlugin(const QString& path) {
    unloadPlugin();

    handle = dlopen(path.toStdString().c_str(), RTLD_LAZY);
    if (!handle) {
        qDebug() << dlerror();
        return;
    }

    auto create = reinterpret_cast<ZigbeeInterface*(*)()>(
        dlsym(handle, "create_zigbee")
    );

    plugin = create();

    qDebug() << "Plugin chargé:" << plugin->name();
    qDebug() << "Compute:" << plugin->compute(4, 5);
}

void MainWindow::unloadPlugin() {
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

void MainWindow::onPluginChanged(int) {
    loadPlugin(combo->currentData().toString());
}
