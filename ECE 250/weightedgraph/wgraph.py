#!/usr/bin/env python3
"""Weighted Graphs in Python

>>> # Harder test cases
>>> graph = DistanceGraph()
>>> graph.edge_count()
0
>>> graph.insert(0, 1, 3.5)
>>> graph.insert(0, 2, 12.2)
>>> graph.insert(0, 3, 17.5)
>>> graph.insert(1, 2, 0.3)
>>> graph.insert(1, 3, 14.5)
>>> graph.insert(2, 3, 0.5)
>>> graph.edge_count()
6
>>> graph.distance(3,4)
inf
>>> graph.distance(0,3)
4.3

>>> # Personal test cases (from GitHub)
>>> # Single Vertex
>>> graph = DistanceGraph(-1)
>>> graph.distance(0,0)
0
>>> #invalid size
>>> graph = DistanceGraph(-1)
>>> graph.edge_count()
0

>>> # Single Edge
>>> graph = DistanceGraph(2)
>>> graph.insert(0,1,1.3)
>>> float("%.1f" % graph.distance(0,1))
1.3
>>> # Two Edges
>>> graph = DistanceGraph(3)
>>> graph.insert(0,1,2.3)
>>> graph.insert(0,2,2.4)
>>> float("%.1f" % graph.distance(0,1))
2.3
>>> float("%.1f" % graph.distance(0,2))
2.4
>>> # Two connected edges
>>> graph = DistanceGraph(3)
>>> graph.insert(0,1,2.3)
>>> graph.insert(1,2,2.4)
>>> float("%.1f" % graph.distance(0,1))
2.3
>>> float("%.1f" % graph.distance(0,2))
4.7
>>> # Redundant edges
>>> graph = DistanceGraph(3)
>>> graph.insert(0,1,2.3)
>>> graph.insert(0,2,2.4)
>>> graph.insert(1,2,0.2)
>>> float("%.1f" % graph.distance(0,1))
2.3
>>> float("%.1f" % graph.distance(0,2))
2.4

>>> # Modified Random Test Case from GitHub
>>> graph = DistanceGraph(5)
>>> graph.insert(0,1,1.0)
>>> graph.insert(0,3,0.5)
>>> graph.insert(1,2,0.1)
>>> graph.insert(1,3,0.2)
>>> graph.insert(2,4,0.6)
>>> graph.insert(3,1,0.3)
>>> graph.insert(3,2,0.9)
>>> graph.insert(3,4,0.2)
>>> graph.insert(4,0,0.7)
>>> graph.insert(4,2,0.4)
>>> float("%.1f" % graph.distance(1,1))
0.0
>>> float("%.1f" % graph.distance(1,2))
0.1
>>> float("%.1f" % graph.distance(1,3))
0.3
>>> float("%.1f" % graph.distance(1,4))
0.5
>>> float("%.1f" % graph.distance(1,0))
0.8
>>> float("%.1f" % graph.distance(0,1))
0.8
>>> float("%.1f" % graph.distance(3,1))
0.3

"""
# Imports
from collections import defaultdict
import doctest

class DistanceTracker:
    """Table for applying Dijkstra's algorithm"""
    def __init__(self, start, num_nodes):
        self.table = {}
        self.start = start
        # I could have used list comprehensions but why obfuscate?
        for node in range(num_nodes):
            dist = float("inf") if node != start else 0
            self.table[node] = {"dist": dist, "visited": False, "prev": None}

    def have_min_distance(self, node):
        """Determine whether the table can say anything about the shortest distance between nodes"""
        return self.next_node() == node or self.visited(node) or self.next_node() is None

    def get_min_distance(self, node):
        """Find the minimum distance between nodes if the shortest distance is known"""
        if self.have_min_distance(node):
            return self.table[node]["dist"]
        return None

    def get_min_path(self, node):
        """Create a minimum path between nodes if the shortest distance is known"""
        if self.have_min_distance(node):
            path = []
            while node != self.start:
                path.insert(0, node)
                node = self.table[node]["prev"]
            path.insert(0, node)
            return path
        return None

    def to_visit(self):
        """Create a iterable of all the nodes will"""
        return filter(lambda n: not self.visited(n), self.table.keys())

    def visited(self, node):
        """Determine if a node has already been visited"""
        return self.table[node]["visited"]

    def next_node(self):
        """Get next minimum node who will be visited next"""
        to_visit = list(self.to_visit())
        if not to_visit:
            return None
        next_up = min(to_visit, key=lambda n: self.table[n]["dist"])
        connected = self.table[next_up]["dist"] != float("inf")
        return next_up if connected else None

    def visit(self, node, neighbours):
        """Visit a node, and update the shortest distances to all neighbours"""
        if self.visited(node):
            return
        curr_dist = self.table[node]["dist"]
        for neighbour in set(neighbours) & set(self.to_visit()):
            dist = neighbours[neighbour]
            if self.table[neighbour]["dist"] > curr_dist + dist:
                self.table[neighbour]["dist"] = curr_dist + dist
                self.table[neighbour]["prev"] = node
        self.table[node]["visited"] = True

    def visit_next(self, edges):
        """Visit the next minimum node as per the table if possible."""
        next_up = self.next_node()
        if next_up is not None:
            self.visit(next_up, edges[next_up])
            return True
        return False

