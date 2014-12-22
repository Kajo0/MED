#include <iostream>
#include <algorithm>
#include <iomanip>

#include "dbscan/dbscan.h"
#include "dbscan/kmeans.h"
#include "dbscan/util.h"
using namespace std;
using namespace med;

/*
 Usage:
 argv[1] - data file
 argv[2] - distance '-e' '-c' '-m'
 argv[3] - minPts
 argv[4] - eps

 */
int main(int argc, char* argv[]) {

	Vector p1 = { 1, 1 }, p2 = { 0, 0 }, p3 = { 0, 1 }, p4 = { 1, 0 };
	Vector r1 = { 11, 11 }, r2 = { 10, 10 }, r3 = { 10, 11 }, r4 = { 11, 10 };
	Vector s1 = { 1, 11 }, s2 = { 0, 10 }, s3 = { 1, 11 }, s4 = { 1, 10 };
	Vector n1 = { 5, 5 }, n2 = { -5, -5 };
	Vector a1 = { 0, -10 }, a2 = { 1, -10 }, a3 = { 0, -11 }, a4 = { 1, -11 };
	Vector b1 = { 10, 0 }, b2 = { 10, 1 }, b3 = { 11, 0 }, b4 = { 11, 1 };

	Cluster points = { p1, p2, p3, p4, r1, r2, r3, r4, s1, s2, s3, s4, n1, n2,
			a1, a2, a3, a4, b1, b2, b3, b4 };

//	Cluster points = readData(argv[1]);
	DistFunc distFunc = euclideanDistance;
	InEpsFunc inEpsFunc = [](double dist, double eps) {return dist < eps;};

	cout << "\nstart\n";

	Kmeans kmeans(distFunc);
	auto result = kmeans.process(points, 7, 0.1f, 100);
	for (auto& cluster : result) {
		std::cout << "clusterId=" << cluster.first << endl;
		for (auto& point : cluster.second) {
			cout << "vector:" << vectorToString(point) << endl;
		}
	}

	cout << "\nend\n";

//	if (strcmp(argv[2], "-e") == 0) {
//		distFunc = euclideanDistance;
//		inEpsFunc = [](double dist, double eps) {return dist < eps;};
//	} else if (strcmp(argv[2], "-c") == 0) {
//		distFunc = cosineSimilarity;
//		inEpsFunc = [](double dist, double eps) {return 1.0-dist < eps;};
//	} else {
//		distFunc = manhattanDistance;
//		inEpsFunc = [](double dist, double eps) {return dist < eps;};
//	}
//
//	if (argc == 3) {
//		print4dist(points, distFunc);
//	}
//
//	if (argc == 5) {
//		int minPts = atoi(argv[3]);
//		int eps = atof(argv[4]);
//		DBScan scan(distFunc, inEpsFunc);
//
//		auto result = scan.dbscan(points, eps, minPts);
//		for (auto& cluster : result) {
//			std::cout << "clusterId=" << cluster.first << std::endl;
//			for (auto& point : cluster.second) {
//				std::cout << "vector:" << vectorToString(point) << std::endl;
//			}
//		}
//		std::cout << "FINISHED" << std::endl;
//	}
	return 0;

}
