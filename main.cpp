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
	result += "]";
	return result;
}

int main(int argc, char* argv[]) {
	Vector p1 = {1,1}, p2 ={0,0}, p3={0,1}, p4={1,0};
	Vector r1 = {11,11}, r2 ={10,10}, r3={10,11}, r4={11,10};
	Vector s1 = {1,11}, s2 ={0,10}, s3={1,11}, s4={1,10};	
	Vector n1 = {5,5}, n2 = {-5, -5};
	Vector a1 = {0, -10}, a2= {1, -10}, a3={0, -11}, a4={1, -11};
	Vector b1 = {10, 0}, b2 = {10, 1}, b3 = {11, 0}, b4 = {11,1};

	Cluster points = {p1, p2, p3, p4, r1, r2, r3, r4,
		s1, s2, s3, s4, n1, n2, a1, a2, a3, a4, b1, b2, b3, b4};
	DBScan scan(euclideanDistance, [](double dist, double eps) { return dist < eps;});

	auto result = scan.dbscan(points, 2.0, 3);
	std::cout << result.size() << std::endl;
	for(auto& cluster : result) {
		std::cout << "clusterId=" << cluster.first << std::endl;
		for(auto& point : cluster.second) {
			std::cout<< "vector:" << vectorToString(point) << std::endl;
		}
	}
	std::cout << "FINISHED euclidean" << std::endl;
	

	scan = DBScan(cosineSimilarity, [](double dist, double eps) { return 1.0-dist < eps;});

        result = scan.dbscan(points, 0.1, 2);
        std::cout << result.size() << std::endl;
        for(auto& cluster : result) {
                std::cout << "clusterId=" << cluster.first << std::endl;
                for(auto& point : cluster.second) {
                        std::cout<< "vector:" << vectorToString(point) << std::endl;
                }
        }
        std::cout << "FINISHED cosine" << std::endl;
        return 0;

}
