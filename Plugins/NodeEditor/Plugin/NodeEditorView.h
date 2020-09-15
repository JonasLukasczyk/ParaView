#pragma once

#include <QGraphicsView>
#include <QWheelEvent>

class NodeEditorView : public QGraphicsView {
    public:

    NodeEditorView(QWidget * parent=0):QGraphicsView(parent){};
    NodeEditorView(QGraphicsScene * scene, QWidget * parent = 0):QGraphicsView(scene, parent){};
    ~NodeEditorView(){};

    void wheelEvent(QWheelEvent *event){
        const ViewportAnchor anchor = transformationAnchor();
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        int angle = event->angleDelta().y();
        qreal factor;
        if (angle > 0) {
            factor = 1.1;
        } else {
            factor = 0.9;
        }
        scale(factor, factor);
        setTransformationAnchor(anchor);
    };
};