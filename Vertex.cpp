#include "Vertex.h"

Vertex::Vertex(int id, const QPoint &position)
    : m_id(id)
    , m_position(position){
}

Vertex::~Vertex(){
    for (Vertex *neighbor : m_neighbors) {
        neighbor->m_neighbors.remove(this);
    }
    m_neighbors.clear();
}

void Vertex::addNeighbor(Vertex *neighbor){
    if (neighbor != this && !hasNeighbor(neighbor)) {
        m_neighbors.insert(neighbor);
        neighbor->m_neighbors.insert(this);
    }
}

void Vertex::removeNeighbor(Vertex *neighbor){
    if (m_neighbors.contains(neighbor)) {
        m_neighbors.remove(neighbor);
        neighbor->m_neighbors.remove(this);
    }
}

bool Vertex::hasNeighbor(Vertex *neighbor) const{
    return m_neighbors.contains(neighbor);
}