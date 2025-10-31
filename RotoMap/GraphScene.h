#pragma once
#include <QGraphicsScene>
#include <QMap>
#include <vector>
#include "GraphNode.h"
#include "GraphEdge.h"
#include "WeightedGraph.h"
#include "Sensor.h"

class GraphScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphScene(QObject* parent = nullptr);
    ~GraphScene();
    
    void clear();
    void displayGraph(WeightedGraph& graph);
    void displayMST(const vector<Edge>& mstEdges);
    void resetVisualization();
    
    GraphNode* getNodeForSensor(Sensor* sensor);

private:
    QMap<Sensor*, GraphNode*> sensorToNodeMap;
    QList<GraphEdge*> allEdges;
    
    QPointF calculateNodePosition(int index, int total);
};
