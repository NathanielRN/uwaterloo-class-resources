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

/****************************************************
 * Weighted_graph Class
 *
 * Implementation Djikstra's algorithm optimized for the speed2.cpp test
 *
 *  Public classes, fields and methods
 *
 *      Constructors
 *
 *          new(int)        constructor     Initializes a 2D matrix and min heap for quick distance calculations,
 *                                          if the passed in value is less than 1, it's default is 1
 *
 *      Deconstructors
 *
 *          delete          deconstructor   Deletes all arrays used to store information about what the weighted
 *                                          graph contains
 *      Accessors
 *
 *          degree(int)                     Returns the degree of the passed in vertex, throw illegal argument if
 *                                          an invalid vertex
 *
 *          edge_count()                    Returns the number of edges currently in the graph
 *
 *          adjacent(int, int)              Returns the weight between two passed in vertices, throws an illegal
 *                                          argument exception if either vertex is invalid
 *
 *      Mutators
 *
 *          distance(int, int)              Returns the distance between the two passed invertices, throws an illegal
 *                                          argument exception if either vertex is invalid
 *
 *          insert(int, int)                Inserts an edge between the two passed in vertices, throws an illegal
 *                                          argument exception if either vertex is invalid
 *
 *  Private classes, fields and methods
 *
 *      Fields
 *
 *          numberOfVertices                Stores how many vertices there are in the graph
 *
 *          edges_count                     Field that keeps track of how many edges are inserted between a new pair
 *                                          of vertices
 *
 *          vertex_degrees                  Array that keeps track of the degrees of each vertex by storing their degree
 *                                          at their vertex number in the index of this array.
 *
 *          vertex_links                    2D matrix that stores the weights between two vertices, implemented as a
 *                                          long array and uses offsets to use the vertices as a row and column
 *
 *          has_link                        2D matrix implemented as a long array to store whether a connection exists
 *                                          between two vertices, as a matrix of booleans this array takes up much less
 *                                          memory
 *
 *          lastStartVertex                 Since speed2.cpp calls for distances from the same start vertex, we
 *                                          remember the last vertex and if it matches our last vertex, we can continue/
 *                                          use our table to find the weight faster
 *
 *          heapIsValid                     Since an insert into the graph will change relative possible smallest
 *                                          distances, this boolean tracks whether our heap is still valid.
 *
 *          allVerticesInOrder              We use an in order vertex array to have a quick way of populating the
 *                                          vertexVisitHeap and notYetVisitedVerticesLocations Heap
 *
 *          vertexDistanceHeap              A minimum heap that keeps only the vertex which is closest (has the smallest
 *                                          distance away from us) and adjusts by percolation for insertion of new nodes
 *
 *          numberOfElementsInTheHeap       Value that tracks how many elements are currently in the heap
 *
 *          vertexLocations                 An array which tracks where the vertices are in the vertexDistanceHeap
 *
 *          vertexSmallestWeights           An array which tracks the smallest distance to every vertex. The smallest
 *                                          distance to any given vertex is found by accessing the index corresponding
 *                                          to the vertex number.
 *
 *          vertexVisitHeap                 A regular heap which contains all the vertices that have not yet been
 *                                          visited. This speeds up our search for connections to other vertices since
 *                                          we do not check vertices that we have already visited.
 *
 *          verticesLeft                    Tracks how many vertices are left to check and is the number of nodes left
 *                                          in the vertexVisitHeap
 *
 *          notYetVisitedVerticesLocations  An array which tracks where the vertices are in the vertexVisitHeap.
 *
 ****************************************************/
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

    int* allVerticesInOrder;

    int* vertexDistanceHeap;
    int numberOfElementsInTheHeap;
    int* vertexLocations;
    double* vertexSmallestWeights;

    int* vertexVisitHeap;
    int verticesLeft;
    int* notYetVisitedVerticesLocations;

    int degree( int ) const;
    int edge_count() const;
    double adjacent( int, int ) const;

    double distance( int, int );
    void insert( int, int, double );

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

    int const linkLength = numberOfVertices*numberOfVertices;
    vertex_links = new double[linkLength];
    has_link = new bool[linkLength];
    vertex_degrees = new int[numberOfVertices];

    notYetVisitedVerticesLocations = new int[numberOfVertices];
    vertexLocations = new int[numberOfVertices];
    vertexSmallestWeights = new double[numberOfVertices];

    int numberOfVerticesPlusOne = numberOfVertices + 1;
    allVerticesInOrder = new int[numberOfVerticesPlusOne];
    vertexDistanceHeap = new int[numberOfVerticesPlusOne];
    vertexVisitHeap = new int[numberOfVerticesPlusOne];

    for(int i = 0; i <= numberOfVertices; ++i) {
        allVerticesInOrder[i] = i;
    }
    std::memset(&vertex_links[0], 0, linkLength* sizeof(double));
    std::memset(&has_link[0], 0, linkLength* sizeof(bool));
    std::memset(&vertex_degrees[0], 0, numberOfVertices * sizeof(int));
    std::memset(&vertexDistanceHeap[1], 0, numberOfVertices * sizeof(int));
}

