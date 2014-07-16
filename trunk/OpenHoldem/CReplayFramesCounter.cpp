//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose: Encapsuling the counter in an extra class
//   and searching the directory only once on startup,
//   as the old solution enumerated all files in the replay-directory
//   for every single replay-frame, which lead to performance issues
//   for some people: http://www.maxinmontreal.com/forums/viewtopic.php?f=110&t=16135
//
//*****************************************************************************

#include "stdafx.h"
#include "CReplayFramesCounter.h"

#include "CFilenames.h"
#include "CPreferences.h"
#include "MagicNumbers.h"

CReplayFramesCounter *p_replayframes_counter = NULL;

CReplayFramesCounter::CReplayFramesCounter()
{
	_current_last_replay_frame = GetNumberOfLastReplayFrameInDirectory();
}

CReplayFramesCounter::~CReplayFramesCounter()
{}

int CReplayFramesCounter::GetNumberOfNextReplayFrame()
{
	CSLock lock(m_critsec);

	_current_last_replay_frame++;
	if ((_current_last_replay_frame < 0) 
		|| (_current_last_replay_frame > preferences.replay_max_frames()))
	{
		_current_last_replay_frame = 0;
	}
	write_log(preferences.debug_replayframes(), "[CReplayFrame] Next frame number: %d\n", 
		_current_last_replay_frame);
	return _current_last_replay_frame;
}

int CReplayFramesCounter::GetNumberOfLastReplayFrameInDirectory()
{
	CTime		time = 0, latest_time = 0;
	int			frame_num = 0;
    CString		path, filename, current_path;
    CFileFind	hFile;
    BOOL		bFound = false;

	CSLock lock(m_critsec);

	// Find next replay frame number
    _current_last_replay_frame = 0;

    path = p_filenames->ReplayBitmapFilename(_current_last_replay_frame);
    bFound = hFile.FindFile(path.GetString());
    while (bFound)
    {
        bFound = hFile.FindNextFile();
        if (!hFile.IsDots() && !hFile.IsDirectory())
        {
            filename = hFile.GetFileName();
            hFile.GetLastWriteTime(time);
            sscanf_s(filename.GetString(), "frame%d.bmp", &frame_num);

            if (time>latest_time)
            {
                _current_last_replay_frame = frame_num;
                latest_time = time;
            }
        }
    }
	return _current_last_replay_frame;
}