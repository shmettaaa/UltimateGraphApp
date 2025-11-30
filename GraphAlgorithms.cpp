#include "GraphAlgorithms.h"
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include <QSet>
#include <QStack>
#include <QMap>
#include <QQueue>


QString GraphAlgorithms::validateGraph(Graph* graph){
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

QString GraphAlgorithms::topologicalSort(Graph* graph){
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

bool GraphAlgorithms::hasCycleDFS(Vertex* vertex, QSet<Vertex*>& visited, QSet<Vertex*>& recursionStack){
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

void GraphAlgorithms::topologicalSortDFS(Vertex* vertex, QSet<Vertex*>& visited, QVector<Vertex*>& result){
    visited.insert(vertex);

    for (Vertex* neighbor : vertex->outNeighbors()) {
        if (!visited.contains(neighbor)) {
            topologicalSortDFS(neighbor, visited, result);
        }
    }

    result.append(vertex);
}

bool GraphAlgorithms::isWeaklyConnected(Graph* graph){
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


QString GraphAlgorithms::eulerianCycle(Graph* graph){
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

bool GraphAlgorithms::hasEulerianCycleConditions(Graph* graph){
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

void GraphAlgorithms::eulerianDFS(Vertex* vertex, QVector<Vertex*>& path, QMap<Vertex*, QList<Vertex*>>& availableEdges){
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

QString GraphAlgorithms::dijkstra(Graph* graph, int startVertexId, int endVertexId){
    QString result = "";
    Vertex* startVertex = nullptr;
    Vertex* endVertex = nullptr;

    QString validationError = validateDijkstraInput(graph, startVertexId, endVertexId, startVertex, endVertex);
    if (!validationError.isEmpty()) {
        result = validationError;
        return result;
    }

    QMap<Vertex*, int> distances;
    QMap<Vertex*, Vertex*> previous;
    QSet<Vertex*> unvisited;
    initializeDijkstra(graph, distances, previous, unvisited, startVertex);

    bool isAlgorithmComplete = false;
    while (!unvisited.isEmpty() && !isAlgorithmComplete) {
        Vertex* current = findMinDistanceVertex(unvisited, distances);

        if (!current || distances[current] == std::numeric_limits<int>::max()) {
            isAlgorithmComplete = true;
        } else {
            unvisited.remove(current);
            updateNeighborDistances(current, graph, distances, previous, unvisited);
        }
    }

    result = buildDijkstraResult(startVertex, endVertex, distances, previous);
    return result;
}

QString GraphAlgorithms::validateDijkstraInput(Graph* graph, int startVertexId, int endVertexId,
                                              Vertex*& startVertex, Vertex*& endVertex){
    QString errorMessage = "";

    if (!graph) {
        errorMessage = "Graph is not initialized.";
    } else {
        startVertex = graph->getVertexById(startVertexId);
        endVertex = graph->getVertexById(endVertexId);

        if (!startVertex) {
            errorMessage = "Start vertex with ID " + QString::number(startVertexId) + " not found.";
        } else if (!endVertex) {
            errorMessage = "End vertex with ID " + QString::number(endVertexId) + " not found.";
        } else if (startVertex == endVertex) {
            errorMessage = "Start and end vertices are the same. Distance: 0";
        }
    }

    return errorMessage;
}

void GraphAlgorithms::initializeDijkstra(Graph* graph, QMap<Vertex*, int>& distances,
                                        QMap<Vertex*, Vertex*>& previous, QSet<Vertex*>& unvisited,
                                        Vertex* startVertex){
    for (Vertex* vertex : graph->vertices()) {
        distances[vertex] = std::numeric_limits<int>::max();
        previous[vertex] = nullptr;
        unvisited.insert(vertex);
    }
    distances[startVertex] = 0;
}

Vertex* GraphAlgorithms::findMinDistanceVertex(const QSet<Vertex*>& unvisited, const QMap<Vertex*, int>& distances){
    Vertex* minVertex = nullptr;
    int minDistance = std::numeric_limits<int>::max();

    for (Vertex* vertex : unvisited) {
        if (distances[vertex] < minDistance) {
            minDistance = distances[vertex];
            minVertex = vertex;
        }
    }

    return minVertex;
}

void GraphAlgorithms::updateNeighborDistances(Vertex* current, Graph* graph, QMap<Vertex*, int>& distances,
                                            QMap<Vertex*, Vertex*>& previous, const QSet<Vertex*>& unvisited){
    for (Vertex* neighbor : current->outNeighbors()) {
        if (unvisited.contains(neighbor)) {
            Edge* edge = graph->getEdge(current, neighbor);
            if (edge) {
                int alternative = distances[current] + edge->weight();
                if (alternative < distances[neighbor]) {
                    distances[neighbor] = alternative;
                    previous[neighbor] = current;
                }
            }
        }
    }
}

QString GraphAlgorithms::buildDijkstraResult(Vertex* startVertex, Vertex* endVertex,
                                            const QMap<Vertex*, int>& distances, const QMap<Vertex*, Vertex*>& previous){
    QString result = "";

    if (distances[endVertex] == std::numeric_limits<int>::max()) {
        result = "No path from vertex " + QString::number(startVertex->id()) +
                 " to vertex " + QString::number(endVertex->id());
    } else {
        QVector<Vertex*> path;
        Vertex* current = endVertex;
        bool isPathComplete = false;

        while (!isPathComplete) {
            path.prepend(current);
            current = previous[current];
            if (!current) {
                isPathComplete = true;
            }
        }

        result = "Shortest path from " + QString::number(startVertex->id()) +
                 " to " + QString::number(endVertex->id()) + ":\n";
        result += "Distance: " + QString::number(distances[endVertex]) + "\n";
        result += "Path: ";

        for (int i = 0; i < path.size(); ++i) {
            result += QString::number(path[i]->id());
            if (i < path.size() - 1) {
                result += " → ";
            }
        }
    }

    return result;
}



QString GraphAlgorithms::maxFlow(Graph* graph, int sourceId, int sinkId){
    QString result = "";
    Vertex* source = nullptr;
    Vertex* sink = nullptr;

    QString validationError = validateMaxFlowInput(graph, sourceId, sinkId, source, sink);
    if (!validationError.isEmpty()) {
        result = validationError;
        return result;
    }

    QMap<Vertex*, QMap<Vertex*, int>> residual;
    initializeResidualNetwork(graph, residual);

    int maxFlow = 0;
    bool isPathFound = true;

    while (isPathFound) {
        QMap<Vertex*, Vertex*> parent;
        isPathFound = findAugmentingPathBFS(source, sink, residual, parent);

        if (isPathFound) {
            int pathFlow = calculatePathFlow(source, sink, parent, residual);
            updateResidualNetwork(source, sink, pathFlow, parent, residual);
            maxFlow += pathFlow;
        }
    }

    result = "Maximum flow from source " + QString::number(sourceId) +
             " to sink " + QString::number(sinkId) + ": " + QString::number(maxFlow);
    return result;
}

QString GraphAlgorithms::validateMaxFlowInput(Graph* graph, int sourceId, int sinkId,
                                             Vertex*& source, Vertex*& sink){
    QString errorMessage = "";

    if (!graph) {
        errorMessage = "Graph is not initialized.";
    } else {
        source = graph->getVertexById(sourceId);
        sink = graph->getVertexById(sinkId);

        if (!source) {
            errorMessage = "Source vertex with ID " + QString::number(sourceId) + " not found.";
        } else if (!sink) {
            errorMessage = "Sink vertex with ID " + QString::number(sinkId) + " not found.";
        } else if (source == sink) {
            errorMessage = "Source and sink vertices are the same. Max flow: 0";
        }
    }

    return errorMessage;
}

void GraphAlgorithms::initializeResidualNetwork(Graph* graph, QMap<Vertex*, QMap<Vertex*, int>>& residual){
    for (Vertex* u : graph->vertices()) {
        for (Vertex* v : u->outNeighbors()) {
            Edge* edge = graph->getEdge(u, v);
            if (edge) {
                residual[u][v] = edge->weight();
                if (!residual[v].contains(u)) {
                    residual[v][u] = 0;
                }
            }
        }
    }
}

bool GraphAlgorithms::findAugmentingPathBFS(Vertex* source, Vertex* sink,
                                           const QMap<Vertex*, QMap<Vertex*, int>>& residual,
                                           QMap<Vertex*, Vertex*>& parent){
    QQueue<Vertex*> queue;
    QSet<Vertex*> visited;

    queue.enqueue(source);
    visited.insert(source);
    parent[source] = nullptr;

    bool isPathFound = false;
    bool isBFSSearching = true;

    while (!queue.isEmpty() && isBFSSearching && !isPathFound) {
        Vertex* current = queue.dequeue();

        QList<Vertex*> neighbors = residual[current].keys();
        int neighborIndex = 0;

        while (neighborIndex < neighbors.size() && !isPathFound) {
            Vertex* neighbor = neighbors[neighborIndex];

            if (!visited.contains(neighbor) && residual[current][neighbor] > 0) {
                visited.insert(neighbor);
                parent[neighbor] = current;
                queue.enqueue(neighbor);

                if (neighbor == sink) {
                    isPathFound = true;
                }
            }

            neighborIndex++;
        }
    }

    return isPathFound;
}

int GraphAlgorithms::calculatePathFlow(Vertex* source, Vertex* sink, const QMap<Vertex*, Vertex*>& parent,
                                      QMap<Vertex*, QMap<Vertex*, int>>& residual){
    int pathFlow = std::numeric_limits<int>::max();
    Vertex* current = sink;
    bool isFlowCalculationComplete = false;

    while (!isFlowCalculationComplete) {
        Vertex* prev = parent[current];
        if (prev) {
            pathFlow = std::min(pathFlow, residual[prev][current]);
            current = prev;
        } else {
            isFlowCalculationComplete = true;
        }
    }

    return pathFlow;
}

void GraphAlgorithms::updateResidualNetwork(Vertex* source, Vertex* sink, int pathFlow,
                                          const QMap<Vertex*, Vertex*>& parent,
                                          QMap<Vertex*, QMap<Vertex*, int>>& residual){
    Vertex* current = sink;
    bool isNetworkUpdateComplete = false;

    while (!isNetworkUpdateComplete) {
        Vertex* prev = parent[current];
        if (prev) {
            residual[prev][current] -= pathFlow;
            residual[current][prev] += pathFlow;
            current = prev;
        } else {
            isNetworkUpdateComplete = true;
        }
    }
}