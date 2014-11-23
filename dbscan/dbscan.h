#include <vector>
#include <map>
#include <list>
#include <algorithm>

typedef std::vector<double> Vector;
typedef std::vector<Vector> Cluster;

struct SetPoint {
	Vector vector;
	int clusterId;
	int index;

	const static int UNCLASSIFIED;
	const static int NOISE;
};

class DBScan {
public:
	// returns map with clusters
	// cluster with key = 0 is noise
	std::map<int, Cluster> dbscan(const std::vector<Vector>& points, double eps, int minPts) const;
private:
	// sets clusterId for every point in set
	// returns number of clusters
	int dbscan(std::list<SetPoint>& setOfPoints, double eps, int minPts) const;

	std::list<SetPoint*> regionQuery(const std::list<SetPoint>& setOfPoints, const SetPoint& point, double eps) const;
	bool expandCluster(std::list<SetPoint>& setOfPoints, SetPoint& point, int clusterId, double eps, int minPts) const;
};

