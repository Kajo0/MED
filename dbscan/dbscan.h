#include <vector>
#include <list>
#include <algorithm>

typedef std::vector<double> Vector;
typedef std::vector<Vector> Cluster;


struct PointData {
	Vector vector;
	int clusterId;
	bool visited;
};

class DBScan {
public:
	DBScan(int minPts_, double eps_, const Cluster& points_);
	std::vector<PointData> getClusters();
private:
	const int UNCLASSIFIED = -1;
	const int NOISE = 0;
	int clusterId;
	int minPts;
	double eps;
	std::vector<PointData> points;

	std::list<PointData*> regionQuery(const PointData& point);
	bool expandCluster(PointData& point, std::list<PointData*>& seeds);
};

