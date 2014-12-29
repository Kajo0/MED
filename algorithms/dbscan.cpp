#include "dbscan.h"
#include <iostream>
#include <cmath>

namespace med {

const int SetPoint::NOISE = 0;
const int SetPoint::UNCLASSIFIED = -1;

DBScan::DBScan(const DistFunc& distanceFunction, const InEpsFunc& isInEps_) :
		distance(distanceFunction), isInEps(isInEps_) {
}

std::map<int, Cluster> DBScan::dbscan(const std::vector<Vector>& points,
		double eps, int minPts) const {
	std::list<SetPoint> setOfPoints;
	for (int i = 0; i < points.size(); i++) {
		SetPoint setPoint;
		setPoint.vector = points[i];
		setPoint.index = i;
		setPoint.clusterId = SetPoint::UNCLASSIFIED;
		setOfPoints.push_back(setPoint);
	}

	dbscan(setOfPoints, eps, minPts);
	std::map<int, Cluster> clusters;
	for (auto setPoint : setOfPoints) {
		clusters[setPoint.clusterId].push_back(setPoint.vector);
	}

	return clusters;
}

int DBScan::dbscan(std::list<SetPoint>& setOfPoints, double eps,
		int minPts) const {
	int clusterId = SetPoint::NOISE + 1;

	for (auto& point : setOfPoints) {
		if (point.clusterId == SetPoint::UNCLASSIFIED) {
			if (expandCluster(setOfPoints, point, clusterId, eps, minPts)) {
				clusterId++;
			}
		}
	}

	return clusterId - SetPoint::NOISE - 2;
}

void remove(std::list<SetPoint*>& seeds, const SetPoint& point) {
	seeds.erase(
			std::remove_if(seeds.begin(), seeds.end(), [&](SetPoint* setPoint) {
				return setPoint->index == point.index;
			}), seeds.end());
}

bool DBScan::expandCluster(std::list<SetPoint>& setOfPoints, SetPoint& point,
		int clusterId, double eps, int minPts) const {
	// std::cout << "seeds=" << seeds.size() << std::endl;
	auto seeds = regionQuery(setOfPoints, point, eps);

	if (seeds.size() < minPts) {
		point.clusterId = SetPoint::NOISE;
		return false;
	} else {
		for (auto seed : seeds) {
			seed->clusterId = clusterId;
		}
		remove(seeds, point);

		while (!seeds.empty()) {
			SetPoint currentP = *(seeds.front());
			auto result = regionQuery(setOfPoints, currentP, eps);
			if (result.size() >= minPts) {
				for (auto resultP : result) {
					if (resultP->clusterId == SetPoint::UNCLASSIFIED
							|| resultP->clusterId == SetPoint::NOISE) {
						if (resultP->clusterId == SetPoint::UNCLASSIFIED) {
							seeds.push_back(resultP);
						}
						resultP->clusterId = clusterId;
					}
				}
			}
			remove(seeds, currentP);
		}
	}
	return true;
}

std::list<SetPoint*> DBScan::regionQuery(const std::list<SetPoint>& setOfPoints,
		const SetPoint& point, double eps) const {
	std::list<SetPoint*> neighbors;
	//std::cout << "eps=" << eps << std::endl;
	for (auto it = setOfPoints.begin(); it != setOfPoints.end(); it++) {
		double dist = distance(point.vector, it->vector, true);
		//std::cout << "dist=" << dist << std::endl;
		if (isInEps(dist, eps)) {
			neighbors.push_back(const_cast<SetPoint*>(&(*it)));
		}
	}

	return neighbors;
}

}
