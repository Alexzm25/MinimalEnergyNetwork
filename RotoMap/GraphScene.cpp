#include "GraphScene.h"
#include <cmath>
#include <QtMath>

GraphScene::GraphScene(QObject* parent)
    : QGraphicsScene(parent)
{
    setSceneRect(-400, -300, 800, 600);
    setBackgroundBrush(QBrush(QColor(245, 245, 245)));
}

GraphScene::~GraphScene()
{
    clear();
}

void GraphScene::clear()
{
    
    for (GraphEdge* edge : allEdges) {
        removeItem(edge);
        delete edge;
    }
    allEdges.clear();
    
    for (GraphNode* node : sensorToNodeMap.values()) {
        removeItem(node);
        delete node;
    }
    sensorToNodeMap.clear();
    
    QGraphicsScene::clear();
    setBackgroundBrush(QBrush(QColor(245, 245, 245)));
}

QPointF GraphScene::calculateNodePosition(int index, int total)
{
    qreal radius = 200.0;
    qreal angle = (2.0 * M_PI * index) / total;
    qreal x = radius * qCos(angle);
    qreal y = radius * qSin(angle);
    return QPointF(x, y);
}

void GraphScene::displayGraph(WeightedGraph& graph)
{
    clear();
    
    const vector<Sensor*>& sensors = graph.getSensors();
    const vector<Edge>& edges = graph.getEdges();
    
    if (sensors.empty()) {
        return;
    }
    
    for (size_t i = 0; i < sensors.size(); i++) {
        Sensor* sensor = sensors[i];
        qreal x = sensor->getX();
        qreal y = sensor->getY();
        
        x = (x - 50.0) * 6.0; 
        y = (y - 50.0) * 6.0;
        
        GraphNode* node = new GraphNode(sensor, x, y);
        addItem(node);
        sensorToNodeMap[sensor] = node;
    }
    
    QSet<QPair<Sensor*, Sensor*>> processedPairs;
    
    for (const Edge& edge : edges) {
        QPair<Sensor*, Sensor*> pair1(edge.from, edge.to);
        QPair<Sensor*, Sensor*> pair2(edge.to, edge.from);
        
        if (processedPairs.contains(pair1) || processedPairs.contains(pair2)) {
            continue;
        }
        
        processedPairs.insert(pair1);
        
        GraphNode* sourceNode = sensorToNodeMap[edge.from];
        GraphNode* destNode = sensorToNodeMap[edge.to];
        
        if (sourceNode && destNode) {
            GraphEdge* graphEdge = new GraphEdge(sourceNode, destNode, edge.weight);
            addItem(graphEdge);
            if (graphEdge->getLabel()) {
                addItem(graphEdge->getLabel());
            }
            allEdges.append(graphEdge);
        }
    }
}

void GraphScene::displayMST(const vector<Edge>& mstEdges)
{
   
    resetVisualization();
    QSet<Sensor*> mstSensors;
    for (const Edge& edge : mstEdges) {
        mstSensors.insert(edge.from);
        mstSensors.insert(edge.to);
    }
    
    for (auto it = sensorToNodeMap.begin(); it != sensorToNodeMap.end(); ++it) {
        if (mstSensors.contains(it.key())) {
            it.value()->setInMST(true);
        }
    }
    
    for (GraphEdge* edge : allEdges) {
        bool isInMST = false;
        
        for (const Edge& mstEdge : mstEdges) {
            if ((edge->sourceNode()->getSensor() == mstEdge.from && 
                 edge->destNode()->getSensor() == mstEdge.to) ||
                (edge->sourceNode()->getSensor() == mstEdge.to && 
                 edge->destNode()->getSensor() == mstEdge.from)) {
                isInMST = true;
                break;
            }
        }
        
        edge->setInMST(isInMST);
    }
}

void GraphScene::resetVisualization()
{
    for (GraphNode* node : sensorToNodeMap.values()) {
        node->setInMST(false);
        node->setHighlighted(false);
    }
    
    for (GraphEdge* edge : allEdges) {
        edge->setInMST(false);
        edge->setHighlighted(false);
    }
}

GraphNode* GraphScene::getNodeForSensor(Sensor* sensor)
{
    return sensorToNodeMap.value(sensor, nullptr);
}
