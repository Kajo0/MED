#!/usr/bin/Rscript

med <- function(filename="dim2result.data") {
	lines <- readLines(filename);
	xVec <- list();
	yVec <- list();
	xRange <- c();
	yRange <- c();

	for (l in 1:length(lines)) {
		a <- lapply(strsplit(lines[l], "\t"), as.numeric)[[1]];
		#a <- a[2:length(a)];
		if (l %% 2 == 0) {
			yVec[[length(yVec) + 1]] <- a;
			yRange <- range(yRange, a);
		} else {
			xVec[[length(xVec) + 1]] <- a;
			xRange <- range(xRange, a);
		}
	}

	seriesAmount <- length(xVec);

	plot(0, 0, xlim=xRange, ylim=yRange, type="n")
	cl <- rainbow(seriesAmount);

	for (i in 1:seriesAmount){
		points(xVec[[i]], yVec[[i]], col=cl[i], pch=(i+3)%%26)
	}

	return ("ok")
}

args <- commandArgs(trailingOnly=TRUE);

if (length(args) == 1) {
	X11();
	med(args[1]);
	message("Press Return To Continue")
	invisible(readLines("stdin", n=1))	
} else {
	print("Give me one data file dude!");
}

