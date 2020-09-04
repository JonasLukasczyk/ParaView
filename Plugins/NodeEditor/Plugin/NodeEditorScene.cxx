#include "NodeEditorScene.h"

#include <QPainter>

#include "pqServerManagerModel.h"
#include "pqPipelineSource.h"
#include "vtkSMProxy.h"

#include "Node.h"

#include <iostream>

NodeEditorScene::NodeEditorScene(pqServerManagerModel* smm, QObject* parent):QGraphicsScene(parent){
    QObject::connect(
        smm, &pqServerManagerModel::sourceAdded,
        [=](pqPipelineSource* source){
            std::cout<<"SourceAdded: "<<source->getSMName().toStdString()<<"<"<<source->getProxy()->GetGlobalID()<<">"<<std::endl;

            this->nodeRegistry.insert({
                source->getProxy()->GetGlobalID(),
                new Node(this, source)
            });

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

            auto it = this->nodeRegistry.find(
                source->getProxy()->GetGlobalID()
            );
            if(it!=this->nodeRegistry.end())
                delete it->second;

            this->nodeRegistry.erase(source->getProxy()->GetGlobalID());

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
}

void NodeEditorScene::drawBackground(QPainter *painter, const QRectF &rect){
    const int gridSize = 25;

    qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += gridSize)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += gridSize)
        lines.append(QLineF(rect.left(), y, rect.right(), y));

    painter->setPen( QColor(60,60,60) );
    painter->drawLines(lines.data(), lines.size());
}