#ifndef _ENTITY_H
#define _ENTITY_H

#include <cstdlib>
#include "common/collection_types.h"
#include "types.h"

#define MINIMUM_FREE_INDICES 1024

const unsigned int ENTITY_INDEX_BITS = 22;
const unsigned int ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS)-1;
const unsigned int ENTITY_GENERATION_BITS = 8;
const unsigned int ENTITY_GENERATION_MASK = (1<<ENTITY_GENERATION_BITS)-1;

namespace Engine {

  struct Entity {
    unsigned int id;

    // bit set to reference components owned by this entity.
    // bit set representation also makes filtering components
    // fast and straightforward.

    u64 componentBitSet;

    unsigned int index() const {
      return id & ENTITY_INDEX_MASK;
    }

    unsigned int generation() const {
      return (id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK;
    }
  };

  class EntityManager {
    foundation::Array<unsigned char> _generation;
    foundation::Queue<unsigned> _free_indices;
    public: 
      Entity create() {
        unsigned int idx;
        if (_free_indices.size() > MINIMUM_FREE_INDICES) {
          idx = _free_indices.front();
          _free_indices.pop_front();
        } else {
          _generation.push_back();
          idx = _generation.size() - 1;
          //XENSURE(idx < (1 << ENTITY_INDEX_BITS));
        }
        return make_entity(idx, _generation[idx]);
      }
      bool alive(Entity e) {
        return _generation[e.index()] == e.generation();
      }
      void destroy(Entity e) {
        const unsigned int idx = e.index();
        ++_generation[idx];
        _free_indices.push_back(idx);
      }
    private: 
  };
}

#endif
