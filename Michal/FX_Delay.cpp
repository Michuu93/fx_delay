#include "FX_Delay.h"
#include "../IPlug_include_in_plug_src.h"
#include "../IControl.h"
#include "ISwitchControl2.h"
#include "resource.h"
#include <math.h>
#include <ctime>

//const int kNumPrograms = 1;
clock_t clock1=0,clock2;

enum EParams 
{
	kLevel=0,
	kFeedback,
	kTime,
	kButton,
	kLed,
	kNumParams
};

//Grafika efektu:
enum ELayout
{
	kW = 280, //szerokosc
	kH = 360, //wys

	//lokalizacja galek X,Y
	kLevel_X = 39,
	kLevel_Y = 36,

	kFeedback_X = 105,
	kFeedback_Y = 124,

	kTime_X = 171,
	kTime_Y = 36,
	
	kButton_X = 72,
	kButton_Y = 250, 
}; 

FX_Delay::FX_Delay(IPlugInstanceInfo instanceInfo)
:IPLUG_CTOR(kNumParams, 4, instanceInfo),
poprz_msTime(0), poprz_dBLevel(0), poprz_dBFeedback(0)
{
  TRACE;
	GetParam(kLevel)->InitDouble("Level", -18.0, -36.0, 0.0, 0.1, "dB");
	GetParam(kFeedback)->InitDouble("Feedback", -9.0, -36.0, -1.0, 0.1, "dB");
	GetParam(kTime)->InitInt("Time", 800, 50, 4000, "ms");

	//tlo:
	IGraphics* pGraphics = MakeGraphics(this, kW, kH);
	pGraphics->AttachBackground(BG_ID, BG_FN);

	//bitmapy:
	IBitmap bitmap		= pGraphics->LoadIBitmap(GALKA_ID, GALKA_FN),
	        bitmapSw	= pGraphics->LoadIBitmap(BUTTON_ID, BUTTON_FN, 2), //tap tempo, ostatni parametr=2 bitmapy
			bitmapLed	= pGraphics->LoadIBitmap(LED_ID, LED_FN, 2);

	pGraphics->AttachControl(new IKnobRotaterControl(this, kLevel_X, kLevel_Y, kLevel, &bitmap, -0.76*PI, 0.76*PI));
	pGraphics->AttachControl(new IKnobRotaterControl(this, kFeedback_X, kFeedback_Y, kFeedback, &bitmap,-0.76*PI, 0.76*PI));
	time_knob = new IKnobRotaterControl(this, kTime_X, kTime_Y, kTime, &bitmap,-0.76*PI, 0.76*PI);
	pGraphics->AttachControl(time_knob);		 

	IText lFont(14, &IColor(255, 77, 77, 77), NULL, IText::kStyleBold, IText::kAlignCenter);		
	pTextControl=new ITextControl(this, &IRECT(kLevel_X, kLevel_Y+80, kLevel_X + 70, kLevel_Y+80 + 20), &lFont,""); //Level
	pTextControl2=new ITextControl(this, &IRECT(kFeedback_X, kFeedback_Y+80, kFeedback_X + 70, kFeedback_Y+80 + 20), &lFont,""); //Feedback
	pTextControl3=new ITextControl(this, &IRECT(kTime_X, kTime_Y+80, kTime_X + 70, kTime_Y+80 + 20), &lFont,""); //Time
	pGraphics->AttachControl(pTextControl);
	pGraphics->AttachControl(pTextControl2);
	pGraphics->AttachControl(pTextControl3);

	text_tempo=new ITextControl(this, &IRECT(100, kButton_Y + 50, 100 + 80, kButton_Y+ 50 + 20), &lFont,""); //tempo	
	pGraphics->AttachControl(new ISwitchControl2(time_knob, text_tempo, this, kButton_X, kButton_Y, kButton, &bitmapSw));
	pGraphics->AttachControl(text_tempo);
	
	pGraphics->AttachControl(new ITextControl(this, &IRECT(131, 331, 131 + 70, 331 + 20), &lFont, "v1.0")); //wersja
	pGraphics->AttachControl(new IURLControl(this, &IRECT(79, 328, 201, 348), "http://fx-team.fulara.com/fx-delay/"));

	dioda = pGraphics->AttachControl(new IBitmapControl(this, kFeedback_X + 25, kLevel_Y + 35, &bitmapLed));


	AttachGraphics(pGraphics);  

	MakePreset("Rhytm", -20.0, -12.0, 400);
	MakePreset("Solo",  -6.0, -8.0, 280);
	MakePreset("One Reflection Follower",  0.0, -36.0, 473);
	MakePreset("Adam Fulara Preset",  -18.0, -10.0, 332);	

	//inicjacja buforów:
	buf_i=0; //indeks bufora delay (przesuwa siê do przodu razem z analiza)
	buforL=new double[buflen];
	buforR=new double[buflen];

	memset(buforL,0, sizeof buforL);
	memset(buforR,0, sizeof buforR);
}

