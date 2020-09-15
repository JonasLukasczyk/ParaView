#pragma once

#include <QGraphicsPathItem>

class NodeEditorScene;
class Node;
class NodeRegistry;

class Edge : public QObject, public QGraphicsPathItem {
    Q_OBJECT

    public:
        Edge(
            Node* producer,
            int producerOutputPortIdx,
            Node* consumer,
            int consumerInputPortIdx,
            QGraphicsItem *parent = nullptr
        );
        ~Edge();

        std::string print();

    protected:
        void updateEndPoints();
        QRectF boundingRect() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    private:
        QPen* edgePen;
        QPointF oPoint;
        QPointF iPoint;

        Node* producer;
        int producerOutputPortIdx;
        Node* consumer;
        int consumerInputPortIdx;
};