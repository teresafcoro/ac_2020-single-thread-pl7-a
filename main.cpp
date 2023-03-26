/*
 * Main.cpp
 *
 * Created on: Fall 2019
 * 
 * Teamwork: 2020-single-thread-pl7-a
 */

#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <CImg.h>
#include <time.h>

using namespace cimg_library;

// Data type for image components
typedef float data_t;

// Pointers to the source image and the destination image
const char* SOURCE_IMG      = "/home/student/Pictures/normal/uniovi_2.bmp";
const char* DESTINATION_IMG = "/home/student/Pictures/uniovi_2_final.bmp";

// Principal method of the class
int main() {

	// Try to load the source image
	// If it is not possible, catch the exception
	cimg::exception_mode(0);
	try {

		// Open file and object initialization
		CImg<data_t> srcImage(SOURCE_IMG);

		// Creating the varibles
		data_t *pRsrc, *pGsrc, *pBsrc; // Pointers to the R, G and B components
		data_t *pLdest; // Pointer to the L component
		data_t *pDstImage; // Pointer to the new image pixels
		uint width, height; // Width and height of the image
		uint nComp; // Number of image components

		// Variables initialization
		srcImage.display(); // Displays the source image
		width  = srcImage.width(); // Getting information from the source image
		height = srcImage.height();
		nComp  = srcImage.spectrum();

		// Allocate memory space for destination image components
		pDstImage = (data_t *) malloc (width * height * nComp * sizeof(data_t));
		if (pDstImage == NULL) {
			perror("Allocating destination image");
			exit(-2);
		}

		// Pointers to the componet arrays of the source image
		pRsrc = srcImage.data(); // pRcomp points to the R component array
		pGsrc = pRsrc + height * width; // pGcomp points to the G component array
		pBsrc = pGsrc + height * width; // pBcomp points to B component array

		// Pointers to the RGB arrays of the destination image
		pLdest = pDstImage;

		// Create the variables for the time measurement
		struct timespec tStart, tEnd;
		double dElapsedTimeS = 0;

		// Variable for the L component
		data_t L;

		// Start time
		if (clock_gettime(CLOCK_REALTIME, &tStart) == -1) {
			printf("ERROR: clock_gettime: %d, \n", errno);
			exit(EXIT_FAILURE);
		}

		// Using nComp = 1 for B/W images
		nComp = 1;

		// Time measurement
		// Making a loop so we obtain significant results
		int nREPS = 30;
		for (int j = 0; j < nREPS; j++) {
			for (long i =0; i < width * height; i++) {
				L = 0.3 * pRsrc[i] + 0.59 * pGsrc[i] + 0.11 * pBsrc[i];
				L = 255 - L;
				pLdest[i] = L;
			}
		}
		
		// End time
		if (clock_gettime(CLOCK_REALTIME, &tEnd) == -1) {
			printf("ERROR: clock_gettime: %d, \n", errno);
			exit(EXIT_FAILURE);
		}
		
		// Calculate the elapsed time in the execution of the algorithm
		dElapsedTimeS = (tEnd.tv_sec - tStart.tv_sec);
		dElapsedTimeS += (tEnd.tv_nsec - tStart.tv_nsec) / 1e+9;

		// Create a new image object with the calculated pixels
		CImg<data_t> dstImage(pDstImage, width, height, 1, nComp);
		dstImage.save(DESTINATION_IMG);

		// Display destination image
		dstImage.display();

		// Showing the elapsed time
		printf("Elapsed time	: %f s.\n", dElapsedTimeS);

		return 0;

	} catch (CImgException& e) {
		std::fprintf(stderr, "Error al cargar la imagen: %s", e.what());
	}
	
}
