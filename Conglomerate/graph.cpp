#include <algorithm>
#include <iostream>

#include "graph.h"


Graph::Graph() {

}

Graph::Graph(Labels labels, Distances distances) {
    this->verticesNumber = labels.size();

    // Create adjacency matriz
    this->distances = distances;

    // Create label index
    for (uint i = 0; i < verticesNumber; i++) {
        labelIndex[i] = labels[i];
    }
}

Graph::~Graph() {

}


//** PUBLIC


std::string Graph::simpleEdgeNewickTree() {
    return newickTree(this->distances, this->labelIndex, [](Distance a, Distance b) -> Distance {
        return std::min(a, b);
    });
}

std::string Graph::completeEdgeNewickTree() {
    return newickTree(this->distances, this->labelIndex, [](Distance a, Distance b) -> Distance {
        return std::max(a, b);
    });
}

std::string Graph::averageEdgeNewickTree() {
    return newickTree(this->distances, this->labelIndex, [](Distance a, Distance b) -> Distance {
        return (a + b) / 2;
    });
}


//** PRIVATE


std::string Graph::newickTree(Distances distances, Index labelIndex, uint verticesNumber, CompareFunc compareFunc) {
    while(verticesNumber > 1) {
        auto minimumEdge = edgeWithMinimumValue(distances, verticesNumber);
        merge(&distances, &labelIndex, minimumEdge, &verticesNumber, compareFunc);
    }
    return "(" + labelIndex[0] + ")";
}

void Graph::merge(Distances *distances, Index *labelIndex, Edge edge, uint *verticesNumber, CompareFunc compareFunc) {
}

Edge Graph::coordinateToEdge(uint y, uint x) {
    return std::make_tuple(0, 1, coordinateToDistance(0, 1));
}

Edge Graph::minEdge(Edge a, Edge b) {
    // Compare just the value of the edge
    return (std::get<2>(a) < std::get<2>(b)) ? a : b;
}

Edge Graph::edgeWithMinimumValue(Distances distances, uint verticesNumber) {
    Edge minimumEdge = coordinateToEdge(0, 1);
    for (uint i = 0; i < verticesNumber - 1; i++) {
        for(uint j = i + 1; j < verticesNumber; j++) {
            minimumEdge = minEdge(minimumEdge, coordinateToEdge(i, j));
        }
    }

    return minimumEdge;
}

Distance Graph::coordinateToDistance(uint y, uint x) {
    return this->distances[y * this->verticesNumber + x];
}

std::string Graph::coordinateToFormatedDistance(uint y, uint x) {
    return formatDistance(coordinateToDistance(y, x));
}

std::string Graph::formatDistance(Distance distance) {
    // Positive integer numbers with format /^[0-9]{3}$/
    return ((distance < 10) ? "00" : (distance < 100) ? "0" : "") + std::to_string(distance);
}

std::string Graph::distanceToString(Distance distance) {
    std::ostringstream stringStream;
    stringStream << distance;

    return stringStream.str();
}


//** DEBUG


void Graph::printLabelIndex() {
    for (auto &label : labelIndex) {
        std::cout << label.first <<": " << label.second << std::endl;
    }
}

void Graph::printMatrix() {
    for(uint i = 0; i < this->verticesNumber; i++) {
        std::cout << "| ";

        for(uint j = 0; j < this->verticesNumber; i++) {
            std::string formatedDistance = coordinateToFormatedDistance(i, j);
            std::cout << formatedDistance << " | ";
        }
        std::cout << std::endl;
    }
}
