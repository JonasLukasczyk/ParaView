#pragma once

#include <QGraphicsItem>

class Edge : public QGraphicsItem {
public:
    Edge(const Node const* n0, const Node const* n1, GraphWidget *graphWidget);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    // void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    // void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:

    const Node const* n0;
    const Node const* n1;
};