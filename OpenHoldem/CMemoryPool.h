#pragma once
//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Memory-pool
//   to avoid allocating countless oversized 4KB blocks
//   when we want to create only small objects.
//   http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=20996
//
//******************************************************************************

#include <afxtempl.h>
#include <map>

void CreateMemoryPools();
void DeleteAllMemoryPools();

class CMemoryPool {
 public:
  CMemoryPool();
  ~CMemoryPool();
 public:
  void* Allocate(size_t size);
  void ReleaseAll();
 private:
  void AllocateNewMemoryBlock();
  void AlignNextMemoryBlock();
  size_t BytesAvailableInCurrentBlock();
 private:
  bool _all_emmory_released;
  int _bytes_used_in_current_block;
  void* _current_memory_block;
 private:
  // For gouse-keeping
  // All blocks to be released at some point
  CArray <void*, void*> _memory_blocks;
};

extern CMemoryPool *p_memory_pool_tablemaps;
extern CMemoryPool *p_memory_pool_scraper;
extern CMemoryPool *p_memory_pool_user_logic;
extern CMemoryPool *p_memory_pool_library_logic;
extern CMemoryPool *p_memory_pool_global;

// Selects either p_memory_pool_library_logic or p_memory_pool_user_logic
// p_memory_pool_library_logic stays till the very end
// p_memory_pool_user_logic will be thrown away when we load a new formula
CMemoryPool *PMemoryPoolParser();