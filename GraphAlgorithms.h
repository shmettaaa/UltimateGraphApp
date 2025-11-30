#ifndef GRAPHALGORITHMS_H
#define GRAPHALGORITHMS_H

#include "Graph.h"
#include <QString>

class GraphAlgorithms
{
public:
    static QString topologicalSort(Graph* graph);
    static QString eulerianCycle(Graph* graph);
    static QString dijkstra(Graph* graph, int startVertexId, int endVertexId);
    static QString maxFlow(Graph* graph, int sourceId, int sinkId);

private:
    static bool hasCycleDFS(Vertex* vertex, QSet<Vertex*>& visited, QSet<Vertex*>& recursionStack);
    static bool isWeaklyConnected(Graph* graph);
    static void topologicalSortDFS(Vertex* vertex, QSet<Vertex*>& visited, QVector<Vertex*>& result);
    static bool hasEulerianCycleConditions(Graph* graph);
    static void eulerianDFS(Vertex* vertex, QVector<Vertex*>& path, QMap<Vertex*, QList<Vertex*>>& availableEdges);
    static QString validateGraph(Graph* graph);

    static QString validateDijkstraInput(Graph* graph, int startVertexId, int endVertexId,
                                        Vertex*& startVertex, Vertex*& endVertex);
    static void initializeDijkstra(Graph* graph, QMap<Vertex*, int>& distances,
                                  QMap<Vertex*, Vertex*>& previous, QSet<Vertex*>& unvisited,
                                  Vertex* startVertex);
    static Vertex* findMinDistanceVertex(const QSet<Vertex*>& unvisited, const QMap<Vertex*, int>& distances);
    static void updateNeighborDistances(Vertex* current, Graph* graph, QMap<Vertex*, int>& distances,
                                      QMap<Vertex*, Vertex*>& previous, const QSet<Vertex*>& unvisited);
    static QString buildDijkstraResult(Vertex* startVertex, Vertex* endVertex,
                                      const QMap<Vertex*, int>& distances, const QMap<Vertex*, Vertex*>& previous);

    static QString validateMaxFlowInput(Graph* graph, int sourceId, int sinkId,
                                       Vertex*& source, Vertex*& sink);
    static void initializeResidualNetwork(Graph* graph, QMap<Vertex*, QMap<Vertex*, int>>& residual);
    static bool findAugmentingPathBFS(Vertex* source, Vertex* sink,
                                     const QMap<Vertex*, QMap<Vertex*, int>>& residual,
                                     QMap<Vertex*, Vertex*>& parent);
    static int calculatePathFlow(Vertex* source, Vertex* sink, const QMap<Vertex*, Vertex*>& parent,
                                QMap<Vertex*, QMap<Vertex*, int>>& residual);
    static void updateResidualNetwork(Vertex* source, Vertex* sink, int pathFlow,
                                     const QMap<Vertex*, Vertex*>& parent,
                                     QMap<Vertex*, QMap<Vertex*, int>>& residual);
};

#endif