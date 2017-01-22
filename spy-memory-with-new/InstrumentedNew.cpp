// Our special new must allocate memory as expected...
#include <cstdio>
#include <cstdlib>
// ...but also inspect the stack and print som e results.
#include <execinfo.h>
#include <unistd.h>
#include <fstream>
// Import bad_alloc, expected in case of errors.
#include <new>
//
/* Opens (once) and return the file to save the results.. */
static std::ofstream& resultFile() {
  static std::ofstream memoryProfile;
  static bool open = false;  // Init on 1st use, as usual.
    if (! open) {
      memoryProfile.open ("allocations.txt");
      open = true;
    }
    // Else, handle errors, close the file...
    // We won't do it, to keep the example simple.
   return memoryProfile;
}
//
/* This is the "magic" function that inspect the stack and writes it in a file. */
static void dumpStackTrace(std::ofstream& memoryProfile) {
  // Record 15 pointers to stackk frame - enough for the example program.
  const int maximumStackSize = 15;
  void *callStack[maximumStackSize];
  size_t framesInUse = backtrace(callStack, maximumStackSize);
  // Now callStack is full of pointers. Request the names of the functions matching each frame.
  char ** mangledFunctionNames = backtrace_symbols(callStack, framesInUse);
  // Scrive tutte le stringhe con i nomi delle funzioni nello stream per il debug.
  for (int i = 0; i < framesInUse; ++i)
    memoryProfile << mangledFunctionNames[i] << std::endl;
  // To be fair, we should release mangledFunctionNames with free...
}
//
/* Now we have all the elements to build the custom operator new. */
void* operator new(std::size_t sz) {
    // Allocate the requested memory for the caller.
    void * requestedMemory = std::malloc(sz);
    if (! requestedMemory)
      throw std::bad_alloc();
    // Share our allocations with the world.
    std::ofstream& memoryProfile = resultFile();
    memoryProfile << "Allocation, size = " << sz << " at " << (unsigned long int) requestedMemory << std::endl;
    dumpStackTrace(memoryProfile);
    memoryProfile << "-----------" << std::endl;  // Poor man's separator.
    
    return requestedMemory;
}