#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTextEdit>
#include "ui_MainWindow.h"
#include "SensorNetwork.h"
#include "GraphScene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadRoutes();
    void onSaveRoutes();
    void onGenerateExample();
    void onCalculateMST();
    void onAddSensor();
    void onShowAllConnections();

private:
    Ui::MainWindowClass ui;
    SensorNetwork network;
    GraphScene* graphScene;
    GraphScene* mstScene;

    void setupUI();
    void setupConnections();
    void displayOutput(const QString& text);
    void updateGraphVisualization();
};

