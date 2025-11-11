#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include "Graph.h"

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    enum Mode { AddVertexMode, AddEdgeMode, SelectMode };

    explicit GraphWidget(QWidget *parent = nullptr);
    ~GraphWidget();

    void setMode(Mode mode);
    void clearGraph();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void drawVertex(QPainter &painter, Vertex *vertex);
    void drawEdge(QPainter &painter, Vertex *v1, Vertex *v2);
    Vertex *m_clickedVertex; //Для режима указателя
    Vertex *m_cursorVertex;  //Под курсором
    bool isReplacing;
    Graph *m_graph;
    Mode m_currentMode;
    Vertex *m_selectedVertex; //Для режима рисования ребер

    static const int VERTEX_RADIUS = 20;
};

#endif