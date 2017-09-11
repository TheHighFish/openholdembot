//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: base class for formula objects 
//   that use memory-pools
//
//******************************************************************************

#include "stdafx.h"
#include "CSpaceOptimizedFormulaObject.h"

#include <assert.h>
#include "CMemoryPool.h"

// Custom memory-allocation to avoid always getting
// a 4KB-block when we create a little object
void* :operator CSpaceOptimizedFormulaObject::new(size_t size) {
}

