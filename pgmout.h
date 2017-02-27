/*************************************************************
* Author: Kevin Sanft
* Date: Feb 24, 2017
* Modified By: Jeremy Wood
* Last Modified: February 26, 2017
*************************************************************/

#include <sys/stat.h>

void cleanupOutput() {
	int ret; // ignores compiler warning
	ret = system("exec rm animation.gif");
	ret = system("exec rm -r ./output/*");
}

int writeHeader(int fStep) {
	mkdir("output", 0777);
	char* fileName = malloc(23 * sizeof(char));
	sprintf(fileName, "output/outfile%04d.pgm", fStep);
	FILE *fp;
	fp = fopen(fileName, "w");
	if (fp == NULL) {
		printf("sorry can't open %s.\n", fileName);
		return -1;
	} else {
		// print a table header
		fprintf(fp, "%s\n%d %d\n%s\n", "P2", N, N, "255");
		fclose(fp);
		return 0;
	}
}

int writeRow(double* rawData, int numCols, int fStep) {
	char* fileName = malloc(23 * sizeof(char));
	sprintf(fileName, "output/outfile%04d.pgm", fStep);
	FILE *fp;
	fp = fopen(fileName, "a");
	if (fp == NULL) {
		printf("sorry can't open %s.\n", fileName);
		return -1;
	}
	else {
		int i;
		for (i=0; i<numCols; i++) {
			int val = rawData[i]*127+127;
			fprintf(fp,"%d ",val);
		}
		fprintf(fp,"\n");
		fflush(fp);
		fclose(fp);
		return 0;
	}
}

int writeMatrix(double** matrix, int numRows, int numCols, int fStep) {
	int i;
	for (i = 0; i < numRows; i++) {
		if (writeRow(matrix[i], numCols, fStep) == -1) {
			return -1;
		}
	}
	return 0;
}

void createAnimationGif() {
	int ret; // ignores compiler warning
	ret = system("exec convert output/outfile*.pgm -resize 400x400 -loop 1 animation.gif");
}
