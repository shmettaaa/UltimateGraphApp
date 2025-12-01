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
    void addEdge(Vertex *from, Vertex *to);
    void removeEdge(Vertex *from, Vertex *to);
    void removeEdge(Edge *edge);

    Edge* getEdge(Vertex *from, Vertex *to) const;
    Edge* findEdgeAt(const QPoint &point, int radius = 5) const;
    Vertex* findVertexAt(const QPoint &point, int radius = 20) const;
    Vertex* getVertexById(int id) const;
    bool areConnected(Vertex *from, Vertex *to) const;

    const QVector<Vertex*>& vertices() const { return m_vertices; }
    const QVector<Edge*>& edges() const { return m_edges; }
    int vertexCount() const { return m_vertices.size(); }
    int edgeCount() const { return m_edges.size(); }

    bool saveToFile(const QString& filename) const;
    bool loadFromFile(const QString& filename);
    void clear();

private:
    QVector<Vertex*> m_vertices;
    QVector<Edge*> m_edges;
    int m_vertexCounter;
    double distanceToLineSegment(const QPoint &point, const QPoint &lineStart, const QPoint &lineEnd) const;

};

#endif