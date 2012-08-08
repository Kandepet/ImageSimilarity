#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <fstream>

#include <ruby.h>
#include "ImgSeekExt.h"
#include "imgseek.h"

using namespace std;

// Forward declarations
void Init_imgseek_ext();

extern "C" void Init_imgseek() {
   Init_imgseek_ext();
}

static void imgseek_delete(ImgSeek* obj) {
   if (obj) delete obj;
}

static VALUE imgseek_init(VALUE self) {

   initImgBin();

   //string words = "/usr/share/dict/words";
   //SpellChecker *new_obj = new SpellChecker(words);

   ImgSeek *new_obj = new ImgSeek(ImgSeek::DB);
   return Data_Wrap_Struct(cImgSeek, 0, imgseek_delete, new_obj);
}


static VALUE load_image(VALUE self, VALUE val) {
   ImgSeek *obj;
   Data_Get_Struct(self, ImgSeek, obj);

   std::string  image_path = RSTRING_PTR(val);
   //cout << "Loading: " << image_path << endl;

   obj->LoadImage(image_path);

   //cout << "Got image id: " << imgID << endl;

   //return INT2NUM(imgID);
   return self;
}

static VALUE similarity(VALUE self, VALUE other) {
   ImgSeek *self_obj, *other_obj;
   Data_Get_Struct(self, ImgSeek, self_obj);
   Data_Get_Struct(other, ImgSeek, other_obj);

   float sim = self_obj -> similarity(other_obj);


   //cout << "Got similarity: " << sim << endl;

   return rb_float_new(sim);
}

/*
static VALUE query(VALUE self, VALUE img, VALUE numres) {
   ImgSeek *obj;
   Data_Get_Struct(self, ImgSeek, obj);

   int imgid = NUM2INT(img);
   int n = NUM2INT(numres);
   obj->query(imgid, n);

   return Qtrue;
}
*/


#if 0
static void speller_delete(SpellChecker* obj) {
   if (obj) delete obj;
}

static VALUE speller_init(VALUE self) {
   //string words = "/usr/share/dict/words";
   //SpellChecker *new_obj = new SpellChecker(words);
   SpellChecker *new_obj = new SpellChecker;
   return Data_Wrap_Struct(cSpellSuggest, 0, speller_delete, new_obj);
}

static VALUE check(VALUE self, VALUE val) {
   SpellChecker *obj;
   Data_Get_Struct(self, SpellChecker, obj);
   char  *word = RSTRING_PTR(val);
   //cout << obj->exists(word);
   if(obj->exists(word))
      return Qtrue;
   else
      return Qfalse;
}

static VALUE load(VALUE self, VALUE val) {
   SpellChecker *obj;
   Data_Get_Struct(self, SpellChecker, obj);
   std::string  words_list_file = RSTRING_PTR(val);
   //cout << obj->exists(word);
   if(obj->load(words_list_file))
      return Qtrue;
   else
      return Qfalse;
}

static VALUE suggest(VALUE self, VALUE val) {
   SpellChecker *obj;
   Data_Get_Struct(self, SpellChecker, obj);
   char  *word = RSTRING_PTR(val);

   typedef pair<string, int> Result;
   typedef vector<Result> ResultsVector;
   ResultsVector results = obj -> suggestions(word);
   VALUE suggestions = rb_ary_new();

   for(ResultsVector::iterator it = results.begin(); it != results.end(); ++it)
      rb_ary_push(suggestions, rb_str_new2(it->first.c_str()));
   //cout << "Result: " << it->first << ", " << it->second << endl;

   return suggestions;
}


#endif

typedef VALUE (*rb_method)(...);

void Init_imgseek_ext() {
   mSimilarity = rb_define_module("Similarity");
   cImgSeek = rb_define_class_under(mSimilarity, "ImgSeek", rb_cObject);

   rb_define_method(cImgSeek, "initialize", (rb_method)imgseek_init, 0);
   rb_define_singleton_method(cImgSeek, "new", (rb_method)imgseek_init, 0);
   rb_define_method(cImgSeek, "load",   (rb_method)load_image, 1);
   rb_define_method(cImgSeek, "similarity",   (rb_method)similarity, 1);
   //rb_define_method(cImgSeek, "query",   (rb_method)query, 2);

   /*
      mSpell = rb_define_module("Spell");

      cSpellSuggest = rb_define_class_under(mSpell, "SpellSuggest", rb_cObject);

      rb_define_method(cSpellSuggest, "initialize", (rb_method)speller_init, 0);
      rb_define_singleton_method(cSpellSuggest, "new", (rb_method)speller_init, 0);

      rb_define_method(cSpellSuggest, "load",   (rb_method)load, 1);
      rb_define_method(cSpellSuggest, "correct?",   (rb_method)check, 1);
      rb_define_method(cSpellSuggest, "suggest", (rb_method)suggest, 1);
      */
}