FX_Delay::~FX_Delay()
{
	delete [] buforL;
	delete [] buforR;
}

void FX_Delay::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	double *in1 = inputs[0],
	       *in2 = inputs[1],
	       *out1 = outputs[0],
	       *out2 = outputs[1],
		   sampleRate=GetSampleRate(),
		   msTime=GetParam(kTime)->Value(),
		   dBFeedback=GetParam(kFeedback)->DBToAmp(),
		   dBFeedbackv=GetParam(kFeedback)->Value(),
		   dBLevel=GetParam(kLevel)->DBToAmp(),
		   dBLevelv=GetParam(kLevel)->Value();

	//wartosc galki Level
	if (poprz_dBLevel != dBLevelv)
	{
	char napis[100];
	sprintf(napis,"%0.0f",dBLevelv); 
	strcat(napis," dB");
	pTextControl->SetTextFromPlug(napis);
	pTextControl->SetDirty();
	poprz_dBLevel=dBLevelv;
	}

	//wartosc galki Feedback
	if (poprz_dBFeedback != dBFeedbackv)
	{
	char napis[100];
	sprintf(napis,"%0.0f",dBFeedbackv); 
	strcat(napis," dB");
	pTextControl2->SetTextFromPlug(napis);
	pTextControl2->SetDirty();
	poprz_dBFeedback=dBFeedbackv;
	}

	//wartosc galki Time
	if (poprz_msTime != msTime)
	{
	char napis[100];
	sprintf(napis,"%0.0f",msTime); 
	strcat(napis," ms");
	pTextControl3->SetTextFromPlug(napis);
	pTextControl3->SetDirty();

	sprintf(napis,"%0.0f",(1000*60)/msTime); 
	strcat(napis," bpm");
	text_tempo->SetTextFromPlug(napis);
	text_tempo->SetDirty();
	poprz_msTime=msTime;
	}

	//dioda tempo
	if (GetGUI())
	{
		if (difftime(clock2,clock1) >=msTime/10)  GetGUI()->SetControlFromPlug(dioda, 0);
		clock2=clock();
		if (msTime>=300)
		if (difftime(clock2,clock1)>=msTime)
		{
			clock1=clock();
			clock2=0;
			GetGUI()->SetControlFromPlug(dioda, 1);
		}
	}


	int	delay=(int)(sampleRate*msTime/1000),
		opoznienie;

	for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
		{
			*out1 = *in1;
			*out2 = *in2;

			opoznienie = buf_i - delay + (buf_i<delay ? buflen : 0); //pozycja w tablicy opoznionego sampla

			//algorytm IIR, najpierw dodac echo, potem zapisac do bufora	
			*out1 += buforL[opoznienie]*dBFeedback; //wejscie + gain * bufor z przeszlosci
			*out2 += buforR[opoznienie]*dBFeedback;

			buforL[buf_i]=*out1;
			buforR[buf_i]=*out2;

			//resetuje sygnal do suchego:		
			*out1 = *in1;
			*out2 = *in2;

			//przemnazam i dodaje do suchego:
			*out1 += dBLevel * buforL[opoznienie];
			*out2 += dBLevel * buforR[opoznienie];

			//zwiekszenie indeksu bufora z przeszlosci o 1
			buf_i = buf_i>=buflen ? 0 : buf_i+1; 			
   }
}