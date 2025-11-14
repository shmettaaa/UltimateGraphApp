#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GraphWidget.h"

class QToolBar;
class QAction;
class QActionGroup;
class QPlainTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onSelectMode();
    void onAddVertexMode();
    void onAddEdgeMode();
    void onClearGraph();

private:
    void createToolBar();
    void createActions();

    GraphWidget *m_graphWidget;
    QToolBar *m_toolBar;
    QAction *m_selectAction;
    QAction *m_addVertexAction;
    QAction *m_addEdgeAction;
    QAction *m_clearAction;
    QActionGroup *m_toolGroup;
    QPlainTextEdit *m_textOutput;
};

#endif