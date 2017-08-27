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

#pragma once
#include <map>

class CMemoryPool {
 public:
  CMemoryPool();
  ~CMemoryPool();
 public:
  void* allocate(size_t size);
  void release_all();
 private:
  void allocate_new_memory_block();
  size_t bytes_available_in_current_block();
 private:
  bool _all_emmory_released;
  int _bytes_used_in_current_block;
  void* _current_memory_block;
 private:
   // For gouse-keeping
   // All blocks to be released at some point
   CArray <void*, void*> _memory_blocks;
};
