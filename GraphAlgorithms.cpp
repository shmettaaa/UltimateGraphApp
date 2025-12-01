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



QString GraphAlgorithms::stronglyConnectedComponents(Graph* graph)
{
    QString result = "";

    if (!graph) {
        result = "Graph is not initialized.";
        return result;
    }

    if (graph->vertexCount() == 0) {
        result = "Graph is empty. No vertices for SCC analysis.";
        return result;
    }

    QSet<Vertex*> visited;
    QStack<Vertex*> finishOrder;

    for (Vertex* vertex : graph->vertices()) {
        if (!visited.contains(vertex)) {
            kosarajuDFSFirstPass(vertex, visited, finishOrder);
        }
    }

    Graph* transposedGraph = transposeGraph(graph);

    visited.clear();
    QVector<QVector<Vertex*>> components;

    while (!finishOrder.isEmpty()) {
        Vertex* vertex = finishOrder.pop();
        Vertex* transposedVertex = transposedGraph->getVertexById(vertex->id());

        if (!visited.contains(transposedVertex)) {
            QVector<Vertex*> component;
            kosarajuDFSSecondPass(transposedVertex, visited, component);
            components.append(component);
        }
    }

    result = "Strongly Connected Components:\n";
    if (components.isEmpty()) {
        result += "No strongly connected components found.";
    } else {
        for (int i = 0; i < components.size(); ++i) {
            result += "Component " + QString::number(i + 1) + " (" +
                     QString::number(components[i].size()) + " vertices): ";

            for (int j = 0; j < components[i].size(); ++j) {
                result += QString::number(components[i][j]->id());
                if (j < components[i].size() - 1) {
                    result += " → ";
                }
            }
            result += "\n";
        }
        result += "Total: " + QString::number(components.size()) + " components";
    }

    delete transposedGraph;
    return result;
}

void GraphAlgorithms::kosarajuDFSFirstPass(Vertex* vertex, QSet<Vertex*>& visited, QStack<Vertex*>& finishOrder)
{
    visited.insert(vertex);

    for (Vertex* neighbor : vertex->outNeighbors()) {
        if (!visited.contains(neighbor)) {
            kosarajuDFSFirstPass(neighbor, visited, finishOrder);
        }
    }

    finishOrder.push(vertex);
}

void GraphAlgorithms::kosarajuDFSSecondPass(Vertex* vertex, QSet<Vertex*>& visited, QVector<Vertex*>& component)
{
    visited.insert(vertex);
    component.append(vertex);

    for (Vertex* neighbor : vertex->outNeighbors()) {
        if (!visited.contains(neighbor)) {
            kosarajuDFSSecondPass(neighbor, visited, component);
        }
    }
}

Graph* GraphAlgorithms::transposeGraph(Graph* graph)
{
    Graph* transposed = new Graph();

    for (Vertex* vertex : graph->vertices()) {
        transposed->addVertex(vertex->position());
    }

    for (Edge* edge : graph->edges()) {
        Vertex* from = transposed->getVertexById(edge->to()->id());
        Vertex* to = transposed->getVertexById(edge->from()->id());
        if (from && to) {
            transposed->addEdge(from, to);
        }
    }

    return transposed;
}


QString GraphAlgorithms::eulerianPath(Graph* graph)
{
    QString result = "";

    if (!graph) {
        result = "Graph is not initialized.";
        return result;
    }

    if (graph->vertexCount() == 0) {
        result = "Graph is empty. No vertices for Eulerian path.";
        return result;
    }

    Vertex* startVertex = nullptr;
    Vertex* endVertex = nullptr;

    bool hasEulerianPath = hasEulerianPathConditions(graph, startVertex, endVertex);

    if (!hasEulerianPath) {
        result = "Graph does not satisfy conditions for Eulerian path.";
        return result;
    }

    if (!startVertex) {
        startVertex = findEulerianStartVertex(graph);
    }

    if (!startVertex) {
        result = "Cannot find start vertex for Eulerian path.";
        return result;
    }

    QMap<Vertex*, QList<Vertex*>> availableEdges;
    for (Vertex* vertex : graph->vertices()) {
        availableEdges[vertex] = vertex->outNeighbors().values();
    }

    QVector<Vertex*> path;
    QStack<Vertex*> stack;
    stack.push(startVertex);

    while (!stack.isEmpty()) {
        Vertex* current = stack.top();

        if (!availableEdges[current].isEmpty()) {
            Vertex* next = availableEdges[current].first();
            availableEdges[current].removeFirst();
            stack.push(next);
        } else {
            path.append(current);
            stack.pop();
        }
    }

    QVector<Vertex*> eulerPath;
    for (int i = path.size() - 1; i >= 0; --i) {
        eulerPath.append(path[i]);
    }

    result = "Eulerian Path found:\n";
    for (int i = 0; i < eulerPath.size(); ++i) {
        result += QString::number(eulerPath[i]->id());
        if (i < eulerPath.size() - 1) {
            result += " → ";
        }
    }

    return result;
}

bool GraphAlgorithms::hasEulerianPathConditions(Graph* graph, Vertex*& startVertex, Vertex*& endVertex)
{
    int startCount = 0;
    int endCount = 0;

    for (Vertex* vertex : graph->vertices()) {
        int outDegree = vertex->outDegree();
        int inDegree = vertex->inDegree();

        if (outDegree - inDegree == 1) {
            startCount++;
            startVertex = vertex;
        } else if (inDegree - outDegree == 1) {
            endCount++;
            endVertex = vertex;
        } else if (inDegree != outDegree) {
            return false;
        }
    }

    bool isValid = (startCount == 0 && endCount == 0) || (startCount == 1 && endCount == 1);
    return isValid;
}

Vertex* GraphAlgorithms::findEulerianStartVertex(Graph* graph)
{
    Vertex* startVertex = nullptr;

    for (Vertex* vertex : graph->vertices()) {
        if (vertex->outDegree() > 0) {
            startVertex = vertex;
            break;
        }
    }

    return startVertex;
}

QString GraphAlgorithms::vertexDegrees(Graph* graph)
{
    QString result = "";

    if (!graph) {
        result = "Graph is not initialized.";
        return result;
    }

    if (graph->vertexCount() == 0) {
        result = "Graph is empty. No vertices for degree analysis.";
        return result;
    }

    result = "Vertex degrees:\n";

    for (Vertex* vertex : graph->vertices()) {
        int inDegree = vertex->inDegree();
        int outDegree = vertex->outDegree();
        int totalDegree = inDegree + outDegree;

        result += "Vertex " + QString::number(vertex->id()) + ": ";
        result += "in=" + QString::number(inDegree) + ", ";
        result += "out=" + QString::number(outDegree) + ", ";
        result += "total=" + QString::number(totalDegree) + "\n";
    }

    return result;
}