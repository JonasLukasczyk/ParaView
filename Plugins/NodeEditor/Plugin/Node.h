#pragma once

#include <QGraphicsItem>

class QGraphicsScene;
class pqPipelineSource;
class pqServerManagerModel;

class Node : public QGraphicsItem {
public:
    Node(QGraphicsScene* scene, pqPipelineSource* source, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    // QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    static int InitRegistry(pqServerManagerModel* smm, QGraphicsScene* scene);

protected:
    // QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    // void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    // void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    pqPipelineSource* source;

    int innerWidth;
    int innerHeight;
    int padding{12};

    // static std::map<int,Node> N
};