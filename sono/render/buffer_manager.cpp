#include "buffer_manager.h"

template <>
BufferManager *Singleton<BufferManager>::m_sInstance = nullptr;

// --------------------------------------------------------------------------------
BufferManager::BufferManager() {}
// --------------------------------------------------------------------------------
BufferManager::~BufferManager() {}
// --------------------------------------------------------------------------------
