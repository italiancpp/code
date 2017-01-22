#ifndef SOMECLASS
#define SOMECLASS

#include <vector>

// Nothing special here, except the vector with a big content.
class SomeClass
{
public:
  SomeClass();
  
private:
  std::vector<char> tenMegabytes;
};

#endif
