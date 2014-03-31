#include "StdAfx.h"
#include "CTablePositioner.h"

#include "CPreferences.h"
#include "CSharedMem.h"
#include "MagicNumbers.h"
#include "Winuser.h"


CTablePositioner *p_table_positioner = NULL;


CTablePositioner::CTablePositioner()
{}

CTablePositioner::~CTablePositioner()
{}

void CTablePositioner::PositionMyWindow()
{		
	// Build list of poker tables (child windows)
	// Use the shared memory (auto-connector) for that. 
	HWND *HWNDs_of_child_windows = p_sharedmem->GetDenseListOfConnectedPokerWindows();
	int number_of_child_windows  = p_sharedmem->SizeOfDenseListOfAttachedPokerWindows();

	if (number_of_child_windows <= 0)
	{
		// Do nothing if there are 0 tables connected.
		// Actually an empty list of tables consists of only NULLs,
		// but if MicroSofts table-arranging functions see a NULL
		// they will arrange all windows at the desktop.
		// That's not what we want.
		return;
	}
	if (prefs.table_positioner_options() == k_position_tables_tiled)
	{
		write_log(prefs.debug_table_positioner(), "[CTablePositioner] Going to tile %d windows...\n", number_of_child_windows);
		// Tiling windows: http://msdn.microsoft.com/en-us/library/windows/desktop/ms633554(v=vs.85).aspx
		TileWindows(
			NULL,				// Parent; NULL = whole desktop
			MDITILE_HORIZONTAL,	// How; either MDITILE_HORIZONTAL or MDITILE_VERTICAL
			NULL,				// Target area; NULL = parent window, here desktop
			number_of_child_windows,
			HWNDs_of_child_windows);
	}
	else if (prefs.table_positioner_options() == k_position_tables_cascaded)
	{
		write_log(prefs.debug_table_positioner(), "[CTablePositioner] Going to cascade %d windows...\n", number_of_child_windows);
		// Cascading windows: http://msdn.microsoft.com/en-us/library/windows/desktop/ms632674(v=vs.85).aspx
		CascadeWindows(
			NULL,				// Parent; NULL = whole desktop
			NULL,				// How; NULL means: order specified in the lpKids array
			NULL,				// Target area; NULL = parent window, here desktop
			number_of_child_windows,
			HWNDs_of_child_windows);
	}
	else
	{
		// prefs.table_positioner_options() == k_position_tables_never
		write_log(prefs.debug_table_positioner(), "[CTablePositioner] Not doing anything because of preferences.\n");
	}
}