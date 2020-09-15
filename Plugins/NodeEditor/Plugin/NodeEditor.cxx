#include "NodeEditor.h"

#include <QVBoxLayout>

#include "NodeEditorView.h"
#include "NodeEditorScene.h"

NodeEditor::NodeEditor(const QString &title, QWidget *parent) : QDockWidget(title, parent){
    // create inner widget
    QWidget* t_widget = new QWidget(this);

    // create layout
    auto layout = new QVBoxLayout;
    t_widget->setLayout(layout);

    // create node editor scene and view
    auto graphicsView = new NodeEditorView;
    graphicsView->setScene( new QGraphicsView(this) );
    graphicsView->setDragMode( QGraphicsView::ScrollHandDrag );
    graphicsView->setSceneRect(-10000,-10000,20000,20000);
    layout->addWidget(graphicsView);

    this->setWidget(t_widget);
}

NodeEditor::NodeEditor(QWidget *parent) : NodeEditor("Node Editor", parent){
}

NodeEditor::~NodeEditor(){
}