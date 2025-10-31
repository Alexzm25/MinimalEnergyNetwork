#pragma once
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include "Sensor.h"

class GraphEdge;

class GraphNode : public QGraphicsEllipseItem
{
public:
    GraphNode(Sensor* sensor, qreal x, qreal y, qreal radius = 25.0);
    
    void addEdge(GraphEdge* edge);
    void removeEdge(GraphEdge* edge);
    QList<GraphEdge*> edges() const { return edgeList; }
    
    Sensor* getSensor() const { return sensor; }
    
    void setHighlighted(bool highlighted);
    void setInMST(bool inMST);
    
    enum { Type = UserType + 1 };
    int type() const override { return Type; }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    Sensor* sensor;
    QGraphicsTextItem* label;
    QList<GraphEdge*> edgeList;
    qreal nodeRadius;
    bool isHighlighted;
    bool isInMST;
    
    void updateAppearance();
};
