#pragma once

#include <fp_plugclass.h>
#include <fp_cplug.h>

#include "Common/Application.h"
#include "Common/Parameter.h"
#include "Common/CircularBuffer.h"

const int NumParamsConst = 4;    // the amount of parameters
const int StateSizeConst = NumParamsConst * sizeof(int);  // the size of all parameters together

#define STD_CALL _stdcall

class DecayPlug : public TCPPFruityPlug
{
public:
	DecayPlug(int tag, TFruityPlugHost* host);

	virtual void STD_CALL DestroyObject();

	virtual int  STD_CALL Dispatcher(int id, int index, int value);
	virtual void STD_CALL Idle_Public();
	virtual void STD_CALL SaveRestoreState(IStream* stream, BOOL save);
	virtual void STD_CALL GetName(int section, int index, int value, char* name);
	virtual int  STD_CALL ProcessParam(int index, int value, int RECFlags);
	virtual void STD_CALL Eff_Render(PWAV32FS sourceBuffer, PWAV32FS destBuffer, int length);

private:
	void UpdateBuffer();

private:
	Application m_app;
	Parameter m_publicParams[NumParamsConst];
	CircularBuffer<float> m_bufferLeft;
	CircularBuffer<float> m_bufferRight;
	int m_sampleRate;
};
