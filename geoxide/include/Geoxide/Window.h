
#ifndef __GX_WINDOW_H__
#define __GX_WINDOW_H__

#include "Event.h"

namespace Geoxide {

	// Currently, Geoxide uses SDLWindow as the only implementation of the window interface.
	// SDLWindow uses SDL which in itself is an abstraction layer, which is extra overhead.
	// Per-platform implementations of the window interface would be a better approach.
	// 

	struct WindowInit
	{
		uint32_t width, height;
		uint32_t x, y;
		std::string title;
	};

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual NativeHandle getNativeHandle() const = 0;
		virtual const char* getTitle() const = 0;
		virtual uint32_t getClientWidth() const = 0;
		virtual uint32_t getClientHeight() const = 0;
		virtual uint32_t getX() const = 0;
		virtual uint32_t getY() const = 0;
		virtual bool getVisibility() const = 0;

		virtual void setVisibility(bool visible) = 0;

		virtual void startImGuiFrame() = 0;
		virtual Event* pollEvent() = 0;
		virtual void destroyWindow() = 0;

		static Window* New(const WindowInit& args);
	};
}

#define GX_WINDOWPOS_CENTERED -1

#endif
