#pragma once
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QPen>

class GraphNode;

class GraphEdge : public QGraphicsLineItem
{
public:
    GraphEdge(GraphNode* source, GraphNode* dest, float weight);
    ~GraphEdge();
    
    GraphNode* sourceNode() const { return source; }
    GraphNode* destNode() const { return dest; }
    float getWeight() const { return weight; }
    QGraphicsTextItem* getLabel() const { return label; }
    
    void updatePosition();
    void setHighlighted(bool highlighted);
    void setInMST(bool inMST);
    
    enum { Type = UserType + 2 };
    int type() const override { return Type; }

private:
    GraphNode* source;
    GraphNode* dest;
    float weight;
    QGraphicsTextItem* label;
    bool isHighlighted;
    bool isInMST;
    
    void updateAppearance();
};
