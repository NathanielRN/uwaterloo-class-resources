/*****************************************
 * Instructions
 *  - Replace 'uwuserid' with your uWaterloo User ID
 *  - Select the current calendar term and enter the year
 *  - List students with whom you had discussions and who helped you
 *
 * uWaterloo User ID:  enruizno @uwaterloo.ca
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  (Winter) 2018
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *
 * The following is a list of uWaterloo User IDs of those students
 * I had discussions with in preparing this project:
 *    gnardyee
 *
 * The following is a list of uWaterloo User IDs of those students
 * who helped me with this project (describe their help; e.g., debugging):
 *    -
 *****************************************/

#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H

#ifndef nullptr
#define nullptr 0
#endif

#include <iostream>
#include <limits>
#include <cstring>
#include "Exception.h"

// include whatever classes you want


class Weighted_graph {
private:
    // your implementation here
    //  you can add both private member variables and private member functions

    static const double INF;

public:
    Weighted_graph( int = 50 );
    ~Weighted_graph();

    int const numberOfVertices;
    int edges_count;
    int* vertex_degrees;
    double** vertex_links;

    int currentNode;

    int degree( int ) const;
    int edge_count() const;
    double adjacent( int, int ) const;
    double distance( int, int );
    void insert( int, int, double );
    void destroy();

    int* allVerticesInOrder;

    int* vertexDistanceHeap;
    int numberOfElementsInTheHeap;
    int* vertexLocations;
    double* vertexSmallestWeights;

    int* vertexVisitHeap;
    int verticesLeft;
    int* notYetVisitedVerticesLocations;

    int pop_closest_vertex();
    void heap_insert(int , double );

    void pop_unvisited_vertex(int);

    // Friends

    friend std::ostream &operator<<( std::ostream &, Weighted_graph const & );
};

const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

// Your implementation here

Weighted_graph::Weighted_graph(int n):
        numberOfVertices((n < 1) ? 1 : n),
        edges_count(0),
        currentNode(0)
{
    allVerticesInOrder = new int[numberOfVertices+1];

    notYetVisitedVerticesLocations = new int[numberOfVertices];
    vertexVisitHeap = new int[numberOfVertices+1];
    vertex_degrees = new int[numberOfVertices];
    vertex_links = new double*[numberOfVertices];
    vertexDistanceHeap = new int[numberOfVertices+1];
    vertexLocations = new int[numberOfVertices];
    vertexSmallestWeights = new double[numberOfVertices];
    for(int i = 0; i < numberOfVertices; i++) {
        allVerticesInOrder[i] = i;

        vertex_links[i] = new double[numberOfVertices];
        for(int j = 0; j < numberOfVertices; j++) {
            vertex_links[i][j] = 0;
        }
    }
    allVerticesInOrder[numberOfVertices] = numberOfVertices;
    std::memset(&vertex_degrees[0], 0, numberOfVertices * sizeof(int));
    std::memset(&vertexDistanceHeap[1], 0, numberOfVertices * sizeof(int));
}

Weighted_graph::~Weighted_graph() {
    for(int i = 0; i < numberOfVertices ; i++) {
        delete[] vertex_links[i];
    }
    delete[] vertex_links;
    delete[] vertex_degrees;
    delete[] vertexDistanceHeap;
    delete[] vertexLocations;
    delete[] vertexSmallestWeights;
    delete[] vertexVisitHeap;
    delete[] notYetVisitedVerticesLocations;

    delete[] allVerticesInOrder;
}

int Weighted_graph::degree( int i ) const {
    return vertex_degrees[i];
}

int Weighted_graph::edge_count() const {
    return edges_count;
}

double Weighted_graph::adjacent( int m, int n ) const {
    if (m < 0 || n < 0 || m > numberOfVertices - 1 || n > numberOfVertices - 1) {
        throw illegal_argument();
    } else if (m == n) {
        return 0;
    }

    double const weight = vertex_links[m][n];
    if (!weight) {
        return INF;
    }
    return weight;
}

double Weighted_graph::distance( int m, int n) {
    if (m < 0 || n < 0 || m > numberOfVertices - 1 || n > numberOfVertices - 1) {
        throw illegal_argument();
    } else if (m == n) {
        return 0;
    }

    //Reset procedure
    currentNode = m;
    verticesLeft = numberOfVertices;
    numberOfElementsInTheHeap = 0;
    std::memcpy(&vertexVisitHeap[1], &allVerticesInOrder[0], numberOfVertices* sizeof(int));
    std::memset(&vertexLocations[0], 0, numberOfVertices * sizeof(int));
    std::memcpy(&notYetVisitedVerticesLocations[0], &allVerticesInOrder[1], numberOfVertices * sizeof(int));
    std::memset(&vertexSmallestWeights[0], 0, numberOfVertices* sizeof(double));

    heap_insert(currentNode, 0);
    while(true) {
        double const distanceToGetHere = vertexSmallestWeights[pop_closest_vertex()];
        pop_unvisited_vertex(currentNode);

        int const numberOfDegrees = vertex_degrees[currentNode];

        int foundLinks = 0;
        for(int i = 1; i <= verticesLeft; i++) {
            int vertexToCheck = vertexVisitHeap[i];
            double weight = vertex_links[currentNode][vertexToCheck];
            if (weight) {
                foundLinks++;
                double currentDisToVertex = distanceToGetHere + weight;
                double currentVertexWeight = vertexSmallestWeights[vertexToCheck];
                if (!currentVertexWeight || currentDisToVertex < currentVertexWeight) {
                    heap_insert(vertexToCheck, currentDisToVertex);
                }
            }
            if (foundLinks == numberOfDegrees) {
                break;
            }
        }

        if (!numberOfElementsInTheHeap) {
            return INF;
        }

        currentNode = vertexDistanceHeap[1];

        if (currentNode == n) {
            return vertexSmallestWeights[currentNode];
        }
    }
}

