app: clean
	g++ -g -std=c++11 main.cpp algorithms/dbscan.cpp algorithms/util.cpp algorithms/kmeans.cpp -o $@

dbscan.o: dbscan.h util.h
kmeans.o: kmeans.h util.h
util.o: util.h
main.o: dbscan.h kmeans.h util.h

clean:
	rm -rf *.o app
