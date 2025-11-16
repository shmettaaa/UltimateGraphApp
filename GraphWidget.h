#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include "Graph.h"
#include "Edge.h"

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    enum Mode { AddVertexMode, AddEdgeMode, SelectMode };

    explicit GraphWidget(QWidget *parent = nullptr);
    ~GraphWidget();

    void setMode(Mode mode);
    void clearGraph();
    Graph* getGraph() const {
        return m_graph;
    }
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void drawVertex(QPainter &painter, Vertex *vertex);
    void drawEdge(QPainter &painter, Edge *edge, const QColor &color = Qt::black, int width = 2);
    void drawArrow(QPainter &painter, const QPointF &start, const QPointF &end, const QColor &color = Qt::black);

    QPointF calculateEdgeStartPoint(Vertex *from, Vertex *to) const;
    QPointF calculateEdgeEndPoint(Vertex *from, Vertex *to) const;
    QPointF calculatePointOnCircle(const QPointF &center, const QPointF &direction, double radius) const;

    Vertex *m_clickedVertex;
    Vertex *m_cursorVertex;
    Edge *m_clickedEdge;
    Edge *m_cursorEdge;

    bool isReplacing;
    Graph *m_graph;
    Mode m_currentMode;
    Vertex *m_selectedVertex;

    static const int VERTEX_RADIUS = 20;
    static const int ARROW_SIZE = 10;

    bool m_isWaitingForWeightInput;
    QString m_tempWeightInput;

    void drawEdgeWeight(QPainter &painter, Edge *edge);
};

#endif