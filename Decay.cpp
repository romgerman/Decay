#include "Decay.h"

#include "Common/BaseKnob.h"

#include <string>

// the information structure describing this plugin to the host
TFruityPlugInfo PlugInfo = {
	CurrentSDKVersion,
	"Decay",
	"Decay",
	FPF_Type_Effect,			// FPF_Type_Visual means we don't process any sound
	NumParamsConst,
	0,                          // infinite polyphony (it's an effect plugin)
	0                           // zero internal controller
};

extern "C" __declspec(dllexport) TFruityPlug* _stdcall CreatePlugInstance(TFruityPlugHost* host, int tag)
{
	DecayPlug* plug = new DecayPlug(tag, host);
	return reinterpret_cast<TFruityPlug*>(plug);
};

DecayPlug::DecayPlug(int tag, TFruityPlugHost* host) : TCPPFruityPlug(tag, host, 0)
{
	Info = &PlugInfo;
	m_app = Application();
	m_sampleRate = 0;

	m_publicParams[0] = Parameter(0, "Example Parameter", 0.5); // So zero parameter is for internal controller i guess
	m_publicParams[1] = Parameter(1, "Time", 0.01, " s", 0.001f, 10.f);
	m_publicParams[2] = Parameter(3, "Dry/Wet", 0.5);
	m_publicParams[3] = Parameter(2, "Feedback", 0.5);
}

void STD_CALL DecayPlug::DestroyObject()
{
	TCPPFruityPlug::DestroyObject();
}

int STD_CALL DecayPlug::Dispatcher(int id, int index, int value)
{
	switch (id)
	{
	case FPD_ShowEditor:
		if (value == 0)
		{
			m_app.SetParentWindow((HWND)value);
			m_app.Show(SW_HIDE);
		}
		else
		{
			if (m_app.GetWindowHandle())
			{
				m_app.SetParentWindow((HWND)value);
				m_app.Show(SW_SHOW);
			}
			else
			{
				m_app.Init((HWND)value, 300, 180, this);
				
				m_app.AddElement(new BaseKnob(50, 50, 50, &m_publicParams[1]));
				m_app.AddElement(new BaseKnob(150, 50, 50, &m_publicParams[2]));
				m_app.AddElement(new BaseKnob(250, 50, 25, &m_publicParams[3]));

				m_app.Show(SW_SHOW);
			}
		}
		EditorHandle = m_app.GetWindowHandle();
		break;
	case FPD_SetSampleRate:
		{
			if (value != 0)
			{
				m_sampleRate = value;
				UpdateBuffer();
			}
		}
		break;
	}

	return 0;
}

void STD_CALL DecayPlug::Idle_Public()
{
	TCPPFruityPlug::Idle_Public();

	// do any idle processing you want here
	// but don't forget to call inherited
}

void STD_CALL DecayPlug::SaveRestoreState(IStream* stream, BOOL save)
{
	if (save)
	{
		stream->Write(m_publicParams, StateSizeConst, 0);
	}
	else
	{
		stream->Read(m_publicParams, StateSizeConst, 0);
		ProcessAllParams();
	}
}

void STD_CALL DecayPlug::GetName(int section, int index, int value, char* name)
{
	switch (section)
	{
		// for the parameter names, we look at the (long) hint of the parameter control
	case FPN_Param:
		strcpy(name, m_publicParams[index].GetName());
		break;

		// the name of our only internal controller
	case FPN_OutCtrl:
		strcpy(name, m_publicParams[index].GetName());
		break;
	}
}

int STD_CALL DecayPlug::ProcessParam(int thisIndex, int thisValue, int RECFlags)
{	
	if (RECFlags & REC_UpdateValue)
	{		
		// update values here
		m_publicParams[thisIndex].SetInternalValue(thisValue);
		m_app.SetNeedRedraw();
	}
	else if (RECFlags & REC_GetValue)
	{
		thisValue = m_publicParams[thisIndex].GetInternalValue();
		
		if (thisIndex == 1)
			UpdateBuffer();
	}
	else if (RECFlags & REC_PlugReserved)
	{
		if (thisIndex == 1)
			UpdateBuffer();
	}

	/*
	// update the parameter control's value
    if (RECFlags & REC_UpdateControl)
        Control->SetPos(thisValue);

    // we show the parameter value as a hint
    if (RECFlags & REC_ShowHint)
        ShowHintMsg_Percent(thisValue - Control->GetRangeMin(), Control->GetRangeMax() - Control->GetRangeMin() + 1);
	*/

	if (RECFlags & REC_ShowHint)
		ShowHintMsg_Percent(thisValue, MAX_PARAM_VALUE);
	
	return thisValue;
}

void STD_CALL DecayPlug::Eff_Render(PWAV32FS sourceBuffer, PWAV32FS destBuffer, int length)
{	
	for (int i = 0; i < length; i++)
	{
		auto left = (*sourceBuffer)[i][0];
		auto right = (*sourceBuffer)[i][1];

		auto dry = 1 - m_publicParams[2].GetNormalizedValue();
		auto wet = m_publicParams[2].GetNormalizedValue();
		auto feedback = m_publicParams[3].GetNormalizedValue();

		m_bufferLeft.put(left + m_bufferLeft.current() * feedback);
		m_bufferRight.put(right + m_bufferRight.current() * feedback);
		
		(*destBuffer)[i][0] = left * dry + m_bufferLeft.current() * wet; // left
		(*destBuffer)[i][1] = right * dry + m_bufferRight.current() * wet; // right
	}
}

void DecayPlug::UpdateBuffer()
{
	auto size = (int)(m_sampleRate * m_publicParams[1].GetValue());

	if (size < 1)
		size = 1;
	
	Lock();
	m_bufferLeft.resize(size);
	m_bufferRight.resize(size);
	Unlock();
}