Weighted_graph::~Weighted_graph() {
    delete[] allVerticesInOrder;

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

    /* If there is no record of an edge between the two vertices, return infinity */
    if (!weight) {
        return INF;
    }
    return weight;
}

double Weighted_graph::distance( int m, int n) {
    if (n < 0 || m < 0 || m > numberOfVertices - 1 || n > numberOfVertices - 1) {
        throw illegal_argument();
    } else if (m == n) {
        return 0;
    }

    int currentNode;

    // If we have searched from this position before, we can either return an already calculated distance or start of
    // where we left off
    if (lastStartVertex == m) {
        if (!notYetVisitedVerticesLocations[n]) {
            return vertexSmallestWeights[n];
        }

    } else {
        // This sequence resets the table that stores the smallest distances.
        verticesLeft = numberOfVertices;
        numberOfElementsInTheHeap = 0;
        std::memcpy(&vertexVisitHeap[1], &allVerticesInOrder[0], numberOfVertices* sizeof(int));
        std::memcpy(&notYetVisitedVerticesLocations[0], &allVerticesInOrder[1], numberOfVertices * sizeof(int));
        std::memset(&vertexLocations[0], 0, numberOfVertices * sizeof(int));
        std::memset(&vertexSmallestWeights[0], 0, numberOfVertices * sizeof(double));
        if (!heapIsValid) {
            heapIsValid = true;
        }

        // This will get immediately popped off, it is the only case where we allow a distance of 0 in the heap
        ++numberOfElementsInTheHeap;
        vertexSmallestWeights[m] = 0;
        vertexDistanceHeap[1] = m;
        lastStartVertex = m;
    }

    while(numberOfElementsInTheHeap) {

        currentNode = vertexDistanceHeap[1];

        if (currentNode == n) {
            return vertexSmallestWeights[n];
        }

        // Pop off the current vertex from the min heap
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

                if (left_weight < right_weight) {
                    if (pushSize > left_weight) {
                        vertexLocations[left_child] = currentParentVertex;
                        vertexDistanceHeap[currentParentVertex] = left_child;
                        currentParentVertex = first_child;
                    } else {
                        break;
                    }
                } else {
                    if (pushSize > right_weight) {
                        vertexLocations[right_child] = currentParentVertex;
                        vertexDistanceHeap[currentParentVertex] = right_child;
                        currentParentVertex = second_child_index;
                    } else {
                        break;
                    }
                }
            }
            vertexDistanceHeap[currentParentVertex] = closestVertexAtTheBack;
            vertexLocations[closestVertexAtTheBack] = currentParentVertex;
            numberOfElementsInTheHeap--;
        }

        // Pop of the vertex from the visited heap.
        int const currentNodeLocation = notYetVisitedVerticesLocations[currentNode];
        int const lastVertexAtBack = vertexVisitHeap[verticesLeft];
        vertexVisitHeap[currentNodeLocation] = lastVertexAtBack;
        notYetVisitedVerticesLocations[lastVertexAtBack] = currentNodeLocation;
        notYetVisitedVerticesLocations[currentNode] = 0;
        verticesLeft--;

        // Add the distances between adjacent vertices
        int const maxLinksToCurrentNode = vertex_degrees[currentNode];
        int foundLinks = 0;
        double const currentDistanceToGetHere = vertexSmallestWeights[currentNode];
        for(int i = 1; i <= verticesLeft; ++i) {
            int const destinationVertex = vertexVisitHeap[i];
            size_t const destinationVertexArrayOffset = size_t(numberOfVertices) * currentNode + destinationVertex;
            if(has_link[destinationVertexArrayOffset]) {
                ++foundLinks;

                // Insert into the min heap
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

                while (currentPlace != 1) {

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
    return INF;
}

void Weighted_graph::insert( int m, int n, double w ) {
    if (w <= 0 || m == n || m < 0 || n < 0 || m > numberOfVertices - 1 || n > numberOfVertices - 1) {
        throw illegal_argument();
    }

    // If the heap is valid, we set it to not be valid anymore because we inserted something.
    if (heapIsValid) {
        lastStartVertex = -1;
        heapIsValid = false;
    }

    int const firstLocation = m*numberOfVertices + n;
    int const secondLocation = n*numberOfVertices + m;

    // If the vertex is new, we add to the degree and edge count and indicate there is a connection here.
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