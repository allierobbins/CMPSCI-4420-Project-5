
/***************************************
* Program 1 for 4420 Assignment 5      *
* Author: Allie Robbins                *
* Language: C++ with OpenCV libraries  *
* IDE: Visual Studio 2017              *
***************************************/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

/* Function Prototypes */
void CallBackFunc(int, int, int, int, void*);

/* Global Variable Declarations */
Point start, stop;

int main(int argc, char *argv[]) {

  /* Variable declarations */
  int x1 = 0, x2 = 0;
  int y1 = 0, y2 = 0;
  /* int numOfRoi = 0; */

  /* Check to make sure an image was given */
   if (argc == 1) {
     printf("You did not provide an image. Ending program 1.\n");
     exit(EXIT_FAILURE);
   }

   /* Assign the input image from command line args */
   Mat image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

   /* Make sure the images can be found/opened */
   if (image.empty()) {
     printf("There was an error opening the image. Ending program1.\n");
     exit(EXIT_FAILURE);
   }

   /* Image declarations */
   Mat padded;
   Mat complexI;
   Mat magI;

   /* Expand the input image to the optimal size */
   int m = getOptimalDFTSize(image.rows);
   int n = getOptimalDFTSize(image.cols);

   /* Add zero values to the border */
   copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));

   /* Add to the expanded image another plane w/ zeros */
   Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
   merge(planes, 2, complexI);

   /* Make the result fit in the source matrix */
   dft(complexI, complexI);

   /* Split the fourier image into */
   split(complexI, planes);

   /* Calculate the magnitude of the image */
   magnitude(planes[0], planes[1], planes[0]);

   /* Set the magnitude image to the value of the magnitude */
   magI = planes[0];

   /* Switch to logarithmic scale => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)) */
   magI += Scalar::all(1);
   log(magI, magI);

   /* Crop the spectrum, if it has an odd # of rows or columns */
   magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

   /* Rearrange the quadrants of Fourier image so that the origin is @ the center */
   int cx = magI.cols / 2;
   int cy = magI.rows / 2;

   Mat q0(magI, Rect(0, 0, cx, cy));    /* Top-Left */
   Mat q1(magI, Rect(cx, 0, cx, cy));   /* Top-Right */
   Mat q2(magI, Rect(0, cy, cx, cy));   /* Bottom-Left */
   Mat q3(magI, Rect(cx, cy, cx, cy));  /* Bottom-Right */

   /* Swap the Top-Left quadrant with the Bottom-Right quadrant */
   Mat tmp;
   q0.copyTo(tmp);
   q3.copyTo(q0);
   tmp.copyTo(q3);

   /* Swap the Top-Right quadrant with the Bottom-Left quadrant */
   q1.copyTo(tmp);
   q2.copyTo(q1);
   tmp.copyTo(q2);

   /* Normalize the magnitude image into viewable image form */
   normalize(magI, magI, 0, 1, CV_MINMAX);

   /* Display the image */
   namedWindow("Magnitude Image");
   imshow("Magnitude Image", magI);


   /****************************************************************************
   * This commented section was designed to prompt for user input of ROI for   *
   * better adapability. However there were bugs in my FOR loop to perform the *
   * noise removal so hardcoded points (gathered during testing) were used.    *
   ****************************************************************************/


   /* Prompt for # of ROI
   printf("Please enter the number of ROI where there is noise: \n");
   cin >> numOfRoi;
   int roiArray[numOfRoi][4];

   Gather ROI from mouse click input
   printf("Please select the ROI using mouse clicks.\n");
   for (int i = 0; i < numOfRoi; i++) {
      setMouseCallback("ImageDisplay", CallBackFunc, NULL);
      roiArray[i][0] = start.x; roiArray[i][1] = stop.x;
      roiArray[i][2] = start.y; roiArray[i][3] = stop.y;

    }

    Remove the ROI noises from the images
    for (int i = 0; i < numOfRoi; i++) {
      x1 = roiArray[i][0]; x2 = roiArray[i][0] + roiArray[i][0].width;
      y1 = roiArray[i][1]; y2 = roiArray[i][1] + roi1.height;
        for (int i = roiArray[i][0]; i < (roiArray[i][1] + roi1.height)); i++) {
          for (int j = roiArray[i][0]; j < x2; j++) {
            planes[0].at<float>(i, j) = 0;
            planes[1].at<float>(i, j) = 0;
          }
        } 
      }

       Merge the planes into the complex image
      merge(planes, 2, complexI);

       Find the inverse DFT to get the real image
      idft(complexI, complexI, DFT_REAL_OUTPUT);

       Normalize the complex image into viewable image form
      normalize(complexI, complexI, 0, 1, CV_MINMAX);

       Display the final image(s) to the screen
      namedWindow("Magnitude Image");
      imshow("Magnitude Image", magI);
      namedWindow("Complex Image");
      imshow("Complex Image", complexI); */


   /* Rounded ROI gathered during testing - back up if user-input fails */
   /* 11/28 - The user-input failed */
   Rect roi1 = Rect(Point(720, 310), Point(740, 330)); Rect roi2 = Rect(Point(715, 360), Point(745, 360));
   Rect roi3 = Rect(Point(710, 400), Point(730, 405)); Rect roi4 = Rect(Point(640, 310), Point(650, 330));
   Rect roi5 = Rect(Point(630, 400), Point(650, 410)); Rect roi6 = Rect(Point(555, 320), Point(570, 325));
   Rect roi7 = Rect(Point(540, 360), Point(570, 360)); Rect roi8 = Rect(Point(540, 400), Point(570, 410));

   /* Remove the ROI noises from the images */
   x1 = roi1.x; x2 = roi1.x + roi1.width;
   y1 = roi1.y; y2 = roi1.y + roi1.height;
   for (int i = y1; i < y2; i++) {
     for (int j = x1; j < x2; j++) {
       planes[0].at<float>(i, j) = 0;
       planes[1].at<float>(i, j) = 0;
     }
   }

   x1 = roi2.x; x2 = roi2.x + roi2.width;
   y1 = roi2.y; y2 = roi2.y + roi2.height;
   for (int i = y1; i < y2; i++) {
     for (int j = x1; j < x2; j++) {
       planes[0].at<float>(i, j) = 0;
       planes[1].at<float>(i, j) = 0;
     }
   }

   x1 = roi3.x; x2 = roi3.x + roi3.width;
   y1 = roi3.y; y2 = roi3.y + roi3.height;
   for (int i = y1; i < y2; i++) {
     for (int j = x1; j < x2; j++) {
       planes[0].at<float>(i, j) = 0;
       planes[1].at<float>(i, j) = 0;
     }
   }

   x1 = roi4.x; x2 = roi4.x + roi4.width;
   y1 = roi4.y; y2 = roi4.y + roi4.height;
   for (int i = y1; i < y2; i++) {
     for (int j = x1; j < x2; j++) {
       planes[0].at<float>(i, j) = 0;
       planes[1].at<float>(i, j) = 0;
     }
   }

   x1 = roi5.x; x2 = roi5.x + roi5.width;
   y1 = roi5.y; y2 = roi5.y + roi5.height;
   for (int i = y1; i < y2; i++) {
     for (int j = x1; j < x2; j++) {
       planes[0].at<float>(i, j) = 0;
       planes[1].at<float>(i, j) = 0;
     }
   }

   x1 = roi6.x; x2 = roi6.x + roi6.width;
   y1 = roi6.y; y2 = roi6.y + roi6.height;
   for (int i = y1; i < y2; i++) {
     for (int j = x1; j < x2; j++) {
       planes[0].at<float>(i, j) = 0;
       planes[1].at<float>(i, j) = 0;
     }
   }

   x1 = roi7.x; x2 = roi7.x + roi7.width;
   y1 = roi7.y; y2 = roi7.y + roi7.height;
   for (int i = y1; i < y2; i++) {
     for (int j = x1; j < x2; j++) {
       planes[0].at<float>(i, j) = 0;
       planes[1].at<float>(i, j) = 0;
     }
   }

   x1 = roi8.x; x2 = roi8.x + roi8.width;
   y1 = roi8.y; y2 = roi8.y + roi8.height;
   for (int i = y1; i < y2; i++) {
     for (int j = x1; j < x2; j++) {
       planes[0].at<float>(i, j) = 0;
       planes[1].at<float>(i, j) = 0;
     }
   }

   /* Merge the planes into the complex image*/
   merge(planes, 2, complexI);

   /* Find the inverse DFT to get the real image */
   idft(complexI, complexI, DFT_REAL_OUTPUT);

   /* Normalize the complex image into viewable image form */
   normalize(complexI, complexI, 0, 1, CV_MINMAX);

   /* Display the final image(s) to the screen */
   namedWindow("Magnitude Image");
   imshow("Magnitude Image", magI);
   namedWindow("Complex Image");
   imshow("Complex Image", complexI);

   /* Pause to admire image(s) */
   waitKey(0);

   /* End of program 1 */
   return 0;
}


/* CallBackFunc used to gather the ROI */
void CallBackFunc(int event, int x, int y, int flags, void *image) {

  if (event == EVENT_LBUTTONDOWN) {
    start.x = x;
    start.y = y;
    printf("%d - %d\n", x, y );
  } else if (event == EVENT_LBUTTONUP) {
    stop.x = x;
    stop.y = y;
    printf("%d - %d\n", x, y );
  }
}


/************** Sources ***************
* https://docs.opencv.org/2.4/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html
* https://dsp.stackexchange.com/questions/20061/removing-periodic-noise-from-image-using-fourier-transform
* https://stackoverflow.com/questions/34027840/removing-periodic-noise-from-an-image-using-the-fourier-transform
**************************************/
