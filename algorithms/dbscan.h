#ifndef DBSCAN_H
#define DBSCAN_H

#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <functional>
#include "util.h"

namespace med {

typedef std::function<bool(double, double)> InEpsFunc;

// SetPoint struct representing point in cluster
struct SetPoint {
	Vector vector;
	int clusterId;
	int index;

	const static int UNCLASSIFIED;
	const static int NOISE;
};

// DBScan class is used to group points
class DBScan {
public:
	DBScan(const DistFunc& distanceFunction, const InEpsFunc& isInEps);
	// returns map with clusters
	// cluster with key = 0 is noise
	std::map<int, Cluster> dbscan(const std::vector<Vector>& points, double eps,
			int minPts) const;
private:
	DistFunc distance;
	InEpsFunc isInEps;
	// sets clusterId for every point in set
	// returns number of clusters
	int dbscan(std::list<SetPoint>& setOfPoints, double eps, int minPts) const;
	// returns list of points in eps neighboorhood of given point
	std::list<SetPoint*> regionQuery(const std::list<SetPoint>& setOfPoints,
			const SetPoint& point, double eps) const;
	// expands cluster
	bool expandCluster(std::list<SetPoint>& setOfPoints, SetPoint& point,
			int clusterId, double eps, int minPts) const;
};

}

#endif // DBSCAN_H
