#ifndef SN_ITOR_WRAPPER_H
#define SN_ITOR_WRAPPER_H

#include "core/common/types.h"

template <typename T, typename ItorType, typename ValType>
class ItorWrapper {
public:
  ItorWrapper(ItorType begin, ItorType end)
    : m_Begin(begin)
    , m_Current(begin)
    , m_End(end) {}

  ItorType &Begin() { return m_Begin; }
  ItorType &Current() { return m_Current; }
  ItorType &End() { return m_End; }

  const ItorType &CBegin() const { return m_Begin; }
  const ItorType &CCurrent() const { return m_Current; }
  const ItorType &CEnd() const { return m_End; }

  void GotoBegin() { m_Current = m_Begin; }
  void GotoEnd() { m_Current = m_End; }

  b8 Advance() {
    if (HasMore()) {
      ++m_Current;
      return true;
    }
    return false;
  };
  b8 HasMore() const { return m_Current != m_End; }

protected:
  ItorType m_Begin;
  ItorType m_Current;
  ItorType m_End;
};

template <typename VecType>
class StdVectorItor
  : public ItorWrapper<VecType, typename VecType::iterator, typename VecType::value_type> {
public:
  typedef typename VecType::iterator VecItorType;
  typedef typename VecType::value_type ValType;

  StdVectorItor(VecItorType start, VecItorType end)
    : ItorWrapper<VecType, VecItorType, ValType>(start, end) {}

  explicit StdVectorItor(VecType &vec)
    : ItorWrapper<VecType, VecItorType, ValType>(vec.begin(), vec.end()) {}

  ValType PeekVal() const { return *this->m_Current; }

  ValType *PeekValPtr() const { return this->m_Current; }
};

template <typename MapType>
class MapItor
  : public ItorWrapper<MapType, typename MapType::iterator, typename MapType::mapped_type> {
public:
  typedef typename MapType::iterator MapItorType;
  typedef typename MapType::mapped_type MapValType;
  typedef typename MapType::key_type KeyType;
  typedef typename MapType::value_type::second_type ValType;

  MapItor(MapItorType start, MapItorType end)
    : ItorWrapper<MapType, MapItorType, MapValType>(start, end) {}

  explicit MapItor(MapType &map)
    : ItorWrapper<MapType, MapItorType, MapValType>(map.begin(), map.end()) {}

  KeyType PeekKey() { return this->m_Current->first; }

  ValType PeekVal() { return this->m_Current->second; }

  ValType *PeekValPtr() { return &(this->m_Current->second); }
};

#endif // !SN_ITOR_WRAPPER_H
