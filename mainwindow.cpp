#include "MainWindow.h"
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPlainTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QSpacerItem>
#include "GraphAlgorithms.h"
#include "VertexInputDialog.h"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
    , m_graphWidget(nullptr)
    , m_drawingToolBar(nullptr)
    , m_algorithmToolBar(nullptr)
    , m_selectAction(nullptr)
    , m_addVertexAction(nullptr)
    , m_addEdgeAction(nullptr)
    , m_clearAction(nullptr)
    , m_toolGroup(nullptr)
    , m_topologicalSortAction(nullptr)
    , m_menuBar(nullptr)
    , m_fileMenu(nullptr)
    , m_instructionMenu(nullptr)
    , m_aboutMenu(nullptr)
    , m_openAction(nullptr)
    , m_saveAction(nullptr)
    , m_exitAction(nullptr)
    , m_instructionAction(nullptr)
    , m_aboutAction(nullptr)
    , m_textOutput(nullptr)
{
    setWindowTitle("Graph Application");
    setMinimumSize(1000, 800);
    createMenus();

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *contentLayout = new QHBoxLayout();
    QVBoxLayout *leftPanelLayout = new QVBoxLayout();

    createToolBars();
    createActions();

    leftPanelLayout->addWidget(m_drawingToolBar);
    leftPanelLayout->addStretch(1);
    leftPanelLayout->addWidget(m_algorithmToolBar);

    contentLayout->addLayout(leftPanelLayout);
    m_graphWidget = new GraphWidget(this);
    contentLayout->addWidget(m_graphWidget, 1);
    mainLayout->addLayout(contentLayout, 1);

    m_textOutput = new QPlainTextEdit(this);
    m_textOutput->setReadOnly(true);
    m_textOutput->setMaximumHeight(200);
    m_textOutput->setPlaceholderText("Results of graph algorithms will be there)");
    m_textOutput->setStyleSheet("QPlainTextEdit { background-color: #ffffff; border: 1px solid #dee2e6; padding: 8px; }");

    mainLayout->addWidget(m_textOutput);
}

void MainWindow::createMenus()
{
    m_menuBar = menuBar();

    m_fileMenu = m_menuBar->addMenu("File");

    m_openAction = new QAction("Open", this);
    m_saveAction = new QAction("Save", this);
    m_exitAction = new QAction("Exit", this);

    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addAction(m_saveAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    m_instructionAction = new QAction("Instruction", this);
    m_menuBar->addAction(m_instructionAction);

    m_aboutAction = new QAction("About developer", this);
    m_menuBar->addAction(m_aboutAction);

    connect(m_openAction, &QAction::triggered, this, &MainWindow::onOpen);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::onSave);
    connect(m_exitAction, &QAction::triggered, this, &MainWindow::onExit);
}

void MainWindow::createToolBars()
{
    m_drawingToolBar = new QToolBar("Drawing Tools", this);
    m_drawingToolBar->setOrientation(Qt::Vertical);
    m_drawingToolBar->setIconSize(QSize(32, 32));
    m_drawingToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_drawingToolBar->setStyleSheet("QToolBar { spacing: 5px; padding: 10px; }");

    m_algorithmToolBar = new QToolBar("Algorithm Tools", this);
    m_algorithmToolBar->setOrientation(Qt::Vertical);
    m_algorithmToolBar->setIconSize(QSize(32, 32));
    m_algorithmToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_algorithmToolBar->setStyleSheet("QToolBar { spacing: 5px; padding: 10px; }");
}

