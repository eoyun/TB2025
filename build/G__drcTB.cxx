// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__drcTB
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBdetector.h"
#include "/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBevt.h"
#include "/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBmid.h"
#include "/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBobject.h"
#include "/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBread.h"
#include "/u/user/eoyun/TB2025/25.08.09/TB2025/include/TButility.h"
#include "TBmid.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *TBwaveform_Dictionary();
   static void TBwaveform_TClassManip(TClass*);
   static void *new_TBwaveform(void *p = nullptr);
   static void *newArray_TBwaveform(Long_t size, void *p);
   static void delete_TBwaveform(void *p);
   static void deleteArray_TBwaveform(void *p);
   static void destruct_TBwaveform(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBwaveform*)
   {
      ::TBwaveform *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBwaveform));
      static ::ROOT::TGenericClassInfo 
         instance("TBwaveform", "TBmid.h", 8,
                  typeid(::TBwaveform), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBwaveform_Dictionary, isa_proxy, 4,
                  sizeof(::TBwaveform) );
      instance.SetNew(&new_TBwaveform);
      instance.SetNewArray(&newArray_TBwaveform);
      instance.SetDelete(&delete_TBwaveform);
      instance.SetDeleteArray(&deleteArray_TBwaveform);
      instance.SetDestructor(&destruct_TBwaveform);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBwaveform*)
   {
      return GenerateInitInstanceLocal(static_cast<::TBwaveform*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TBwaveform*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBwaveform_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::TBwaveform*>(nullptr))->GetClass();
      TBwaveform_TClassManip(theClass);
   return theClass;
   }

   static void TBwaveform_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBfastmode_Dictionary();
   static void TBfastmode_TClassManip(TClass*);
   static void *new_TBfastmode(void *p = nullptr);
   static void *newArray_TBfastmode(Long_t size, void *p);
   static void delete_TBfastmode(void *p);
   static void deleteArray_TBfastmode(void *p);
   static void destruct_TBfastmode(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBfastmode*)
   {
      ::TBfastmode *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBfastmode));
      static ::ROOT::TGenericClassInfo 
         instance("TBfastmode", "TBmid.h", 36,
                  typeid(::TBfastmode), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBfastmode_Dictionary, isa_proxy, 4,
                  sizeof(::TBfastmode) );
      instance.SetNew(&new_TBfastmode);
      instance.SetNewArray(&newArray_TBfastmode);
      instance.SetDelete(&delete_TBfastmode);
      instance.SetDeleteArray(&deleteArray_TBfastmode);
      instance.SetDestructor(&destruct_TBfastmode);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBfastmode*)
   {
      return GenerateInitInstanceLocal(static_cast<::TBfastmode*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TBfastmode*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBfastmode_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::TBfastmode*>(nullptr))->GetClass();
      TBfastmode_TClassManip(theClass);
   return theClass;
   }

   static void TBfastmode_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBmidbase_Dictionary();
   static void TBmidbase_TClassManip(TClass*);
   static void *new_TBmidbase(void *p = nullptr);
   static void *newArray_TBmidbase(Long_t size, void *p);
   static void delete_TBmidbase(void *p);
   static void deleteArray_TBmidbase(void *p);
   static void destruct_TBmidbase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBmidbase*)
   {
      ::TBmidbase *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBmidbase));
      static ::ROOT::TGenericClassInfo 
         instance("TBmidbase", "TBmid.h", 56,
                  typeid(::TBmidbase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBmidbase_Dictionary, isa_proxy, 4,
                  sizeof(::TBmidbase) );
      instance.SetNew(&new_TBmidbase);
      instance.SetNewArray(&newArray_TBmidbase);
      instance.SetDelete(&delete_TBmidbase);
      instance.SetDeleteArray(&deleteArray_TBmidbase);
      instance.SetDestructor(&destruct_TBmidbase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBmidbase*)
   {
      return GenerateInitInstanceLocal(static_cast<::TBmidbase*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TBmidbase*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBmidbase_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::TBmidbase*>(nullptr))->GetClass();
      TBmidbase_TClassManip(theClass);
   return theClass;
   }

   static void TBmidbase_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBmidlETBwaveformgR_Dictionary();
   static void TBmidlETBwaveformgR_TClassManip(TClass*);
   static void *new_TBmidlETBwaveformgR(void *p = nullptr);
   static void *newArray_TBmidlETBwaveformgR(Long_t size, void *p);
   static void delete_TBmidlETBwaveformgR(void *p);
   static void deleteArray_TBmidlETBwaveformgR(void *p);
   static void destruct_TBmidlETBwaveformgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBmid<TBwaveform>*)
   {
      ::TBmid<TBwaveform> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBmid<TBwaveform>));
      static ::ROOT::TGenericClassInfo 
         instance("TBmid<TBwaveform>", "TBmid.h", 101,
                  typeid(::TBmid<TBwaveform>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBmidlETBwaveformgR_Dictionary, isa_proxy, 4,
                  sizeof(::TBmid<TBwaveform>) );
      instance.SetNew(&new_TBmidlETBwaveformgR);
      instance.SetNewArray(&newArray_TBmidlETBwaveformgR);
      instance.SetDelete(&delete_TBmidlETBwaveformgR);
      instance.SetDeleteArray(&deleteArray_TBmidlETBwaveformgR);
      instance.SetDestructor(&destruct_TBmidlETBwaveformgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBmid<TBwaveform>*)
   {
      return GenerateInitInstanceLocal(static_cast<::TBmid<TBwaveform>*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TBmid<TBwaveform>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBmidlETBwaveformgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::TBmid<TBwaveform>*>(nullptr))->GetClass();
      TBmidlETBwaveformgR_TClassManip(theClass);
   return theClass;
   }

   static void TBmidlETBwaveformgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBmidlETBfastmodegR_Dictionary();
   static void TBmidlETBfastmodegR_TClassManip(TClass*);
   static void *new_TBmidlETBfastmodegR(void *p = nullptr);
   static void *newArray_TBmidlETBfastmodegR(Long_t size, void *p);
   static void delete_TBmidlETBfastmodegR(void *p);
   static void deleteArray_TBmidlETBfastmodegR(void *p);
   static void destruct_TBmidlETBfastmodegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBmid<TBfastmode>*)
   {
      ::TBmid<TBfastmode> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBmid<TBfastmode>));
      static ::ROOT::TGenericClassInfo 
         instance("TBmid<TBfastmode>", "TBmid.h", 101,
                  typeid(::TBmid<TBfastmode>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBmidlETBfastmodegR_Dictionary, isa_proxy, 4,
                  sizeof(::TBmid<TBfastmode>) );
      instance.SetNew(&new_TBmidlETBfastmodegR);
      instance.SetNewArray(&newArray_TBmidlETBfastmodegR);
      instance.SetDelete(&delete_TBmidlETBfastmodegR);
      instance.SetDeleteArray(&deleteArray_TBmidlETBfastmodegR);
      instance.SetDestructor(&destruct_TBmidlETBfastmodegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBmid<TBfastmode>*)
   {
      return GenerateInitInstanceLocal(static_cast<::TBmid<TBfastmode>*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TBmid<TBfastmode>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBmidlETBfastmodegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::TBmid<TBfastmode>*>(nullptr))->GetClass();
      TBmidlETBfastmodegR_TClassManip(theClass);
   return theClass;
   }

   static void TBmidlETBfastmodegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBcid_Dictionary();
   static void TBcid_TClassManip(TClass*);
   static void *new_TBcid(void *p = nullptr);
   static void *newArray_TBcid(Long_t size, void *p);
   static void delete_TBcid(void *p);
   static void deleteArray_TBcid(void *p);
   static void destruct_TBcid(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBcid*)
   {
      ::TBcid *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBcid));
      static ::ROOT::TGenericClassInfo 
         instance("TBcid", "TBdetector.h", 7,
                  typeid(::TBcid), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBcid_Dictionary, isa_proxy, 4,
                  sizeof(::TBcid) );
      instance.SetNew(&new_TBcid);
      instance.SetNewArray(&newArray_TBcid);
      instance.SetDelete(&delete_TBcid);
      instance.SetDeleteArray(&deleteArray_TBcid);
      instance.SetDestructor(&destruct_TBcid);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBcid*)
   {
      return GenerateInitInstanceLocal(static_cast<::TBcid*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TBcid*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBcid_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::TBcid*>(nullptr))->GetClass();
      TBcid_TClassManip(theClass);
   return theClass;
   }

   static void TBcid_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBevtlETBfastmodegR_Dictionary();
   static void TBevtlETBfastmodegR_TClassManip(TClass*);
   static void *new_TBevtlETBfastmodegR(void *p = nullptr);
   static void *newArray_TBevtlETBfastmodegR(Long_t size, void *p);
   static void delete_TBevtlETBfastmodegR(void *p);
   static void deleteArray_TBevtlETBfastmodegR(void *p);
   static void destruct_TBevtlETBfastmodegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBevt<TBfastmode>*)
   {
      ::TBevt<TBfastmode> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBevt<TBfastmode>));
      static ::ROOT::TGenericClassInfo 
         instance("TBevt<TBfastmode>", "TBevt.h", 11,
                  typeid(::TBevt<TBfastmode>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBevtlETBfastmodegR_Dictionary, isa_proxy, 4,
                  sizeof(::TBevt<TBfastmode>) );
      instance.SetNew(&new_TBevtlETBfastmodegR);
      instance.SetNewArray(&newArray_TBevtlETBfastmodegR);
      instance.SetDelete(&delete_TBevtlETBfastmodegR);
      instance.SetDeleteArray(&deleteArray_TBevtlETBfastmodegR);
      instance.SetDestructor(&destruct_TBevtlETBfastmodegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBevt<TBfastmode>*)
   {
      return GenerateInitInstanceLocal(static_cast<::TBevt<TBfastmode>*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TBevt<TBfastmode>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBevtlETBfastmodegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::TBevt<TBfastmode>*>(nullptr))->GetClass();
      TBevtlETBfastmodegR_TClassManip(theClass);
   return theClass;
   }

   static void TBevtlETBfastmodegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBevtlETBwaveformgR_Dictionary();
   static void TBevtlETBwaveformgR_TClassManip(TClass*);
   static void *new_TBevtlETBwaveformgR(void *p = nullptr);
   static void *newArray_TBevtlETBwaveformgR(Long_t size, void *p);
   static void delete_TBevtlETBwaveformgR(void *p);
   static void deleteArray_TBevtlETBwaveformgR(void *p);
   static void destruct_TBevtlETBwaveformgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBevt<TBwaveform>*)
   {
      ::TBevt<TBwaveform> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBevt<TBwaveform>));
      static ::ROOT::TGenericClassInfo 
         instance("TBevt<TBwaveform>", "TBevt.h", 11,
                  typeid(::TBevt<TBwaveform>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBevtlETBwaveformgR_Dictionary, isa_proxy, 4,
                  sizeof(::TBevt<TBwaveform>) );
      instance.SetNew(&new_TBevtlETBwaveformgR);
      instance.SetNewArray(&newArray_TBevtlETBwaveformgR);
      instance.SetDelete(&delete_TBevtlETBwaveformgR);
      instance.SetDeleteArray(&deleteArray_TBevtlETBwaveformgR);
      instance.SetDestructor(&destruct_TBevtlETBwaveformgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBevt<TBwaveform>*)
   {
      return GenerateInitInstanceLocal(static_cast<::TBevt<TBwaveform>*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TBevt<TBwaveform>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBevtlETBwaveformgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::TBevt<TBwaveform>*>(nullptr))->GetClass();
      TBevtlETBwaveformgR_TClassManip(theClass);
   return theClass;
   }

   static void TBevtlETBwaveformgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBreadlETBfastmodegR_Dictionary();
   static void TBreadlETBfastmodegR_TClassManip(TClass*);
   static void delete_TBreadlETBfastmodegR(void *p);
   static void deleteArray_TBreadlETBfastmodegR(void *p);
   static void destruct_TBreadlETBfastmodegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBread<TBfastmode>*)
   {
      ::TBread<TBfastmode> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBread<TBfastmode>));
      static ::ROOT::TGenericClassInfo 
         instance("TBread<TBfastmode>", "TBread.h", 136,
                  typeid(::TBread<TBfastmode>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBreadlETBfastmodegR_Dictionary, isa_proxy, 4,
                  sizeof(::TBread<TBfastmode>) );
      instance.SetDelete(&delete_TBreadlETBfastmodegR);
      instance.SetDeleteArray(&deleteArray_TBreadlETBfastmodegR);
      instance.SetDestructor(&destruct_TBreadlETBfastmodegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBread<TBfastmode>*)
   {
      return GenerateInitInstanceLocal(static_cast<::TBread<TBfastmode>*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TBread<TBfastmode>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBreadlETBfastmodegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::TBread<TBfastmode>*>(nullptr))->GetClass();
      TBreadlETBfastmodegR_TClassManip(theClass);
   return theClass;
   }

   static void TBreadlETBfastmodegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TBreadlETBwaveformgR_Dictionary();
   static void TBreadlETBwaveformgR_TClassManip(TClass*);
   static void delete_TBreadlETBwaveformgR(void *p);
   static void deleteArray_TBreadlETBwaveformgR(void *p);
   static void destruct_TBreadlETBwaveformgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TBread<TBwaveform>*)
   {
      ::TBread<TBwaveform> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TBread<TBwaveform>));
      static ::ROOT::TGenericClassInfo 
         instance("TBread<TBwaveform>", "TBread.h", 136,
                  typeid(::TBread<TBwaveform>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TBreadlETBwaveformgR_Dictionary, isa_proxy, 4,
                  sizeof(::TBread<TBwaveform>) );
      instance.SetDelete(&delete_TBreadlETBwaveformgR);
      instance.SetDeleteArray(&deleteArray_TBreadlETBwaveformgR);
      instance.SetDestructor(&destruct_TBreadlETBwaveformgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TBread<TBwaveform>*)
   {
      return GenerateInitInstanceLocal(static_cast<::TBread<TBwaveform>*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::TBread<TBwaveform>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TBreadlETBwaveformgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::TBread<TBwaveform>*>(nullptr))->GetClass();
      TBreadlETBwaveformgR_TClassManip(theClass);
   return theClass;
   }

   static void TBreadlETBwaveformgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBwaveform(void *p) {
      return  p ? new(p) ::TBwaveform : new ::TBwaveform;
   }
   static void *newArray_TBwaveform(Long_t nElements, void *p) {
      return p ? new(p) ::TBwaveform[nElements] : new ::TBwaveform[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBwaveform(void *p) {
      delete (static_cast<::TBwaveform*>(p));
   }
   static void deleteArray_TBwaveform(void *p) {
      delete [] (static_cast<::TBwaveform*>(p));
   }
   static void destruct_TBwaveform(void *p) {
      typedef ::TBwaveform current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TBwaveform

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBfastmode(void *p) {
      return  p ? new(p) ::TBfastmode : new ::TBfastmode;
   }
   static void *newArray_TBfastmode(Long_t nElements, void *p) {
      return p ? new(p) ::TBfastmode[nElements] : new ::TBfastmode[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBfastmode(void *p) {
      delete (static_cast<::TBfastmode*>(p));
   }
   static void deleteArray_TBfastmode(void *p) {
      delete [] (static_cast<::TBfastmode*>(p));
   }
   static void destruct_TBfastmode(void *p) {
      typedef ::TBfastmode current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TBfastmode

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBmidbase(void *p) {
      return  p ? new(p) ::TBmidbase : new ::TBmidbase;
   }
   static void *newArray_TBmidbase(Long_t nElements, void *p) {
      return p ? new(p) ::TBmidbase[nElements] : new ::TBmidbase[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBmidbase(void *p) {
      delete (static_cast<::TBmidbase*>(p));
   }
   static void deleteArray_TBmidbase(void *p) {
      delete [] (static_cast<::TBmidbase*>(p));
   }
   static void destruct_TBmidbase(void *p) {
      typedef ::TBmidbase current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TBmidbase

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBmidlETBwaveformgR(void *p) {
      return  p ? new(p) ::TBmid<TBwaveform> : new ::TBmid<TBwaveform>;
   }
   static void *newArray_TBmidlETBwaveformgR(Long_t nElements, void *p) {
      return p ? new(p) ::TBmid<TBwaveform>[nElements] : new ::TBmid<TBwaveform>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBmidlETBwaveformgR(void *p) {
      delete (static_cast<::TBmid<TBwaveform>*>(p));
   }
   static void deleteArray_TBmidlETBwaveformgR(void *p) {
      delete [] (static_cast<::TBmid<TBwaveform>*>(p));
   }
   static void destruct_TBmidlETBwaveformgR(void *p) {
      typedef ::TBmid<TBwaveform> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TBmid<TBwaveform>

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBmidlETBfastmodegR(void *p) {
      return  p ? new(p) ::TBmid<TBfastmode> : new ::TBmid<TBfastmode>;
   }
   static void *newArray_TBmidlETBfastmodegR(Long_t nElements, void *p) {
      return p ? new(p) ::TBmid<TBfastmode>[nElements] : new ::TBmid<TBfastmode>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBmidlETBfastmodegR(void *p) {
      delete (static_cast<::TBmid<TBfastmode>*>(p));
   }
   static void deleteArray_TBmidlETBfastmodegR(void *p) {
      delete [] (static_cast<::TBmid<TBfastmode>*>(p));
   }
   static void destruct_TBmidlETBfastmodegR(void *p) {
      typedef ::TBmid<TBfastmode> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TBmid<TBfastmode>

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBcid(void *p) {
      return  p ? new(p) ::TBcid : new ::TBcid;
   }
   static void *newArray_TBcid(Long_t nElements, void *p) {
      return p ? new(p) ::TBcid[nElements] : new ::TBcid[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBcid(void *p) {
      delete (static_cast<::TBcid*>(p));
   }
   static void deleteArray_TBcid(void *p) {
      delete [] (static_cast<::TBcid*>(p));
   }
   static void destruct_TBcid(void *p) {
      typedef ::TBcid current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TBcid

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBevtlETBfastmodegR(void *p) {
      return  p ? new(p) ::TBevt<TBfastmode> : new ::TBevt<TBfastmode>;
   }
   static void *newArray_TBevtlETBfastmodegR(Long_t nElements, void *p) {
      return p ? new(p) ::TBevt<TBfastmode>[nElements] : new ::TBevt<TBfastmode>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBevtlETBfastmodegR(void *p) {
      delete (static_cast<::TBevt<TBfastmode>*>(p));
   }
   static void deleteArray_TBevtlETBfastmodegR(void *p) {
      delete [] (static_cast<::TBevt<TBfastmode>*>(p));
   }
   static void destruct_TBevtlETBfastmodegR(void *p) {
      typedef ::TBevt<TBfastmode> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TBevt<TBfastmode>

namespace ROOT {
   // Wrappers around operator new
   static void *new_TBevtlETBwaveformgR(void *p) {
      return  p ? new(p) ::TBevt<TBwaveform> : new ::TBevt<TBwaveform>;
   }
   static void *newArray_TBevtlETBwaveformgR(Long_t nElements, void *p) {
      return p ? new(p) ::TBevt<TBwaveform>[nElements] : new ::TBevt<TBwaveform>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TBevtlETBwaveformgR(void *p) {
      delete (static_cast<::TBevt<TBwaveform>*>(p));
   }
   static void deleteArray_TBevtlETBwaveformgR(void *p) {
      delete [] (static_cast<::TBevt<TBwaveform>*>(p));
   }
   static void destruct_TBevtlETBwaveformgR(void *p) {
      typedef ::TBevt<TBwaveform> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TBevt<TBwaveform>

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TBreadlETBfastmodegR(void *p) {
      delete (static_cast<::TBread<TBfastmode>*>(p));
   }
   static void deleteArray_TBreadlETBfastmodegR(void *p) {
      delete [] (static_cast<::TBread<TBfastmode>*>(p));
   }
   static void destruct_TBreadlETBfastmodegR(void *p) {
      typedef ::TBread<TBfastmode> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TBread<TBfastmode>

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TBreadlETBwaveformgR(void *p) {
      delete (static_cast<::TBread<TBwaveform>*>(p));
   }
   static void deleteArray_TBreadlETBwaveformgR(void *p) {
      delete [] (static_cast<::TBread<TBwaveform>*>(p));
   }
   static void destruct_TBreadlETBwaveformgR(void *p) {
      typedef ::TBread<TBwaveform> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::TBread<TBwaveform>

namespace ROOT {
   static TClass *vectorlEshortgR_Dictionary();
   static void vectorlEshortgR_TClassManip(TClass*);
   static void *new_vectorlEshortgR(void *p = nullptr);
   static void *newArray_vectorlEshortgR(Long_t size, void *p);
   static void delete_vectorlEshortgR(void *p);
   static void deleteArray_vectorlEshortgR(void *p);
   static void destruct_vectorlEshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<short>*)
   {
      vector<short> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<short>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<short>", -2, "vector", 389,
                  typeid(vector<short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEshortgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<short>) );
      instance.SetNew(&new_vectorlEshortgR);
      instance.SetNewArray(&newArray_vectorlEshortgR);
      instance.SetDelete(&delete_vectorlEshortgR);
      instance.SetDeleteArray(&deleteArray_vectorlEshortgR);
      instance.SetDestructor(&destruct_vectorlEshortgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<short> >()));

      ::ROOT::AddClassAlternate("vector<short>","std::vector<short, std::allocator<short> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<short>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<short>*>(nullptr))->GetClass();
      vectorlEshortgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEshortgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<short> : new vector<short>;
   }
   static void *newArray_vectorlEshortgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<short>[nElements] : new vector<short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEshortgR(void *p) {
      delete (static_cast<vector<short>*>(p));
   }
   static void deleteArray_vectorlEshortgR(void *p) {
      delete [] (static_cast<vector<short>*>(p));
   }
   static void destruct_vectorlEshortgR(void *p) {
      typedef vector<short> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<short>

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = nullptr);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 389,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));

      ::ROOT::AddClassAlternate("vector<int>","std::vector<int, std::allocator<int> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<int>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<int>*>(nullptr))->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete (static_cast<vector<int>*>(p));
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] (static_cast<vector<int>*>(p));
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlETBwaveformgR_Dictionary();
   static void vectorlETBwaveformgR_TClassManip(TClass*);
   static void *new_vectorlETBwaveformgR(void *p = nullptr);
   static void *newArray_vectorlETBwaveformgR(Long_t size, void *p);
   static void delete_vectorlETBwaveformgR(void *p);
   static void deleteArray_vectorlETBwaveformgR(void *p);
   static void destruct_vectorlETBwaveformgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TBwaveform>*)
   {
      vector<TBwaveform> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TBwaveform>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TBwaveform>", -2, "vector", 389,
                  typeid(vector<TBwaveform>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETBwaveformgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TBwaveform>) );
      instance.SetNew(&new_vectorlETBwaveformgR);
      instance.SetNewArray(&newArray_vectorlETBwaveformgR);
      instance.SetDelete(&delete_vectorlETBwaveformgR);
      instance.SetDeleteArray(&deleteArray_vectorlETBwaveformgR);
      instance.SetDestructor(&destruct_vectorlETBwaveformgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TBwaveform> >()));

      ::ROOT::AddClassAlternate("vector<TBwaveform>","std::vector<TBwaveform, std::allocator<TBwaveform> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<TBwaveform>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETBwaveformgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<TBwaveform>*>(nullptr))->GetClass();
      vectorlETBwaveformgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETBwaveformgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETBwaveformgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TBwaveform> : new vector<TBwaveform>;
   }
   static void *newArray_vectorlETBwaveformgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TBwaveform>[nElements] : new vector<TBwaveform>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETBwaveformgR(void *p) {
      delete (static_cast<vector<TBwaveform>*>(p));
   }
   static void deleteArray_vectorlETBwaveformgR(void *p) {
      delete [] (static_cast<vector<TBwaveform>*>(p));
   }
   static void destruct_vectorlETBwaveformgR(void *p) {
      typedef vector<TBwaveform> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<TBwaveform>

namespace ROOT {
   static TClass *vectorlETBfastmodegR_Dictionary();
   static void vectorlETBfastmodegR_TClassManip(TClass*);
   static void *new_vectorlETBfastmodegR(void *p = nullptr);
   static void *newArray_vectorlETBfastmodegR(Long_t size, void *p);
   static void delete_vectorlETBfastmodegR(void *p);
   static void deleteArray_vectorlETBfastmodegR(void *p);
   static void destruct_vectorlETBfastmodegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TBfastmode>*)
   {
      vector<TBfastmode> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TBfastmode>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TBfastmode>", -2, "vector", 389,
                  typeid(vector<TBfastmode>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETBfastmodegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TBfastmode>) );
      instance.SetNew(&new_vectorlETBfastmodegR);
      instance.SetNewArray(&newArray_vectorlETBfastmodegR);
      instance.SetDelete(&delete_vectorlETBfastmodegR);
      instance.SetDeleteArray(&deleteArray_vectorlETBfastmodegR);
      instance.SetDestructor(&destruct_vectorlETBfastmodegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TBfastmode> >()));

      ::ROOT::AddClassAlternate("vector<TBfastmode>","std::vector<TBfastmode, std::allocator<TBfastmode> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<TBfastmode>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETBfastmodegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<TBfastmode>*>(nullptr))->GetClass();
      vectorlETBfastmodegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETBfastmodegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETBfastmodegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TBfastmode> : new vector<TBfastmode>;
   }
   static void *newArray_vectorlETBfastmodegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TBfastmode>[nElements] : new vector<TBfastmode>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETBfastmodegR(void *p) {
      delete (static_cast<vector<TBfastmode>*>(p));
   }
   static void deleteArray_vectorlETBfastmodegR(void *p) {
      delete [] (static_cast<vector<TBfastmode>*>(p));
   }
   static void destruct_vectorlETBfastmodegR(void *p) {
      typedef vector<TBfastmode> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<TBfastmode>

namespace ROOT {
   static TClass *maplEintcOTBmidlETBwaveformgRsPgR_Dictionary();
   static void maplEintcOTBmidlETBwaveformgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOTBmidlETBwaveformgRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOTBmidlETBwaveformgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOTBmidlETBwaveformgRsPgR(void *p);
   static void deleteArray_maplEintcOTBmidlETBwaveformgRsPgR(void *p);
   static void destruct_maplEintcOTBmidlETBwaveformgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,TBmid<TBwaveform> >*)
   {
      map<int,TBmid<TBwaveform> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,TBmid<TBwaveform> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,TBmid<TBwaveform> >", -2, "map", 100,
                  typeid(map<int,TBmid<TBwaveform> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOTBmidlETBwaveformgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,TBmid<TBwaveform> >) );
      instance.SetNew(&new_maplEintcOTBmidlETBwaveformgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOTBmidlETBwaveformgRsPgR);
      instance.SetDelete(&delete_maplEintcOTBmidlETBwaveformgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOTBmidlETBwaveformgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOTBmidlETBwaveformgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,TBmid<TBwaveform> > >()));

      ::ROOT::AddClassAlternate("map<int,TBmid<TBwaveform> >","std::map<int, TBmid<TBwaveform>, std::less<int>, std::allocator<std::pair<int const, TBmid<TBwaveform> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<int,TBmid<TBwaveform> >*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOTBmidlETBwaveformgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<int,TBmid<TBwaveform> >*>(nullptr))->GetClass();
      maplEintcOTBmidlETBwaveformgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOTBmidlETBwaveformgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOTBmidlETBwaveformgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TBmid<TBwaveform> > : new map<int,TBmid<TBwaveform> >;
   }
   static void *newArray_maplEintcOTBmidlETBwaveformgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TBmid<TBwaveform> >[nElements] : new map<int,TBmid<TBwaveform> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOTBmidlETBwaveformgRsPgR(void *p) {
      delete (static_cast<map<int,TBmid<TBwaveform> >*>(p));
   }
   static void deleteArray_maplEintcOTBmidlETBwaveformgRsPgR(void *p) {
      delete [] (static_cast<map<int,TBmid<TBwaveform> >*>(p));
   }
   static void destruct_maplEintcOTBmidlETBwaveformgRsPgR(void *p) {
      typedef map<int,TBmid<TBwaveform> > current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<int,TBmid<TBwaveform> >

namespace ROOT {
   static TClass *maplEintcOTBmidlETBfastmodegRsPgR_Dictionary();
   static void maplEintcOTBmidlETBfastmodegRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOTBmidlETBfastmodegRsPgR(void *p = nullptr);
   static void *newArray_maplEintcOTBmidlETBfastmodegRsPgR(Long_t size, void *p);
   static void delete_maplEintcOTBmidlETBfastmodegRsPgR(void *p);
   static void deleteArray_maplEintcOTBmidlETBfastmodegRsPgR(void *p);
   static void destruct_maplEintcOTBmidlETBfastmodegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,TBmid<TBfastmode> >*)
   {
      map<int,TBmid<TBfastmode> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,TBmid<TBfastmode> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,TBmid<TBfastmode> >", -2, "map", 100,
                  typeid(map<int,TBmid<TBfastmode> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOTBmidlETBfastmodegRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,TBmid<TBfastmode> >) );
      instance.SetNew(&new_maplEintcOTBmidlETBfastmodegRsPgR);
      instance.SetNewArray(&newArray_maplEintcOTBmidlETBfastmodegRsPgR);
      instance.SetDelete(&delete_maplEintcOTBmidlETBfastmodegRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOTBmidlETBfastmodegRsPgR);
      instance.SetDestructor(&destruct_maplEintcOTBmidlETBfastmodegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,TBmid<TBfastmode> > >()));

      ::ROOT::AddClassAlternate("map<int,TBmid<TBfastmode> >","std::map<int, TBmid<TBfastmode>, std::less<int>, std::allocator<std::pair<int const, TBmid<TBfastmode> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<int,TBmid<TBfastmode> >*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOTBmidlETBfastmodegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<int,TBmid<TBfastmode> >*>(nullptr))->GetClass();
      maplEintcOTBmidlETBfastmodegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOTBmidlETBfastmodegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOTBmidlETBfastmodegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TBmid<TBfastmode> > : new map<int,TBmid<TBfastmode> >;
   }
   static void *newArray_maplEintcOTBmidlETBfastmodegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,TBmid<TBfastmode> >[nElements] : new map<int,TBmid<TBfastmode> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOTBmidlETBfastmodegRsPgR(void *p) {
      delete (static_cast<map<int,TBmid<TBfastmode> >*>(p));
   }
   static void deleteArray_maplEintcOTBmidlETBfastmodegRsPgR(void *p) {
      delete [] (static_cast<map<int,TBmid<TBfastmode> >*>(p));
   }
   static void destruct_maplEintcOTBmidlETBfastmodegRsPgR(void *p) {
      typedef map<int,TBmid<TBfastmode> > current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<int,TBmid<TBfastmode> >

namespace ROOT {
   static TClass *maplEintcOFileControllerlETBwaveformgRmUgR_Dictionary();
   static void maplEintcOFileControllerlETBwaveformgRmUgR_TClassManip(TClass*);
   static void *new_maplEintcOFileControllerlETBwaveformgRmUgR(void *p = nullptr);
   static void *newArray_maplEintcOFileControllerlETBwaveformgRmUgR(Long_t size, void *p);
   static void delete_maplEintcOFileControllerlETBwaveformgRmUgR(void *p);
   static void deleteArray_maplEintcOFileControllerlETBwaveformgRmUgR(void *p);
   static void destruct_maplEintcOFileControllerlETBwaveformgRmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,FileController<TBwaveform>*>*)
   {
      map<int,FileController<TBwaveform>*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,FileController<TBwaveform>*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,FileController<TBwaveform>*>", -2, "map", 100,
                  typeid(map<int,FileController<TBwaveform>*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOFileControllerlETBwaveformgRmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,FileController<TBwaveform>*>) );
      instance.SetNew(&new_maplEintcOFileControllerlETBwaveformgRmUgR);
      instance.SetNewArray(&newArray_maplEintcOFileControllerlETBwaveformgRmUgR);
      instance.SetDelete(&delete_maplEintcOFileControllerlETBwaveformgRmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOFileControllerlETBwaveformgRmUgR);
      instance.SetDestructor(&destruct_maplEintcOFileControllerlETBwaveformgRmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,FileController<TBwaveform>*> >()));

      ::ROOT::AddClassAlternate("map<int,FileController<TBwaveform>*>","std::map<int, FileController<TBwaveform>*, std::less<int>, std::allocator<std::pair<int const, FileController<TBwaveform>*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<int,FileController<TBwaveform>*>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOFileControllerlETBwaveformgRmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<int,FileController<TBwaveform>*>*>(nullptr))->GetClass();
      maplEintcOFileControllerlETBwaveformgRmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOFileControllerlETBwaveformgRmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOFileControllerlETBwaveformgRmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,FileController<TBwaveform>*> : new map<int,FileController<TBwaveform>*>;
   }
   static void *newArray_maplEintcOFileControllerlETBwaveformgRmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,FileController<TBwaveform>*>[nElements] : new map<int,FileController<TBwaveform>*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOFileControllerlETBwaveformgRmUgR(void *p) {
      delete (static_cast<map<int,FileController<TBwaveform>*>*>(p));
   }
   static void deleteArray_maplEintcOFileControllerlETBwaveformgRmUgR(void *p) {
      delete [] (static_cast<map<int,FileController<TBwaveform>*>*>(p));
   }
   static void destruct_maplEintcOFileControllerlETBwaveformgRmUgR(void *p) {
      typedef map<int,FileController<TBwaveform>*> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<int,FileController<TBwaveform>*>

namespace ROOT {
   static TClass *maplEintcOFileControllerlETBfastmodegRmUgR_Dictionary();
   static void maplEintcOFileControllerlETBfastmodegRmUgR_TClassManip(TClass*);
   static void *new_maplEintcOFileControllerlETBfastmodegRmUgR(void *p = nullptr);
   static void *newArray_maplEintcOFileControllerlETBfastmodegRmUgR(Long_t size, void *p);
   static void delete_maplEintcOFileControllerlETBfastmodegRmUgR(void *p);
   static void deleteArray_maplEintcOFileControllerlETBfastmodegRmUgR(void *p);
   static void destruct_maplEintcOFileControllerlETBfastmodegRmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,FileController<TBfastmode>*>*)
   {
      map<int,FileController<TBfastmode>*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,FileController<TBfastmode>*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,FileController<TBfastmode>*>", -2, "map", 100,
                  typeid(map<int,FileController<TBfastmode>*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOFileControllerlETBfastmodegRmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,FileController<TBfastmode>*>) );
      instance.SetNew(&new_maplEintcOFileControllerlETBfastmodegRmUgR);
      instance.SetNewArray(&newArray_maplEintcOFileControllerlETBfastmodegRmUgR);
      instance.SetDelete(&delete_maplEintcOFileControllerlETBfastmodegRmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOFileControllerlETBfastmodegRmUgR);
      instance.SetDestructor(&destruct_maplEintcOFileControllerlETBfastmodegRmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,FileController<TBfastmode>*> >()));

      ::ROOT::AddClassAlternate("map<int,FileController<TBfastmode>*>","std::map<int, FileController<TBfastmode>*, std::less<int>, std::allocator<std::pair<int const, FileController<TBfastmode>*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<int,FileController<TBfastmode>*>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOFileControllerlETBfastmodegRmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<int,FileController<TBfastmode>*>*>(nullptr))->GetClass();
      maplEintcOFileControllerlETBfastmodegRmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOFileControllerlETBfastmodegRmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOFileControllerlETBfastmodegRmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,FileController<TBfastmode>*> : new map<int,FileController<TBfastmode>*>;
   }
   static void *newArray_maplEintcOFileControllerlETBfastmodegRmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,FileController<TBfastmode>*>[nElements] : new map<int,FileController<TBfastmode>*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOFileControllerlETBfastmodegRmUgR(void *p) {
      delete (static_cast<map<int,FileController<TBfastmode>*>*>(p));
   }
   static void deleteArray_maplEintcOFileControllerlETBfastmodegRmUgR(void *p) {
      delete [] (static_cast<map<int,FileController<TBfastmode>*>*>(p));
   }
   static void destruct_maplEintcOFileControllerlETBfastmodegRmUgR(void *p) {
      typedef map<int,FileController<TBfastmode>*> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<int,FileController<TBfastmode>*>

namespace {
  void TriggerDictionaryInitialization_libdrcTB_Impl() {
    static const char* headers[] = {
"/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBdetector.h",
"/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBevt.h",
"/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBmid.h",
"/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBobject.h",
"/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBread.h",
"/u/user/eoyun/TB2025/25.08.09/TB2025/include/TButility.h",
nullptr
    };
    static const char* includePaths[] = {
"/u/user/eoyun/TB2025/25.08.09/TB2025/include",
"/cvmfs/sft.cern.ch/lcg/views/LCG_104/x86_64-el9-gcc11-opt/include",
"/u/user/eoyun/TB2025/25.08.09/TB2025",
"/cvmfs/sft.cern.ch/lcg/releases/ROOT/6.28.04-740f6/x86_64-el9-gcc11-opt/include/",
"/u/user/eoyun/TB2025/25.08.09/TB2025/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libdrcTB dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$TBmid.h")))  TBwaveform;
class __attribute__((annotate("$clingAutoload$TBmid.h")))  TBfastmode;
class __attribute__((annotate("$clingAutoload$TBmid.h")))  TBmidbase;
template <class T> class __attribute__((annotate("$clingAutoload$TBmid.h")))  TBmid;

class __attribute__((annotate("$clingAutoload$/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBdetector.h")))  TBcid;
template <class T> class __attribute__((annotate("$clingAutoload$/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBevt.h")))  TBevt;

template <class T> class __attribute__((annotate("$clingAutoload$TBread.h")))  __attribute__((annotate("$clingAutoload$/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBobject.h")))  TBread;

)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libdrcTB dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBdetector.h"
#include "/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBevt.h"
#include "/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBmid.h"
#include "/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBobject.h"
#include "/u/user/eoyun/TB2025/25.08.09/TB2025/include/TBread.h"
#include "/u/user/eoyun/TB2025/25.08.09/TB2025/include/TButility.h"
#include "TBmid.h"

#if defined(__CLING__) || defined(__CINT__)
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class TBcid + ;
#pragma link C++ class TBwaveform + ;
#pragma link C++ class TBfastmode + ;

#pragma link C++ class TBread < TBwaveform> + ;
#pragma link C++ class TBread < TBfastmode> + ;

#pragma link C++ class TBmidbase + ;
#pragma link C++ class TBmid < TBwaveform> + ;
#pragma link C++ class TBmid < TBfastmode> + ;
#pragma link C++ class TBevt < TBwaveform> + ;
#pragma link C++ class TBevt < TBfastmode> + ;

#endif

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"TBcid", payloadCode, "@",
"TBevt<TBfastmode>", payloadCode, "@",
"TBevt<TBwaveform>", payloadCode, "@",
"TBfastmode", payloadCode, "@",
"TBmid<TBfastmode>", payloadCode, "@",
"TBmid<TBwaveform>", payloadCode, "@",
"TBmidbase", payloadCode, "@",
"TBread<TBfastmode>", payloadCode, "@",
"TBread<TBwaveform>", payloadCode, "@",
"TBwaveform", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libdrcTB",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libdrcTB_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libdrcTB_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libdrcTB() {
  TriggerDictionaryInitialization_libdrcTB_Impl();
}
