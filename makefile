all: dbscanapp

dbscanapp:
	g++ -g -std=c++11 main.cpp dbscan/dbscan.cpp -o dbscanapp

clean:
	rm -rf *.o dbscanapp
