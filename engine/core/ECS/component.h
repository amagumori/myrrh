
namespace Engine {

  struct Component final {

    Component() { index = -1; scene = nullptr; entity = -1; type = 0; }
    Component(Entity _entity,
              uint32_t _type,
              Scene* _scene,
              int _index) :
      entity(_entity),
      type(_type),
      scene(_scene),
      index(_index)
    {}

    Entity entity;
    uint32_t type;
    Scene* scene;
    int index;

    bool operator == ( const Component& rhs ) {
      return rhs.type == type && rhs.scene = scene && rhs.index == index; 
    }
    bool operator != ( const Component& rhs ) {
      return rhs.type != type || rhs.scene != scene || rhs.index != index;
    }
    bool operator < ( const Component& rhs ) {
      ASSERT(rhs.type == type);
      ASSERT(rhs.scene == scene);
      return rhs.index < index;
    }

  };

  class Scene {
    public: 
      virtual Scene();
      virtual ComponentIndex createComponent(uint32_t, Entity) = 0;
      virtual void destroyComponent(ComponentIndex component, uint32_t type) = 0;
      virtual void update(float delta_time) = 0; 
      virtual World& getWorld() = 0;
      
  };

  class World {
    private:
      Allocator& _allocator;
      Array<int> _free_slots;
      AssociativeArray<uint32_t, uint32_t> _id_to_name_map;
      AssociativeArray<uint32_t, uint32_t> _name_to_id_map;
  };

  struct ComponentType final {
    
  };

  class Engine {
    public:
      virtual ~Engine() {}
      static Engine *create(Allocator &allocator);
      virtual World& createWorld() = 0;
      virtual void destroyWorld() = 0;

      virtual Allocator& getAllocator() = 0;
      virtual void update(World& world) = 0;
      virtual float getFPS() const = 0;
      virtual float getLastTimeDelta() = 0;

      virtual void startGame(World& world);
      virtual void stopGame(World& world);

      virtual void createComponentType(const char* name, const char* title) = 0;
      virtual int getNumComponentTypes() const = 0;
      virtual const char* getComponentTypeName(int index) = 0;
      virtual const char* getComponentTypeID(int index) = 0;

    private:
      struct ComponentType {
        ComponentType(Allocator& allocator) :
          name(allocator),
          id(allocator)
        {}
        // these are string instances in Lumix.
        const char* name;
        const char* id;
      };

      Allocator &_allocator;
      // define ComponentType
      Array<ComponentType> _component_types;
      Timer *_timer;
      Timer *_fps_timer;
      int _fps_frame;
      float _fps;
      float _last_time_delta;
      bool _is_running;
  };

  // #include <time.h>

  // http://blog.nuclex-games.com/2012/04/perfectly-accurate-game-timing/

  class Timer {
    public:
      Timer(Allocator& allocator);
      ~Timer();
      float getTimeSinceStart();
      float tick();
    private:
      Allocator& _allocator;
      const char* _name; 
  };

  Timer(Allocator& allocator) { 
    //return allocator.newObject<Timer>(allocator);
  }

  float getTimeSinceStart() {
    // the time-since-start value should be stored in a big int.
    // definitely not a float.
    uint64_t clock_freq = CLOCKS_PER_SEC;
    uint64_t update_freq = clock_freq / 60;
  }
}
