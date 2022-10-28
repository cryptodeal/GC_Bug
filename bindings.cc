#include <cstdlib>
#include <iostream>
#include <unordered_set>

static std::unordered_set<float *> alreadyDestroyed;

extern "C" {

void *createBuffer(int64_t numel) {
  // this is weird but technically correct C++ syntax, don't copy this lol
  auto out = new float[numel]();
  std::cerr << "create " << out << "\n";
  return out;
}

void destroyBuffer(void *t, void * /*ignore*/) {
  auto *buffer = reinterpret_cast<float *>(t);
  std::cerr << "GC destroy: " << t << "\n";
  if (alreadyDestroyed.find(buffer) == alreadyDestroyed.end()) {
    delete buffer;
  } else {
    alreadyDestroyed.erase(alreadyDestroyed.find(buffer));
  }
}

void dispose(void *t) {
  std::cerr << "manual dispose: " << t << "\n";
  auto *buffer = reinterpret_cast<float *>(t);
  alreadyDestroyed.insert(buffer);
  delete buffer;
}

typedef void (*JSTypedArrayBytesDeallocator)(void *bytes,
                                             void *deallocatorContext);

JSTypedArrayBytesDeallocator genBufferDestroyer() { return destroyBuffer; }
}
