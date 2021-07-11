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

#ifndef WEIGHTED_GRAPH_H_DOUBLE
#define WEIGHTED_GRAPH_H_DOUBLE

#ifndef nullptr
#define nullptr 0
#endif

#include <iostream>
#include <limits>
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

    int numberOfVertices;
    int edges_count;
    int* vertex_degrees;
    double** vertex_links;

    int currentNode;
    int* visitedVertices;
    double* smallestDisToVertex;
    int minIndex;
    int maxIndex;

    int degree( int ) const;
    int edge_count() const;
    double adjacent( int, int ) const;
    double distance( int, int );

    void insert( int, int, double );
//    void destroy();

    // Friends

    friend std::ostream &operator<<( std::ostream &, Weighted_graph const & );
};

const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

// Your implementation here

Weighted_graph::Weighted_graph(int n):
        numberOfVertices(n),
        edges_count(0),
        currentNode(0),
        minIndex(0),
        maxIndex(0)
{
    if (n <= 0) {
        numberOfVertices = 1;
    }

    vertex_degrees = new int[numberOfVertices];
    vertex_links = new double*[numberOfVertices];
    visitedVertices = new int[numberOfVertices];
    smallestDisToVertex = new double[numberOfVertices];
    for(int i = 0; i < numberOfVertices; i++) {
        vertex_degrees[i] = 0;
        vertex_links[i] = new double[numberOfVertices - i - 1];
        for(int j = 0; j < numberOfVertices - i - 1; j++) {
            vertex_links[i][j] = INF;
        }
    }
}

Weighted_graph::~Weighted_graph() {
    for(int i = 0; i < numberOfVertices - 1; i++) {
        delete[] vertex_links[i];
    }
    delete[] vertex_links;
    delete[] vertex_degrees;
    delete[] smallestDisToVertex;
    delete[] visitedVertices;
}

int Weighted_graph::degree( int i ) const {
    return vertex_degrees[i];
}

int Weighted_graph::edge_count() const {
    return edges_count;
}

double Weighted_graph::adjacent( int m, int n ) const {
    int v = std::min(m, n);
    int x = (v == m) ? n : m;

    if (v < 0 || x > numberOfVertices - 1) {
        throw illegal_argument();
    } else if (m == n) {
        return 0;
    }

    double weight = vertex_links[v][x-v-1];
    if (weight < INF) {
        return weight;
    }
    return INF;
}

double Weighted_graph::distance( int m, int n) {
    currentNode = std::min(m, n);
    int targetNode = (currentNode == m) ? n : m;
    if (currentNode < 0 || targetNode > numberOfVertices - 1) {
        throw illegal_argument();
    }
    if (m == n) {
        return 0;
    }

    for(int j = 0; j < numberOfVertices; j++) {
        visitedVertices[j] = 0;
        smallestDisToVertex[j] = INF;
    }
    smallestDisToVertex[currentNode] = 0;
    while(true) {

        double distanceToGetHere = smallestDisToVertex[currentNode];
        int numberOfDegreesForCurrentNode = vertex_degrees[currentNode];
        int foundLinks = 0;
        for(int i = 0; i < numberOfVertices - 1 - currentNode; i++) {
            if (vertex_links[currentNode][i] != INF) {
                foundLinks++;
                double currentDisToVertex = distanceToGetHere + vertex_links[currentNode][i];
                if (currentDisToVertex < smallestDisToVertex[currentNode+i+1]) {
                    smallestDisToVertex[currentNode+i+1] = currentDisToVertex;
                }
            }
            if (foundLinks == numberOfDegreesForCurrentNode) {
                break;
            }
        }
        for(int j = 0; j < currentNode; j++) {
            if (foundLinks == numberOfDegreesForCurrentNode) {
                break;
            }
            if (vertex_links[j][currentNode-j-1] != INF) {
                foundLinks++;
                double currentDisToVertex = distanceToGetHere + vertex_links[j][currentNode-j-1];
                if (currentDisToVertex < smallestDisToVertex[j]) {
                    smallestDisToVertex[j] = currentDisToVertex;
                }
            }
        }
        visitedVertices[currentNode] = 1;

        double smallestNext = INF;
        for(int j = 0; j < numberOfVertices; j++) {
            if (!visitedVertices[j] && smallestDisToVertex[j] < smallestNext) {
                currentNode = j;
                smallestNext = smallestDisToVertex[j];
            }
        }
        if (currentNode == targetNode) {
            return smallestDisToVertex[currentNode];
        } else if (smallestNext == INF) {
            return INF;
        }
    }
}

void Weighted_graph::insert( int m, int n, double w ) {
    minIndex = std::min(m, n);
    maxIndex = (minIndex == m) ? n : m;

    if (w <= 0 || m == n || minIndex < 0 || maxIndex > numberOfVertices - 1) {
        throw illegal_argument();
    }

    if (vertex_links[minIndex][maxIndex-minIndex-1] == INF) {
        edges_count++;
        vertex_degrees[minIndex]++;
        vertex_degrees[maxIndex]++;
    }

    vertex_links[minIndex][maxIndex-minIndex-1] = w;
}

//void Weighted_graph::destroy() {
//    edges_count = 0;
//    for(int i = 0; i < numberOfVertices; i++) {
//        vertex_links[i] = new double[numberOfVertices - 1 - i];
//        vertex_degrees[i] = 0;
//    }
//}

// You can modify this function however you want:  it will not be tested

std::ostream &operator<<( std::ostream &out, Weighted_graph const &graph ) {
    return out;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif