#ifndef KMEANS_H
#define KMEANS_H

#include <map>
#include <vector>
#include "util.h"

namespace med {

class Kmeans {
private:
	DistFunc distFunc_;

public:
	Kmeans(const DistFunc distFunc);
	std::map<int, Cluster> process(const std::vector<Vector>& points,
			int groups, float epsilon = 0, int iterations = -1) const;

private:
};

}

#endif // KMEANS_H
