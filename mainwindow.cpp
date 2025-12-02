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
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QFont>
#include <QPalette>

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
    setMinimumSize(1100, 800);

    setStyleSheet(
        "QMainWindow {"
        "   background-color: #f0f0f0;"
        "}"
    );

    createMenus();

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setStyleSheet(
        "QWidget {"
        "   background-color: #f0f0f0;"
        "}"
    );
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(10);

    QVBoxLayout *leftPanelLayout = new QVBoxLayout();
    leftPanelLayout->setSpacing(5);

    createToolBars();
    createActions();

    leftPanelLayout->addWidget(m_drawingToolBar);
    leftPanelLayout->addStretch(1);
    leftPanelLayout->addWidget(m_algorithmToolBar);

    leftPanelLayout->setAlignment(Qt::AlignTop);
    contentLayout->addLayout(leftPanelLayout);

    QWidget *graphContainer = new QWidget(this);
    graphContainer->setStyleSheet(
        "QWidget {"
        "   background-color: white;"
        "   border: 2px solid #4CAF50;"
        "   border-radius: 8px;"
        "}"
    );
    QVBoxLayout *graphContainerLayout = new QVBoxLayout(graphContainer);
    graphContainerLayout->setContentsMargins(0, 0, 0, 0);

    m_graphWidget = new GraphWidget(this);
    graphContainerLayout->addWidget(m_graphWidget);

    contentLayout->addWidget(graphContainer, 1);

    mainLayout->addLayout(contentLayout, 1);

    QWidget *textContainer = new QWidget(this);
    textContainer->setStyleSheet(
        "QWidget {"
        "   background-color: white;"
        "   border: 2px solid #4CAF50;"
        "   border-radius: 8px;"
        "}"
    );
    QVBoxLayout *textContainerLayout = new QVBoxLayout(textContainer);
    textContainerLayout->setContentsMargins(5, 5, 5, 5);

    m_textOutput = new QPlainTextEdit(this);
    m_textOutput->setReadOnly(true);
    m_textOutput->setMaximumHeight(180);

    m_textOutput->setStyleSheet(
        "QPlainTextEdit {"
        "   background-color: white;"
        "   border: none;"
        "   padding: 6px;"
        "   font-family: 'Segoe UI';"
        "   font-size: 18px;"
        "}"
        "QScrollBar:vertical {"
        "   border: none;"
        "   background: #f0f0f0;"
        "   width: 8px;"
        "   margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #4CAF50;"
        "   min-height: 15px;"
        "   border-radius: 4px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "   border: none;"
        "   background: none;"
        "}"
    );

    m_textOutput->setPlaceholderText("Results of graph algorithms will appear here...");

    textContainerLayout->addWidget(m_textOutput);
    mainLayout->addWidget(textContainer);
}

void MainWindow::createMenus()
{
    m_menuBar = menuBar();

    m_menuBar->setStyleSheet(
        "QMenuBar {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   font-weight: bold;"
        "   padding: 3px;"
        "   font-size: 10px;"
        "}"
        "QMenuBar::item {"
        "   background-color: transparent;"
        "   padding: 4px 8px;"
        "   border-radius: 3px;"
        "}"
        "QMenuBar::item:selected {"
        "   background-color: #45a049;"
        "}"
        "QMenuBar::item:pressed {"
        "   background-color: #3d8b40;"
        "}"
        "QMenu {"
        "   background-color: white;"
        "   border: 1px solid #4CAF50;"
        "   padding: 3px;"
        "   font-size: 10px;"
        "}"
        "QMenu::item {"
        "   padding: 4px 20px 4px 15px;"
        "   border-radius: 2px;"
        "}"
        "QMenu::item:selected {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "}"
        "QMenu::separator {"
        "   height: 1px;"
        "   background: #4CAF50;"
        "   margin: 3px 8px;"
        "}"
    );

    m_fileMenu = m_menuBar->addMenu("File");

    m_openAction = new QAction("Open", this);
    m_saveAction = new QAction("Save", this);
    m_exitAction = new QAction("Exit", this);

    QFont menuFont("Segoe UI", 9);
    m_openAction->setFont(menuFont);
    m_saveAction->setFont(menuFont);
    m_exitAction->setFont(menuFont);

    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addAction(m_saveAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    m_instructionAction = new QAction("Instruction", this);
    m_instructionAction->setFont(menuFont);
    m_menuBar->addAction(m_instructionAction);

    m_aboutAction = new QAction("About", this);
    m_aboutAction->setFont(menuFont);
    m_menuBar->addAction(m_aboutAction);

    connect(m_openAction, &QAction::triggered, this, &MainWindow::onOpen);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::onSave);
    connect(m_exitAction, &QAction::triggered, this, &MainWindow::onExit);
}

