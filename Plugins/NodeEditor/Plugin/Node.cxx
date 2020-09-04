#include <Node.h>

#include <QPainter>
#include <QRadialGradient>
#include <QStyleOptionGraphicsItem>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QApplication>

// #include "pqProxyWidget.h"
#include "pqProxiesWidget.h"

#include "pqServerManagerModel.h"
#include "pqPipelineSource.h"
#include "pqPipelineFilter.h"
#include "pqOutputPort.h"
#include <vtkSMProxy.h>


#include <iostream>

class ResizeInterceptor : public QObject {
    public:
        Node* node;

        ResizeInterceptor(Node* n) : QObject(n->container){
            this->node = n;
        }

        bool eventFilter(QObject *object, QEvent *event){
            // std::cout<<"Intercepted: "<<event->type()<<std::endl;
            if(event->type()==QEvent::LayoutRequest){

                this->node->container->resize(
                    this->node->container->layout()->sizeHint()
                );
                this->node->prepareGeometryChange();
            }
            return false;
        }
};

Node::Node(QGraphicsScene* scene, pqPipelineSource* source, QGraphicsItem *parent) : QGraphicsItem(parent) {
    std::cout<<"NewNode"<<std::endl;

    this->source = source;

    this->setFlag(ItemIsMovable);
    this->setFlag(ItemSendsGeometryChanges);
    this->setCacheMode(DeviceCoordinateCache);
    this->setZValue(-1);

    scene->addItem(this);

    this->container = new QWidget;
    auto containerLayout = new QVBoxLayout;
    this->container->setLayout(containerLayout);

    this->container->setMinimumWidth(this->width);
    this->container->setMaximumWidth(this->width);

    this->container->installEventFilter(
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
        auto label = scene->addText("");
        label->setParentItem(this);
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

            auto label = scene->addText("");
            label->setParentItem(this);
            label->setPos(
                isInputPort ? this->padding : this->width*0.5,
                y-this->portRadius-3
            );
            label->setTextWidth(0.5*this->width-this->padding);
            // label->setHtml("<h4 align='right'>"+port->getPortName()+"&lt;"+port->getDataClassName()+"&gt;</h4>");
            label->setHtml("<h4 align='"+QString(isInputPort ? "left" : "right")+"'>"+portLabel+"</h4>");

            scene
                ->addEllipse(
                    x-this->portRadius,
                    y-this->portRadius,
                    2*this->portRadius,
                    2*this->portRadius,
                    pen,
                    QBrush(Qt::red)
                )
                ->setParentItem(this);
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
        containerLayout->addWidget(proxiesWidget);

        QObject::connect(
            proxiesWidget, &pqProxiesWidget::changeFinished,
            [=](){
                std::cout<<"Change Finished"<<std::endl;
                this->source->setModifiedState(pqProxy::MODIFIED);
            }
        );
    }

    // embed widget in scene
    {
        auto graphicsWidget = scene->addWidget(this->container);
        graphicsWidget->setParentItem(this);
        graphicsWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    }

    // this->container->installEventFilter( new XXXX(this) );
    // this->container->resize(containerLayout->sizeHint());

    // // test->filterWidgets(true);
    // auto test = new pqProxiesWidget(innerWidget);

    // layout->addWidget(test);

    // // QObject::connect(
    // //     test, &pqProxiesWidget::restartRequired,
    // //     [=](vtkSMProxy *proxy){
    // //         std::cout<<"xxxx"<<std::endl;
    // //         // source->modifiedStateChanged(proxy);
    // //         // source->setModifiedState( pqPipelineSource::MODIFIED );
    // //         source->updatePipeline();
    // //     }
    // // );


    // // layout->addWidget(new QPushButton("A"));
    // // layout->addWidget(new QPushButton("B"));
    // // layout->addWidget(new QPushButton("C"));
    // // layout->addWidget(new QPushButton("D"));




    // innerWidget->resize(layout->sizeHint());

    // scene->addItem(this);

    // auto proxy = scene->addWidget(innerWidget);
    // proxy->setParentItem(this);
    // proxy->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    // this->innerWidth = innerWidget->width();
    // this->innerHeight = innerWidget->height();
    // // this->innerWidth = innerWidget->width();
    // // this->innerHeight = innerWidget->height();
}

