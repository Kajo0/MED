#include "util.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <utility>

using namespace std;

namespace med {

double euclideanDistance(const Vector& v1, const Vector& v2, bool ommitFirst) {
	assert(v1.size() == v2.size());

	double dist = 0.0;
	for (int i = ommitFirst ? 1 : 0; i < v1.size(); i++) {
		dist += ((v1[i] - v2[i]) * (v1[i] - v2[i]));
	}

	return sqrt(dist);
}

double cosineSimilarity(const Vector& v1, const Vector& v2, bool ommitFirst) {
	assert(v1.size() == v2.size());

	double ab = 0.0, a2 = 0.0, b2 = 0.0;
	for (int i = ommitFirst ? 1 : 0; i < v1.size(); i++) {
		ab += v1[i] * v2[i];
		a2 += v1[i] * v1[i];
		b2 += v2[i] * v2[i];
	}
	a2 = sqrt(a2);
	b2 = sqrt(b2);

	return ab / (a2 * b2 + 0.000001);
}

double manhattanDistance(const Vector& v1, const Vector& v2, bool ommitFirst) {
	assert(v1.size() == v2.size());

	double dist = 0.0;
	for (int i = ommitFirst ? 1 : 0; i < v1.size(); i++) {
		dist += abs(v1[i] - v2[i]);
	}

	return dist;
}

string vectorToString(const vector<double>& vector) {
	string result = "[";
	for (int i = 0; i < vector.size(); i++) {
		result += to_string(vector[i]);
		if (i != vector.size() - 1)
			result += ",";
	}
	result += "]";
	return result;
}

Cluster readData(const string& filename) {
	ifstream input(filename);
	if (!input.is_open()) {
		throw runtime_error(string("Cannot read data file!"));
	}
	int count = 0, size = 0;
	input >> count;
	input >> size;
	cout << "Count= " << count << " size= " << size << endl;
	Cluster result;
	for (int i = 0; i < count; i++) {
		Vector vector;
		vector.push_back(i);
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
	cout << "Read data completed!" << endl;
	return result;
}

void print4dist(Cluster data, const DistFunc& distFunc,
		const std::string& out4distFile) {
	vector<double> kdistances;
	for (int i = 0; i < data.size(); i++) {
		vector<double> distances;
		for (int j = 0; j < data.size(); j++) {
			if (i == j)
				continue;
			distances.push_back(distFunc(data[i], data[j], true));
		}
		sort(distances.begin(), distances.end());
		kdistances.push_back(distances[3]);
	}
	sort(kdistances.begin(), kdistances.end());
	double min = *(kdistances.begin());
	double max = *(kdistances.end() - 1);

	ofstream outFile(out4distFile);
	for (double dist : kdistances) {
		outFile << dist << std::endl;
	}
}

void printClusters(const map<int, Cluster> clusters) {
	for (auto& cluster : clusters) {
		cout << "clusterId=" << cluster.first << " (size="
				<< cluster.second.size() << ")" << endl;
//		for (auto& point : cluster.second) {
//			cout << "\tvector: " << vectorToString(point) << endl;
//		}
	}
}

void printResultClusters(const map<int, Cluster> clusters,
		const char *resultFile) {
	ostream *out = &cout;
	if (resultFile) {
		ofstream *fout = new ofstream();
		fout->open(resultFile, ios_base::in | ios_base::trunc);
		if (fout->is_open()) {
			out = fout;
		} else {
			cout << "Error while opening '" << resultFile
					<< "' file, so print to stdout" << endl;
		}
	}

	// sort them (id, group)
	vector<pair<int, int>> res;
	for (auto& cl : clusters) {
		for (auto& cluster : cl.second) {
			res.push_back(make_pair(cluster[0], cl.first));
		}
	}
	sort(res.begin(), res.end(),
			[](pair<int, int> p1, pair<int, int> p2) {return p1.first < p2.first;});

	for (auto& r : res) {
		*out << r.second << endl;
	}

	if (resultFile) {
		try {
			dynamic_cast<std::ofstream&>(*out).close();
			delete out;
		} catch (std::exception &) {
		}
	}
}

void print2DimVectorsForRClusters(const std::map<int, Cluster> clusters,
		const char *resultFile) {
	ostream *out = &cout;
	if (resultFile) {
		ofstream *fout = new ofstream();
		fout->open(resultFile, ios_base::in | ios_base::trunc);
		if (fout->is_open()) {
			out = fout;
		} else {
			cout << "Error while opening '" << resultFile
					<< "' file, so print to stdout" << endl;
		}
	}

	for (auto& cl : clusters) {
		// first is id of row data
		for (int i = 1; i < 3; ++i) {
			for (auto& cluster : cl.second) {
				assert(cluster.size() == 3);
				*out << cluster[i] << "\t";
			}
			*out << std::endl;
		}
	}

	if (resultFile) {
		try {
			dynamic_cast<std::ofstream&>(*out).close();
			delete out;
		} catch (std::exception &) {
		}
	}
}

}
