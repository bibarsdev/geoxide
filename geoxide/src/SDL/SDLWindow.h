
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
		uint32_t getClientWidth() const override;
		uint32_t getClientHeight() const override;
		uint32_t getX() const override;
		uint32_t getY() const override;
		bool getVisibility() const override { return mVisible; }

		void setVisibility(bool visible) override;

		Event* pollEvent() override;
		void destroyWindow() override;

	private:
		SDL_Window* mWindow;
		bool mVisible;
		inline static bool sSDLInitialized = false;
	};
}

#endif
