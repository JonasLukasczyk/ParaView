#pragma once

#include <QGraphicsScene>

class NodeEditorScene : public QGraphicsScene {
    public:

    NodeEditorScene(QObject* parent=0):QGraphicsScene(parent){};
    ~NodeEditorScene(){};

    void drawBackground(QPainter *painter, const QRectF &rect)
    {

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
};