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
 *    cxlei
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
    double* vertex_links;
    bool* has_link;

    int lastStartVertex;
    bool heapIsValid;

    int degree( int ) const;
    int edge_count() const;
    double adjacent( int, int ) const;
    double distance( int, int );
    void insert( int, int, double );

    int* allVerticesInOrderArrayForSwapping;

    int* vertexDistanceHeap;
    int numberOfElementsInTheHeap;
    int* vertexLocations;
    double* vertexSmallestWeights;

    int* vertexVisitHeap;
    int verticesLeft;
    int* notYetVisitedVerticesLocations;

    // Friends

    friend std::ostream &operator<<( std::ostream &, Weighted_graph const & );
};

const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

// Your implementation here

Weighted_graph::Weighted_graph(int n):
        numberOfVertices((n < 1) ? 1 : n),
        edges_count(0),
        lastStartVertex(-1),
        heapIsValid(false)
{
    /* We will use a 2D array to store the edges and information about
     * connections between edges */
    int const linkLength = numberOfVertices*numberOfVertices;
    vertex_links = new double[linkLength];
    has_link = new bool[linkLength];
    vertex_degrees = new int[numberOfVertices];

    /* Keep track of whether where
     * */
    notYetVisitedVerticesLocations = new int[numberOfVertices];
    vertexLocations = new int[numberOfVertices];
    vertexSmallestWeights = new double[numberOfVertices];

    int numberOfVerticesPlusOne = numberOfVertices + 1;
    allVerticesInOrderArrayForSwapping = new int[numberOfVerticesPlusOne];
    vertexDistanceHeap = new int[numberOfVerticesPlusOne];
    vertexVisitHeap = new int[numberOfVerticesPlusOne];

    for(int i = 0; i <= numberOfVertices; ++i) {
        allVerticesInOrderArrayForSwapping[i] = i;
    }
    std::memset(&vertex_links[0], 0, linkLength* sizeof(double));
    std::memset(&has_link[0], 0, linkLength* sizeof(bool));
    std::memset(&vertex_degrees[0], 0, numberOfVertices * sizeof(int));
    std::memset(&vertexDistanceHeap[1], 0, numberOfVertices * sizeof(int));
}

Weighted_graph::~Weighted_graph() {
    delete[] allVerticesInOrderArrayForSwapping;

    delete[] vertex_links;
    delete[] has_link;
    delete[] vertex_degrees;
    delete[] vertexDistanceHeap;

    delete[] notYetVisitedVerticesLocations;
    delete[] vertexVisitHeap;
    delete[] vertexLocations;
    delete[] vertexSmallestWeights;
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

    double const weight = vertex_links[size_t(numberOfVertices) * n + m];
    if (!weight) {
        return INF;
    }
    return weight;
}

