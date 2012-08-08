#ifndef __IMG_SEEK_H__
#define __IMG_SEEK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <Magick++.h>

#include "haar.h"
#include <fstream>
#include <iostream>

//using namespace haar;

#define POSIX_SYSTEM 1

/*
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

void initImgBin();

/* persisted signature structure */
class DiskSigStruct {
public:

  haar::Idx sig1[NUM_COEFS];    /* Y positions with largest magnitude */
  haar::Idx sig2[NUM_COEFS];    /* I positions with largest magnitude */
  haar::Idx sig3[NUM_COEFS];    /* Q positions with largest magnitude */
  double avgl[3];    /* YIQ for position [0,0] */
  /* image properties extracted when opened for the first time */
  int width;      /* in pixels */
  int height;      /* in pixels */

  DiskSigStruct() {}
  ~DiskSigStruct() {}
};

/* in memory signature structure */
class SigStruct: public DiskSigStruct {
public:

  SigStruct(DiskSigStruct* ds) {

    memcpy(sig1,ds->sig1,sizeof(sig1));
    memcpy(sig2,ds->sig2,sizeof(sig2));
    memcpy(sig3,ds->sig3,sizeof(sig3));
    memcpy(avgl,ds->avgl,sizeof(avgl));

    width=ds->width;
    height=ds->height;

  }

  SigStruct() {}
  ~SigStruct() {}
};


SigStruct *analyzeImage(Magick::Image *image);
SigStruct * addImage(const int dbId, const long int id, char *filename);
double calcAvglDiff(SigStruct *sigs1, SigStruct *sigs2);
double calcDiff(SigStruct *sigs1, SigStruct *sigs2);

#endif // __IMG_SEEK_H__
