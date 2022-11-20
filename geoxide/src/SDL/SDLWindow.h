
#ifndef __GX_SDL_WINDOW_H__
#define __GX_SDL_WINDOW_H__

#include "Geoxide/Window.h"

#include <SDL.h>

namespace Geoxide {

	class SDLWindow : public Window
	{
	public:
		SDLWindow(const WindowInit& args);
		~SDLWindow();

		NativeHandle getNativeHandle() const override;
		const char* getTitle() const override;
		uint32_t getWidth() const override;
		uint32_t getHeight() const override;
		uint32_t getX() const override;
		uint32_t getY() const override;

		void setVisibility(bool visible) override;

		Event* pollEvent() override;
		void destroyWindow() override;

	private:
		SDL_Window* mWindow;
		inline static bool sSDLInitialized = false;
		GX_DECLARE_LOG("SDLWindow");
	};
}

#endif
