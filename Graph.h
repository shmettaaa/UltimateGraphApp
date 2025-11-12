#ifndef GRAPH_H
#define GRAPH_H

#include "Vertex.h"
#include "Edge.h"
#include <QVector>

class Graph
{
public:
    Graph();
    ~Graph();

    Vertex* addVertex(const QPoint &position);
    void removeVertex(Vertex *vertex);
    void addEdge(Vertex *v1, Vertex *v2);
    void removeEdge(Vertex *v1, Vertex *v2);
    void removeEdge(Edge *edge);

    Edge* findEdgeAt(const QPoint &point, int radius = 5) const;
    Vertex* findVertexAt(const QPoint &point, int radius = 20) const;
    Vertex* getVertexById(int id) const;
    bool areConnected(Vertex *v1, Vertex *v2) const;

    const QVector<Vertex*>& vertices() const { return m_vertices; }
    const QVector<Edge*>& edges() const { return m_edges; }
    int vertexCount() const { return m_vertices.size(); }
    int edgeCount() const { return m_edges.size(); }

    void clear();

private:
    QVector<Vertex*> m_vertices;
    QVector<Edge*> m_edges;
    int m_vertexCounter;
    double distanceToLineSegment(const QPoint &point, const QPoint &lineStart, const QPoint &lineEnd) const;
    Edge* getEdge(Vertex *v1, Vertex *v2) const;
};

#endif