/* Program that allocates some memory when it feels like.
   No delete - today's essay is not about memory leaks.*/
#include <string>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "SomeClass.h"
//
void h() {
  SomeClass* t = new SomeClass();
}
void g() {  h(); }
void f() {  g(); }
void MakeSomeClass() {  f(); }
//
int main(int argc, char **argv) {
   int * number = new int(89);
    std::string * test = new std::string("abc");
//
    SomeClass * oggetto = new SomeClass();
    SomeClass();
//
    boost::shared_ptr<SomeClass> smartPointer = boost::make_shared<SomeClass>();
    std::shared_ptr<SomeClass> stdSmartPointer = std::make_shared<SomeClass>();
    return 0;
}