#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <map>
#include <functional>

namespace med {

typedef std::vector<double> Vector;
typedef std::vector<Vector> Cluster;
typedef std::function<double(const Vector&, const Vector&)> DistFunc;

double euclideanDistance(const Vector& v1, const Vector& v2);
double cosineSimilarity(const Vector& v1, const Vector& v2);
double manhattanDistance(const Vector& v1, const Vector& v2);

std::string vectorToString(const std::vector<double>& vector);
Cluster readData(const std::string& filename);
void print4dist(Cluster data, const DistFunc& distFunc);
void printClusters(const std::map<int, Cluster> clusters);
/**
 * Print pseudo table for 2-dimensional data
 */
void print2DimTableClusters(const std::map<int, Cluster> clusters);

}

#endif // UTIL_H