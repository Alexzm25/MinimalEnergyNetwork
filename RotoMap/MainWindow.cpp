#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    setWindowTitle("Sistema de Gestión de Red de Sensores Inteligentes");
    
    // Crear escenas para los gráficos
    graphScene = new GraphScene(this);
    mstScene = new GraphScene(this);
    
    // Asignar escenas a los QGraphicsView
    ui.graphicsView->setScene(graphScene);
    ui.graphicsView_2->setScene(mstScene);
    
    // Configurar las vistas
    ui.graphicsView->setRenderHint(QPainter::Antialiasing);
    ui.graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui.graphicsView_2->setRenderHint(QPainter::Antialiasing);
    ui.graphicsView_2->setDragMode(QGraphicsView::ScrollHandDrag);
    
    setupUI();
    setupConnections();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // Configurar toolbar
    ui.mainToolBar->addAction(ui.actionLoadRoutes);
    ui.mainToolBar->addAction(ui.actionSaveRoutes);
    
    // Renombrar botones para el proyecto de sensores
    ui.pushButton_2->setText("Agregar Sensor");
    ui.pushButton_3->setText("Generar Ejemplo");
    ui.pushButton_4->setText("Mostrar Conexiones");
    ui.pushButton->setText("Limpiar");
    
    ui.pushButton_5->setText("Calcular MST");
    ui.pushButton_6->setText("Exportar Resultado");
    ui.pushButton_7->setText("Guardar Resultado");
    
    // Configurar tabs
    ui.mainTabs->setTabText(0, "Red de Sensores");
    ui.mainTabs->setTabText(1, "Arbol de Expansion Minima");
    
    // Configurar output
    ui.textEdit->setReadOnly(true);
    ui.textEdit->setPlainText("Bienvenido al Sistema de Gestion de Red de Sensores\n"
                              "==================================================\n\n"
                              "Instrucciones:\n"
                              "1. Agregue sensores manualmente o genere un ejemplo\n"
                              "2. El grafo se mostrara automaticamente\n"
                              "3. Use la pestana 'Arbol de Expansion Minima' para calcular la red optima\n");
}

void MainWindow::setupConnections()
{
    // Conectar señales y slots del toolbar
    connect(ui.actionLoadRoutes, &QAction::triggered, this, &MainWindow::onLoadRoutes);
    connect(ui.actionSaveRoutes, &QAction::triggered, this, &MainWindow::onSaveRoutes);
    
    // Conectar botones de la primera pestaña
    connect(ui.pushButton_2, &QPushButton::clicked, this, &MainWindow::onAddSensor);
    connect(ui.pushButton_3, &QPushButton::clicked, this, &MainWindow::onGenerateExample);
    connect(ui.pushButton_4, &QPushButton::clicked, this, &MainWindow::onShowAllConnections);
    connect(ui.pushButton, &QPushButton::clicked, [this]() {
        network.getGraph().clear();
        graphScene->clear();
        mstScene->clear();
        ui.textEdit->clear();
        ui.textEdit->append("Red limpiada exitosamente.\n");
    });
    
    // Conectar botones de la segunda pestaña
    connect(ui.pushButton_5, &QPushButton::clicked, this, &MainWindow::onCalculateMST);
    connect(ui.pushButton_6, &QPushButton::clicked, this, &MainWindow::onSaveRoutes);
    connect(ui.pushButton_7, &QPushButton::clicked, [this]() {
        if (network.getGraph().saveResults("resultado.txt", {}, 0.0f)) {
            QMessageBox::information(this, "Éxito", 
                "Resultado guardado en resultado.txt");
        }
    });
}

void MainWindow::onLoadRoutes()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        "Cargar configuración de sensores",
        "",
        "Archivos de texto (*.txt);;Todos los archivos (*.*)"
    );

    if (!filename.isEmpty()) {
        if (network.loadFromFile(filename.toStdString())) {
            updateGraphVisualization();
            ui.textEdit->append("Sensores cargados exitosamente desde: " + filename + "\n");
            QMessageBox::information(this, "Éxito", 
                "Sensores cargados exitosamente desde el archivo.");
        } else {
            QMessageBox::warning(this, "Error", 
                "No se pudo cargar el archivo de sensores.");
        }
    }
}

void MainWindow::onSaveRoutes()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        "Guardar configuracion de sensores",
        "sensores.txt",
        "Archivos de texto (*.txt);;Todos los archivos (*.*)"
    );

    if (!filename.isEmpty()) {
        if (network.saveToFile(filename.toStdString())) {
            ui.textEdit->append("Sensores guardados en: " + filename + "\n");
            QMessageBox::information(this, "Exito", 
                "Sensores guardados exitosamente en el archivo.");
        } else {
            QMessageBox::warning(this, "Error", 
                "No se pudo guardar el archivo de sensores.");
        }
    }
}

void MainWindow::onGenerateExample()
{
    network.generateExampleSensors();
    updateGraphVisualization();
    
    ui.textEdit->append("\n=== Sensores de Ejemplo Generados ===\n");
    const vector<Sensor*>& sensors = network.getGraph().getSensors();
    for (size_t i = 0; i < sensors.size(); i++) {
        ui.textEdit->append(QString("• %1 - Posicion: (%2, %3)")
            .arg(QString::fromStdString(sensors[i]->getName()))
            .arg(sensors[i]->getX())
            .arg(sensors[i]->getY()));
    }
    ui.textEdit->append("\n");
    
    QMessageBox::information(this, "Exito", 
        QString("Se generaron %1 sensores de ejemplo.").arg(sensors.size()));
}

