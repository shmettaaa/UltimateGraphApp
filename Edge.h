#ifndef EDGE_H
#define EDGE_H

class Vertex;

class Edge
{
public:
    Edge(Vertex *from, Vertex *to);

    Vertex* from() const { return m_from; }
    Vertex* to() const { return m_to; }

private:
    Vertex *m_from;
    Vertex *m_to;
};

#endif