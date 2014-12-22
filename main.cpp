#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <getopt.h>
#include "algorithms/dbscan.h"
#include "algorithms/kmeans.h"
#include "algorithms/util.h"
using namespace std;
using namespace med;

void usage() {
	cout << "-d, --dbscan\tDBSCAN algorithm" << endl;
	cout << "-k, --kmeans\tKmeans algorithm" << endl;
	cout << "-e, --euclidean\tEuclidean distance" << endl;
	cout << "-c, --cosine\tCosine distance" << endl;
	cout << "-m, --manhattan\tManhattan distance" << endl;
	cout << "-f, --file\tPath to file withh data" << endl;
	cout << "-p, --eps\tEps parameter (0; ~)" << endl;
	cout << "-s, --minPts\tMinPts parameter [0; ~)" << endl;
	cout << "-g, --groups\tGrups amount (0; ~)" << endl;
	cout << "-l, --epsilon\tEpsilon parameter [0; ~)" << endl;
	cout << "-i, --iterations\tMax iterations [1; ~)" << endl;
	cout << "-n, --print4dist\tPrint 4dist" << endl;
	cout << "-u, --debug\tDebug on" << endl;
	cout << "-t, --test\tRun with test data" << endl;
	cout << endl << "expamples:" << endl;
	cout << "\t./app -ukte -g 5 -i 100" << endl;
	cout << "\t./app -dte -s 2 -p 2" << endl;
	cout << "\t./app -den -f wine.data" << endl;
	cout << "\t./app -de -s 5 -p 48 -f wine.data" << endl;
}

vector<Vector> testData() {
	Vector p1 = { 1, 1 }, p2 = { 0, 0 }, p3 = { 0, 1 }, p4 = { 1, 0 };
	Vector r1 = { 11, 11 }, r2 = { 10, 10 }, r3 = { 10, 11 }, r4 = { 11, 10 };
	Vector s1 = { 1, 11 }, s2 = { 0, 10 }, s3 = { 0, 11 }, s4 = { 1, 10 };
	Vector n1 = { 5, 5 }, n2 = { -5, -5 };
	Vector a1 = { 0, -10 }, a2 = { 1, -10 }, a3 = { 0, -11 }, a4 = { 1, -11 };
	Vector b1 = { 10, 0 }, b2 = { 10, 1 }, b3 = { 11, 0 }, b4 = { 11, 1 };

	vector<Vector> points = { p1, p2, p3, p4, r1, r2, r3, r4, s1, s2, s3, s4,
			n1, n2, a1, a2, a3, a4, b1, b2, b3, b4 };

	return points;
}

