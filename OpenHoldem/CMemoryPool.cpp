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
//
// Limitations:
//   * does not handle memory-requests that are larger as the default buffer
//   * does not handle multiple pools that could be released inbetween
//       (but this does not really matter if we formerly wasted a 4KN-block
//       for a 24-byte-request)
//
//******************************************************************************

#include "stdafx.h"
#include "CMemoryPool.h"
#include "OH_MessageBox.h"

// 64 KB = 1 default block at 64-bit-Windows 
//       = 16 default blocks at 52-bit-Windows
const int kMemoryBlockSize = 65536;

CMemoryPool::CMemoryPool() {
  _all_emmory_released = true;
  _current_memory_block = NULL;
  _bytes_used_in_current_block = kMemoryBlockSize;
  _memory_blocks.RemoveAll();
}

CMemoryPool::~CMemoryPool() {
  if (!_all_emmory_released) {
    release_all();
  }
}

size_t CMemoryPool::bytes_available_in_current_block() {
  if (_current_memory_block == NULL) {
    return 0;
  }
  AssertRange(_bytes_used_in_current_block, 0, kMemoryBlockSize);
  return (kMemoryBlockSize - _bytes_used_in_current_block);
}

void* CMemoryPool::allocate(size_t size) {
  if (size > kMemoryBlockSize) {
    OH_MessageBox_Error_Warning(
      "CMemoryPool received oversized request.\n"
      "Going to terminate.\n");
    PostQuitMessage(0);
    return NULL;
  }
  if (_current_memory_block == NULL) {
    allocate_new_memory_block();
  } else if (size > bytes_available_in_current_block()) {
    allocate_new_memory_block();
  }
  assert(size < _bytes_available_in_current_block);
  _all_emmory_released = false;
  size_t offset_to_allocation = _bytes_used_in_current_block;
  _bytes_used_in_current_block += size;
  return ((char*)_current_memory_block + offset_to_allocation);
}

void CMemoryPool::allocate_new_memory_block() {
  _current_memory_block = malloc(kMemoryBlockSize);
  if (_current_memory_block == NULL) {
    OH_MessageBox_Error_Warning(
      "CMemoryPool received oversized request.\n"
      "Going to terminate.\n");
    PostQuitMessage(0);
    return;
  }
  _bytes_used_in_current_block = 0;
  _memory_blocks.Add(_current_memory_block);
}

void CMemoryPool::release_all() {
  for (int i = 0; i < _memory_blocks.GetSize(); ++i) {
    _current_memory_block = _memory_blocks[i];
    delete _current_memory_block;
  }
  _all_emmory_released = true;
  _current_memory_block = NULL;
  _bytes_used_in_current_block = kMemoryBlockSize;
  _memory_blocks.RemoveAll();
}