Node::~Node(){
    // std::cout<<"Destroyed"<<std::endl;
    // if(this->container)
    delete this->container;
}

// int Node::getPortContainerHeight()const{
//     auto nOutputPorts = this->source->getNumberOfOutputPorts();
//     int nInputPorts = 0;
//     // auto sourceAsFilter = pqPipelineFilter
//     if(this->source->IsA("pqPipelineFilter")){
//         nInputPorts = ((pqPipelineFilter*) this->source )->getNumberOfInputPorts();
//     }
//     return nOutputPorts*this->portHeight;
// }

QRectF Node::boundingRect() const{
    // qreal adjust = 2;
    // return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
    // if(!this->container)
        // return QRectF(0,0,0,0);

    auto offset = this->borderWidth+this->padding;
    // auto nInputPorts = this->source->;
    // std::cout<<portOffset<<std::endl;

    return QRectF(
        -offset,
        -offset - this->portContainerHeight - this->labelHeight,
        this->container->width()  + 2*offset,
        this->container->height() + 2*offset + this->portContainerHeight + this->labelHeight
    );
}

// QPainterPath Node::shape() const
// {
//     QPainterPath path;
//     path.addEllipse(-10, -10, 20, 20);
//     return path;
// }

// QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
// {
//     switch (change) {
//     case ItemPositionHasChanged:
//         // for (Edge *edge : qAsConst(edg-this->borderWidtheList))
//         //     edge->adjust();
//         // graph->itemMoved();
//         break;
//     default:
//         break;
//     };

//     return QGraphicsItem::itemChange(change, value);
// }

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    // std::cout<<"RENDERING"<<std::endl;
    // if(this->clear){
    //     painter->eraseRect( this->boundingRect() );
    //     return;
    // }

    // painter->setPen(Qt::NoPen);
    // painter->setRenderHint(QPainter::Antialiasing);

    auto palette = QApplication::palette();

    QPainterPath path;
    path.addRoundedRect(
        QRect(
            -this->padding,
            -this->padding - this->portContainerHeight - this->labelHeight,
            this->container->width() + 2*this->padding,
            this->container->height() + 2*this->padding + this->portContainerHeight + this->labelHeight
        ),
        10, 10
    );
    QPen pen(palette.light(), this->borderWidth);
    painter->setPen(pen);
    painter->fillPath(path, palette.window());
    painter->drawPath(path);

    // auto nPorts =





    // painter->drawRect(
    //     -this->padding,
    //     -this->padding,
    //     this->innerWidth+2*this->padding,
    //     this->innerHeight+2*this->padding
    // );

    // painter->setBrush(Qt::red);
    // painter->drawEllipse(0, -5, 10,10);
    // painter->drawRect(-this->padding, -this->padding, this->width+2*this->padding, this->height+2*this->padding);

    // QRadialGradient gradient(-3, -3, 10);
    // if (option->state & QStyle::State_Sunken) {
    //     gradient.setCenter(3, 3);
    //     gradient.setFocalPoint(3, 3);
    //     gradient.setColorAt(1, QColor(Qt::yellow).lighter(120));
    //     gradient.setColorAt(0, QColor(Qt::darkYellow).lighter(120));
    // } else {
    //     gradient.setColorAt(0, Qt::yellow);
    //     gradient.setColorAt(1, Qt::darkYellow);
    // }
    // painter->setBrush(gradient);

    // painter->setPen(QPen(Qt::black, 0));
    // painter->drawEllipse(-10, -10, 20, 20);
}