int main(int argc, char* argv[]) {
	char *file;
	int minPts = 0;
	double eps = 0;
	double epsilon = 0;
	int iterations = -1;
	int groups = 0;
	bool print4distFlag = false;
	bool debug = false;
	bool test = false;
	DistFunc distanceFunction;
	InEpsFunc inEpsFunction;
	vector<Vector> points;

	enum Algorithm {
		DBSCAN, KMEANS
	};
	Algorithm algorithm;

	int c;

	while (1) {
		static struct option long_options[] = {
				{ "dbscan", no_argument, 0, 'd' },
				{ "kmeans", no_argument, 0, 'k' },
				{ "print4dist", no_argument, 0, 'n' },
				{ "debug", no_argument, 0, 'u' },
				{ "test", no_argument, 0, 't' },
				{ "euclidean", no_argument, 0, 'e' },
				{ "cosine", no_argument, 0, 'c' },
				{ "manhattan", no_argument, 0, 'm' },
				{ "minPts", required_argument, 0, 's' },
				{ "eps", required_argument, 0, 'p' },
				{ "epsilon", required_argument, 0, 'l' },
				{ "groups", required_argument, 0, 'g' },
				{ "iterations", required_argument, 0, 'i' },
				{ "file", required_argument, 0, 'f' },
				{ 0, 0, 0, 0 } };
		int option_index = 0;
		c = getopt_long(argc, argv, "dknutecms:p:l:g:i:f:", long_options,
				&option_index);

		if (c == -1) {
			break;
		}

		switch (c) {
		case 'd':
			algorithm = DBSCAN;
			cout << "DBSCAN algorithm" << endl;
			break;
		case 'k':
			algorithm = KMEANS;
			cout << "K-means algorithm" << endl;
			break;
		case 'e':
			distanceFunction = euclideanDistance;
			inEpsFunction = [](double dist, double eps) {return dist < eps;};
			cout << "Euclidean distance" << endl;
			break;
		case 'c':
			distanceFunction = cosineSimilarity;
			inEpsFunction =
					[](double dist, double eps) {return 1.0-dist < eps;};
			cout << "Cosine metrics" << endl;
			break;
		case 'm':
			distanceFunction = manhattanDistance;
			inEpsFunction = [](double dist, double eps) {return dist < eps;};
			cout << "Manhattan distance" << endl;
			break;
		case 'n':
			print4distFlag = true;
			cout << "Print 4dist 'chart'" << endl;
			break;
		case 'u':
			debug = true;
			cout << "Debug=" << debug << endl;
			break;
		case 't':
			test = true;
			cout << "TestData=" << test << endl;
			break;
		case 's':
			minPts = atoi(optarg);
			cout << "MinPts=" << minPts << endl;
			break;
		case 'p':
			eps = atof(optarg);
			cout << "Eps=" << eps << endl;
			break;
		case 'g':
			groups = atoi(optarg);
			cout << "Groups=" << groups << endl;
			break;
		case 'l':
			epsilon = atof(optarg);
			cout << "Epsilon=" << epsilon << endl;
			break;
		case 'i':
			iterations = atoi(optarg);
			cout << "Iterations=" << iterations << endl;
			break;
		case 'f':
			file = optarg;
			cout << "Data file='" << file << "'" << endl;
			break;

		default:
			usage();
			abort();
		}
	}

	if (test) {
		points = testData();
	} else {
		ifstream input(file);
		if (input) {
			input.close();
			points = readData(file);
		} else {
			input.close();
			cout << "\tBad path to file!" << endl;
			usage();
			return 1;
		}
	}

	switch (algorithm) {
	case DBSCAN: {
		if (print4distFlag) {
			if (!distanceFunction) {
				cout << "\tNo distance function!" << endl;
				usage();
				return 1;
			}

			print4dist(points, distanceFunction);
		} else {
			bool cancel = false;
			if (minPts < 1) {
				cout << "\tNo minPts param!" << endl;
				cancel = true;
			}
			if (eps <= 0) {
				cout << "\tNo eps param!" << endl;
				cancel = true;
			}
			if (!distanceFunction) {
				cout << "\tNo distance function!" << endl;
				cancel = true;
			}
			if (cancel) {
				usage();
				return 1;
			}

			DBScan scan(distanceFunction, inEpsFunction);
			auto result = scan.dbscan(points, eps, minPts);
			printClusters(result);
		}
	}
		break;
	case KMEANS: {
		bool cancel = false;
		if (groups < 1) {
			cout << "\tBad groups param" << endl;
			cancel = true;
		}
		if (!distanceFunction) {
			cout << "\tNo distance function!" << endl;
			cancel = true;
		}
		if (cancel) {
			usage();
			return 1;
		}

		Kmeans kmeans(distanceFunction);
		kmeans.debug(debug);
		auto result = kmeans.process(points, groups, epsilon, iterations);

		cout << endl;
		printClusters(result);

		if (debug) {
			cout << endl;
			print2DimTableClusters(result);
			cout << endl;
		}
	}
		break;

	default:
		cout << "\tNo algorithm chosen!" << endl;
		usage();
		return 1;
	}

	cout << "FINISHED" << endl;

	return 0;
}
