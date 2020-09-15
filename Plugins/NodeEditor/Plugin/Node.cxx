#include <Node.h>

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QApplication>

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsEllipseItem>

// #include "pqProxyWidget.h"
#include "pqProxiesWidget.h"

#include "pqServerManagerModel.h"
#include "pqPipelineSource.h"
#include "pqPipelineFilter.h"
#include "pqOutputPort.h"
#include <vtkSMProxy.h>

#include "Edge.h"

#include <iostream>
#include <sstream>

class ResizeInterceptor : public QObject {
    public:
        Node* node;
        ResizeInterceptor(Node* n) : QObject(n->getWidgetContainer()){
            this->node = n;
        }
        bool eventFilter(QObject *object, QEvent *event){
            if(event->type()==QEvent::LayoutRequest)
                this->node->resize();
            return false;
        }
};

int todoXOffset = 0;

Node::Node(pqPipelineSource* source, QGraphicsItem *parent) :
    QObject(),
    QGraphicsItem(parent),
    source(source)
{
    std::cout<<"Creating Node: "<< this->print() <<std::endl;

    this->setPos(
        todoXOffset+=400,
        0
    );

    this->setFlag(ItemIsMovable);
    this->setFlag(ItemSendsGeometryChanges);
    this->setCacheMode(DeviceCoordinateCache);
    this->setZValue(1);

    this->widgetContainer = new QWidget;
    this->widgetContainer->setMinimumWidth(this->width);
    this->widgetContainer->setMaximumWidth(this->width);

    this->widgetContainer->installEventFilter(
        new ResizeInterceptor(this)
    );

    int nOutputPorts = this->source->getNumberOfOutputPorts();
    int nInputPorts = 0;
    auto sourceAsFilter = dynamic_cast<pqPipelineFilter*>(this->source);
    if(sourceAsFilter){
        nInputPorts = sourceAsFilter->getNumberOfInputPorts();
    }
    this->portContainerHeight = std::max(nOutputPorts,nInputPorts)*this->portHeight;

    // label
    {
        auto label = new QGraphicsTextItem("", this);
        label->setPos(
            0,
            -this->portContainerHeight - this->labelHeight
        );
        label->setTextWidth(this->width);
        label->setHtml("<h2 align='center'>"+source->getSMName()+"</h2>");
        QObject::connect(
            source, &pqPipelineSource::nameChanged,
            [=](){ label->setHtml("<h2 align='center'>"+source->getSMName()+"</h2>"); }
        );
    }

    // ports
    {
        auto palette = QApplication::palette();
        QPen pen(palette.light(), this->borderWidth);

        auto addPort = [=](const int x, const int y, const QString& portLabel){
            bool isInputPort = x<0;

            auto label = new QGraphicsTextItem("", this);
            label->setPos(
                isInputPort
                    ? 2*this->padding
                    : this->width*0.5,
                y-this->portRadius-3
            );
            label->setTextWidth(0.5*this->width - 2*this->padding);
            // label->setHtml("<h4 align='right'>"+port->getPortName()+"&lt;"+port->getDataClassName()+"&gt;</h4>");
            label->setHtml("<h4 align='"+QString(isInputPort ? "left" : "right")+"'>"+portLabel+"</h4>");

            auto port = new QGraphicsEllipseItem(
                -this->portRadius,
                -this->portRadius,
                2*this->portRadius,
                2*this->portRadius,
                this
            );
            port->setPos(
                x,
                y
            );
            port->setPen(pen);
            port->setBrush( palette.dark() );

            if(isInputPort)
                this->iPorts.push_back( port );
            else
                this->oPorts.push_back( port );
        };

        for(int i=0; i<nInputPorts; i++){
            addPort(
                -this->padding,
                -this->portContainerHeight + (i+0.5)*this->portHeight,
                sourceAsFilter->getInputPortName(i)
            );
        }

        for(int i=0; i<nOutputPorts; i++){
            addPort(
                this->width + this->padding,
                -this->portContainerHeight + (i+0.5)*this->portHeight,
                this->source->getOutputPort(i)->getPortName()
            );
        }
    }

    // proxy widgets
    {
        auto proxiesWidget = new pqProxiesWidget;
        proxiesWidget->addProxy( source->getProxy(), "Properties" );
        proxiesWidget->updateLayout();
        proxiesWidget->filterWidgets(true);

        auto containerLayout = new QVBoxLayout;
        containerLayout->addWidget(proxiesWidget);

        QObject::connect(
            proxiesWidget, &pqProxiesWidget::changeFinished,
            [=](){
                std::cout<<"Change Finished"<<std::endl;
                this->source->setModifiedState(pqProxy::MODIFIED);
            }
        );

        this->widgetContainer->setLayout(containerLayout);
    }

    // embed widget in scene
    {
        auto graphicsProxyWidget = new QGraphicsProxyWidget(this);
        graphicsProxyWidget->setWidget( this->widgetContainer );
        graphicsProxyWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    }
}

Node::~Node(){
    std::cout<<"Deleting Node: "<< this->print() <<std::endl;
    delete this->widgetContainer;
}

int Node::resize(){
    this->widgetContainer->resize(
        this->widgetContainer->layout()->sizeHint()
    );
    this->prepareGeometryChange();

    return 1;
}

int Node::setState(int state){
    this->state = state;

    this->update(this->boundingRect());

    return this->state;
}

std::string Node::print(){
    std::stringstream ss;
    ss
        <<this->source->getSMName().toStdString()
        <<"<"<<this->source->getProxy()->GetGlobalID()<<">"
    ;
    return ss.str();
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value){
    switch (change) {
        case ItemPositionHasChanged:
            emit this->nodeMoved();
            break;
        default:
            break;
    };

    return QGraphicsItem::itemChange(change, value);
}

QRectF Node::boundingRect() const {
    auto offset = this->borderWidth+this->padding;

    return QRectF(
        -offset,
        -offset - this->portContainerHeight - this->labelHeight,
        this->widgetContainer->width()  + 2*offset,
        this->widgetContainer->height() + 2*offset + this->portContainerHeight + this->labelHeight
    );
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *){
    // std::cout<<"RENDERING"<<std::endl;
    auto palette = QApplication::palette();

    QPainterPath path;
    path.addRoundedRect(
        QRect(
            -this->padding,
            -this->padding - this->portContainerHeight - this->labelHeight,
            this->widgetContainer->width() + 2*this->padding,
            this->widgetContainer->height() + 2*this->padding + this->portContainerHeight + this->labelHeight
        ),
        10, 10
    );
    QPen pen(
        this->state
            ? palette.highlight()
            : palette.light(),
        this->borderWidth
    );
    painter->setPen(pen);
    painter->fillPath(path, palette.window());
    painter->drawPath(path);
}