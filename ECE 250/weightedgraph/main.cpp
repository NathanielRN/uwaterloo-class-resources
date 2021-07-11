#include <iostream>
#include "Weighted_graph.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Weighted_graph* graph = new Weighted_graph(3);
    graph->insert(0, 1, 1);
    std::cout << graph->vertex_links[0].weight << std::endl;
    std::cout << graph->vertex_degrees[0] << std::endl;
    std::cout << graph->vertex_degrees[1] << std::endl;
    std::cout << "Distance between 0 and 1: " << graph->adjacent(0, 1) << std::endl;
    std::cout << "Distance between 0 and 2: " << graph->adjacent(0, 2) << std::endl;
    try {
        std::cout << "Distance between 5 and 1: " << graph->adjacent(0, 8) << std::endl;
    } catch (const illegal_argument&) {
        std::cout << "Caught an 'illegal argument' error";
    }
    return 0;
}