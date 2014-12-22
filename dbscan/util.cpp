#include "util.h"

#include <iostream>
#include <cassert>
#include <string>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <iomanip>

namespace med {

double euclideanDistance(const Vector& v1, const Vector& v2) {
	assert(v1.size() == v2.size());

	double dist = 0.0;
	for (int i = 0; i < v1.size(); i++) {
		dist += ((v1[i] - v2[i]) * (v1[i] - v2[i]));
	}

	return sqrt(dist);
}

double cosineSimilarity(const Vector& v1, const Vector& v2) {
	assert(v1.size() == v2.size());

	double ab = 0.0, a2 = 0.0, b2 = 0.0;
	for (int i = 0; i < v1.size(); i++) {
		ab += v1[i] * v2[i];
		a2 += v1[i] * v1[i];
		b2 += v2[i] * v2[i];
	}
	a2 = sqrt(a2);
	b2 = sqrt(b2);

	return ab / (a2 * b2 + 0.000001);
}

double manhattanDistance(const Vector& v1, const Vector& v2) {
	assert(v1.size() == v2.size());

	double dist = 0.0;
	for (int i = 0; i < v1.size(); i++) {
		dist += abs(v1[i] - v2[i]);
	}

	return dist;
}

std::string vectorToString(const std::vector<double>& vector) {
	std::string result = "[";
	for (int i = 0; i < vector.size(); i++) {
		result += std::to_string(vector[i]);
		if (i != vector.size() - 1)
			result += ",";
	}
	result += "]";
	return result;
}

Cluster readData(const std::string& filename) {
	std::ifstream input(filename);
	if (!input.is_open()) {
		throw std::runtime_error(std::string("Cannot read data file!"));
	}
	int count = 0, size = 0;
	input >> count;
	input >> size;
	std::cout << "Count= " << count << " size= " << size << std::endl;
	Cluster result;
	for (int i = 0; i < count; i++) {
		Vector vector;
		for (int j = 0; j < size; j++) {
			double x;
			input >> x;
			//if (j != 0) {
			// first attribute is class
			vector.push_back(x);
			//}
		}
		result.push_back(vector);
	}
	std::cout << "Read data completed!" << std::endl;
	return result;
}

void print4dist(Cluster data, const DistFunc& distFunc) {
	std::vector<double> kdistances;
	for (int i = 0; i < data.size(); i++) {
		std::vector<double> distances;
		for (int j = 0; j < data.size(); j++) {
			if (i == j)
				continue;
			distances.push_back(distFunc(data[i], data[j]));
		}
		std::sort(distances.begin(), distances.end());
		kdistances.push_back(distances[3]);
	}
	std::sort(kdistances.begin(), kdistances.end());
	double min = *(kdistances.begin());
	double max = *(kdistances.end() - 1);

	for (double dist : kdistances) {
		int size = 120 * (1 - (max - dist) / (max - min));
		std::cout << std::setprecision(8) << dist << "\t";
		for (int i = 0; i < size; i++)
			std::cout << "=";
		std::cout << std::endl;
	}
}

}
