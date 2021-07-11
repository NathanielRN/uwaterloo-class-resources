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
 *    -
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
#include "Exception.h"

typedef struct edge {
    int fromVertex;
    int toVertex;
    double weight;
    struct edge* next;
} edges_t;

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
    edges_t* vertex_links;

    int currentNode;
    int* visitedVertices;
    double* smallestDisToVertex;

    int degree( int ) const;
    int edge_count() const;
    double adjacent( int, int ) const;
    double distance( int, int );

    void insert( int, int, double );
    void destroy();

    // Friends

    friend std::ostream &operator<<( std::ostream &, Weighted_graph const & );
};

const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

// Your implementation here

Weighted_graph::Weighted_graph(int n):
numberOfVertices(n),
edges_count(0),
vertex_degrees(new int[n]),
vertex_links(new edges_t[n]),
visitedVertices(new int[n]),
smallestDisToVertex(new double[n]),
currentNode(0)
{
    for(int i = 0; i < numberOfVertices; i++) {
        vertex_degrees[i] = 0;
        vertex_links[i] = edges_t();
    }
}

Weighted_graph::~Weighted_graph() {
    for(int i = 0; i < numberOfVertices; i++) {
        edges_t* removeThisEdge = vertex_links[i].next;
        edges_t* followBehind;
        for(int k = 1; k < vertex_degrees[i]; k++) {
            followBehind = removeThisEdge;
            removeThisEdge = removeThisEdge->next;
            delete followBehind;
        }
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
    if (m == n) {
        return 0;
    } else if (m < 0 || n < 0 || m > numberOfVertices - 1 || n > numberOfVertices - 1) {
        throw illegal_argument();
    }

    int v = std::min(m, n);
    int x = (v == m) ? n : m;
    edges_t* current_edge = &vertex_links[v];
    for(int i = 0; i < vertex_degrees[v]; i++) {
        if (current_edge->toVertex == x) {
            return current_edge->weight;
        }
        current_edge = current_edge->next;
    }
    return INF;
}

double Weighted_graph::distance( int m, int n) {
    if (m < 0 || n < 0 || m > numberOfVertices - 1 || n > numberOfVertices - 1) {
        throw illegal_argument();
    }

    for(int j = 0; j < numberOfVertices; j++) {
        visitedVertices[j] = 0;
        smallestDisToVertex[j] = INF;
    }
    currentNode = m;
    smallestDisToVertex[m] = 0;
    while(true) {

        edges_t* currentEdge = &vertex_links[currentNode];
        double distanceToGetHere = smallestDisToVertex[currentNode];
        for(int i = 0; i < vertex_degrees[currentNode]; i++) {
            if (distanceToGetHere + currentEdge->weight < smallestDisToVertex[currentEdge->toVertex]) {
                smallestDisToVertex[currentEdge->toVertex] = distanceToGetHere + currentEdge->weight;
            }
            currentEdge = currentEdge->next;
        }
        visitedVertices[currentNode] = 1;

        double smallestNext = INF;
        for(int j = 0; j < numberOfVertices; j++) {
            if (!visitedVertices[j] && smallestDisToVertex[j] < smallestNext) {
                currentNode = j;
                smallestNext = smallestDisToVertex[j];
            }
        }
        if (currentNode == n) {
            return smallestDisToVertex[currentNode];
        } else if (smallestNext == INF) {
            return INF;
        }
    }
}

void Weighted_graph::insert( int m, int n, double w ) {
    if (m == n || m < 0 || n < 0 || m > numberOfVertices - 1 || n > numberOfVertices - 1) {
        throw illegal_argument();
    }

    bool replaceEdge = false;
    edges_t* lastEdge = &vertex_links[m];
    if (vertex_degrees[m] == 0) {
        vertex_links[m].fromVertex = m;
        vertex_links[m].toVertex = n;
        vertex_links[m].weight = w;
        vertex_degrees[m]++;
    } else {
        if (lastEdge->toVertex == n) {
            replaceEdge = true;
        }
        for (int i = 1; i < vertex_degrees[m]; i++) {
            if (lastEdge->toVertex == n) {
                replaceEdge = true;
                break;
            }
            lastEdge = lastEdge->next;
        }
        if (replaceEdge) {
            lastEdge->weight = w;
        } else {
            lastEdge->next = new edges_t({m, n, w});
            vertex_degrees[m]++;
        }
    }

    replaceEdge = false;
    if (vertex_degrees[n] == 0) {
        vertex_links[n].fromVertex = n;
        vertex_links[n].toVertex = m;
        vertex_links[n].weight = w;
        vertex_degrees[n]++;
    } else {
        lastEdge = &vertex_links[n];
        if (lastEdge->toVertex == m) {
            replaceEdge = true;
        }
        for (int i = 1; i < vertex_degrees[n]; i++) {
            if (lastEdge->toVertex == m) {
                replaceEdge = true;
                break;
            }
            lastEdge = lastEdge->next;
        }
        if (replaceEdge) {
            lastEdge->weight = w;
            return;
        } else {
            lastEdge->next = new edges_t({n, m, w});
            vertex_degrees[n]++;
        }
    }

    edges_count++;
}

void Weighted_graph::destroy() {
    edges_count = 0;
    for(int i = 0; i < numberOfVertices; i++) {
        vertex_links[i] = edges_t();
        vertex_degrees[i] = 0;
    }
}

// You can modify this function however you want:  it will not be tested

std::ostream &operator<<( std::ostream &out, Weighted_graph const &graph ) {
    return out;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif