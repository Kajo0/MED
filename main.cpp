#include <iostream>
#include <string>

#include "dbscan/dbscan.h"

std::string vectorToString(const std::vector<double>& vector) {
	std::string result = "[";
	for(int i=0; i<vector.size(); i++) {
		result += std::to_string(vector[i]);
		if (i != vector.size()-1)
			result += ",";
	}
	return result;
}

int main(int argc, char* argv[]) {
	Vector p1 = {1,1}, p2 ={0,0}, p3={0,1}, p4={1,0};
	Vector r1 = {11,1}, r2 ={10,0}, r3={10,1}, r4={11,0};
	Vector s1 = {1,11}, s2 ={0,10}, s3={1,11}, s4={1,10};	
	
	Cluster points = {p1, p2, p3, p4, r1, r2, r3, r4,
		s1, s2, s3, s4};
	DBScan scan(4, 2.0, points);

	auto result = scan.getClusters();
	std::cout << result.size() << std::endl;
	for(auto& point : result) {
		std::cout << vectorToString(point.vector) << " cluster:" << point.clusterId << std::endl;
	}
	std::cout << "juz po" << std::endl;
	return 0;
}