void MainWindow::createToolBars()
{
    QString toolbarStyle =
        "QToolBar {"
        "   background-color: white;"
        "   border: 2px solid #4CAF50;"
        "   border-radius: 6px;"
        "   padding: 5px;"
        "   spacing: 3px;"
        "}"
        "QToolButton {"
        "   background-color: #4CAF50;"
        "   border: none;"
        "   color: white;"
        "   padding: 4px;"
        "   border-radius: 4px;"
        "   min-width: 80px;"
        "   min-height: 28px;"
        "   font-weight: bold;"
        "   font-size: 9px;"
        "}"
        "QToolButton:hover {"
        "   background-color: #45a049;"
        "}"
        "QToolButton:pressed {"
        "   background-color: #3d8b40;"
        "}"
        "QToolButton:checked {"
        "   background-color: #2E7D32;"
        "   border: 1px solid #1B5E20;"
        "}"
        "QToolButton:checked:hover {"
        "   background-color: #388E3C;"
        "}";

    m_drawingToolBar = new QToolBar("Drawing Tools", this);
    m_drawingToolBar->setOrientation(Qt::Vertical);
    m_drawingToolBar->setIconSize(QSize(20, 20));
    m_drawingToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_drawingToolBar->setStyleSheet(toolbarStyle);
    m_drawingToolBar->setFixedWidth(100);

    m_algorithmToolBar = new QToolBar("Algorithm Tools", this);
    m_algorithmToolBar->setOrientation(Qt::Vertical);
    m_algorithmToolBar->setIconSize(QSize(20, 20));
    m_algorithmToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_algorithmToolBar->setStyleSheet(toolbarStyle);
    m_algorithmToolBar->setFixedWidth(100);
}

void MainWindow::createActions()
{
    m_toolGroup = new QActionGroup(this);
    m_toolGroup->setExclusive(true);

    QFont actionFont("Segoe UI", 8, QFont::Bold);

    m_selectAction = new QAction("Select", this);
    m_selectAction->setCheckable(true);
    m_selectAction->setChecked(true);
    m_selectAction->setFont(actionFont);
    m_toolGroup->addAction(m_selectAction);
    m_drawingToolBar->addAction(m_selectAction);

    m_addVertexAction = new QAction("Add Vertex", this);
    m_addVertexAction->setCheckable(true);
    m_addVertexAction->setFont(actionFont);
    m_toolGroup->addAction(m_addVertexAction);
    m_drawingToolBar->addAction(m_addVertexAction);

    m_addEdgeAction = new QAction("Add Edge", this);
    m_addEdgeAction->setCheckable(true);
    m_addEdgeAction->setFont(actionFont);
    m_toolGroup->addAction(m_addEdgeAction);
    m_drawingToolBar->addAction(m_addEdgeAction);

    m_drawingToolBar->addSeparator();

    m_clearAction = new QAction("Clear", this);
    m_clearAction->setFont(actionFont);
    m_drawingToolBar->addAction(m_clearAction);

    m_topologicalSortAction = new QAction("Topological", this);
    m_topologicalSortAction->setFont(actionFont);
    m_algorithmToolBar->addAction(m_topologicalSortAction);

    m_eulerianCycleAction = new QAction("Euler Cycle", this);
    m_eulerianCycleAction->setFont(actionFont);
    m_algorithmToolBar->addAction(m_eulerianCycleAction);

    m_dijkstraAction = new QAction("Dijkstra", this);
    m_dijkstraAction->setFont(actionFont);
    m_algorithmToolBar->addAction(m_dijkstraAction);

    m_maxFlowAction = new QAction("Max Flow", this);
    m_maxFlowAction->setFont(actionFont);
    m_algorithmToolBar->addAction(m_maxFlowAction);

    m_sccAction = new QAction("SCC", this);
    m_sccAction->setFont(actionFont);
    m_algorithmToolBar->addAction(m_sccAction);

    m_eulerianPathAction = new QAction("Euler Path", this);
    m_eulerianPathAction->setFont(actionFont);
    m_algorithmToolBar->addAction(m_eulerianPathAction);

    m_vertexDegreesAction = new QAction("Degrees", this);
    m_vertexDegreesAction->setFont(actionFont);
    m_algorithmToolBar->addAction(m_vertexDegreesAction);

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
    connect(m_vertexDegreesAction, &QAction::triggered, this, &MainWindow::onVertexDegrees);
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

void MainWindow::onVertexDegrees()
{

    Graph* graph = m_graphWidget->getGraph();
    QString result = GraphAlgorithms::vertexDegrees(graph);

    m_textOutput->appendPlainText("=== Vertex Degrees ===");
    m_textOutput->appendPlainText(result);
    m_textOutput->appendPlainText("");
}

void MainWindow::onOpen()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        "Open Graph File",
        "",
        "Graph Files (*.graph);;All Files (*.*)"
    );

    if (filename.isEmpty()) {
        return;
    }

    if (!m_graphWidget) {
        m_textOutput->appendPlainText("Error: GraphWidget is not initialized.");
        return;
    }

    Graph* graph = m_graphWidget->getGraph();
    bool isLoadSuccessful = graph->loadFromFile(filename);

    if (isLoadSuccessful) {
        m_textOutput->appendPlainText("Graph loaded successfully from: " + filename);
        m_graphWidget->update();
    } else {
        m_textOutput->appendPlainText("Error: Failed to load graph from: " + filename);
    }

    m_textOutput->appendPlainText("");
}


void MainWindow::onSave()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        "Save Graph File",
        "",
        "Graph Files (*.graph);;All Files (*.*)"
    );

    if (filename.isEmpty()) {
        return;
    }

    if (!filename.endsWith(".graph", Qt::CaseInsensitive)) {
        filename += ".graph";
    }

    if (!m_graphWidget) {
        m_textOutput->appendPlainText("Error: GraphWidget is not initialized.");
        return;
    }

    Graph* graph = m_graphWidget->getGraph();
    bool isSaveSuccessful = graph->saveToFile(filename);

    if (isSaveSuccessful) {
        m_textOutput->appendPlainText("Graph saved successfully to: " + filename);
    } else {
        m_textOutput->appendPlainText("Error: Failed to save graph to: " + filename);
    }

    m_textOutput->appendPlainText("");
}


void MainWindow::onExit()
{
    QApplication::quit();
}