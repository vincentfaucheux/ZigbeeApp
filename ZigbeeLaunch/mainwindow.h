#pragma once

#include <QMainWindow>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <dlfcn.h>
#include "../ZigbeeSo/ZigbeeSo_interface.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private slots:
    //void onPluginChanged(int index);

private:
    QComboBox* comboModules = nullptr;
    QComboBox* comboStates= nullptr;
    void* handle = nullptr;
    ZigbeeInterface* plugin = nullptr;

    //void commandZigbeeObsolete(const QString& path);
    void commandZigbee(const QString& ModuleSelected, const QString& StateSelected);
    void closeZigbeeSo();
    bool openZigbeeSo();
};
