#ifndef EDGE_H
#define EDGE_H

class Vertex;

class Edge
{
public:
    Edge(Vertex *from, Vertex *to, int weight);

    Vertex* from() const {
        return m_from;
    }
    Vertex* to() const {
        return m_to;
    }
    int weight() const{
        return m_weight;
    }
    void setWeight(int weight){
        m_weight = weight;
    }
private:
    Vertex *m_from;
    Vertex *m_to;
    int m_weight;

};

#endif