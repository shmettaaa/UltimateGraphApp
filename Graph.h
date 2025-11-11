#ifndef GRAPH_H
#define GRAPH_H

#include "Vertex.h"
#include <QVector>
#include <QSet>

class Graph
{
public:
    Graph();
    ~Graph();

    Vertex* addVertex(const QPoint &position);
    void removeVertex(Vertex *vertex);
    void addEdge(Vertex *v1, Vertex *v2);
    void removeEdge(Vertex *v1, Vertex *v2);

    Vertex* findVertexAt(const QPoint &point, int radius = 20) const;
    Vertex* getVertexById(int id) const;
    static bool areConnected(Vertex *v1, Vertex *v2) ;


    const QVector<Vertex*>& vertices() const {
        return m_vertices;
    }
    int vertexCount() const {
        return m_vertices.size();
    }
    int edgeCount() const;


    void clear();

private:
    QVector<Vertex*> m_vertices;
    int m_vertexCounter;
};

#endif