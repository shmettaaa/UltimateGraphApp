#ifndef GRAPHALGORITHMS_H
#define GRAPHALGORITHMS_H

#include "Graph.h"
#include <QString>

class GraphAlgorithms
{
public:
    static QString topologicalSort(Graph* graph);

private:
    static bool hasCycleDFS(Vertex* vertex, QSet<Vertex*>& visited, QSet<Vertex*>& recursionStack);
    static bool isWeaklyConnected(Graph* graph);
    static void topologicalSortDFS(Vertex* vertex, QSet<Vertex*>& visited, QVector<Vertex*>& result);
    static QString validateGraph(Graph* graph);
};

#endif