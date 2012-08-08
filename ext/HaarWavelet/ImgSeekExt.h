#ifndef __IMGSEEK_H__
#define __IMGSEEK_H__

#include <ruby.h>
#include "imgseek.h"

/*VALUE mSpell;
  VALUE cSpellSuggest;
  */
VALUE mSimilarity;
VALUE cImgSeek;

class ImgSeek {
   private:
      int dbID;
      SigStruct *sig;

   public:
      static const int DB = 1;
      static int nextPictureID;

      ImgSeek(int id) : dbID(id) {}

      int LoadImage(std::string & path) {

         char *img = strdup(path.c_str());
         //std::cout << "Loading: " << img << std::endl;
         sig = addImage(dbID, nextPictureID, img);

         //std::cout << "Added picture: " << nextPictureID << " returned id: " << id << std::endl;

         nextPictureID++;

         return 0;
      }

      SigStruct * getSignature() { return sig; }

      float similarity(ImgSeek *other) {
         float sim =  calcDiff(this->getSignature(), other->getSignature());
         //std::cout << "C: Comparing " << id1 << " & " << id2 << " for similarity" << std::endl;
         //float sim = calcDiff(dbID, id1, id2);
         //float sim = calcAvglDiff(dbID, id1, id2);

         sim = -1*sim/38.70;

         if(sim > 1.0) sim = 1.0;

         return sim;
      }

      /*
      void query(int id1, int numres) {
         std::vector<double> sim_v = queryImgID(dbID, id1, numres);

         std::cout << "Size: " << sim_v.size() << std::endl;
         for(std::vector<double>::iterator itr = sim_v.begin(); itr != sim_v.end(); itr++)
            std::cout << "Sim_v: " << *itr << std::endl;

      }*/


};

int ImgSeek::nextPictureID = 1;

#endif //__IMGSEEK_H__
