#include "galka.h"


galka::galka(IPlugBase* pPlug, int x, int y, int paramIdx, IBitmap* pBitmap,
		double minAngle, double maxAngle, int yOffsetZeroDeg, 
		EDirection direction, double gearing):
IKnobRotaterControl(pPlug, x, y, paramIdx, pBitmap,
		minAngle,  maxAngle,yOffsetZeroDeg, 
		direction, gearing)

{
}


galka::~galka(void)
{
}
bool galka::Draw(IGraphics* pGraphics)
{
	int paramIdx;
	IRECT polozenie = mRECT;
	polozenie.T+=80;
	char napis[100];
	itoa(paramIdx, napis, 10);
	//sprintf(napis, "%0.2f", paramIdx);
  pGraphics->DrawIText(&IText(14, &IColor(255, 77, 77, 77), NULL, IText::kStyleBold, IText::kAlignCenter), napis, &polozenie);
  SetDirty();
  return IKnobRotaterControl::Draw(pGraphics);

}