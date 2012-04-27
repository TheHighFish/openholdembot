#include "StdAfx.h"

// http://msdn.microsoft.com/en-us/library/system.io.filesystemwatcher.aspx
//
// Class monitors the scraper directory for
// * changed files
// * new files
// * renamed files
// and sets a flag on these events.
// The autoconnector will then automatically 
// reload all tablemaps on the next attempt to connect.