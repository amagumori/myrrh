#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "../Entity.h"

namespace Engine {

  // this goes in its own ComponentManager.h.

  class ComponentManager {

    // make it all public for now? 
    public:
   
      struct Instance { int i; };

      // implement a hashmap.  HashMap<Entity, unsigned int> _map;

      Instance make_instance(int i) { 
        Instance inst = {};
        return inst;
      }

      Instance lookup(Entity e) {
        return make_instance(_map.get(e, 0)); 
      }

    private:

  }

  class TransformComponentManager : ComponentManager {
    public:

      struct InstanceData {
        unsigned size;      // # of used entries in arrays.
        unsigned capacity;  // # allocated entries in arrays.
        void *buffer;       // raw buffer of data.
        
        Entity *entity;     // * to the entity owning this instance.
        mat4 *local;        // local transform wrt parent
        mat4 *world;        // world transform
        Instance *parent;   // parent instance of this
        Instance *first_child;  // child instance of this
        Instance *next_sibling; // next sibling instance of this
        Instance *prev_sibling; // prev sibling instance of this
      };

      InstanceData _data;

      void set_local(Instance i, const mat4 &m);
      void transform(const mat4 &parent, Instance i);

    private:
  };

}

#endif
