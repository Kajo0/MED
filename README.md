MED
===

MED project, DBSCAN

#Example for 2 dimensional data followed by plot

    make
    ./app -kte -g 5 -i 100 -o test/2dim/dim2result.data
    ./test/2dim/med2dimChart.r ./test/2dim/dim2result.data

More examples

    ./app -kte -g 5 -i 100 -o test/2dim/dim2result.data; ./test/2dim/med2dimChart.r ./test/2dim/dim2result.data
    ./app -dte -s 2 -p 2 -o test/2dim/dim2result.data; ./test/2dim/med2dimChart.r ./test/2dim/dim2result.data

#For R plot `Rscript` required

    sudo apt-get install r-base
