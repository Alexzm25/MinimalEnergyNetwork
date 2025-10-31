#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.mainToolBar->addAction(ui.actionLoadRoutes);
    ui.mainToolBar->addAction(ui.actionSaveRoutes);

}

MainWindow::~MainWindow()
{}

