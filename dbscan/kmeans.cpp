#include "kmeans.h"
using namespace std;

namespace med {

Kmeans::Kmeans(const DistFunc distFunc) :
		distFunc_(distFunc) {
}

std::map<int, Cluster> Kmeans::process(const std::vector<Vector>& points,
		int groups, float epsilon, int iterations) const {
	map<int, Cluster> a;
	a[1] = Cluster();
	Vector v;
	v.push_back(1);
	v.push_back(2);
	a[1].push_back(v);
	a[1].push_back(v);
	return a;
}

}
