#include "dbscan.h"

#include <iostream>
#include <cassert>
#include <cmath>

double euclideanDistance(const Vector& v1, const Vector& v2) {
	assert(v1.size() == v2.size());
	
	double dist = 0.0;
	for(int i=0; i<v1.size(); i++) {
		dist += ((v1[i] - v2[i]) * (v1[i] - v2[i]));
	}
	
	return sqrt(dist);
} 

DBScan::DBScan(int minPts_, double eps_, const Cluster& points_):
	minPts(minPts_), eps(eps_), clusterId(1) {
	for(unsigned i=0; i< points_.size(); i++) {
		PointData data;
		data.vector = points_[i];
		data.visited = false;
		data.clusterId = UNCLASSIFIED;
		points.push_back(data);
	}
}

std::vector<PointData> DBScan::getClusters() {
	for(auto& point : points) {
		if(point.visited) {
			continue;
		}
		point.visited = true;
		auto neighbors = regionQuery(point);
		if(neighbors.size() < minPts) {
			point.clusterId = NOISE;
		} else {
			expandCluster(point, neighbors);
			clusterId++;
		}
	
	}

	return points;
}

bool DBScan::expandCluster(PointData& point, std::list<PointData*>& seeds) {
	// std::cout << "seeds=" << seeds.size() << std::endl;
	point.clusterId = clusterId;
	for(auto& seed : seeds) {
		if(! seed->visited) {
			seed->visited = true;
			auto npts = regionQuery(*seed);
			if(npts.size() >= minPts) {
				seeds.insert(seeds.end(), npts.begin(), npts.end());
			}
		}
		if( seed->clusterId == UNCLASSIFIED) {
			seed->clusterId = clusterId;
		}
	}		
}

std::list<PointData*> DBScan::regionQuery(const PointData& vector) {
	std::list<PointData*> neighbors;
	//std::cout << "eps=" << eps << std::endl;
	for(auto it = points.begin(); it != points.end(); it++) {
		double dist = euclideanDistance(vector.vector, it->vector);
		//std::cout << "dist=" << dist << std::endl;
		if (dist < eps) {
			neighbors.push_back(&(*it));
		}	
	}

	return neighbors;
}
