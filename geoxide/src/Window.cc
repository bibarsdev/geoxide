
#ifdef GX_USE_SDL_WINDOW
#include "SDL/SDLWindow.h"
#endif // GX_USE_SDL

namespace Geoxide {

	Window* Window::New(const WindowInit& args)
	{
#ifdef GX_USE_SDL_WINDOW
		return new SDLWindow(args);
#endif // GX_USE_SDL
	}

}
