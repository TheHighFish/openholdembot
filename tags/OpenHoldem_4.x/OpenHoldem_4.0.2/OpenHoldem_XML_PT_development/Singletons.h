#ifndef C_SINGLETONS_H
#define C_SINGLETONS_H

// Creation and deletion of singletons (global objects)
// in well-defined order.
// Order is very important, as these objects might depend on each other;
// an undefined order will lead to very hard to trace crashes
// during startup and to 0-byte crash-dumps.

void StopThreads();
void InstantiateAllSingletons();
void DeleteAllSingletons();

#endif // C_SINGLETONS_H