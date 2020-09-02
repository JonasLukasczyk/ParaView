
#pragma once

#include <QDockWidget>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

#include <unordered_map>

class Node;

class NodeEditor : public QDockWidget
{
  Q_OBJECT

public:

  NodeEditor(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags())
    : QDockWidget(parent, flags)
  {
    this->constructor();
  }

  NodeEditor(const QString &title, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags())
    : QDockWidget(title, parent, flags)
  {
    this->constructor();
  }

  ~NodeEditor(){}

private:
  void constructor();

    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;
    QGraphicsTextItem *text;

    // std::unordered_map<int,Node> nodes;
};
