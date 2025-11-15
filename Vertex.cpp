#include "Vertex.h"

Vertex::Vertex(int id, const QPoint &position)
    : m_id(id)
    , m_position(position)
{
}

Vertex::~Vertex()
{
    for (Vertex *neighbor : m_outNeighbors) {
        neighbor->m_inNeighbors.remove(this);
    }

    for (Vertex *neighbor : m_inNeighbors) {
        neighbor->m_outNeighbors.remove(this);
    }

    m_outNeighbors.clear();
    m_inNeighbors.clear();
}

void Vertex::addOutNeighbor(Vertex *neighbor)
{
    if (neighbor != this && !hasOutNeighbor(neighbor)) {
        m_outNeighbors.insert(neighbor);
        neighbor->addInNeighbor(this);
    }
}

void Vertex::addInNeighbor(Vertex *neighbor)
{
    if (neighbor != this && !hasInNeighbor(neighbor)) {
        m_inNeighbors.insert(neighbor);
    }
}

void Vertex::removeOutNeighbor(Vertex *neighbor)
{
    if (m_outNeighbors.contains(neighbor)) {
        m_outNeighbors.remove(neighbor);
        neighbor->removeInNeighbor(this);
    }
}

void Vertex::removeInNeighbor(Vertex *neighbor)
{
    if (m_inNeighbors.contains(neighbor)) {
        m_inNeighbors.remove(neighbor);
    }
}

bool Vertex::hasOutNeighbor(Vertex *neighbor) const
{
    return m_outNeighbors.contains(neighbor);
}

bool Vertex::hasInNeighbor(Vertex *neighbor) const
{
    return m_inNeighbors.contains(neighbor);
}