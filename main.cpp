#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <iomanip>

#include "dbscan/dbscan.h"

std::string vectorToString(const std::vector<double>& vector) {
	std::string result = "[";
	for(int i=0; i<vector.size(); i++) {
		result += std::to_string(vector[i]);
		if (i != vector.size()-1)
			result += ",";
	}
	result += "]";
	return result;
}

Cluster readData(const std::string& filename) {
	std::ifstream input(filename);
	if (! input.is_open()) {
		throw std::runtime_error(std::string("Cannot read data file!"));
	}
	int count = 0, size = 0;
	input >> count;
	input >> size;
	std::cout << "Count= " << count << " size= " << size << std::endl;
	Cluster result;
	for(int i=0; i< count; i++) {
		Vector vector;
		for(int j=0; j< size; j++) {
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
	for(int i=0; i< data.size() ;i++) {
		std::vector<double> distances;
		for(int j=0; j< data.size(); j++) {
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
	
	for(double dist : kdistances) {
		int size = 120 * (1 - (max - dist) / (max - min));
		std::cout << std::setprecision(8) << dist << "\t";
		for(int i=0; i< size; i++)
			std::cout << "=";
		std::cout << std::endl;
	}
}

/*
Usage:
argv[1] - data file
argv[2] - distance '-e' '-c' '-m'
argv[3] - minPts
argv[4] - eps

*/
int main(int argc, char* argv[]) {
	/*
	Vector p1 = {1,1}, p2 ={0,0}, p3={0,1}, p4={1,0};
	Vector r1 = {11,11}, r2 ={10,10}, r3={10,11}, r4={11,10};
	Vector s1 = {1,11}, s2 ={0,10}, s3={1,11}, s4={1,10};	
	Vector n1 = {5,5}, n2 = {-5, -5};
	Vector a1 = {0, -10}, a2= {1, -10}, a3={0, -11}, a4={1, -11};
	Vector b1 = {10, 0}, b2 = {10, 1}, b3 = {11, 0}, b4 = {11,1};

	Cluster points = {p1, p2, p3, p4, r1, r2, r3, r4,
		s1, s2, s3, s4, n1, n2, a1, a2, a3, a4, b1, b2, b3, b4};
	*/
	Cluster points = readData(argv[1]);
	DistFunc distFunc;
	InEpsFunc inEpsFunc;
	if (strcmp(argv[2], "-e") == 0) {
		distFunc = euclideanDistance;
		inEpsFunc = [](double dist, double eps) { return dist < eps;};
	} else if (strcmp(argv[2], "-c") == 0) {
		distFunc = cosineSimilarity;
		inEpsFunc = [](double dist, double eps) { return 1.0-dist < eps;};
	} else {
		distFunc = manhattanDistance;
		inEpsFunc = [](double dist, double eps) { return dist < eps;};
	}
	
	if (argc == 3) {
		print4dist(points, distFunc);
	}

	if (argc == 5) {
		int minPts = atoi(argv[3]);
		int eps = atof(argv[4]);
		DBScan scan(distFunc, inEpsFunc);

		auto result = scan.dbscan(points, eps, minPts);
		for(auto& cluster : result) {
			std::cout << "clusterId=" << cluster.first << std::endl;
			for(auto& point : cluster.second) {
				std::cout<< "vector:" << vectorToString(point) << std::endl;
			}
		}
		std::cout << "FINISHED" << std::endl;
	}
	return 0;

}
