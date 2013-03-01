//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//                  CPOL:   http://www.codeproject.com/info/cpol10.aspx
//
//*****************************************************************************
//
// Purpose: calculating a checksum for the logs
//   to verify that we have an "official" file
//   and sort out some PEBKACs
//
//*****************************************************************************
//
// Original sources: http://www.codeproject.com/Articles/32202/MD5-File-Creation-and-Verification
//                   http://www.rohitab.com/discuss/topic/21740-md5-hashing/
// Original authors: Shaheer Abdulrahiman
//                   "Napalm"
// Slight simplifications (and all the bugs): TheHighFish
//
//*****************************************************************************

#ifndef INC_MD5_CHECKSUM_H
#define INC_MD5_CHECKSUM_H

CString CalculateMD5(const CString FileName);

#endif INC_MD5_CHECKSUM_H