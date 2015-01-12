MED
===

MED project, DBSCAN

###Example for 2 dimensional data followed by plot

    make
    ./app -kte -g 5 -i 100 -o test/2dim/dim2result.data
    ./test/2dim/med2dimChart.r ./test/2dim/dim2result.data

More examples

    ./app -kte -g 5 -i 100 -o test/2dim/dim2result.data; ./test/2dim/med2dimChart.r ./test/2dim/dim2result.data
    ./app -dte -s 2 -p 2 -o test/2dim/dim2result.data; ./test/2dim/med2dimChart.r ./test/2dim/dim2result.data

Rand index calculation for results

	./app -dte -s 2 -p 2 -r ./test/rand/resLabels.data
	./test/rand/randInd.r ./test/rand/resLabels.data ./test/data/test-labels.txt

###For R plot `Rscript` required

    sudo apt-get install r-base

###For Rand Index calculation using R also `Rscript` with `mclust` package required

		# installation 'in' R
	install.packages("mclust", dependencies=T)
		# loading 'in' R
	library(mclust)
