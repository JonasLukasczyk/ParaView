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

#include "pqProxyWidget.h"
#include "pqProxiesWidget.h"

#include "pqServerManagerModel.h"
#include "pqPipelineSource.h"
#include <vtkSMProxy.h>


#include <iostream>

#include <unordered_map>

std::unordered_map<int, Node> nodeRegistry;

int Node::InitRegistry(pqServerManagerModel* smm, QGraphicsScene* scene){

    QObject::connect(
        smm, &pqServerManagerModel::sourceAdded,
        [=](pqPipelineSource* source){
            std::cout<<"SourceAdded: "<<source->getSMName().toStdString()<<"<"<<source->getProxy()->GetGlobalID()<<">"<<std::endl;

            nodeRegistry.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(source->getProxy()->GetGlobalID()),
                std::forward_as_tuple(scene, source, nullptr)
            );

            // connect(
            //     source,
            //     &pqPipelineSource::connectionAdded,
            //     [=](pqPipelineSource *source, pqPipelineSource *consumer, int srcOutputPort){
            //         std::cout<<"ConnectionAdded: "
            //             <<source->getSMName().toStdString()
            //             <<"["<<std::to_string(srcOutputPort)<<"]"
            //             << "-"<<consumer->getSMName().toStdString()
            //             <<std::endl;
            //     }
            // );
        }
    );

    QObject::connect(
        smm, &pqServerManagerModel::sourceRemoved,
        [=](pqPipelineSource* source){
            std::cout<<"SourceRemoved: "<<source->getSMName().toStdString()<<"<"<<source->getProxy()->GetGlobalID()<<">"<<std::endl;

            nodeRegistry.erase(source->getProxy()->GetGlobalID());

            // connect(
            //     source,
            //     &pqPipelineSource::connectionAdded,
            //     [=](pqPipelineSource *source, pqPipelineSource *consumer, int srcOutputPort){
            //         std::cout<<"ConnectionAdded: "
            //             <<source->getSMName().toStdString()
            //             <<"["<<std::to_string(srcOutputPort)<<"]"
            //             << "-"<<consumer->getSMName().toStdString()
            //             <<std::endl;
            //     }
            // );
        }
    );

    return 1;
}

Node::Node(QGraphicsScene* scene, pqPipelineSource* source, QGraphicsItem *parent) : QGraphicsItem(parent) {
    std::cout<<"NewNode"<<std::endl;

    // this->source

    this->setFlag(ItemIsMovable);
    this->setFlag(ItemSendsGeometryChanges);
    this->setCacheMode(DeviceCoordinateCache);
    this->setZValue(-1);

    auto innerWidget = new QWidget();

    auto layout = new QVBoxLayout;
    innerWidget->setLayout(layout);


    // // auto A = new QPushButton("A");
    // // connect(
    // //     A,
    // //     &QPushButton::released,
    // //     [=](){
    // //         std::cout<<"A\n";
    // // });

    auto label = new QLabel( source->getSMName() );
    QObject::connect(
        source, &pqPipelineSource::nameChanged,
        [=](){ label->setText( source->getSMName() ); }
    );
    layout->addWidget(label);

    // auto test = new pqProxyWidget(source->getProxy());
    auto test = new pqProxiesWidget(innerWidget);
    test->addProxy( source->getProxy(), "Test" );
    layout->addWidget(test);
    test->updateLayout();
    test->resize(400,600);
    test->updateLayout();

    // layout->addWidget(new QPushButton("A"));
    // layout->addWidget(new QPushButton("B"));
    // layout->addWidget(new QPushButton("C"));
    // layout->addWidget(new QPushButton("D"));

    innerWidget->resize(layout->sizeHint());

    scene->addItem(this);

    auto proxy = scene->addWidget(innerWidget);
    proxy->setParentItem(this);

    this->innerWidth = innerWidget->width();
    this->innerHeight = innerWidget->height();
    // this->innerWidth = innerWidget->width();
    // this->innerHeight = innerWidget->height();
}

QRectF Node::boundingRect() const
{
    // qreal adjust = 2;
    // return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
    return QRectF(
        -this->padding-2,
        -this->padding-2,
        this->innerWidth+2*this->padding+4,
        this->innerHeight+2*this->padding+4
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
//         // for (Edge *edge : qAsConst(edgeList))
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
    // painter->setPen(Qt::NoPen);

    // painter->setRenderHint(QPainter::Antialiasing);

    // auto palette = QApplication::palette();

    // QPainterPath path;
    // path.addRoundedRect(
    //     QRectF(
    //         -this->padding,
    //         -this->padding,
    //         this->innerWidth+2*this->padding,
    //         this->innerHeight+2*this->padding
    //     ),
    //     10, 10
    // );
    // auto pen = QPen(palette.light(), 2);
    // painter->setPen(pen);

    // painter->fillPath(path, palette.window());

    // painter->drawPath(path);




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