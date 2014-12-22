#include <iostream>
#include <cassert>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <limits>
#include "kmeans.h"
using namespace std;

namespace med {

Kmeans::Kmeans(const DistFunc distFunc) :
		distFunc_(distFunc), debug_(false) {
}

std::map<int, Cluster> Kmeans::process(const std::vector<Vector>& points,
		int groups, float epsilon, int iterations) const {
	assert(groups > 0);

	vector<KmeansCluster> clusters;
	auto centroids = findRandomCentroids(points, groups);
	for (auto& point : centroids) {
		KmeansCluster cluster;
		cluster.centroid_ = point;
		clusters.push_back(cluster);
	}

	if (debug_) {
		cout << "Step 1: centroids" << endl;
		for (auto &cluster : clusters) {
			cout << vectorToString(cluster.centroid_) << endl;
		}
	}

	assignPoints(clusters, points);

	double error = numeric_limits<double>::max();
	for (int iteration = 0; iteration != iterations; ++iteration) {
		recalculateCentroids(clusters);
		if (!reassignPoints(clusters)) {
			if (debug_) {
				cout << "No reassign in " << iteration << " iteration so stop."
						<< endl;
			}
			break;
		}
		double qError = error;
		error = calculateQuantizationError(clusters);
		if (error < 0) {
			if (debug_) {
				cout << "Worse result, so don't calc new epsilon error."
						<< endl;
			}
		} else {
			qError = (qError - error) / error;
		}

		if (debug_) {
			cout << "qError (epsilon): " << qError << " in " << iteration
					<< " iteration." << endl;
		}

		if (qError < epsilon) {
			break;
		}
	}

	map<int, Cluster> result;
	int id = 1;
	for (auto& cluster : clusters) {
		result[id++] = cluster.cluster_;
	}

	return result;
}

vector<Vector> Kmeans::findRandomCentroids(const vector<Vector>& points,
		int groups) const {
	// TODO optimize it
	srand(unsigned(time(0)));
	vector<int> randomInc;
	for (int i = 0; i < points.size(); ++i) {
		randomInc.push_back(i);
	}

	random_shuffle(randomInc.begin(), randomInc.end());

	vector<Vector> centroids;
	for (int i = 0; i < groups; ++i) {
		centroids.push_back(points[randomInc[i]]);
	}

	return centroids;
}

void Kmeans::assignPoints(vector<KmeansCluster>& clusters,
		const vector<Vector>& points) const {
	for (auto& point : points) {
		double minDistance = numeric_limits<double>::max();
		int clu = -1;
		for (int c = 0; c < clusters.size(); ++c) {
			double distance = distFunc_(clusters[c].centroid_, point);
			if (distance < minDistance) {
				minDistance = distance;
				clu = c;
			}
		}
		assert(clu != -1);
		clusters[clu].cluster_.push_back(point);
	}
}

void Kmeans::recalculateCentroids(std::vector<KmeansCluster>& clusters) const {
	for (auto& cluster : clusters) {
		Vector newCentroid;
		for (auto& point : cluster.cluster_) {
			if (newCentroid.size() == 0) {
				for (auto& p : point) {
					newCentroid.push_back(0);
				}
			}
			for (int i = 0; i < point.size(); ++i) {
				newCentroid[i] += point[i];
			}
		}
		int clusterSize = cluster.cluster_.size();
		if (clusterSize > 0) {
			for (int i = 0; i < cluster.centroid_.size(); ++i) {
				cluster.centroid_[i] = newCentroid[i] / clusterSize;
			}
		}

		if (debug_) {
			cout << "Recalculated centroid: "
					<< vectorToString(cluster.centroid_) << endl;
		}
	}

	if (debug_) {
		cout << endl;
	}
}

bool Kmeans::reassignPoints(vector<KmeansCluster>& clusters) const {
	vector<Vector> points;
	for (auto& cluster : clusters) {
		points.insert(points.end(), cluster.cluster_.begin(),
				cluster.cluster_.end());
		cluster.cluster_.clear();
	}
	assignPoints(clusters, points);
	// TODO think about - different approach
	return true;
}

double Kmeans::calculateQuantizationError(
		const vector<KmeansCluster>& clusters) const {
	int points = 0;
	double result = 0;
	for (auto& cluster : clusters) {
		points += cluster.cluster_.size();
		for (auto& point : cluster.cluster_) {
			result += distFunc_(point, cluster.centroid_);
		}
	}
	result /= points;
	if (debug_) {
		cout << "Quantization error=" << result << endl;
	}

	return result;
}

}
