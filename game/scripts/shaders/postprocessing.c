#ifndef postprocessing_c
#define postprocessing_c

#include "postprocessing.h"

void initPostprocessing (VIEW* viewStart)
{
	resetPpSwirl();
	
	viewStart->stage = pp_viewLuma;
	pp_viewLuma->stage = pp_viewFxaa;
	
	//pp_viewFxaa->stage = pp_viewSwirl;
	//pp_viewSwirl->stage = pp_viewRedness;
	pp_set(pp_viewFxaa, mtl_hdr);

	while (1)
	{
		updateHdr();
		wait(1);
	}
}

// swirl

void resetPpSwirl ()
{
	setPpSwirl(1, 0, .5, 0.5, 0);
}

void setPpSwirl (float fRadius, float fAngle, float fX, float fY, float fBlend)
{
	setPpSwirlRadius(fRadius);
	setPpSwirlAngle(fAngle);
	setPpSwirlCenter(fX, fY);
	setPpSwirlBlend(fBlend);
}

void setPpSwirlRadius (float fRadius)
{
	g_mtlSwirl->ppSwirlRadius = floatv(fRadius);
}

void setPpSwirlAngle (float fAngle)
{
	g_mtlSwirl->ppSwirlAngle = floatv(fAngle);
}

void setPpSwirlCenter (float fX, float fY)
{
	g_mtlSwirl->ppSwirlCenterX = floatv(fX);
	g_mtlSwirl->ppSwirlCenterY = floatv(fY);
}

void setPpSwirlBlend (float fBlend)
{
	g_mtlSwirl->ppSwirlBlend = floatv(fBlend);
}

// HDR

void setHdr (float fStrength, float fThreshold, float fExposure)
{
	hdrStrength = fStrength;
	hdrThreshold = fThreshold;
	hdrExposure = fExposure;
}

void updateHdr ()
{
	mtl_hdr.skill1 = floatv(hdrStrength);
	mtl_hdr.skill2 = floatv(hdrThreshold);
	mtl_hdr.skill3 = floatv(hdrExposure);
}

	
#endif /* postprocessing_c */
