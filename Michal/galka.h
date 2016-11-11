#pragma once
#include "../IControl.h"
class galka :
	public IKnobRotaterControl
{
public:
	galka(IPlugBase* pPlug, int x, int y, int paramIdx, IBitmap* pBitmap,
		double minAngle = -0.75 * PI, double maxAngle = 0.75 * PI, int yOffsetZeroDeg = 0, 
		EDirection direction = kVertical, double gearing = DEFAULT_GEARING);
	~galka(void);
	bool Draw(IGraphics* pGraphics);
};