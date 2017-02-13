#include "core/path.h"

namespace Engine {

  class Resource {
    friend class ResourceManager;
    public:
      enum class State : uint32_t {
        EMPTY = 0,
        LOADING,
        READY,
        UNLOADING,
        FAILURE
      };
      // this is the list of function pointers for observer
      // callback but don't dick around with this rn
      // typedef DelegateList<void (State, State)> ObserverCallback;

      State getState() const { return _state; }
      bool isEmpty() const { return State::EMPTY == _state; }
      bool isLoading() const { return State::LOADING == _state; }
      bool isReady() const { return State::READY == _state; }
      bool isUnloading() const { return State::UNLOADING == _state; }
      bool isFailure() const { return State::FAILURE == _state; }

    protected:
      Resource(const Path& path, ResourceManager& manager, Allocator& allocator);
      virtual ~Resource();
    
      virtual void onReady(void);
      void onEmpty(void);
      void onLoading(void);
      void onUnloading(void);
      void onReloading(void);
      void onFailure(void);
 
      void doLoad(void);
      virtual void doUnload(void) = 0;
      virtual void loaded(FS::File& file, bool success, FS::FileSystem& fs) = 0;

      uint32_t addRef(void) { return ++_ref_count; }
      uint32_t removeRef(void) { return --_ref_count; }

      void addDependency(Resource& dependent_resource);
      void removeDependency(Resource& dependent_resource);
      void onStateChange(State old_state, State new_state);
      void incrementDepCount();
      void decrementDepCount();
 
    protected:
      Path _path;
      size_t _size;
      ResourceDeity& _manager;

    private:
      void operator = (const Resource&);
      uint32_t _ref_count;
      uint32_t _dep_count;
      State _state;
  };
}
