#pragma once

#include <QGraphicsScene>
#include <unordered_map>

class Node;
class Edge;
class pqPipelineSource;
class pqView;

class NodeEditorScene : public QGraphicsScene {
    public:

        NodeEditorScene(QObject* parent=nullptr);
        ~NodeEditorScene();

        int createNode(pqPipelineSource* source);
        int removeNode(pqPipelineSource* source);
        int createEdges(pqPipelineSource *source, pqPipelineSource *consumer, int srcOutputPort);

    protected:
        void drawBackground(QPainter *painter, const QRectF &rect);

    private:

        std::unordered_map<int,Node*> nodeRegistry;

        // the edge registry stores all incoming edges of a node
        std::unordered_map<int,std::vector<Edge*>> edgeRegistry;
};