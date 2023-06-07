#pragma once

#if defined ( _WIN32 )
#include <Windows.h>
#endif

namespace Engine
{
	namespace Rendering
	{
		extern bool noWindowExit;

		class Window final
		{
		private:
#if defined ( _WIN32 )
			HINSTANCE hInstance;
			HGLRC renderContext;
			HWND windowHandle;
			DWORD threadID;

			static LRESULT CALLBACK WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) noexcept;
			static HWND CreateGenericWindow(HINSTANCE hInst, const wchar_t* title, int xpos, int ypos, int xsize, int ysize) noexcept;
#endif
		public:
			const int sizex, sizey;

			Window(const char* title, unsigned int sizeX = 800, unsigned int sizeY = 600) noexcept;
			~Window() noexcept;

			void ShowWindow(bool enable) noexcept;
			void EnterMessageLoop() noexcept;
			void GiveupContext() noexcept;
			void AcquireContext() noexcept;
			void SwapWindowBuffers() noexcept;
			bool CheckFocus();

#if defined ( _WIN32 )
			HWND GetHandle() noexcept;
#endif
		};
	}
}