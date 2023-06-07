// stackoverflow-example-cmep.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iterator>
#include <assert.h>
#include <string>

#include "gl_calls.hpp"
#include <Windows.h>
#include "Window.hpp"
#include <thread>

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "gdi32.lib")

namespace Engine
{
	namespace Rendering
	{
		static bool noWindowExit;

		Window::Window(const char* title, unsigned int sizeX, unsigned int sizeY) noexcept : sizex(sizeX), sizey(sizeY)
		{
#if defined ( _WIN32 )
			// Convert char* to wchar_t*
			std::wstring wtitle;
			std::copy(title, title + strlen(title), std::back_inserter(wtitle));

			this->hInstance = GetModuleHandle(nullptr);

			// Create a generic window class
			{
				WNDCLASS window_class = {};

				window_class.style = CS_OWNDC;
				window_class.lpfnWndProc = Engine::Rendering::Window::WinProc;
				window_class.hInstance = this->hInstance;
				window_class.lpszClassName = TEXT("GENERIC_WINDOW");

				const ATOM result = RegisterClass(&window_class);
				assert(result != 0);
			}

			// Create a window to be able to load GL calls
			{
				if ((this->windowHandle = Window::CreateGenericWindow(this->hInstance, wtitle.c_str(), 0, 0, sizeX, sizeY)) == NULL)
				{
					assert(0);
					return;
				}

				HDC dc = GetDC(this->windowHandle);
				assert(dc != NULL);

				PIXELFORMATDESCRIPTOR descriptor = { };

				descriptor.nSize = sizeof(descriptor);
				descriptor.nVersion = 1;
				descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER | PFD_SWAP_LAYER_BUFFERS;
				descriptor.iPixelType = PFD_TYPE_RGBA;
				descriptor.cDepthBits = 24;
				descriptor.cColorBits = 32;
				descriptor.cStencilBits = descriptor.cRedBits = descriptor.cGreenBits = descriptor.cBlueBits = descriptor.cAlphaBits = 8;

				const int pixel_format = ChoosePixelFormat(dc, &descriptor);
				assert(pixel_format != 0);
				const BOOL result = SetPixelFormat(dc, pixel_format, &descriptor);
				assert(result == TRUE);

				this->renderContext = wglCreateContext(dc);
				this->AcquireContext();

				if (load_GL_calls())
				{
					assert(0);
					return;
				}

				noWindowExit = true;
				this->GiveupContext();
				wglDeleteContext(this->renderContext);
				ReleaseDC(this->windowHandle, dc);
				DestroyWindow(this->windowHandle);
				noWindowExit = false;
			}

			// Create a new window with modern GL context
			{
				if ((this->windowHandle = Window::CreateGenericWindow(this->hInstance, wtitle.c_str(), CW_USEDEFAULT, CW_USEDEFAULT, sizeX, sizeY)) == NULL)
				{
					assert(0);
					return;
				}

				HDC dc = GetDC(this->windowHandle);
				assert(dc != NULL);

				int pixel_format_arb = 0;
				UINT pixel_formats_found = 0;
				const int pixel_attributes[] = {
					WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
					WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
					WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
					WGL_SWAP_LAYER_BUFFERS_ARB, GL_TRUE,
					WGL_COLOR_BITS_ARB, 24,
					WGL_ALPHA_BITS_ARB, 8,
					WGL_DEPTH_BITS_ARB, 16,
					WGL_STENCIL_BITS_ARB, 8,
					WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
					WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
					WGL_SAMPLES_ARB, 1,
					0
				};

				if (!wglChoosePixelFormatARB)
				{
					printf("wglChoosePixelFormatARB not supported!");
					exit(1);
					return;
				}

				SetLastError(0);

				BOOL result = wglChoosePixelFormatARB(dc, pixel_attributes, NULL, 1, &pixel_format_arb, &pixel_formats_found);

				if (result == FALSE)
				{
					const DWORD err = GetLastError();
					printf("wglChoosePixelFormatARB failed! %u %u 0x%x", pixel_format_arb, pixel_formats_found, err);
					exit(1);
					return;
				}

				PIXELFORMATDESCRIPTOR blank_descriptor = { };
				result = SetPixelFormat(dc, pixel_format_arb, &blank_descriptor);
				assert(result == TRUE);

				const int context_ver_major = 4;
				const int context_ver_minor = 0;
				const int context_attributes[] = {
					WGL_CONTEXT_MAJOR_VERSION_ARB, context_ver_major, WGL_CONTEXT_MINOR_VERSION_ARB, context_ver_minor,
					WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifdef _DEBUG
					WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
					0
				};

				SetLastError(0);
				this->renderContext = wglCreateContextAttribsARB(dc, 0, context_attributes);
				const DWORD err = GetLastError();

				if (wglMakeCurrent(dc, this->renderContext) == FALSE)
				{
					printf("wglMakeCurrent failed! wglCreateContextAttribsARB has GetLastError: 0x%x", err);
					exit(1);
					return;
				}
				printf("Created OpenGL %u.%u core context", context_ver_major, context_ver_minor);

				glViewport(0, 0, this->sizex, this->sizey);
			}
#endif
		}

