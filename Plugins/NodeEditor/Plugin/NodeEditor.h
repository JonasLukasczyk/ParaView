#pragma once

#include <QDockWidget>

/// This is the root widget of the node editor that can be docked
class NodeEditor : public QDockWidget {
  Q_OBJECT

public:
  NodeEditor(QWidget *parent = nullptr);
  NodeEditor(const QString &title, QWidget *parent = nullptr);
  ~NodeEditor();

};