# Had to hack defaultdict so that the length was actually as expected -___-
class DistMap(defaultdict):
    """Hacking the defaultdict class such that"""
    def __init__(self, default_factory):
        super().__init__(default_factory)
        self.default = default_factory()
    def __contains__(self, node):
        return super().__contains__(node) and node != self.default
    def __iter__(self):
        for node in super().__iter__():
            if node in self:
                yield node
    def __len__(self):
        return len(list(filter(lambda e: e != float("inf"), self.values())))

class DistanceGraph:
    """Graph on which Dijkstra's algorithm is expected to be applied"""
    def __init__(self, size=50):
        self.size = size if size >= 1 else 1
        self.edges = {node: DistMap(lambda: float("inf")) for node in range(self.size)}

    def degree(self, node):
        """Determines the degree of an node (number of neighbours)"""
        if not 0 <= node < self.size:
            raise ValueError("Cannot find degree for a node not in the graph")
        return len(self.edges[node])

    def edge_count(self):
        """Determines the number of edges in agraph."""
        return int(sum(self.degree(node) for node in range(self.size))/2)

    def adjacent(self, first, second):
        """Determines whether or not two nodes are directly connnected"""
        if not((0 <= first < self.size) and  (0 <= second < self.size)):
            raise ValueError("Cannot find adjacency for nodes not in the graph")
        if first == second:
            return 0
        return self.edges[first][second]

    def insert(self, first, second, distance):
        """Insert an edge in the graph. Changes the weight if the edge already exists"""
        # Duplicate data for easy life
        if not((0 <= first < self.size) and  (0 <= second < self.size)):
            raise ValueError("Cannot insert an edge for nodes not in graph")
        if first == second:
            raise ValueError("Cannot insert a loop into the graph")
        if distance <= 0:
            raise ValueError("Cannot add non-positive edge weights")
        self.edges[first][second] = distance
        self.edges[second][first] = distance

    def _perform_dijkstra(self, first, second):
        assert (0 <= first < self.size) and (0 <= second < self.size)
        dist_tracker = DistanceTracker(first, self.size)
        while not dist_tracker.have_min_distance(second):
            if not dist_tracker.visit_next(self.edges):
                break
        return dist_tracker

    def distance(self, first, second):
        """
        Determines the distance between 2 nodes.
        The distance is infinite if they aren't connected
        """
        if not((0 <= first < self.size) and  (0 <= second < self.size)):
            raise ValueError("Cannot find distances for nodes not in the graph")
        if first == second:
            return 0
        dist_tracker = self._perform_dijkstra(first, second)
        return dist_tracker.get_min_distance(second)

    def path(self, first, second):
        """
        Determines the distance between 2 nodes.
        The distance is infinite if they aren't connected
        """
        if not((0 <= first < self.size) and  (0 <= second < self.size)):
            raise ValueError("Cannot find distances for nodes not in the graph")
        if first == second:
            return 0
        dist_tracker = self._perform_dijkstra(first, second)
        first_dist = dist_tracker.get_min_distance(first)
        second_dist = dist_tracker.get_min_distance(second)
        if first_dist == float('inf')  or second_dist == float('inf'):
            return []
        furthest = first if first_dist > second_dist else second
        potential_path = dist_tracker.get_min_path(furthest)
        if first in potential_path and second in potential_path:
            return potential_path
        return []

    def duplicate(self):
        """Duplicates a graph data structure"""
        graph = DistanceGraph(self.size)
        for node in self.edges:
            for edge in self.edges[node]:
                graph.edges[node][edge] = self.edges[node][edge]
        return graph


if __name__ == "__main__":
    doctest.testmod()
