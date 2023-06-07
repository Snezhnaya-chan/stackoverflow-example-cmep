#include <stdio.h>

#include "gl_calls.hpp"
#include <Windows.h>

PFNGLCLEARPROC glClear;
PFNGLENABLEPROC glEnable;
PFNGLDISABLEPROC glDisable;
PFNGLVIEWPORTPROC glViewport;
PFNGLGETSTRINGPROC glGetString;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLBLENDFUNCPROC glBlendFunc;
PFNGLDRAWARRAYSPROC glDrawArrays;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLCLEARCOLORPROC glClearColor;
PFNGLBINDTEXTUREPROC glBindTexture;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLPOLYGONMODEPROC glPolygonMode;
PFNGLDRAWELEMENTSPROC glDrawElements;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLBLENDEQUATIONPROC glBlendEquation;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATEBUFFERSPROC glCreateBuffers;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLDELETETEXTURESPROC glDeleteTextures;
PFNGLCREATETEXTURESPROC glCreateTextures;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLNAMEDBUFFERDATAPROC glNamedBufferData;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLTEXTURESTORAGE2DPROC glTextureStorage2D;
PFNGLTEXTURESUBIMAGE2DPROC glTextureSubImage2D;
PFNGLTEXTUREPARAMETERIPROC glTextureParameteri;
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
PFNGLNAMEDBUFFERSUBDATAPROC glNamedBufferSubData;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;


PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

static inline void* load_proc(HMODULE gl_module, LPCSTR name)
{
	void* tmp = GetProcAddress(gl_module, name);

	if (!tmp)
	{
		tmp = wglGetProcAddress(name);
	}

	return tmp;
}

int load_GL_calls()
{
	// Load the DLL
	HMODULE gl_module = LoadLibrary(TEXT("opengl32.dll"));
	if (gl_module == NULL)
	{
		return 1;
	}

	/// GL stuff
	// Generic
	glClear = (PFNGLCLEARPROC)load_proc(gl_module, "glClear");
	glEnable = (PFNGLENABLEPROC)load_proc(gl_module, "glEnable");
	glDisable = (PFNGLDISABLEPROC)load_proc(gl_module, "glDisable");
	glViewport = (PFNGLVIEWPORTPROC)load_proc(gl_module, "glViewport");
	glGetString = (PFNGLGETSTRINGPROC)load_proc(gl_module, "glGetString");
	glBlendFunc = (PFNGLBLENDFUNCPROC)load_proc(gl_module, "glBlendFunc");
	glClearColor = (PFNGLCLEARCOLORPROC)load_proc(gl_module, "glClearColor");
	glDrawArrays = (PFNGLDRAWARRAYSPROC)load_proc(gl_module, "glDrawArrays");
	glPolygonMode = (PFNGLPOLYGONMODEPROC)load_proc(gl_module, "glPolygonMode");
	glDrawElements = (PFNGLDRAWELEMENTSPROC)load_proc(gl_module, "glDrawElements");
	glBlendEquation = (PFNGLBLENDEQUATIONPROC)load_proc(gl_module, "glBlendEquation");
	glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)load_proc(gl_module, "glBlendFuncSeparate");
	glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)load_proc(gl_module, "glPrimitiveRestartIndex");
	// Debug
	glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)load_proc(gl_module, "glDebugMessageCallback");
	// Shader
	glUseProgram = (PFNGLUSEPROGRAMPROC)load_proc(gl_module, "glUseProgram");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)load_proc(gl_module, "glLinkProgram");
	glDeleteShader = (PFNGLDELETESHADERPROC)load_proc(gl_module, "glDeleteShader");
	glAttachShader = (PFNGLATTACHSHADERPROC)load_proc(gl_module, "glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC)load_proc(gl_module, "glDetachShader");
	glCreateShader = (PFNGLCREATESHADERPROC)load_proc(gl_module, "glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)load_proc(gl_module, "glShaderSource");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)load_proc(gl_module, "glCreateProgram");
	glCompileShader = (PFNGLCOMPILESHADERPROC)load_proc(gl_module, "glCompileShader");
	// Vertex array
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)load_proc(gl_module, "glBindVertexArray");
	glCreateVertexArrays = (PFNGLCREATEVERTEXARRAYSPROC)load_proc(gl_module, "glCreateVertexArrays");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)load_proc(gl_module, "glDeleteVertexArrays");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)load_proc(gl_module, "glVertexAttribPointer");
	glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)load_proc(gl_module, "glVertexAttribIPointer");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)load_proc(gl_module, "glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)load_proc(gl_module, "glDisableVertexAttribArray");
	// Buffer
	glBindBuffer = (PFNGLBINDBUFFERPROC)load_proc(gl_module, "glBindBuffer");
	glCreateBuffers = (PFNGLCREATEBUFFERSPROC)load_proc(gl_module, "glCreateBuffers");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)load_proc(gl_module, "glDeleteBuffers");
	glNamedBufferData = (PFNGLNAMEDBUFFERDATAPROC)load_proc(gl_module, "glNamedBufferData");
	glNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC)load_proc(gl_module, "glNamedBufferSubData");
	// Texture
	glBindTexture = (PFNGLBINDTEXTUREPROC)load_proc(gl_module, "glBindTexture");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)load_proc(gl_module, "glActiveTexture");
	glDeleteTextures = (PFNGLDELETETEXTURESPROC)load_proc(gl_module, "glDeleteTextures");
	glCreateTextures = (PFNGLCREATETEXTURESPROC)load_proc(gl_module, "glCreateTextures");
	glTextureStorage2D = (PFNGLTEXTURESTORAGE2DPROC)load_proc(gl_module, "glTextureStorage2D");
	glTextureSubImage2D = (PFNGLTEXTURESUBIMAGE2DPROC)load_proc(gl_module, "glTextureSubImage2D");
	glTextureParameteri = (PFNGLTEXTUREPARAMETERIPROC)load_proc(gl_module, "glTextureParameteri");
	// Uniform
	glUniform1i = (PFNGLUNIFORM1IPROC)load_proc(gl_module, "glUniform1i");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)load_proc(gl_module, "glUniformMatrix4fv");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)load_proc(gl_module, "glGetUniformLocation");

	/// WGL stuff
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)load_proc(gl_module, "wglSwapIntervalEXT");
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)load_proc(gl_module, "wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)load_proc(gl_module, "wglCreateContextAttribsARB");

	// Get rid of the library
	FreeLibrary(gl_module);

	return 0;
}
