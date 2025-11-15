#ifndef VERTEX_H
#define VERTEX_H

#include <QPoint>
#include <QSet>

class Vertex
{
public:
    Vertex(int id, const QPoint &position);
    ~Vertex();

    int id() const { return m_id; }
    QPoint position() const { return m_position; }
    const QSet<Vertex*>& outNeighbors() const { return m_outNeighbors; }
    const QSet<Vertex*>& inNeighbors() const { return m_inNeighbors; }

    void setPosition(const QPoint &position) { m_position = position; }

    void addOutNeighbor(Vertex *neighbor);
    void removeOutNeighbor(Vertex *neighbor);
    bool hasOutNeighbor(Vertex *neighbor) const;

    void addInNeighbor(Vertex *neighbor);
    void removeInNeighbor(Vertex *neighbor);
    bool hasInNeighbor(Vertex *neighbor) const;

    int outDegree() const { return m_outNeighbors.size(); }
    int inDegree() const { return m_inNeighbors.size(); }

private:
    int m_id;
    QPoint m_position;
    QSet<Vertex*> m_outNeighbors;
    QSet<Vertex*> m_inNeighbors;
};

#endif