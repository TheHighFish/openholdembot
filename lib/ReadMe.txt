The files "OpenHoldem.lib" and "Reference User DLL.lib"
exist in this directory to avoid problems with circular dependencies.
This works, as long as the export definitions of OpenHoldem
and the DLL don't change (not expected, as we are happy with the new interface:
http://www.maxinmontreal.com/forums/viewtopic.php?f=174&t=18675)
In case the interface changes these two files should be updated.
(http://www.maxinmontreal.com/forums/viewtopic.php?f=114&t=18881)
