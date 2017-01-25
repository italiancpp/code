/* Program that allocates some memory when it feels like.
   No delete - today's essay is not about memory leaks.*/
#include <string>
#include <memory>
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
    MakeSomeClass();
//
// The boost::shared_ptr example has been removed to simplify the dependencies.
//
    std::shared_ptr<SomeClass> stdSmartPointer = std::make_shared<SomeClass>();
    return 0;
}