void Weighted_graph::heap_insert(int destinationVertex, double totalDistance) {
    int currentPlace;
    if (vertexSmallestWeights[destinationVertex] == 0) {
        numberOfElementsInTheHeap++;
        currentPlace = numberOfElementsInTheHeap;
        vertexLocations[destinationVertex] = numberOfElementsInTheHeap;
    } else {
        currentPlace = vertexLocations[destinationVertex];
    }

    while (true) {
        int const parent = currentPlace >> 1;
        int const parentVertex = vertexDistanceHeap[parent];
        // Vertex weights can't be 0 here because we wouldn't put 0 into the heap
        if (currentPlace == 1 || vertexSmallestWeights[parentVertex] < totalDistance) {
            vertexDistanceHeap[currentPlace] = destinationVertex;
            vertexSmallestWeights[destinationVertex] = totalDistance;
            vertexLocations[destinationVertex] = currentPlace;
            return;
        }
        vertexLocations[parentVertex] = currentPlace;
        vertexDistanceHeap[currentPlace] = parentVertex;
        currentPlace = parent;
    }

}

void Weighted_graph::pop_unvisited_vertex(int v) {
    int currentLocation = notYetVisitedVerticesLocations[v];
    int backElement = vertexVisitHeap[verticesLeft];
    vertexVisitHeap[currentLocation] = backElement;
    notYetVisitedVerticesLocations[backElement] = currentLocation;
    verticesLeft--;
}

int Weighted_graph::pop_closest_vertex() {
    int const minVertex = vertexDistanceHeap[1];
    if (numberOfElementsInTheHeap == 1) {
        vertexDistanceHeap[numberOfElementsInTheHeap] = 0;
        numberOfElementsInTheHeap--;
        return minVertex;
    }

    int const elementAtTheBack = vertexDistanceHeap[numberOfElementsInTheHeap];
    double const pushSize = vertexSmallestWeights[elementAtTheBack];
    int k = 1;
    int first_child = k << 1;
    while (true) {
        int const second_child_index = first_child | 1;
        int const left_child = vertexDistanceHeap[first_child];
        double const left_weight = vertexSmallestWeights[left_child];
        if (second_child_index > numberOfVertices || second_child_index > numberOfElementsInTheHeap) {
            if (pushSize > left_weight) {
                vertexLocations[left_child] = k;
                vertexDistanceHeap[k] = left_child;
                k = second_child_index;
            }
            break;
        }
        int const right_child = vertexDistanceHeap[first_child | 1];
        double const right_weight = vertexSmallestWeights[right_child];

        if (left_weight >
            right_weight) {
            if (pushSize > right_weight) {
                vertexLocations[right_child] = k;
                vertexDistanceHeap[k] = right_child;
                k = second_child_index;
                first_child = k << 1;
                if (first_child > numberOfVertices || first_child > numberOfElementsInTheHeap) {
                    break;
                }
            } else {
                break;
            }
        } else {
            if (pushSize > left_weight) {

                vertexLocations[left_child] = k;
                vertexDistanceHeap[k] = left_child;
                k = first_child;
                first_child = k << 1;
                if (first_child > numberOfVertices || first_child > numberOfElementsInTheHeap) {
                    break;
                }
            } else {
                break;
            }
        }
    }
    vertexDistanceHeap[k] = elementAtTheBack;
    vertexLocations[elementAtTheBack] = k;
    numberOfElementsInTheHeap--;
    return minVertex;
}

void Weighted_graph::insert( int m, int n, double w ) {
    if (w <= 0 || m == n || m < 0 || n < 0 || m > numberOfVertices - 1 || n > numberOfVertices - 1) {
        throw illegal_argument();
    }

    if (vertex_links[m][n] == 0) {
        edges_count++;
        vertex_degrees[m]++;
        vertex_degrees[n]++;
    }

    vertex_links[m][n] = w;
    vertex_links[n][m] = w;
}

void Weighted_graph::destroy() {
    edges_count = 0;
    std::memset(&vertex_degrees[0], 0, numberOfVertices * sizeof(int));
    for(int i = 0; i < numberOfVertices; i++) {
        vertex_links[i] = new double[numberOfVertices];
    }

}

// You can modify this function however you want:  it will not be tested

std::ostream &operator<<( std::ostream &out, Weighted_graph const &graph ) {
    return out;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif