#include <cstdlib>
#include <unordered_set>

static std::unordered_set<float *> alreadyDestroyed;

extern "C"
{

  void* createBuffer(int64_t numel) {
    // this is weird but technically correct C++ syntax, don't copy this lol
    return new float[numel]();
  }

  void destroyBuffer(void *t, void * /*ignore*/)
  {
    auto *buffer = reinterpret_cast<float *>(t);
    if (alreadyDestroyed.find(buffer) == alreadyDestroyed.end())
    {
      // std::cout << t << " was not found in `alreadyDestroyed`" << std::endl;
      delete buffer;
    }
    else
    {
      // std::cout << "found " << t << " in `alreadyDestroyed`" << std::endl;
      alreadyDestroyed.erase(alreadyDestroyed.find(buffer));
    }
  }

  void dispose(void *t)
  {
    auto *buffer = reinterpret_cast<float *>(t);
    alreadyDestroyed.insert(buffer);
    delete buffer;
  }

  typedef void (*JSTypedArrayBytesDeallocator)(void *bytes,
                                               void *deallocatorContext);

  JSTypedArrayBytesDeallocator genBufferDestroyer()
  {
    return destroyBuffer;
  }
}
