#include "MainWindow.h"
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
    , m_graphWidget(nullptr)
    , m_toolBar(nullptr)
    , m_selectAction(nullptr)
    , m_addVertexAction(nullptr)
    , m_addEdgeAction(nullptr)
    , m_clearAction(nullptr)
    , m_toolGroup(nullptr)
    , m_textOutput(nullptr)
{
    setWindowTitle("Graph Application");
    setMinimumSize(1000, 800);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *graphAreaLayout = new QHBoxLayout();

    createToolBar();
    createActions();
    graphAreaLayout->addWidget(m_toolBar);
    m_graphWidget = new GraphWidget(this);
    m_graphWidget->setStyleSheet("background-color: #e6e6fa;");

    graphAreaLayout->addWidget(m_graphWidget, 1);
    mainLayout->addLayout(graphAreaLayout, 1);
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    QWidget *leftSpace = new QWidget();
    leftSpace->setFixedWidth(200);
    bottomLayout->addWidget(leftSpace);
    m_textOutput = new QPlainTextEdit(this);
    m_textOutput->setReadOnly(true);
    m_textOutput->setMaximumHeight(150);
    m_textOutput->setPlaceholderText("Здесь будут отображаться результаты работы алгоритмов с графом...");
    bottomLayout->addWidget(m_textOutput, 1);
    mainLayout->addLayout(bottomLayout);
}

void MainWindow::createToolBar()
{
    m_toolBar = new QToolBar("Graph Tools", this);
    m_toolBar->setOrientation(Qt::Vertical);
    m_toolBar->setIconSize(QSize(32, 32));
    m_toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void MainWindow::createActions()
{
    m_toolGroup = new QActionGroup(this);
    m_toolGroup->setExclusive(true);

    m_selectAction = new QAction("Select", this);
    m_selectAction->setCheckable(true);
    m_selectAction->setChecked(true);
    m_toolGroup->addAction(m_selectAction);
    m_toolBar->addAction(m_selectAction);

    m_addVertexAction = new QAction("Add Vertex", this);
    m_addVertexAction->setCheckable(true);
    m_toolGroup->addAction(m_addVertexAction);
    m_toolBar->addAction(m_addVertexAction);

    m_addEdgeAction = new QAction("Add Edge", this);
    m_addEdgeAction->setCheckable(true);
    m_toolGroup->addAction(m_addEdgeAction);
    m_toolBar->addAction(m_addEdgeAction);

    m_toolBar->addSeparator();

    m_clearAction = new QAction("Clear", this);
    m_toolBar->addAction(m_clearAction);

    connect(m_addVertexAction, &QAction::triggered, this, &MainWindow::onAddVertexMode);
    connect(m_addEdgeAction, &QAction::triggered, this, &MainWindow::onAddEdgeMode);
    connect(m_clearAction, &QAction::triggered, this, &MainWindow::onClearGraph);
    connect(m_selectAction, &QAction::triggered, this, &MainWindow::onSelectMode);
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