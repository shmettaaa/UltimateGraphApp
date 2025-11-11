#include "Graph.h"
#include <cmath>

Graph::Graph()
    : m_vertexCounter(1)
{
}

Graph::~Graph()
{
    clear();
}

Vertex* Graph::addVertex(const QPoint &position)
{
    Vertex *newVertex = new Vertex(m_vertexCounter++, position);
    m_vertices.append(newVertex);
    return newVertex;
}

void Graph::removeVertex(Vertex *vertex)
{
    if (m_vertices.contains(vertex)) {
        m_vertices.removeAll(vertex);
        delete vertex;
    }
}

void Graph::addEdge(Vertex *v1, Vertex *v2)
{
    if (v1 && v2 && v1 != v2) {
        v1->addNeighbor(v2);
    }
}

void Graph::removeEdge(Vertex *v1, Vertex *v2)
{
    if (v1 && v2) {
        v1->removeNeighbor(v2);
    }
}

Vertex* Graph::findVertexAt(const QPoint &point, int radius) const
{
    for (Vertex *vertex : m_vertices) {
        QPoint delta = vertex->position() - point;
        int distanceSquared = delta.x() * delta.x() + delta.y() * delta.y();

        if (distanceSquared <= radius * radius) {
            return vertex;
        }
    }
    return nullptr;
}

Vertex* Graph::getVertexById(int id) const
{
    for (Vertex *vertex : m_vertices) {
        if (vertex->id() == id) {
            return vertex;
        }
    }
    return nullptr;
}

bool Graph::areConnected(Vertex *v1, Vertex *v2)
{
    return v1 && v2 && v1->hasNeighbor(v2);
}

int Graph::edgeCount() const
{
    int count = 0;
    for (Vertex *vertex : m_vertices) {
        count += vertex->degree();
    }
    return count / 2;
}

void Graph::clear()
{
    qDeleteAll(m_vertices);
    m_vertices.clear();
    m_vertexCounter = 1;
}