void MainWindow::createActions()
{
    m_toolGroup = new QActionGroup(this);
    m_toolGroup->setExclusive(true);


    m_selectAction = new QAction("Select", this);
    m_selectAction->setCheckable(true);
    m_selectAction->setChecked(true);
    m_toolGroup->addAction(m_selectAction);
    m_drawingToolBar->addAction(m_selectAction);

    m_addVertexAction = new QAction("Add Vertex", this);
    m_addVertexAction->setCheckable(true);
    m_toolGroup->addAction(m_addVertexAction);
    m_drawingToolBar->addAction(m_addVertexAction);

    m_addEdgeAction = new QAction("Add Edge", this);
    m_addEdgeAction->setCheckable(true);
    m_toolGroup->addAction(m_addEdgeAction);
    m_drawingToolBar->addAction(m_addEdgeAction);

    m_drawingToolBar->addSeparator();

    m_clearAction = new QAction("Clear", this);
    m_drawingToolBar->addAction(m_clearAction);


    m_topologicalSortAction = new QAction("Topological Sort", this);
    m_algorithmToolBar->addAction(m_topologicalSortAction);

    m_eulerianCycleAction = new QAction("Eulerian Cycle", this);
    m_algorithmToolBar->addAction(m_eulerianCycleAction);

    m_dijkstraAction = new QAction("Dijkstra", this);
    m_algorithmToolBar->addAction(m_dijkstraAction);

    m_maxFlowAction = new QAction("Max Flow", this);
    m_algorithmToolBar->addAction(m_maxFlowAction);


    m_sccAction = new QAction("Strongly Connected Components", this);
    m_algorithmToolBar->addAction(m_sccAction);

    m_eulerianPathAction = new QAction("Eulerian Path", this);
    m_algorithmToolBar->addAction(m_eulerianPathAction);

    connect(m_addVertexAction, &QAction::triggered, this, &MainWindow::onAddVertexMode);
    connect(m_addEdgeAction, &QAction::triggered, this, &MainWindow::onAddEdgeMode);
    connect(m_clearAction, &QAction::triggered, this, &MainWindow::onClearGraph);
    connect(m_selectAction, &QAction::triggered, this, &MainWindow::onSelectMode);
    connect(m_topologicalSortAction, &QAction::triggered, this, &MainWindow::onTopologicalSort);
    connect(m_eulerianCycleAction, &QAction::triggered, this, &MainWindow::onEulerianCycle);
    connect(m_dijkstraAction, &QAction::triggered, this, &MainWindow::onDijkstra);
    connect(m_maxFlowAction, &QAction::triggered, this, &MainWindow::onMaxFlow);
    connect(m_sccAction, &QAction::triggered, this, &MainWindow::onStronglyConnectedComponents);
    connect(m_eulerianPathAction, &QAction::triggered, this, &MainWindow::onEulerianPath);
}

void MainWindow::onSelectMode() {
    m_graphWidget->setMode(GraphWidget::SelectMode);
}

void MainWindow::onAddVertexMode(){
    m_graphWidget->setMode(GraphWidget::AddVertexMode);
}

void MainWindow::onAddEdgeMode(){
    m_graphWidget->setMode(GraphWidget::AddEdgeMode);
}

void MainWindow::onClearGraph(){
    m_graphWidget->clearGraph();
    m_textOutput->clear();
}

void MainWindow::onTopologicalSort(){

    Graph* graph = m_graphWidget->getGraph();
    QString result = GraphAlgorithms::topologicalSort(graph);

    m_textOutput->appendPlainText("=== Topological Sort ===");
    m_textOutput->appendPlainText(result);
    m_textOutput->appendPlainText("");
}

void MainWindow::onEulerianCycle(){

    Graph* graph = m_graphWidget->getGraph();
    QString result = GraphAlgorithms::eulerianCycle(graph);

    m_textOutput->appendPlainText("=== Eulerian Cycle ===");
    m_textOutput->appendPlainText(result);
    m_textOutput->appendPlainText("");
}

void MainWindow::onDijkstra(){

    VertexInputDialog dialog("Dijkstra Algorithm", this);

    if (dialog.exec() == QDialog::Accepted) {
        int startId = dialog.getStartVertexId();
        int endId = dialog.getEndVertexId();

        Graph* graph = m_graphWidget->getGraph();
        QString result = GraphAlgorithms::dijkstra(graph, startId, endId);

        m_textOutput->appendPlainText("=== Dijkstra Algorithm ===");
        m_textOutput->appendPlainText(result);
        m_textOutput->appendPlainText("");
    }
}

void MainWindow::onMaxFlow(){

    VertexInputDialog dialog("Max Flow Algorithm", this);

    if (dialog.exec() == QDialog::Accepted) {
        int sourceId = dialog.getStartVertexId();
        int sinkId = dialog.getEndVertexId();

        Graph* graph = m_graphWidget->getGraph();
        QString result = GraphAlgorithms::maxFlow(graph, sourceId, sinkId);

        m_textOutput->appendPlainText("=== Max Flow Algorithm ===");
        m_textOutput->appendPlainText(result);
        m_textOutput->appendPlainText("");
    }
}

void MainWindow::onStronglyConnectedComponents()
{

    Graph* graph = m_graphWidget->getGraph();
    QString result = GraphAlgorithms::stronglyConnectedComponents(graph);

    m_textOutput->appendPlainText("=== Strongly Connected Components ===");
    m_textOutput->appendPlainText(result);
    m_textOutput->appendPlainText("");
}

void MainWindow::onEulerianPath()
{

    Graph* graph = m_graphWidget->getGraph();
    QString result = GraphAlgorithms::eulerianPath(graph);

    m_textOutput->appendPlainText("=== Eulerian Path ===");
    m_textOutput->appendPlainText(result);
    m_textOutput->appendPlainText("");
}

void MainWindow::onOpen()
{

}

void MainWindow::onSave()
{

}

void MainWindow::onExit()
{

}