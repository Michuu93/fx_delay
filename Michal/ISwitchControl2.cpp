#include "ISwitchControl2.h"
#include <ctime>

	clock_t tap1=0,tap2;
	double dif, tempo, ttime;

ISwitchControl2::ISwitchControl2(IKnobRotaterControl* _time_knob, ITextControl* _text_tempo, IPlugBase* pPlug, int x, int y, int paramIdx, IBitmap* pBitmap, IChannelBlend::EBlendMethod blendMethod )
 : ISwitchControl(pPlug,x,y,paramIdx,pBitmap, blendMethod)
{
	 mMouseDown = false;
	 text_tempo = _text_tempo;
	 time_knob = _time_knob;
}

ISwitchControl2::~ISwitchControl2(void)
{
}

void ISwitchControl2::tap_tempo()
{
	tap2=clock();             
    dif=difftime(tap2,tap1); 

	if (dif<5000)
		{
			tempo=60000/((int)dif);
			if (tempo<=15) tempo=15;
			if (tempo>=1200) tempo=1200;                                      
		}
    tap1=tap2;
	
	char tempoo[100];
	sprintf(tempoo,"%0.0f",tempo); 
	strcat(tempoo," bpm");
	ttime = 1000*(60/tempo);

	text_tempo->SetTextFromPlug(tempoo);
	text_tempo->SetDirty();
	time_knob ->SetValueFromUserInput(ttime/3950-0.0126582278); //odejmuje 0.0126582278 poniewa¿ 0.25 przy zakresie 50-4000 != 1000 tylko 1038
	SetDirty();
	Redraw();
}

void ISwitchControl2::OnMouseUp(int x, int y, IMouseMod* pMod)
{
	mMouseDown=false;
	SetDirty();
}

void ISwitchControl2::OnMouseDown(int x, int y, IMouseMod* pMod)
{
	mMouseDown=true;
	SetDirty();
	tap_tempo();	
}

void ISwitchControl2::OnMouseDblClick(int x, int y, IMouseMod* pMod)
{
	mMouseDown=true;
	SetDirty();
	tap_tempo();
}

bool ISwitchControl2::Draw(IGraphics* pGraphics)
{
	return pGraphics->DrawBitmap(&mBitmap, &mRECT, mMouseDown ? 1 : 2, &mBlend);
}