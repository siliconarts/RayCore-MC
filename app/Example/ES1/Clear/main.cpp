#include <stdio.h>
#include "WindowSurface.h"

#include <EGL/egl.h>
#include <GLES/gl.h>
// 간단한 버스 테스트

EGLDisplay eglDisplay = EGL_NO_DISPLAY;
EGLContext eglContext = EGL_NO_CONTEXT;
EGLSurface eglSurface = EGL_NO_SURFACE;

extern "C" static void GetWindowSize(int& width, int& height){
#if defined(USE_TESTDRIVE)
	width	= 800;
	height	= 480;
#else
	width	= 1920;
	height	= 1080;
#endif
}

static EGL_WINDOW_SURFACE	__window_surface = {
	GetWindowSize
};

int main(int argc, char ** argv)
{
	EGLint	attribConfig[] = {
		EGL_RED_SIZE,	8,
		EGL_GREEN_SIZE,	8,
		EGL_BLUE_SIZE,	8,
		EGL_DEPTH_SIZE,	16,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
		EGL_NONE
	};
	EGLint	attribContext[] = {
		EGL_CONTEXT_CLIENT_VERSION, 1,
		EGL_NONE
	};
	EGLConfig eglConfig;
	EGLint nConfigs = 0;

	eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if(eglGetError() != EGL_SUCCESS)
	{
		printf("There is an error while the function, eglGetDisplay.\n");
		goto terminate;
	}
	eglInitialize(eglDisplay, NULL, NULL);
	if(eglGetError() != EGL_SUCCESS)
	{
		printf("There is an error while the function, eglInitialize.\n");
		goto terminate;
	}
	eglChooseConfig(eglDisplay, attribConfig, &eglConfig, 1, &nConfigs);
	if(eglGetError() != EGL_SUCCESS)
	{
		printf("There is an error while the function, eglChooseConfig.\n");
		goto terminate;
	}
	if(nConfigs == 0)
	{
		printf("There is no config selected.\n");
		goto terminate;
	}

	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (NativeWindowType)&__window_surface, 0);
	if(eglGetError() != EGL_SUCCESS)
	{
		printf("There is an error while the function, eglCreateWindowSurface.\n");
		goto terminate;
	}
	eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, attribContext);
	if(eglGetError() != EGL_SUCCESS)
	{
		printf("There is an error while the function, eglCreateContext.\n");
		goto terminate;
	}
	if(eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) != EGL_TRUE)
	{
		printf("There is an error while the function, eglMakeCurrent.\n");
		goto terminate;
	}

	// 3 times loop with each color.
// 	{
// 		const GLfloat clearcolor[][3] = {
// 			{0.f, 0.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
// 		};
// 		for(int i=0;i<3;i++)
// 		{
// 			glClearColor(clearcolor[i][0], clearcolor[i][1], clearcolor[i][2], 1.f);
// 			glClear(GL_COLOR_BUFFER_BIT);
// 
// 			//Sleep(1);
// 			if(glGetError() == GL_NO_ERROR)
				eglSwapBuffers(eglDisplay, eglSurface);
// 			else
// 				printf("There was an error while clearing.\n");
// 		}
// 	}

	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

terminate:
	if(eglSurface != EGL_NO_SURFACE) eglDestroySurface(eglDisplay, eglSurface);
	if(eglContext != EGL_NO_CONTEXT) eglDestroyContext(eglDisplay, eglContext);
	if(eglDisplay != EGL_NO_DISPLAY) eglTerminate(eglDisplay);
	return 0;
}
