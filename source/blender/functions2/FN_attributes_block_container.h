#ifndef __FN_ATTRIBUTES_BLOCK_CONTAINER_H__
#define __FN_ATTRIBUTES_BLOCK_CONTAINER_H__

#include <mutex>

#include "FN_attributes_ref.h"

namespace FN {

class AttributesBlock;

class AttributesBlockContainer : BLI::NonCopyable, BLI::NonMovable {
 private:
  AttributesInfo m_attributes_info;
  uint m_block_size;
  VectorSet<AttributesBlock *> m_active_blocks;
  std::mutex m_blocks_mutex;

 public:
  AttributesBlockContainer(AttributesInfo attributes_info, uint block_size);
  ~AttributesBlockContainer();

  const AttributesInfo &info() const
  {
    return m_attributes_info;
  }

  uint block_size() const
  {
    return m_block_size;
  }

  AttributesBlock &new_block();
  void release_block(AttributesBlock &block);
};

class AttributesBlock : BLI::NonCopyable, BLI::NonMovable {
 private:
  AttributesBlockContainer &m_owner;
  Vector<void *> m_buffers;
  uint m_used_size;

 public:
  AttributesBlock(AttributesBlockContainer &owner);
  ~AttributesBlock();

  const AttributesInfo &info() const
  {
    return m_owner.info();
  }

  uint used_size() const
  {
    return m_used_size;
  }

  uint capacity() const
  {
    return m_owner.block_size();
  }

  IndexRange used_range() const
  {
    return IndexRange(m_used_size);
  }

  void set_used_size(uint new_used_size)
  {
    BLI_assert(new_used_size <= this->capacity());
    m_used_size = new_used_size;
  }

  AttributesBlockContainer &owner()
  {
    return m_owner;
  }

  AttributesRef as_ref()
  {
    return AttributesRef(m_owner.info(), m_buffers, m_used_size);
  }

  AttributesRef as_ref__all()
  {
    return AttributesRef(m_owner.info(), m_buffers, this->capacity());
  }
};

}  // namespace FN

#endif /* __FN_ATTRIBUTES_BLOCK_CONTAINER_H__ */
