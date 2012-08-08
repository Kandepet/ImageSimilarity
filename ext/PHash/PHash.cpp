
// requires CImg.h, adjust the include as needed
// cvs -d:pserver:anonymous@cimg.cvs.sourceforge.net:/cvsroot/cimg co CImg

// compile: g++ -o phash phash.cpp

// function ph_dct_matrix and the PHASH section of do_hash (ph_dct_imagehash)
// from pHash.ccp which had the following blurb:
/*
   pHash, the open source perceptual hash library
   Copyright (C) 2009 Aetilius, Inc.
   All rights reserved.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Evan Klinger - eklinger@phash.org
   D Grant Starkweather - dstarkweather@phash.org
   */

// based on ph_dct_imagehash from http://www.phash.org/
#define PHASH 1
// based on average hash from http://www.hackerfactor.com/blog/index.php?/archives/432-Looks-Like-It.html
#define AHASH 1
// based on rgb4x4 from http://www.stonehenge.com/merlyn/LinuxMag/col50.html
#define RHASH 1

// enables for various debugprints all over the place. noisy.
#define DEBUG_PHASH 0
#define DEBUG_AHASH 0
#define DEBUG 0

#define cimg_debug 0
#define cimg_display 0
#include "CImg.h"
using namespace cimg_library;

#include <limits.h>
#include <math.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define __STDC_CONSTANT_MACROS
#include <stdint.h>


CImg<float>* ph_dct_matrix(const int N){
   CImg<float> *ptr_matrix = new CImg<float>(N,N,1,1,1/sqrt((float)N));
   const float c1 = sqrt(2.0/N);
   for (int x=0;x<N;x++){
      for (int y=1;y<N;y++){
         *ptr_matrix->data(x,y) = c1*cos((cimg::PI/2/N)*y*(2*x+1));
      }
   }
   return ptr_matrix;
}

//assumes little endian
void printbits(size_t const size, void const * const ptr) {
   unsigned char *b = (unsigned char*) ptr;
   unsigned char byte;
   int i, j;

   for (i=size-1;i>=0;i--)
   {
      for (j=7;j>=0;j--)
      {
         byte = b[i] & (1<<j);
         byte >>= j;
         printf("%u", byte);
      }
   }
   puts("");
}

void hashgen(CImg<float> *data, char *hash, int bitcount, float limit) {
   for (int i=0;i< bitcount;i++){
      float current = (*data)(i);
      if (current >= limit) {
         hash[int(i/8)] |= (0x80 >> (i%8));
      }
   }
}


extern "C" int do_phash(const char *file, unsigned long long &hash);

int do_phash(const char *file, unsigned long long &hash) {
   if (!file){
      return -1;
   }
   //printf("Calculating PHash for %s\n", file);
   CImg<float> osrc;
   try {
      osrc.load(file);
   } catch (CImgIOException ex){
      return -1;
   }
   // working copy
   CImg<float> src = osrc;

   float median;
   //unsigned long long hash;

   CImg<float> img;
   CImg<float> subsec;


   // The PHASH section is from :ph_dct_imagehash(const char* file,ulong64 &hash) in phash.c

   CImg<float> meanfilter(7,7,1,1,1);
   CImg<float> *C  = ph_dct_matrix(32);
   CImg<float> Ctransp = C->get_transpose();

   if (src.spectrum() == 3){
      img = src.RGBtoYCbCr().channel(0).get_convolve(meanfilter);
   } else if (src.spectrum() == 4){
      int width = src.width();
      int height = src.height();
      int depth = src.depth();
      img = src.crop(0,0,0,0,width-1,height-1,depth-1,2).RGBtoYCbCr().channel(0).get_convolve(meanfilter);
   } else {
      img = src.channel(0).get_convolve(meanfilter);
   }

   img.resize(32,32);

   CImg<float> dctImage = (*C)*img*Ctransp;

   dctImage.crop(1,1,8,8);

   subsec = dctImage.unroll('x');

   median = subsec.median();

   hash = 0;
   hashgen(&subsec, (char*)&hash, 64, median);
   //printf("PHASH: %llu\n", hash);
   //printbits(sizeof(hash), &hash);
   //puts(" ");

   delete C;

   return 0;
}

typedef unsigned long long ulong64;

extern "C" int phash_hamming_distance(const ulong64 hash1,const ulong64 hash2){
    ulong64 x = hash1^hash2;
    const ulong64 m1  = 0x5555555555555555ULL;
    const ulong64 m2  = 0x3333333333333333ULL;
    const ulong64 h01 = 0x0101010101010101ULL;
    const ulong64 m4  = 0x0f0f0f0f0f0f0f0fULL;
    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    return (x * h01)>>56;
}


int main(int argc,char*argv[]){

   // process files from commandline
   if (argc > 1) {
      for (int i=1;i<argc;i++){
         unsigned long long hash;
         do_phash(argv[i], hash);
      }
   } else {
      // no commandline
      printf("SYNTAX: %s image_file\n", argv[0]);
      return 1;
   }

   return 0;
}

