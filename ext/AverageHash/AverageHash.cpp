
// compile: g++ -o ahash ahash.cpp

// based on average hash from http://www.hackerfactor.com/blog/index.php?/archives/432-Looks-Like-It.html
#define AHASH 1

// enables for various debugprints all over the place. noisy.
#define DEBUG_AHASH 0

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


void hexprint(char *a, int count, int spacer){
   int i;
   char h[] = "0123456789abcdef";
   for (i=0;i<count;i++) {
      if (i && !(i % spacer)) {
         printf(" ");
      }
      printf("%c%c",h[(a[i]>>4)&0x0f],h[a[i]&0x0f]);
   }
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

extern "C" int do_averagehash(const char *file, unsigned long long &hash);

int do_averagehash(const char *file, unsigned long long &hash) {

   if (!file){
      return -1;
   }

   //printf("Calculating Ahash of : %s\n", file);

   CImg<float> osrc;
   try {
      osrc.load(file);
   } catch (CImgIOException ex){
      return -1;
   }

   // working copy
   CImg<float> src = osrc;

   float mean;
   //unsigned long long hash;

   CImg<float> img;
   CImg<float> subsec;

   src = osrc;

   //Step 1. Resize to 8x8
   src.resize(8,8,-100,-100,2);


   // Get rid of alpha channel if it exists
   if (src.spectrum() == 4){
      int width = src.width();
      int height = src.height();
      int depth = src.depth();
      src = src.crop(0,0,0,0,width-1,height-1,depth-1,2);
   }

   // Step 2. Convert to grayscale
   // If we have 3 channels (r,g,b)
   if (src.spectrum() == 3){
      img = src.RGBtoYCbCr().channel(0); //.get_convolve(meanfilter);
   } else {
      // This is already a gray scale image. Just get the first channel.
      img = src.channel(0);
   }

   subsec = img.unroll('x');

   // Step 3. Calculate the mean value
   mean = subsec.mean();

   // Step 4. Calculate hash based on the mean of the colors.
   hash = 0;
   hashgen(&subsec, (char*)&hash, 64, mean);

   //printf("AHASH: %llu\n", hash);
   //printbits(sizeof(hash), &hash);
   //printf("\n");

   return 0;
}

typedef unsigned long long ulong64;

extern "C" int averagehash_hamming_distance(const ulong64 hash1,const ulong64 hash2){
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
         do_averagehash(argv[i], hash);
      }

   } else {
      // no commandline, process files from stdin
      printf("SYNTAX: %s image_file\n", argv[0]);
   }

   return 0;
}

#if 0
// Forward declarations
void startup();

extern "C" void Init_AverageHash() {
  startup();
}


typedef VALUE (*rb_method)(...);

void startup() {
  mSpell = rb_define_module("Spell");

  cSpellSuggest = rb_define_class_under(mSpell, "SpellSuggest", rb_cObject);

  rb_define_method(cSpellSuggest, "initialize", (rb_method)speller_init, 0);
  rb_define_singleton_method(cSpellSuggest, "new", (rb_method)speller_init, 0);

  rb_define_method(cSpellSuggest, "load",   (rb_method)load, 1);
  rb_define_method(cSpellSuggest, "correct?",   (rb_method)check, 1);
  rb_define_method(cSpellSuggest, "suggest", (rb_method)suggest, 1);
}

#endif

