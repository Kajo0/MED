#!/usr/bin/Rscript

med <- function(firstLabels, secondLabels) {
	first <- read.table(firstLabels)[,1];
	second <- read.table(secondLabels)[,1];

	if (length(first) != length(second)) {
		return ("length of first != second")
	}

	return (adjustedRandIndex(first, second))
}

args <- commandArgs(trailingOnly=TRUE);

if (length(args) == 2) {
	suppressMessages(library(mclust))
	med(args[1], args[2]);	
} else {
	print("Give me two files with labels dude!");
}

