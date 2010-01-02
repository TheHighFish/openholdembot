#ifndef _INC_CSTABLEFRAMESCOUNTER_H
#define _INC_CSTABLEFRAMESCOUNTER_H

class CStableFramesCounter
{
public:
	CStableFramesCounter();
	~CStableFramesCounter();
public:
	void ResetOnAutoplayerAction();
	int  GetNumberOfStableFrames();
private:
	void Reset();
};

#endif // _INC_CSTABLEFRAMESCOUNTER_H
