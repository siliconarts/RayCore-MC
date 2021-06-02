#include <math.h>
#include <stdio.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <EGL/egl.h>

#include "WindowSurface.h"
#ifdef linux
#include <unistd.h>
#endif

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

namespace __TUTORIAL1__
{
	#define PRECISION 16
	#define ONE	(1 << PRECISION)
	#define ZERO 0

	inline GLfixed FixedFromInt(int value) {return value << PRECISION;};
	GLboolean InitOGLES();// Our GL initialization function
	void Render();  // Our Render function
	void Clean();   //Our clean function. It will clean all used resources

	//Some useful global handles
	NativeWindowType hNativeWnd = 0; // A handle to the window we will create


	// OpenGL variables
	EGLDisplay glesDisplay;  // EGL display
	EGLSurface glesSurface;	 // EGL rendering surface
	EGLContext glesContext;	 // EGL rendering context

	GLboolean InitOGLES()
	{
	  EGLConfig configs[10];
	  EGLint matchingConfigs;

	  /*configAttribs is a integers list that holds the desired format of
	   our framebuffer. We will ask for a framebuffer with 24 bits of
	   color and 16 bits of z-buffer. We also ask for a window buffer, not
	   a pbuffer or pixmap buffer*/
	  const EGLint configAttribs[] =
	  {
	      EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
	      EGL_NONE,           EGL_NONE
	  };

	  glesDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);	 //Ask for an default display

	  //Display initialization (we don't care about the OGLES version numbers)
	  if(!eglInitialize(glesDisplay, 0, 0))
	    return GL_FALSE;

	  /*Ask for the framebuffer confiburation that best fits our
	  parameters. At most, we want 10 configurations*/
	  if(!eglChooseConfig(glesDisplay, configAttribs, &configs[0], 10,  &matchingConfigs))
	   return GL_FALSE;

	  //If there isn't any configuration enough good
	  if (matchingConfigs < 1){
		  printf("No matching config!\n");
		  return GL_FALSE;
	  }

	  /*eglCreateWindowSurface creates an onscreen EGLSurface and returns
	  a handle  to it. Any EGL rendering context created with a
	  compatible EGLConfig can be used to render into this surface.*/
	  glesSurface = eglCreateWindowSurface(glesDisplay, configs[0], hNativeWnd, configAttribs);
	  if(!glesSurface) return GL_FALSE;

	  // Let's create our rendering context
	  glesContext=eglCreateContext(glesDisplay,configs[0],0,configAttribs);

	  if(!glesContext) return GL_FALSE;

	  //Now we will activate the context for rendering
	  eglMakeCurrent(glesDisplay, glesSurface, glesSurface, glesContext);

	  /*Remember: because we are programming for a mobile device, we cant
	  use any of the OpenGL ES functions that finish in 'f', we must use
	  the fixed point version (they finish in 'x'*/
	  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	  glShadeModel(GL_SMOOTH);

	  /*Setup of the projection matrix. We will use an ortho cube centered
	  at (0,0,0) with 100 units of edge*/
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();

	  glOrthox(FixedFromInt(-50), FixedFromInt(50),
	           FixedFromInt(-50), FixedFromInt(50),
	           FixedFromInt(-50), FixedFromInt(50));

	  glMatrixMode(GL_MODELVIEW);
	  glLoadIdentity();

	  glDisable( GL_DEPTH_TEST);
	  glDisable( GL_CULL_FACE );

	  return GL_TRUE;
	}
	//----------------------------------------------------------------------------
        static GLfixed mantissa[16];
        static GLint exponent[16];

	void Render()
	{

	  static int rotation = 0;

	  //GLshort vertexArray[9] = {-25,-25,0,   25,-25,0,     0,25,0 };
	  GLfloat vertexArray[12] = {-25,-25,0,1,   25,-25,0,1,     0,25,0,1 };
	  /*
	  GLfloat vertexArray[] = {
	  	0.1f, 0.1f, 0.5f, 1.0f,
	  	0.9f, 0.1f, 0.5f, 1.0f,
	  	0.5f, 0.9f, 0.5f, 1.0f
	  };
	  */
	  GLubyte colorArray[12] = {255,0,0,0,   0,255,0,0,    0,0,255,0};

	  {
			//glQueryMatrixxOES(mantissa, exponent);
      }

	  ///*
	  int test = rotation%3;
	  switch( test )
	  {
	  	case 0:
	  		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	  		break;
	  	case 1:
	  		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	  		break;
	  	case 2:
	  		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	  		break;
	  	default:
	  		break;
	  }
	  //rotation++;
	  //*/

	  //_asm int 3;

	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  glLoadIdentity();

	  glTranslatex(0, 0, FixedFromInt(-10));
	  glRotatex(FixedFromInt(rotation++)*30, 0, ONE,0);
	  //glRotatex(FixedFromInt(0), 0, ONE,0);

	  //Enable the vertices array
	  glEnableClientState(GL_VERTEX_ARRAY);
	  //glVertexPointer(3, GL_SHORT, 0, vertexArray);
	  glVertexPointer(4, GL_FLOAT, 0, vertexArray);
	  //3 = XYZ coordinates, GL_SHORT = data type, 0 = 0 stride bytes

	  //Enable the vertex color array
	  glEnableClientState(GL_COLOR_ARRAY);
	  glColorPointer(4,GL_UNSIGNED_BYTE, 0, colorArray);
	  //4 = RGBA format, GL_UNSIGNED_BYTE = data type,0=0 stide    bytes

	  glDrawArrays(GL_TRIANGLES, 0, 3);

	  glDisableClientState(GL_VERTEX_ARRAY);
	  glDisableClientState(GL_COLOR_ARRAY);

	  //printf("%d\n", rotation );

	  eglSwapBuffers(glesDisplay, glesSurface);
	}
	//----------------------------------------------------------------------------
	void Clean()
	{
	  if(glesDisplay)
	  {
	    eglMakeCurrent(glesDisplay, 0, 0, 0);
	    if(glesContext) eglDestroyContext(glesDisplay, glesContext);
	    if(glesSurface) eglDestroySurface(glesDisplay, glesSurface);
	    eglTerminate(glesDisplay);
	  }
	}

}

using namespace __TUTORIAL1__;

/*This is the main function. Here we will create the rendering window, initialize OpenGL ES, write the message loop, and, at the end, clean all and release all used resources*/
int main( void )
{
	// Initialize native OS
	//OS_InitFakeOS();
	GLboolean done = GL_FALSE;

	// Create native window.
	hNativeWnd = (NativeWindowType)&__window_surface;//(void*)1;//OS_CreateWindow();
	if(!hNativeWnd) return GL_FALSE;

	if(!InitOGLES()) return GL_FALSE; //OpenGL ES Initialization

	// test
	printf( "start!!!\n" );

	Render();

#ifdef USE_TESTDRIVE
	unsigned int frames = 2;
#else
	unsigned int frames = 300;
#endif
	//Message Loop
	//while(!done)
	while(frames--)
	{
		Render();
#ifndef USE_TESTDRIVE
		{
			int i,t;
			for(i=0;i<100000;i++)
			for(t=0;t<100000;t++);
		}
#endif
	}
	//Clean up all
	printf( "clean\n" );
	Clean();

	printf( "end!!!\n" );

	return GL_TRUE;
}










