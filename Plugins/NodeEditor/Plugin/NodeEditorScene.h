#pragma once

#include <QGraphicsScene>
#include <unordered_map>

class Node;
class pqServerManagerModel;

class NodeEditorScene : public QGraphicsScene {
    public:

    NodeEditorScene(pqServerManagerModel* smm, QObject* parent=0);
    ~NodeEditorScene(){};

    std::unordered_map<int,Node*> nodeRegistry;

    void drawBackground(QPainter *painter, const QRectF &rect);
};