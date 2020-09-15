#pragma once

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class pqPipelineSource;
class pqServerManagerModel;

class QWidget;
class QGraphicsEllipseItem;

class Edge;
class Node : public QObject, public QGraphicsItem {
    Q_OBJECT

    public:
        /// Creates a node for a pqPipelineSource element that consists of
        /// * an encapsulating rectangle
        /// * input and output ports
        /// * a widgetContainer for properties (in the future also display)
        Node(pqPipelineSource* source, QGraphicsItem *parent = nullptr);
        ~Node();

        /// Delete copy constructors
        Node(const Node&) =delete;
        Node& operator=(const Node&) =delete;

        int setState(int state);

        pqPipelineSource* getSource(){
            return this->source;
        }
        std::vector<QGraphicsEllipseItem*>& getInputPorts(){
            return this->iPorts;
        }
        std::vector<QGraphicsEllipseItem*>& getOutputPorts(){
            return this->oPorts;
        }
        QWidget* getWidgetContainer(){
            return this->widgetContainer;
        }

        int resize();

        std::string print();

    signals:
        void nodeMoved();
        void nodeClicked();

    protected:
        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
        QRectF boundingRect() const override;

        void mousePressEvent(QGraphicsSceneMouseEvent * event){
            QGraphicsItem::mousePressEvent(event);
            emit nodeClicked();
        }

    private:
        pqPipelineSource* source;
        QWidget* widgetContainer;

        std::vector<QGraphicsEllipseItem*> iPorts;
        std::vector<QGraphicsEllipseItem*> oPorts;

        int state{0}; // 0: normal, 1: selected

        int labelHeight{35};
        int padding{4};
        int borderWidth{4};
        int width{300};
        int portHeight{30};
        int portRadius{10};
        int portContainerHeight{0};
};