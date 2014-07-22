#ifndef postprocessing_h
#define postprocessing_h

// prototypes

	void initPostprocessing(VIEW* viewStart);
	
	void resetPpSwirl ();
	void setPpSwirl (float fRadius, float fAngle, float fX, float fY, float fBlend);
	void setPpSwirlRadius (float fRadius);
	void setPpSwirlAngle (float fAngle);
	void setPpSwirlCenter (float fX, float fY);
	void setPpSwirlBlend (float fBlend);

// effects

	// HDR
	
		float hdrThreshold = 20;
		float hdrStrength = 10;
		float hdrExposure = 0;
		
		void updateHdr ();		
		void setHdr (float fStrength, float fThreshold, float fExposure);

	// swirl
		
		#define ppSwirlRadius skill1
		#define ppSwirlAngle skill2
		#define ppSwirlCenterX skill3
		#define ppSwirlCenterY skill4
		#define ppSwirlBlend skill5
		
		MATERIAL* g_mtlSwirl = { effect = "swirl.fx"; }
		VIEW* pp_viewSwirl = { material = g_mtlSwirl; flags = CHILD | PROCESS_TARGET; }
		
	// fxaa
	
		MATERIAL* g_mtlLuma = { effect = "luma.fx"; }
		VIEW* pp_viewLuma = { material = g_mtlLuma; flags = CHILD | PROCESS_TARGET; }

		MATERIAL* g_mtlFxaa = { effect = "fxaa.fx"; }
		VIEW* pp_viewFxaa = { material = g_mtlFxaa; flags = CHILD | PROCESS_TARGET; }	
		
	// redness
	
		var redness = 0;
	
		MATERIAL* g_mtlRedness = { effect = "red.fx"; }
		VIEW* pp_viewRedness = { material = g_mtlRedness; flags = CHILD | PROCESS_TARGET; }	

#include "postprocessing.c"

#endif /* postprocessing_h */
