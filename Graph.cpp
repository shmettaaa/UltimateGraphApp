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
        QVector<Edge*> edgesToRemove;
        for (Edge *edge : m_edges) {
            if (edge->from() == vertex || edge->to() == vertex) {
                edgesToRemove.append(edge);
            }
        }
        for (Edge *edge : edgesToRemove) {
            removeEdge(edge);
        }

        m_vertices.removeAll(vertex);
        delete vertex;
    }
}

void Graph::addEdge(Vertex *v1, Vertex *v2)
{
    if (v1 && v2 && v1 != v2 && !getEdge(v1, v2)) {
        v1->addNeighbor(v2);
        Edge *newEdge = new Edge(v1, v2);
        m_edges.append(newEdge);
    }
}

void Graph::removeEdge(Vertex *v1, Vertex *v2)
{
    Edge *edge = getEdge(v1, v2);
    if (edge) {
        removeEdge(edge);
    }
}

void Graph::removeEdge(Edge *edge)
{
    if (m_edges.contains(edge)) {
        if (edge->from()) edge->from()->removeNeighbor(edge->to());
        m_edges.removeAll(edge);
        delete edge;
    }
}

Edge* Graph::findEdgeAt(const QPoint &point, int radius) const {
    Edge* closestEdge = nullptr;
    double minDistance = radius;

    for (Edge* edge : m_edges) {
        double distance = distanceToLineSegment(point, edge->from()->position(), edge->to()->position());
        if (distance < minDistance) {
            minDistance = distance;
            closestEdge = edge;
        }
    }
    return closestEdge;
}

double Graph::distanceToLineSegment(const QPoint &point, const QPoint &lineStart, const QPoint &lineEnd) const {
    QPoint lineVec = lineEnd - lineStart;
    QPoint pointVec = point - lineStart;

    double lineLengthSquared = lineVec.x() * lineVec.x() + lineVec.y() * lineVec.y();
    if (lineLengthSquared == 0) {
        return sqrt(pointVec.x() * pointVec.x() + pointVec.y() * pointVec.y());
    }

    double t = std::max(0.0, std::min(1.0,
        (pointVec.x() * lineVec.x() + pointVec.y() * lineVec.y()) / lineLengthSquared));

    QPoint projection = lineStart + t * lineVec;
    QPoint diff = point - projection;
    return sqrt(diff.x() * diff.x() + diff.y() * diff.y());
}

Edge* Graph::getEdge(Vertex *v1, Vertex *v2) const {
    for (Edge *edge : m_edges) {
        if ((edge->from() == v1 && edge->to() == v2) ||
            (edge->from() == v2 && edge->to() == v1)) {
            return edge;
        }
    }
    return nullptr;
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

bool Graph::areConnected(Vertex *v1, Vertex *v2) const
{
    return v1 && v2 && v1->hasNeighbor(v2);
}

void Graph::clear()
{
    qDeleteAll(m_edges);
    m_edges.clear();
    qDeleteAll(m_vertices);
    m_vertices.clear();

    m_vertexCounter = 1;
}