void MainWindow::onAddSensor()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Nuevo Sensor",
        "Nombre del sensor:", QLineEdit::Normal, "", &ok);
    
    if (!ok || name.isEmpty()) return;
    
    double x = QInputDialog::getDouble(this, "Posicion X",
        "Coordenada X:", 50.0, 0.0, 100.0, 1, &ok);
    
    if (!ok) return;
    
    double y = QInputDialog::getDouble(this, "Posicion Y",
        "Coordenada Y:", 50.0, 0.0, 100.0, 1, &ok);
    
    if (!ok) return;
    
    network.addSensor(name.toStdString(), x, y);
    updateGraphVisualization();
    
    ui.textEdit->append(QString("Sensor agregado: %1 en (%2, %3)\n")
        .arg(name).arg(x).arg(y));
    
    QMessageBox::information(this, "Exito", 
        "Sensor agregado exitosamente.");
}

void MainWindow::onShowAllConnections()
{
    const vector<Sensor*>& sensors = network.getGraph().getSensors();
    
    if (sensors.size() < 2) {
        QMessageBox::warning(this, "Advertencia", 
            "Se necesitan al menos 2 sensores para mostrar conexiones.");
        return;
    }
    
    network.buildCompleteNetwork();
    updateGraphVisualization();
    
    ui.textEdit->append("\n=== Todas las Conexiones Posibles ===\n");
    
    const vector<Edge>& edges = network.getGraph().getEdges();
    QSet<QPair<QString, QString>> shown;
    
    int count = 0;
    for (const Edge& edge : edges) {
        QString from = QString::fromStdString(edge.from->getName());
        QString to = QString::fromStdString(edge.to->getName());
        
        QPair<QString, QString> pair1(from, to);
        QPair<QString, QString> pair2(to, from);
        
        if (!shown.contains(pair1) && !shown.contains(pair2)) {
            shown.insert(pair1);
            count++;
            ui.textEdit->append(QString("%1. %2 <--> %3 (Costo: %4)")
                .arg(count).arg(from).arg(to).arg(edge.weight, 0, 'f', 2));
        }
    }
    
    ui.textEdit->append(QString("\nTotal de conexiones: %1\n").arg(count));
}

void MainWindow::onCalculateMST()
{
    const vector<Sensor*>& sensors = network.getGraph().getSensors();
    
    if (sensors.size() < 2) {
        QMessageBox::warning(this, "Advertencia", 
            "Se necesitan al menos 2 sensores para calcular el MST.");
        return;
    }
    
    // Asegurar que el grafo esté completo
    if (network.getGraph().getEdges().empty()) {
        network.buildCompleteNetwork();
        updateGraphVisualization();
    }
    
    // Calcular MST
    pair<vector<Edge>, float> result = network.getGraph().primMST();
    vector<Edge> mst = result.first;
    float totalCost = result.second;
    
    if (mst.empty()) {
        QMessageBox::warning(this, "Error", 
            "No se pudo calcular el MST.");
        return;
    }
    
    // Mostrar MST en la segunda vista
    mstScene->displayGraph(network.getGraph());
    mstScene->displayMST(mst);
    
    // Mostrar resultados en texto
    ui.textEdit->clear();
    ui.textEdit->append("=================================================");
    ui.textEdit->append("  ARBOL DE EXPANSION MÍNIMA (Algoritmo de Prim)");
    ui.textEdit->append("=================================================\n");
    
    ui.textEdit->append("Conexiones seleccionadas para la red óptima:\n");
    for (size_t i = 0; i < mst.size(); i++) {
        ui.textEdit->append(QString("%1. %2 <--> %3 (Costo: %4 unidades)")
            .arg(i + 1)
            .arg(QString::fromStdString(mst[i].from->getName()))
            .arg(QString::fromStdString(mst[i].to->getName()))
            .arg(mst[i].weight, 0, 'f', 2));
    }
    
    ui.textEdit->append("\n=================================================");
    ui.textEdit->append(QString("COSTO TOTAL DE ENERGÍA: %1 unidades")
        .arg(totalCost, 0, 'f', 2));
    ui.textEdit->append("=================================================\n");
    
    // Guardar resultados
    if (network.getGraph().saveResults("resultado.txt", mst, totalCost)) {
        ui.textEdit->append("\n? Resultados guardados en 'resultado.txt'");
    }
    
    // Cambiar a la pestaña de resultados
    ui.mainTabs->setCurrentIndex(1);
    
    QMessageBox::information(this, "MST Calculado", 
        QString("MST calculado exitosamente.\nCosto total: %1 unidades")
        .arg(totalCost, 0, 'f', 2));
}

void MainWindow::updateGraphVisualization()
{
    graphScene->displayGraph(network.getGraph());
    ui.graphicsView->fitInView(graphScene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::displayOutput(const QString& text)
{
    ui.textEdit->append(text);
}

