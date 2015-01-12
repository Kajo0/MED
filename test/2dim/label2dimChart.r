#!/usr/bin/Rscript

med <- function(pointsFile, labelsFile) {
	points <- read.table(pointsFile, header=T);
	labels <- scan(labelsFile);

	pointsAmount = nrow(points)
	labelsAmount = length(unique(labels))

	if (pointsAmount != length(labels)) {
		cat("Lengths not match ", pointsAmount, " != ", length(labels), "\n");
		return ("not ok")
	}

	points <- as.data.frame(points);
	points$label <- labels;

	xVec <- points[,1];
	yVec <- points[,2];
	cl <- rainbow(labelsAmount);
	plot(0, 0, xlim=range(xVec), ylim=range(yVec), type="n")
	points(xVec, yVec, col=cl[points$label], pch=(points$label + 3) %% 26)

	return ("ok")
}

args <- commandArgs(trailingOnly=TRUE);

if (length(args) == 2) {
	X11();
	med(args[1], args[2]);
	message("Press Return To Continue")
	invisible(readLines("stdin", n=1))
} else {
	print("Give me two files with data and labels dude!");
}

