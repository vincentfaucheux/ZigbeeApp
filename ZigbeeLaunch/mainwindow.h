#pragma once

#include <QMainWindow>
#include <QComboBox>
#include <dlfcn.h>
#include "../ZigbeeSo/ZigbeeSo_interface.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private slots:
    void onPluginChanged(int index);

private:
    QComboBox* combo;
    void* handle = nullptr;
    ZigbeeInterface* plugin = nullptr;

    void loadPlugin(const QString& path);
    void unloadPlugin();
};
