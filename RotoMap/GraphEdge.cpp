#include "GraphEdge.h"
#include "GraphNode.h"
#include <QPainter>
#include <cmath>

GraphEdge::GraphEdge(GraphNode* source, GraphNode* dest, float weight)
    : source(source), dest(dest), weight(weight), isHighlighted(false), isInMST(false)
{
    setPen(QPen(QColor(128, 128, 128), 2)); 
    setZValue(0);
    
    label = new QGraphicsTextItem(QString::number(weight, 'f', 2));
    QFont font = label->font();
    font.setPointSize(8);
    label->setFont(font);
    label->setDefaultTextColor(Qt::black);
    label->setZValue(2);
    
    source->addEdge(this);
    dest->addEdge(this);
    
    updatePosition();
}

GraphEdge::~GraphEdge()
{
    if (label && label->scene()) {
        label->scene()->removeItem(label);
    }
    delete label;
}

void GraphEdge::updatePosition()
{
    QLineF line(source->pos(), dest->pos());
    setLine(line);
    
    if (label) {
        QPointF midPoint = (source->pos() + dest->pos()) / 2;
        QRectF labelRect = label->boundingRect();
        label->setPos(midPoint.x() - labelRect.width() / 2, 
                     midPoint.y() - labelRect.height() / 2);
    }
}

void GraphEdge::setHighlighted(bool highlighted)
{
    isHighlighted = highlighted;
    updateAppearance();
}

void GraphEdge::setInMST(bool inMST)
{
    isInMST = inMST;
    updateAppearance();
}

void GraphEdge::updateAppearance()
{
    if (isInMST) {
        setPen(QPen(QColor(50, 205, 50), 4)); 
        if (label) {
            label->setDefaultTextColor(Qt::darkGreen);
            QFont font = label->font();
            font.setBold(true);
            label->setFont(font);
        }
    } else if (isHighlighted) {
        setPen(QPen(QColor(255, 215, 0), 3)); 
        if (label) {
            label->setDefaultTextColor(Qt::darkYellow);
        }
    } else {
        setPen(QPen(QColor(128, 128, 128), 2)); 
        if (label) {
            label->setDefaultTextColor(Qt::black);
            QFont font = label->font();
            font.setBold(false);
            label->setFont(font);
        }
    }
    update();
}
