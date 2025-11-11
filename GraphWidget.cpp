#include "GraphWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

GraphWidget::GraphWidget(QWidget *parent) : QWidget(parent)
    , m_graph(new Graph())
    , m_currentMode(AddVertexMode)
    , m_clickedVertex(nullptr)
    , m_cursorVertex(nullptr)
    , isReplacing(false)
    , m_selectedVertex(nullptr)
{
    setMinimumSize(600, 400);
    setStyleSheet("background-color: white;");
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

GraphWidget::~GraphWidget()
{
    delete m_graph;
}

void GraphWidget::setMode(Mode mode)
{
    m_currentMode = mode;
    m_selectedVertex = nullptr;
    update();
}

void GraphWidget::clearGraph()
{
    m_graph->clear();
    m_selectedVertex = nullptr;
    update();
}

void GraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    for (Vertex *vertex : m_graph->vertices()) {
        for (Vertex *neighbor : vertex->neighbors()) {
            if (vertex->id() < neighbor->id()) {
                drawEdge(painter, vertex, neighbor);
            }
        }
    }

    for (Vertex *vertex : m_graph->vertices()) {
        drawVertex(painter, vertex);
    }

    if (m_selectedVertex) {
        painter.setPen(QPen(Qt::red, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(m_selectedVertex->position(),
                           VERTEX_RADIUS + 2, VERTEX_RADIUS + 2);
    }
    if (m_clickedVertex) {
        painter.setPen(QPen(Qt::red, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(m_clickedVertex->position(),
            VERTEX_RADIUS + 2, VERTEX_RADIUS+2 );
    }
    if (m_cursorVertex) {
        painter.setPen(QPen(QColor(255,0,0,128), 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(m_cursorVertex->position(),
            VERTEX_RADIUS + 2, VERTEX_RADIUS+2 );
    }
}

void GraphWidget::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        QPoint clickPos = event->pos();

        switch (m_currentMode) {

        case AddVertexMode:
            m_graph->addVertex(clickPos);
            update();
            break;

        case AddEdgeMode:
            if (Vertex *vertex = m_graph->findVertexAt(clickPos)) {
                if (!m_selectedVertex) {
                    m_selectedVertex = vertex;
                } else if (m_selectedVertex != vertex) {
                    m_graph->addEdge(m_selectedVertex, vertex);
                    m_selectedVertex = nullptr;
                }
            } else {
                m_selectedVertex = nullptr;
            }
            update();
            break;

            case SelectMode:
                if (Vertex *vertex = m_graph->findVertexAt(clickPos)) {
                    m_clickedVertex = vertex;
                    isReplacing = true;
                }
                else {
                    m_clickedVertex = nullptr;
                    isReplacing = false;
                }
                update();

        }

    }
}

void GraphWidget::mouseMoveEvent(QMouseEvent *event) {
    QPoint clickPos = event->pos();

    if (m_currentMode == SelectMode) {
        Vertex *vertex = m_graph->findVertexAt(clickPos);



        if (m_cursorVertex != vertex) {
            m_cursorVertex = vertex;

            update();
        }
    }
}

void GraphWidget::drawVertex(QPainter &painter, Vertex *vertex)
{
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(QBrush(Qt::lightGray));
    painter.drawEllipse(vertex->position(), VERTEX_RADIUS, VERTEX_RADIUS);

    painter.setPen(Qt::black);
    QRect textRect(vertex->position().x() - VERTEX_RADIUS/2,
                   vertex->position().y() - VERTEX_RADIUS/2,
                   VERTEX_RADIUS, VERTEX_RADIUS);
    painter.drawText(textRect, Qt::AlignCenter, QString::number(vertex->id()));
}

void GraphWidget::drawEdge(QPainter &painter, Vertex *v1, Vertex *v2)
{
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(v1->position(), v2->position());
}

void GraphWidget::keyPressEvent(QKeyEvent *event) {
    if (m_currentMode == SelectMode && event->key() == Qt::Key_Delete) {
        if (m_clickedVertex) {
            m_graph->removeVertex(m_clickedVertex);
            if (m_cursorVertex == m_clickedVertex) {
                m_cursorVertex = nullptr;
            }
            m_clickedVertex = nullptr;

            update();
        }
    }
}