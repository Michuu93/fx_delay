#pragma once
#include "../IControl.h"

class ISwitchControl2:
public ISwitchControl
{
public:
	ISwitchControl2(IKnobRotaterControl* _time_knob, ITextControl* _text_tempo, IPlugBase* pPlug, int x, int y, int paramIdx, IBitmap* pBitmap, IChannelBlend::EBlendMethod blendMethod= IChannelBlend::kBlendNone);
	~ISwitchControl2(void);
	void tap_tempo();
 	void OnMouseUp(int x, int y, IMouseMod* pMod);
 	void OnMouseDown(int x, int y, IMouseMod* pMod);
	void OnMouseDblClick(int x, int y, IMouseMod* pMod);
	bool Draw(IGraphics* pGraphics);	
	bool mMouseDown;
	ITextControl* text_tempo;
	IKnobRotaterControl* time_knob;
};

