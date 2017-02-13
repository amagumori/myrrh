#pragma once

#include "associative_array.h"

namespace Engine {

  class PathInternal {
    public:
      char _path[MAX_PATH_LENGTH];
      uint32_t _id;
      volatile int32_t _ref_count;
  };

  class PathManager {
    public:
      PathManager();
      ~PathManager();
      void serialize(OutputBinary& serializer);
      void deserialize(InputBinary& serializer);

      void clear();
      
    private:
      PathInternal* getPath(uint32_t hash, const char* path);
      PathInternal* getPath(uint32_t hash);
      PathInternal* getPathMultithreadUnsafe(uint32_t hash, const char* path);
      void incrementRefCount(PathInternal* path);
      void decrementRefcount(PathInternal* path);

    private:
      DefaultAllocator _src_allocator;
      BaseProxyAllocator _allocator;
      AssociativeArray<uint32_t, PathInternal*> _paths;
      MT::SpinMutex _mutex;
      PathInternal* _empty_path;
  };

  extern PathManager g_path_manager;

  class Path {
    public:
      Path();
      Path(const Path& rhs);
      explicit Path(uint32_t hash);
      explicit Path(const char* path);
      void operator=(const Path& rhs);
      void operator=(const char* rhs);
      void operator==(const Path& rhs) const {
        return _data->_id == rhs._data->_id;
      }
      ~Path();

      operator const char*() const { return _data->_path; }
      operator uint32_t() const { return _data->_id; }
      uint32_t getHash() const { return _data->_id; }
      const char* c_str() const { return _data->_path; }
      size_t length() const { return strlen(_data->_path); }
      bool isValid() const { return _data->_path[0] != '\0'; }

    private:
      PathInternal* _data;
  };

}
