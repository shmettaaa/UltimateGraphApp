#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GraphWidget.h"
#include "GraphAlgorithms.h"

class QToolBar;
class QAction;
class QActionGroup;
class QPlainTextEdit;
class QMenu;
class QMenuBar;

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
    void onTopologicalSort();
    void onEulerianCycle();
    void onDijkstra();
    void onMaxFlow();
    void onStronglyConnectedComponents();
    void onEulerianPath();
    void onVertexDegrees();
    void onOpen();
    void onSave();
    void onExit();

private:
    void createToolBars();
    void createActions();
    void createMenus();

    GraphWidget *m_graphWidget;


    QToolBar *m_drawingToolBar;
    QToolBar *m_algorithmToolBar;


    QAction *m_selectAction;
    QAction *m_addVertexAction;
    QAction *m_addEdgeAction;
    QAction *m_clearAction;
    QActionGroup *m_toolGroup;


    QAction *m_topologicalSortAction;
    QAction *m_eulerianCycleAction;

    QAction *m_sccAction;
    QAction *m_eulerianPathAction;

    QMenuBar *m_menuBar;
    QMenu *m_fileMenu;
    QMenu *m_instructionMenu;
    QMenu *m_aboutMenu;
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_exitAction;
    QAction *m_instructionAction;
    QAction *m_aboutAction;
    QAction *m_dijkstraAction;
    QAction *m_maxFlowAction;
    QAction *m_vertexDegreesAction;

    QPlainTextEdit *m_textOutput;
};

#endif