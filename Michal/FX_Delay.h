#ifndef __PLUGEXAMPLE__
#define __PLUGEXAMPLE__

// In the project settings, define either VST_API or AU_API.
#include "../IPlug_include_in_plug_hdr.h"

class FX_Delay : public IPlug
{
public:
	FX_Delay(IPlugInstanceInfo instanceInfo);
	~FX_Delay();	// Nothing to clean up.

	// Implement these if your audio or GUI logic requires doing something 
	// when params change or when audio processing stops/starts.
	void Reset() {}
	void OnParamChange(int paramIdx) {}
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
	double *buforL, *buforR;  // bufor L i R kanalow	   	  
	int buf_i, dioda; //indeks i dla bufora, pseudo globalny,  
	static const int buflen=500000; //rozmiar bufora w samplach
	ITextControl *pTextControl, *pTextControl2, *pTextControl3, *text_tempo;
	IKnobRotaterControl *time_knob;	
	double poprz_msTime, poprz_dBLevel, poprz_dBFeedback;	  
};

////////////////////////////////////////
#endif
