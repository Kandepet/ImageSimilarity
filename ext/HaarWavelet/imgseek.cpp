
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <Magick++.h>

#include "haar.h"
#include "imgseek.h"

#include <fstream>
#include <iostream>

using namespace std;
using Magick::Image;
using MagickCore::DestroyImage;
using MagickCore::ExceptionInfo;

using namespace haar;

// Weights for the Haar coefficients.
// Straight from the referenced paper:
const float weights[2][6][3] = {
  // For scanned picture (sketch=0):
  //    Y      I      Q       idx total occurs
  {{ 5.00f, 19.21f, 34.37f},  // 0   58.58      1 (`DC' component)
   { 0.83f,  1.26f,  0.36f},  // 1    2.45      3
   { 1.01f,  0.44f,  0.45f},  // 2    1.90      5
   { 0.52f,  0.53f,  0.14f},  // 3    1.19      7
   { 0.47f,  0.28f,  0.18f},  // 4    0.93      9
   { 0.30f,  0.14f,  0.27f}}, // 5    0.71      16384-25=16359

  // For handdrawn/painted sketch (sketch=1):
  //    Y      I      Q
  {{ 4.04f, 15.14f, 22.62f},
   { 0.78f,  0.92f,  0.40f},
   { 0.46f,  0.53f,  0.63f},
   { 0.42f,  0.26f,  0.25f},
   { 0.41f,  0.14f,  0.15f},
   { 0.32f,  0.07f,  0.38f}}
};

/*
#define POSIX_SYSTEM 1

// STL includes
#include <map>
#include <queue>
#include <list>
#ifdef POSIX_SYSTEM
  #include <ext/hash_set>
#else
  #include <hash_set>
#endif
*/


/* Fixed weight mask for pixel positions (i,j).
Each entry x = i*NUM_PIXELS + j, gets value max(i,j) saturated at 5.
To be treated as a constant.
 */
unsigned char imgBin[16384];
int imgBinInited = 0;

// Macros
#define validate_dbid(dbId) (dbSpace.count(dbId))
#define validate_imgid(dbId, imgId) (dbSpace.count(dbId) && (dbSpace[dbId]->sigs.count(imgId)))

void initImgBin()
{
  imgBinInited = 1;
  srand((unsigned)time(0));

  /* setup initial fixed weights that each coefficient represents */
  int i, j;

  /*
  0 1 2 3 4 5 6 i
  0 0 1 2 3 4 5 5
  1 1 1 2 3 4 5 5
  2 2 2 2 3 4 5 5
  3 3 3 3 3 4 5 5
  4 4 4 4 4 4 5 5
  5 5 5 5 5 5 5 5
  5 5 5 5 5 5 5 5
  j
   */

  /* Every position has value 5, */
  memset(imgBin, 5, NUM_PIXELS_SQUARED);

  /* Except for the 5 by 5 upper-left quadrant: */
  for (i = 0; i < 5; i++)
    for (j = 0; j < 5; j++)
      imgBin[i * 128 + j] = max(i, j);
  // Note: imgBin[0] == 0

}

SigStruct *analyzeImage(Image *image) {
  if (!image) {
    throw std::string("unable to add null image");
  }

  unsigned int width, height;

  width = image->columns();
  height = image->rows();
  image->sample(Magick::Geometry(128, 128));

  // store color value for basic channels
  unsigned char rchan[16384];
  unsigned char gchan[16384];
  unsigned char bchan[16384];

  const Magick::PixelPacket *pixel_cache;
  pixel_cache = image->getConstPixels(0, 0, 128, 128);

  for (int idx = 0; idx < 16384; idx++) {
    rchan[idx] = pixel_cache->red;
    gchan[idx] = pixel_cache->green;
    bchan[idx] = pixel_cache->blue;
    ++pixel_cache;
  }

  // Made static for speed; only used locally
  static Unit cdata1[16384];
  static Unit cdata2[16384];
  static Unit cdata3[16384];

  haar::transformChar(rchan, gchan, bchan, cdata1, cdata2, cdata3);

  SigStruct *nsig = new SigStruct();
  nsig->width = width;
  nsig->height = height;

  haar::calcHaar(cdata1, cdata2, cdata3,
    nsig->sig1, nsig->sig2, nsig->sig3, nsig->avgl);

  return nsig;
}

SigStruct * addImage(const int dbId, const long int id, char *filename) {
  Image image(filename);
  SigStruct *nsig = analyzeImage(&image);

  return nsig;
}

double calcAvglDiff(SigStruct *sigs1, SigStruct *sigs2) {

  /* return the average luminance difference */

  return fabs(sigs1->avgl[0] - sigs2->avgl[0])
  + fabs(sigs1->avgl[1] - sigs2->avgl[1])
  + fabs(sigs1->avgl[2] - sigs2->avgl[2]);
}


double calcDiff(SigStruct *sigs1, SigStruct *sigs2) {
  /* use it to tell the content-based difference between two images
   */

  double diff = calcAvglDiff(sigs1, sigs2);
  Idx *sig1[3] = { sigs1->sig1, sigs1->sig2, sigs1->sig3 };
  Idx *sig2[3] = { sigs2->sig1, sigs2->sig2, sigs2->sig3 };

  for (int b = 0; b < NUM_COEFS; b++)
    for (int c = 0; c < 3; c++)
      for (int b2 = 0; b2 < NUM_COEFS; b2++)
        if (sig2[c][b2] == sig1[c][b])
          diff -= weights[0][imgBin[abs(sig1[c][b])]][c];

  return diff;
}

#if 0
int main() {

   initImgBin();

   SigStruct *sig1 = addImage(1, 1, "IMG_3965.JPG");
   SigStruct *sig2 = addImage(1, 1, "IMG_3966.JPG");

   std::cout << "Diff: " << calcDiff(sig1, sig2) << std::endl;
}
#endif
