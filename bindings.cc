#include <cstdlib>

static std::unordered_set<void *> alreadyDestroyed;

extern "C"
{

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

  void *deallocationCtx()
  {
    return &alreadyDestroyed;
  }

  void dispose(void *t)
  {
    auto *buffer = reinterpret_cast<float *>(t);
    alreadyDestroyed.insert(buffer);
    delete buffer;
  }
}