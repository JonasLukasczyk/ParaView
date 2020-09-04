#pragma once

#include <QGraphicsItem>

class QGraphicsScene;
class pqPipelineSource;
class pqServerManagerModel;

class QWidget;

class Node : public QGraphicsItem {

public:
    Node(QGraphicsScene* scene, pqPipelineSource* source, QGraphicsItem *parent = nullptr);
    ~Node();

    QRectF boundingRect() const override;
    // QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void prepareGeometryChange(){
        QGraphicsItem::prepareGeometryChange();
    }

protected:
    // QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    // void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    // void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

public:
    pqPipelineSource* source;
    QWidget* container;

    int labelHeight{35};
    int padding{4};
    int borderWidth{4};
    int width{300};
    int portHeight{30};
    int portRadius{10};
    int portContainerHeight{0};

    // static std::map<int,Node> N
};