double Weighted_graph::distance( int m, int n) {
    int const minIndex = (m < n) ? m : n;
    int const maxIndex = (minIndex == m) ? n : m;
    if (minIndex < 0 || maxIndex > numberOfVertices - 1) {
        throw illegal_argument();
    } else if (m == n) {
        return 0;
    }

    int currentNode;

    if (lastStartVertex == minIndex) {
        if (!notYetVisitedVerticesLocations[maxIndex]) {
            return vertexSmallestWeights[maxIndex];
        } else if (!numberOfElementsInTheHeap) {
            return INF;
        }

    } else {
//        reset_distance();
        verticesLeft = numberOfVertices;
        numberOfElementsInTheHeap = 0;
        std::memcpy(&vertexVisitHeap[1], &allVerticesInOrderArrayForSwapping[0], numberOfVertices* sizeof(int));
        std::memcpy(&notYetVisitedVerticesLocations[0], &allVerticesInOrderArrayForSwapping[1], numberOfVertices * sizeof(int));
        std::memset(&vertexLocations[0], 0, numberOfVertices * sizeof(int));
        std::memset(&vertexSmallestWeights[0], 0, numberOfVertices * sizeof(double));
        if (!heapIsValid) {
            heapIsValid = true;
        }

        // This will get immediately popped off, it is the only case where we allow a distance of 0 in the heap
        ++numberOfElementsInTheHeap;
        vertexSmallestWeights[minIndex] = 0;
        vertexDistanceHeap[1] = minIndex;
        lastStartVertex = minIndex;
    }

    while(numberOfElementsInTheHeap) {

        currentNode = vertexDistanceHeap[1];

        if (currentNode == maxIndex) {
            return vertexSmallestWeights[maxIndex];
        }

        //pop_closest_vertex()
        if (numberOfElementsInTheHeap == 1) {
            numberOfElementsInTheHeap--;
        } else {
            int const closestVertexAtTheBack = vertexDistanceHeap[numberOfElementsInTheHeap];
            double const pushSize = vertexSmallestWeights[closestVertexAtTheBack];
            int currentParentVertex = 1;
            while (true) {
                int const first_child = currentParentVertex << 1;
                // Needs to be at the top here to catch first edge case
                if (first_child >= numberOfElementsInTheHeap) {
                    break;
                }
                int const second_child_index = first_child | 1;
                int const left_child = vertexDistanceHeap[first_child];
                double const left_weight = vertexSmallestWeights[left_child];
                int const right_child = vertexDistanceHeap[second_child_index];
                double const right_weight = vertexSmallestWeights[right_child];

                if (left_weight > right_weight) {
                    if (pushSize > right_weight) {
                        vertexLocations[right_child] = currentParentVertex;
                        vertexDistanceHeap[currentParentVertex] = right_child;
                        currentParentVertex = second_child_index;
                    } else {
                        break;
                    }
                } else {
                    if (pushSize > left_weight) {
                        vertexLocations[left_child] = currentParentVertex;
                        vertexDistanceHeap[currentParentVertex] = left_child;
                        currentParentVertex = first_child;
                    } else {
                        break;
                    }
                }
            }
            vertexDistanceHeap[currentParentVertex] = closestVertexAtTheBack;
            vertexLocations[closestVertexAtTheBack] = currentParentVertex;
            numberOfElementsInTheHeap--;
        }

//        pop_unvisited_vertex(currentNode);
        int const currentNodeLocation = notYetVisitedVerticesLocations[currentNode];
        int const lastVertexAtBack = vertexVisitHeap[verticesLeft];
        vertexVisitHeap[currentNodeLocation] = lastVertexAtBack;
        notYetVisitedVerticesLocations[lastVertexAtBack] = currentNodeLocation;
        notYetVisitedVerticesLocations[currentNode] = 0;
        verticesLeft--;

//        add_surrounding_weights(distanceToGetHere);
        int const maxLinksToCurrentNode = vertex_degrees[currentNode];
        int foundLinks = 0;
        double const currentDistanceToGetHere = vertexSmallestWeights[currentNode];
        for(int i = 1; i <= verticesLeft; ++i) {
            int const destinationVertex = vertexVisitHeap[i];
            size_t const destinationVertexArrayOffset = size_t(numberOfVertices) * currentNode + destinationVertex;
            if(has_link[destinationVertexArrayOffset]) {
                ++foundLinks;

                // heapInsert
                int currentPlace;
                double const currentVertexWeight = vertexSmallestWeights[destinationVertex];
                double const totalDistance = currentDistanceToGetHere + vertex_links[destinationVertexArrayOffset];
                if (!currentVertexWeight) {
                    ++numberOfElementsInTheHeap;
                    currentPlace = numberOfElementsInTheHeap;
                } else if (totalDistance >= currentVertexWeight) {
                    if (foundLinks == maxLinksToCurrentNode) {
                        break;
                    }
                    continue;
                } else {
                    currentPlace = vertexLocations[destinationVertex];
                }

                while (true) {
                    if (currentPlace == 1) {
                        break;
                    }
                    int const parent = currentPlace >> 1;
                    int const parentVertex = vertexDistanceHeap[parent];
                    // Vertex weights can't be 0 here because we wouldn't put 0 into the heap
                    if (vertexSmallestWeights[parentVertex] < totalDistance) {
                        break;
                    }
                    vertexLocations[parentVertex] = currentPlace;
                    vertexDistanceHeap[currentPlace] = parentVertex;
                    currentPlace = parent;
                }
                vertexDistanceHeap[currentPlace] = destinationVertex;
                vertexSmallestWeights[destinationVertex] = totalDistance;
                vertexLocations[destinationVertex] = currentPlace;

                if (foundLinks == maxLinksToCurrentNode) {
                    break;
                }
            }
        }
    }
    vertexSmallestWeights[maxIndex] = INF;
    notYetVisitedVerticesLocations[maxIndex] = 0;
    return INF;
}

void Weighted_graph::insert( int m, int n, double w ) {
    if (w <= 0 || m == n || m < 0 || n < 0 || m > numberOfVertices - 1 || n > numberOfVertices - 1) {
        throw illegal_argument();
    }
    if (heapIsValid) {
        lastStartVertex = -1;
        heapIsValid = false;
    }

    int const firstLocation = m*numberOfVertices + n;
    int const secondLocation = n*numberOfVertices + m;
    if (!vertex_links[firstLocation]) {
        ++edges_count;
        ++vertex_degrees[m];
        ++vertex_degrees[n];
        has_link[firstLocation] = true;
        has_link[secondLocation] = true;
    }

    vertex_links[firstLocation] = w;
    vertex_links[secondLocation] = w;
}

// You can modify this function however you want:  it will not be tested

std::ostream &operator<<( std::ostream &out, Weighted_graph const &graph ) {
    return out;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif