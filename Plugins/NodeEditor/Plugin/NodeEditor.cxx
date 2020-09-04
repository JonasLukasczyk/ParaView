#include "NodeEditor.h"

#include "pqApplicationCore.h"
#include "pqServerManagerModel.h"
#include "pqPipelineSource.h"
#include "vtkSMSourceProxy.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QWheelEvent>

#include "NodeEditorView.h"
#include "NodeEditorScene.h"
#include "Node.h"

#include <iostream>

void NodeEditor::constructor(){
    this->setWindowTitle("Node Editor");

    QWidget* t_widget = new QWidget(this);

    auto layout = new QVBoxLayout;
    t_widget->setLayout(layout);

    auto core = pqApplicationCore::instance();
    auto smm = core->getServerManagerModel();

    auto scene = new NodeEditorScene(smm,this);

    auto graphicsView = new NodeEditorView;
    graphicsView->setScene(scene);
    graphicsView->setDragMode( QGraphicsView::ScrollHandDrag );
    graphicsView->setSceneRect(-10000,-10000,20000,20000);

    layout->addWidget(graphicsView);

    // connect(
    //     smm,
    //     &pqServerManagerModel::sourceAdded,
    //     [=](pqPipelineSource* source){
    //         std::cout<<"SourceAdded: "<<source->getSMName().toStdString()<<std::endl;

    //         connect(
    //             source,
    //             &pqPipelineSource::connectionAdded,
    //             [=](pqPipelineSource *source, pqPipelineSource *consumer, int srcOutputPort){
    //                 std::cout<<"ConnectionAdded: "
    //                     <<source->getSMName().toStdString()
    //                     <<"["<<std::to_string(srcOutputPort)<<"]"
    //                     << "-"<<consumer->getSMName().toStdString()
    //                     <<std::endl;
    //             }
    //         );
    //     }
    // );

    // connect(
    //     smm,
    //     &pqServerManagerModel::sourceRemoved,
    //     [=](pqPipelineSource* source){
    //         std::cout<<"SourceRemoved: "<<source->getSMName().toStdString()<<std::endl;
    //     }
    // );

    // auto x = new QPushButton("Debug", this);
    // connect(
    //     x,
    //     &QPushButton::released,
    //     [=](){



    //         // smm->Print(std::cout);
    // });
    // layout->addWidget(x);

  this->setWidget(t_widget);
}

