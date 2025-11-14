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
    , m_selectedVertex(nullptr),
    m_clickedEdge(nullptr), m_cursorEdge(nullptr)
{
    setMinimumSize(600, 400);
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
    m_clickedEdge = nullptr;
    m_cursorEdge = nullptr;
    update();
}

void GraphWidget::clearGraph()
{
    m_graph->clear();
    m_selectedVertex = nullptr;
    m_clickedVertex = nullptr;
    m_clickedEdge = nullptr;

    update();
}
void GraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    for (Edge *edge : m_graph->edges()) {
        drawEdge(painter, edge->from(), edge->to());
    }

    for (Vertex *vertex : m_graph->vertices()) {
        drawVertex(painter, vertex);
    }

    if (m_selectedVertex) {
        painter.setPen(QPen(Qt::red, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(m_selectedVertex->position(), VERTEX_RADIUS + 2, VERTEX_RADIUS + 2);
    }
    if (m_clickedVertex) {
        painter.setPen(QPen(Qt::red, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(m_clickedVertex->position(), VERTEX_RADIUS + 2, VERTEX_RADIUS + 2);
    }
    if (m_cursorVertex) {
        painter.setPen(QPen(QColor(255, 0, 0, 128), 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(m_cursorVertex->position(), VERTEX_RADIUS + 2, VERTEX_RADIUS + 2);
    }

    if (m_cursorEdge) {
        painter.setPen(QPen(QColor(255, 0, 0, 128), 4));
        painter.drawLine(m_cursorEdge->from()->position(), m_cursorEdge->to()->position());
    }
    if (m_clickedEdge) {
        painter.setPen(QPen(Qt::red, 4));
        painter.drawLine(m_clickedEdge->from()->position(), m_clickedEdge->to()->position());
    }
}

void GraphWidget::mouseMoveEvent(QMouseEvent *event) {
    QPoint pos = event->pos();

    if (m_currentMode == SelectMode) {
        Vertex* vertex = m_graph->findVertexAt(pos);
        Edge* edge = m_graph->findEdgeAt(pos);
        if (vertex) {
            m_cursorVertex = vertex;
            m_cursorEdge = nullptr;
        } else if (edge) {
            m_cursorVertex = nullptr;
            m_cursorEdge = edge;
        } else {
            m_cursorVertex = nullptr;
            m_cursorEdge = nullptr;
        }
        update();
    }
}

void GraphWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QPoint pos = event->pos();

        switch (m_currentMode) {
        case AddVertexMode:
            m_graph->addVertex(pos);
            update();
            break;

        case AddEdgeMode:
            if (Vertex *vertex = m_graph->findVertexAt(pos)) {
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
            Vertex* vertex = m_graph->findVertexAt(pos);
            Edge* edge = m_graph->findEdgeAt(pos);

            if (vertex) {
                m_clickedVertex = vertex;
                m_clickedEdge = nullptr;
            } else if (edge) {
                m_clickedVertex = nullptr;
                m_clickedEdge = edge;
            } else {
                m_clickedVertex = nullptr;
                m_clickedEdge = nullptr;
            }
            update();
            break;
        }
    }
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
        } else if (m_clickedEdge) {
            m_graph->removeEdge(m_clickedEdge);
            if (m_cursorEdge == m_clickedEdge) {
                m_cursorEdge = nullptr;
            }
            m_clickedEdge = nullptr;
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

