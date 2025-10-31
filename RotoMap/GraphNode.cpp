#include "GraphNode.h"
#include "GraphEdge.h"
#include <QGraphicsScene>

GraphNode::GraphNode(Sensor* sensor, qreal x, qreal y, qreal radius)
    : sensor(sensor), nodeRadius(radius), isHighlighted(false), isInMST(false)
{
    setRect(-radius, -radius, radius * 2, radius * 2);
    setPos(x, y);
    
    setBrush(QBrush(QColor(100, 149, 237))); 
    setPen(QPen(Qt::black, 2));
    
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setZValue(1);
    
    label = new QGraphicsTextItem(QString::fromStdString(sensor->getName()), this);
    QFont font = label->font();
    font.setPointSize(10);
    font.setBold(true);
    label->setFont(font);
    
    
    QRectF labelRect = label->boundingRect();
    label->setPos(-labelRect.width() / 2, -labelRect.height() / 2);
    label->setDefaultTextColor(Qt::white);
}

void GraphNode::addEdge(GraphEdge* edge)
{
    edgeList.append(edge);
}

void GraphNode::removeEdge(GraphEdge* edge)
{
    edgeList.removeOne(edge);
}

void GraphNode::setHighlighted(bool highlighted)
{
    isHighlighted = highlighted;
    updateAppearance();
}

void GraphNode::setInMST(bool inMST)
{
    isInMST = inMST;
    updateAppearance();
}

void GraphNode::updateAppearance()
{
    if (isInMST) {
        setBrush(QBrush(QColor(50, 205, 50))); 
        setPen(QPen(Qt::darkGreen, 3));
    } else if (isHighlighted) {
        setBrush(QBrush(QColor(255, 215, 0))); 
        setPen(QPen(Qt::darkYellow, 3));
    } else {
        setBrush(QBrush(QColor(100, 149, 237)));
        setPen(QPen(Qt::black, 2));
    }
    update();
}

QVariant GraphNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged) {
        for (GraphEdge* edge : edgeList) {
            edge->updatePosition();
        }
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}