		Window::~Window() noexcept
		{
			this->GiveupContext();
#if defined ( _WIN32 )
			HDC hdc = GetDC(this->windowHandle);
			wglDeleteContext(this->renderContext);
			ReleaseDC(this->windowHandle, hdc);
			DestroyWindow(this->windowHandle);
#endif
		}


		void Window::ShowWindow(bool enable) noexcept
		{
#if defined ( _WIN32 )
			::ShowWindow(this->windowHandle, SW_SHOW);
#endif
		}

		void Window::AcquireContext() noexcept
		{
#if defined ( _WIN32 )
			HDC hdc = GetDC(this->windowHandle);
			wglMakeCurrent(hdc, this->renderContext);
#endif
		}

		void Window::GiveupContext() noexcept
		{
#if defined ( _WIN32 )
			wglMakeCurrent(NULL, NULL);
#endif
		}

		void Window::EnterMessageLoop() noexcept
		{
#if defined ( _WIN32 )
			this->threadID = GetCurrentThreadId();

			MSG msg = {};
			while (GetMessage(&msg, this->windowHandle, 0, 0) > 0)
			{
				//TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
#endif
		}

		void Window::SwapWindowBuffers() noexcept
		{
#if defined ( _WIN32 )
			HDC hdc = GetDC(this->windowHandle);
			BOOL result = SwapBuffers(hdc);
			//assert(result == TRUE); // False assert failures sometimes?
#endif
		}

		bool Window::CheckFocus()
		{
			GUITHREADINFO info = {};
			info.cbSize = sizeof(GUITHREADINFO);
			GetGUIThreadInfo(this->threadID, &info);
			return info.hwndActive;
		}

#if defined ( _WIN32 )
		LRESULT CALLBACK Window::WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) noexcept
		{
			switch (uMsg)
			{
			case WM_DESTROY:
			{
				if (!Engine::Rendering::noWindowExit)
				{
					printf("Got a WM_DESTROY message!");
					PostQuitMessage(0);
				}
				return 0;
			}
			default:
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
			return 0;
		}

		HWND Window::CreateGenericWindow(HINSTANCE hInst, const wchar_t* title, int xpos, int ypos, int xsize, int ysize) noexcept
		{
			static int winStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

			// Weird hack to make Windows create the correct sized window
			RECT myRect = {};
			myRect.bottom = ysize;
			myRect.right = xsize;
			AdjustWindowRectEx(&myRect, winStyle, FALSE, 0);

			HWND hWind = CreateWindowEx(0, TEXT("GENERIC_WINDOW"), title, winStyle, xpos, ypos, myRect.right, myRect.bottom, NULL, NULL, hInst, NULL);

			printf("Created a generic window");

			return hWind;
		}
		HWND Window::GetHandle() noexcept
		{
			return this->windowHandle;
		}
#endif
	}
}

void spinSleep(double seconds)
{
	static double estimate = 5e-3;
	static double mean = 5e-3;
	static double m2 = 0;
	static int64_t count = 1;

	while (seconds > estimate) {
		const auto start = std::chrono::steady_clock::now();
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
		const auto end = std::chrono::steady_clock::now();

		const double observed = (end - start).count() / 1e9;
		seconds -= observed;

		count++;
		const double delta = observed - mean;
		mean += delta / count;
		m2 += delta * (observed - mean);
		const double stddev = sqrt(m2 / (count - 1));
		estimate = mean + stddev;
	}

	// spin lock
	const auto start = std::chrono::steady_clock::now();
	while ((std::chrono::steady_clock::now() - start).count() / 1e9 < seconds) {};
}

Engine::Rendering::Window* window = nullptr;

static void renderLoop()
{
	while (true)
	{
		window->AcquireContext();
		glClearColor(0.1, 0.13, 0.12, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		window->SwapWindowBuffers();

		spinSleep(0.014);
	}
}

int main()
{	
	window = new Engine::Rendering::Window("test", 500, 500);

	window->ShowWindow(true);

	window->GiveupContext();

	std::thread renderThread(&renderLoop);

	AttachThreadInput(GetThreadId(renderThread.native_handle()), GetCurrentThreadId(), true);

	window->EnterMessageLoop();

	renderThread.join();
}