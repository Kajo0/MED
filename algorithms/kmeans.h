#ifndef KMEANS_H
#define KMEANS_H

#include <map>
#include <vector>
#include "util.h"

namespace med {

struct KmeansCluster {
	Vector centroid_;
	Cluster cluster_;
};

class Kmeans {
private:
	DistFunc distFunc_;
	bool debug_;

public:
	Kmeans(const DistFunc &distFunc);
	std::map<int, Cluster> process(const std::vector<Vector>& points,
			int groups, float epsilon = 0, int iterations = -1) const;
	void debug(bool enable) {
		debug_ = enable;
	}

private:
	std::vector<Vector> findRandomCentroids(const std::vector<Vector>& points,
			int groups) const;
	void assignPoints(std::vector<KmeansCluster>& clusters,
			const std::vector<Vector>& points) const;
	void recalculateCentroids(std::vector<KmeansCluster>& clusters) const;
	/**
	 * @return true if at lest one point has changed assignment
	 */
	bool reassignPoints(std::vector<KmeansCluster>& clusters) const;
	double calculateQuantizationError(
			const std::vector<KmeansCluster>& clusters) const;
};

}

#endif // KMEANS_H
