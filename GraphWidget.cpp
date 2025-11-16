#include "GraphWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <cmath>
#include <QInputDialog>

GraphWidget::GraphWidget(QWidget *parent) : QWidget(parent)
    , m_graph(new Graph())
    , m_currentMode(AddVertexMode)
    , m_clickedVertex(nullptr)
    , m_cursorVertex(nullptr)
    , isReplacing(false)
    , m_selectedVertex(nullptr),
    m_clickedEdge(nullptr), m_cursorEdge(nullptr)
     , m_isWaitingForWeightInput(false)
   , m_tempWeightInput("")
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

QPointF GraphWidget::calculatePointOnCircle(const QPointF &center, const QPointF &direction, double radius) const
{
    double length = sqrt(direction.x() * direction.x() + direction.y() * direction.y());
    if (length == 0)
        return center;

    QPointF unitVector(direction.x() / length, direction.y() / length);
    return center + unitVector * radius;
}

QPointF GraphWidget::calculateEdgeStartPoint(Vertex *from, Vertex *to) const
{
    QPointF fromPos = from->position();
    QPointF toPos = to->position();
    QPointF direction = toPos - fromPos;

    return calculatePointOnCircle(fromPos, direction, VERTEX_RADIUS);
}

QPointF GraphWidget::calculateEdgeEndPoint(Vertex *from, Vertex *to) const
{
    QPointF fromPos = from->position();
    QPointF toPos = to->position();
    QPointF direction = fromPos - toPos;

    return calculatePointOnCircle(toPos, direction, VERTEX_RADIUS);
}

void GraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.fillRect(rect(), QColor(255, 240, 240));
    painter.setRenderHint(QPainter::Antialiasing);

    for (Edge *edge : m_graph->edges()) {
        if (edge != m_cursorEdge && edge != m_clickedEdge) {
            drawEdge(painter, edge, Qt::black, 2);
        }
    }

    for (Vertex *vertex : m_graph->vertices()) {
        drawVertex(painter, vertex);
    }

    if (m_cursorEdge) {
        drawEdge(painter, m_cursorEdge, QColor(255, 0, 0, 128), 4);
    }
    if (m_clickedEdge) {
        drawEdge(painter, m_clickedEdge, Qt::red, 4);
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
    if (m_isWaitingForWeightInput && m_clickedEdge) {
        painter.setPen(Qt::blue);
        painter.drawText(10, 20, "Enter weight: " + m_tempWeightInput);
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

void GraphWidget::drawEdge(QPainter &painter, Edge *edge, const QColor &color, int width)
{
    Vertex *from = edge->from();
    Vertex *to = edge->to();

    QPointF startPoint = calculateEdgeStartPoint(from, to);
    QPointF endPoint = calculateEdgeEndPoint(from, to);

    painter.setPen(QPen(color, width));
    painter.drawLine(startPoint, endPoint);

    drawArrow(painter, startPoint, endPoint, color);
    drawEdgeWeight(painter, edge);
}

void GraphWidget::drawArrow(QPainter &painter, const QPointF &start, const QPointF &end, const QColor &color)
{
    QPointF direction = end - start;
    double length = sqrt(direction.x() * direction.x() + direction.y() * direction.y());
    if (length == 0) return;
    QPointF unitVector = direction / length;
    QPointF arrowEnd = end;
    double angle = atan2(unitVector.y(), unitVector.x());
    double arrowAngle = M_PI / 6;
    QPointF arrowPoint1 = arrowEnd - QPointF(cos(angle - arrowAngle) * ARROW_SIZE,
                                           sin(angle - arrowAngle) * ARROW_SIZE);
    QPointF arrowPoint2 = arrowEnd - QPointF(cos(angle + arrowAngle) * ARROW_SIZE,
                                           sin(angle + arrowAngle) * ARROW_SIZE);
    QPolygonF arrowHead;
    arrowHead << arrowEnd << arrowPoint1 << arrowPoint2;

    painter.setBrush(QBrush(color));
    painter.setPen(QPen(color, 1));
    painter.drawPolygon(arrowHead);
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
    else if (event->key() == Qt::Key_Return) {

        if (m_clickedEdge && !m_isWaitingForWeightInput) {
            m_isWaitingForWeightInput = true;
            m_tempWeightInput = "";
            update();
        }
        else if (m_isWaitingForWeightInput) {
            if (!m_tempWeightInput.isEmpty()) {
                int weight = m_tempWeightInput.toInt();
                if (weight >= 0) {
                    m_clickedEdge->setWeight(weight);
                }
            }
            m_isWaitingForWeightInput = false;
            m_tempWeightInput = "";
            update();
        }
    }
    else if (m_isWaitingForWeightInput) {
        if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
            m_tempWeightInput += event->text();
            update();
        }
        else if (event->key() == Qt::Key_Escape) {
            m_isWaitingForWeightInput = false;
            m_tempWeightInput = "";
            update();
        }
    }
}

void GraphWidget::drawEdgeWeight(QPainter &painter, Edge *edge){

    QPointF startPoint = calculateEdgeStartPoint(edge->from(), edge->to());
    QPointF endPoint = calculateEdgeEndPoint(edge->from(), edge->to());
    QPointF center = (startPoint + endPoint) / 2;

    QPointF textPos = center - QPointF(0, 10);

    QFont originalFont = painter.font();
    QFont smallFont = originalFont;
    smallFont.setPointSize(9);

    painter.setFont(smallFont);
    painter.setPen(Qt::darkMagenta);

    if (m_isWaitingForWeightInput && edge == m_clickedEdge) {
        painter.drawText(textPos, m_tempWeightInput.isEmpty() ? "0" : m_tempWeightInput);
    }
    else {
        painter.drawText(textPos, QString::number(edge->weight()));
    }

    painter.setFont(originalFont);
}