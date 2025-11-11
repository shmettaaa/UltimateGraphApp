#ifndef VERTEX_H
#define VERTEX_H

#include <QPoint>
#include <QSet>

class Vertex
{
public:
    Vertex(int id, const QPoint &position);
    ~Vertex();


    int id() const {
        return m_id;
    }
    QPoint position() const {
        return m_position;
    }
    const QSet<Vertex*>& neighbors() const {
        return m_neighbors;
    }


    void setPosition(const QPoint &position) {
        m_position = position;
    }

    void addNeighbor(Vertex *neighbor);
    void removeNeighbor(Vertex *neighbor);
    bool hasNeighbor(Vertex *neighbor) const;
    int degree() const { return m_neighbors.size(); }

private:
    int m_id;
    QPoint m_position;
    QSet<Vertex*> m_neighbors;
};

#endif