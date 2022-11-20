
#include "SDLWindow.h"

#include "Geoxide/MouseEvent.h"
#include "Geoxide/KeyEvent.h"
#include "Geoxide/WindowEvent.h"

#include <SDL_syswm.h>

namespace Geoxide {

	SDLWindow::SDLWindow(const WindowInit& args)
	{
		if (!sSDLInitialized)
		{
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
			{
				std::string SDLError = SDL_GetError();
				sLog.error("SDL initialization failed! " + SDLError);
			}
			sSDLInitialized = true;
		}
		

		mWindow = SDL_CreateWindow(
			args.title.c_str(),
			args.x == GX_WINDOWPOS_CENTERED ? SDL_WINDOWPOS_CENTERED : args.x,
			args.y == GX_WINDOWPOS_CENTERED ? SDL_WINDOWPOS_CENTERED : args.y,
			args.width, args.height, SDL_WINDOW_HIDDEN);

		if (!mWindow)
		{
			std::string SDLError = SDL_GetError();
			sLog.error("Failed to create window \'" + args.title + "\', SDLError=\'" + SDLError + "\'");
		}

		sLog.info("Created a new window");

		sLog.trace("Title: " + std::string(getTitle()));
		sLog.trace("Width: " + std::to_string(getWidth()));
		sLog.trace("Height: " + std::to_string(getHeight()));
		sLog.trace("X: " + std::to_string(getX()));
		sLog.trace("Y: " + std::to_string(getY()));
	}

	SDLWindow::~SDLWindow()
	{
		std::string name = getTitle();

		SDL_DestroyWindow(mWindow);

		sLog.info("Destroyed window \'" + name + "\'");
	}

	NativeHandle SDLWindow::getNativeHandle() const
	{
#ifdef GX_PLATFORM_WIN32
		SDL_SysWMinfo info = {};
		SDL_GetWindowWMInfo(mWindow, &info);

		return info.info.win.window;
#elif
		// TODO
#endif // GX_PLATFORM_WIN32

		return 0;
	}

	const char* SDLWindow::getTitle() const
	{
		return SDL_GetWindowTitle(mWindow);
	}

	uint32_t SDLWindow::getWidth() const
	{
		int i = 0;
		SDL_GetWindowSize(mWindow, &i, 0);
		return i;
	}

	uint32_t SDLWindow::getHeight() const
	{
		int i = 0;
		SDL_GetWindowSize(mWindow, 0, &i);
		return i;
	}

	uint32_t SDLWindow::getX() const
	{
		int i = 0;
		SDL_GetWindowPosition(mWindow, &i, 0);
		return i;
	}

	uint32_t SDLWindow::getY() const
	{
		int i = 0;
		SDL_GetWindowPosition(mWindow, 0, &i);
		return i;
	}

	void SDLWindow::setVisibility(bool visible)
	{
		if (visible)
			SDL_ShowWindow(mWindow);
		else
			SDL_HideWindow(mWindow);
	}

	Event* SDLWindow::pollEvent()
	{
		SDL_Event SDLev = {};
		Event* ev = 0;

		if(!SDL_PollEvent(&SDLev))
			return 0;

		switch (SDLev.type)
		{
		case SDL_WINDOWEVENT:

			switch (SDLev.window.type)
			{
			case SDL_WINDOWEVENT_MOVED:
				return new WindowMovedEvent(SDLev.window.data1, SDLev.window.data2);
			case SDL_WINDOWEVENT_RESIZED:
				return new WindowResizedEvent(SDLev.window.data1, SDLev.window.data2);
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				return new Event(kEventWindowFocus);
			case SDL_WINDOWEVENT_FOCUS_LOST:
				return new Event(kEventWindowLostFocus);
			case SDL_WINDOWEVENT_CLOSE:
				return new Event(kEventWindowClose);
			}
			break;

		case SDL_KEYUP:
			return new KeyUpEvent((KeyCode)SDLev.key.keysym.sym);
		case SDL_KEYDOWN:
			return new KeyDownEvent((KeyCode)SDLev.key.keysym.sym, SDLev.key.repeat);
		case SDL_MOUSEBUTTONUP:
			return new MouseButtonUpEvent((MouseButton)(SDLev.button.button + 1));
		case SDL_MOUSEBUTTONDOWN:
			return new MouseButtonDownEvent((MouseButton)(SDLev.button.button + 1));
		case SDL_MOUSEWHEEL:
			return new MouseWheelEvent(SDLev.wheel.y);
		case SDL_MOUSEMOTION:
			return new MouseMovedEvent(SDLev.motion.x, SDLev.motion.y, SDLev.motion.xrel, SDLev.motion.yrel);
		}

		return 0;
	}
}
