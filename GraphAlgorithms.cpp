#include "GraphAlgorithms.h"
#include "Vertex.h"
#include "Edge.h"
#include <QSet>
#include <QStack>
#include <QMap>
#include <QQueue>


QString GraphAlgorithms::validateGraph(Graph* graph)
{
    QString errorMessage = "";

    if (!graph) {
        errorMessage = "Graph is not initialized.";
    }
    else if (graph->vertexCount() == 0) {
        errorMessage = "Graph is empty. No vertices for sorting.";
    }
    else if (!isWeaklyConnected(graph)) {
        errorMessage = "Graph is not weakly connected. Topological sort is only possible for weakly connected directed graphs.";
    }
    else {
        QSet<Vertex*> visited;
        QSet<Vertex*> recursionStack;
        bool hasCycle = false;

        for (Vertex* vertex : graph->vertices()) {
            if (!visited.contains(vertex)) {
                if (hasCycleDFS(vertex, visited, recursionStack)) {
                    hasCycle = true;
                }
            }
        }

        if (hasCycle) {
            errorMessage = "Graph contains cycles. Topological sort is not possible for graphs with cycles.";
        }
    }

    return errorMessage;
}

QString GraphAlgorithms::topologicalSort(Graph* graph)
{
    QString result = "";
    QString error = validateGraph(graph);
    if (!error.isEmpty()) {
        result = error;
    }
    else {
        QSet<Vertex*> sortVisited;
        QVector<Vertex*> sortedVertices;

        for (Vertex* vertex : graph->vertices()) {
            if (!sortVisited.contains(vertex)) {
                topologicalSortDFS(vertex, sortVisited, sortedVertices);
            }
        }
        for (int i = sortedVertices.size() - 1; i >= 0; --i) {
            result += QString::number(sortedVertices[i]->id());
            if (i > 0) {
                result += " -> ";
            }
        }
    }

    return result;
}

bool GraphAlgorithms::hasCycleDFS(Vertex* vertex, QSet<Vertex*>& visited, QSet<Vertex*>& recursionStack)
{
    bool hasCycle = false;

    if (recursionStack.contains(vertex)) {
        hasCycle = true;
    }
    else if (!visited.contains(vertex)) {
        visited.insert(vertex);
        recursionStack.insert(vertex);

        for (Vertex* neighbor : vertex->outNeighbors()) {
            if (hasCycleDFS(neighbor, visited, recursionStack)) {
                hasCycle = true;
            }
        }

        recursionStack.remove(vertex);
    }

    return hasCycle;
}

void GraphAlgorithms::topologicalSortDFS(Vertex* vertex, QSet<Vertex*>& visited, QVector<Vertex*>& result)
{
    visited.insert(vertex);

    for (Vertex* neighbor : vertex->outNeighbors()) {
        if (!visited.contains(neighbor)) {
            topologicalSortDFS(neighbor, visited, result);
        }
    }

    result.append(vertex);
}

bool GraphAlgorithms::isWeaklyConnected(Graph* graph)
{
    bool isConnected = true;

    if (graph->vertexCount() == 0) {
        isConnected = true;
    }
    else {
        QSet<Vertex*> visited;
        QQueue<Vertex*> queue;

        queue.enqueue(graph->vertices().first());
        visited.insert(graph->vertices().first());

        while (!queue.isEmpty()) {
            Vertex* current = queue.dequeue();

            for (Vertex* neighbor : current->outNeighbors()) {
                if (!visited.contains(neighbor)) {
                    visited.insert(neighbor);
                    queue.enqueue(neighbor);
                }
            }

            for (Vertex* neighbor : current->inNeighbors()) {
                if (!visited.contains(neighbor)) {
                    visited.insert(neighbor);
                    queue.enqueue(neighbor);
                }
            }
        }

        isConnected = (visited.size() == graph->vertexCount());
    }

    return isConnected;
}


QString GraphAlgorithms::eulerianCycle(Graph* graph)
{
    QString result = "";

    if (!graph) {
        result = "Graph is not initialized.";
    }
    else if (graph->vertexCount() == 0) {
        result = "Graph is empty. No vertices for finding Eulerian cycle.";
    }
    else if (!hasEulerianCycleConditions(graph)) {
        result = "Graph does not satisfy conditions for Eulerian cycle.";
    }
    else {
        QMap<Vertex*, QList<Vertex*>> availableEdges;
        for (Vertex* vertex : graph->vertices()) {
            availableEdges[vertex] = vertex->outNeighbors().values();
        }
        Vertex* startVertex = nullptr;
        bool isStartVertexFound = false;
        for (Vertex* vertex : graph->vertices()) {
            if (!isStartVertexFound && vertex->outDegree() > 0) {
                startVertex = vertex;
                isStartVertexFound = true;
            }
        }

        if (!startVertex) {
            result = "No edges found in the graph.";
        }
        else {
            QVector<Vertex*> path;
            eulerianDFS(startVertex, path, availableEdges);

            if (path.isEmpty()) {
                result = "No Eulerian cycle found.";
            }
            else {
                for (int i = 0; i < path.size(); ++i) {
                    result += QString::number(path[i]->id());
                    if (i < path.size() - 1) {
                        result += " -> ";
                    }
                }
            }
        }
    }

    return result;
}

bool GraphAlgorithms::hasEulerianCycleConditions(Graph* graph)
{
    bool conditionsSatisfied = true;

    if (!isWeaklyConnected(graph)) {
        conditionsSatisfied = false;
    }
    else {
        bool hasDegreeMismatch = false;
        for (Vertex* vertex : graph->vertices()) {
            if (!hasDegreeMismatch && vertex->inDegree() != vertex->outDegree()) {
                hasDegreeMismatch = true;
            }
        }

        if (hasDegreeMismatch) {
            conditionsSatisfied = false;
        }
    }

    return conditionsSatisfied;
}

void GraphAlgorithms::eulerianDFS(Vertex* vertex, QVector<Vertex*>& path, QMap<Vertex*, QList<Vertex*>>& availableEdges)
{
    QStack<Vertex*> stack;
    stack.push(vertex);

    QVector<Vertex*> circuit;

    while (!stack.isEmpty()) {
        Vertex* current = stack.top();

        if (!availableEdges[current].isEmpty()) {
            Vertex* next = availableEdges[current].first();
            availableEdges[current].removeFirst();
            stack.push(next);
        }
        else {
            circuit.append(current);
            stack.pop();
        }
    }
    for (int i = circuit.size() - 1; i >= 0; --i) {
        path.append(circuit[i]);
    }
}

QString GraphAlgorithms::dijkstra(Graph* graph, int startVertexId, int endVertexId) {
    return 0;
}

QString GraphAlgorithms::maxFlow(Graph* graph, int sourceId, int sinkId) {
    return 0;
}