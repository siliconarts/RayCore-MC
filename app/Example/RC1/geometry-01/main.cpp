
#include "RCFramework.h"

#include <stdio.h>
#include "../common/SceneData.h"



class Tutorial : public RCFramework
{
public:
	
	
	Tutorial(void) : RCFramework("Demo - TestMe01", 640, 480) {
	//Tutorial(void) : RCFramework("Demo - TestMe01",  1024, 768) {
	//Tutorial(void) : RCFramework("Demo - TestMe01", 1920, 1056) {
	}
	virtual ~Tutorial(void) {
	}

	BOOL			m_static_On;
	SceneData  sceneData;

	void StaticSceneDraw(void) 
	{
		rcStaticSceneBegin();
			sceneData.DrawScene();
		//upload tree data		
		rcStaticSceneEnd();
	}

protected:
	virtual BOOL OnIntialize() 
	{
		
		
		char foldername[] = "./scenedata/geometry-01/";



		m_static_On =  TRUE;

		rcSceneAllInit();
		{				
			sceneData.SetFolderName(foldername);

			if (!sceneData.SceneDataLoad(Width(), Height()))
				return FALSE;			
			//if (!sceneData.KeyAnimationDataLoad())
			//	return FALSE;

			//if (!sceneData.BoneAnimationDataLoad())
			//	return FALSE;

			//if (!sceneData.MeshAnimationDataLoad())
			//	return FALSE;

			sceneData.InitScene();
			sceneData.InitLights();
			sceneData.InitMaterials();

			//이후 material data 출력하면 됨: baseaddress는?
			//     lights도 출력			
			if (m_static_On)
				StaticSceneDraw();
		}

		return TRUE;
	}

	virtual BOOL OnDraw(void) 
	{
		static int i = 0;
		int nSkipFrame = SkipFrame();
		i += nSkipFrame;

		if (i >= sceneData.FrameCount()) i = 0;

		if (sceneData.FrameCount() > 0) {
			sceneData.InitScene(i);			
			if (sceneData.LightAniFlag())	sceneData.AnimationLights(i);

			sceneData.KeyAnimationDrawScene(i);
			sceneData.BoneAnimationDrawScene(i);
			sceneData.MeshAnimationDrawScene(i);
		}

		return TRUE;
	}

	virtual void OnKeyDown(char cKey) {
		switch (cKey) {
		case '1':			
			break;
		case '2':	
			break;
		case '3':	
			break;
		case '4':	
			break;
		default:
			sceneData.OnKeyDown(cKey);
			break;
		}
	}

	virtual void OnMouseEvent(float x, float y, DWORD dwBtn = MOUSE_BTN_NULL) {
		sceneData.OnMouseEvent(x, y, dwBtn);
	}
};

Tutorial 	g_Tutorial;
