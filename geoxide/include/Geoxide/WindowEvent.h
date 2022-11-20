
#ifndef __GX_WINDOW_EVENT_H__
#define __GX_WINDOW_EVENT_H__

#include "Event.h"

namespace Geoxide {

	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(uint32_t width, uint32_t height) : Event(kEventWindowResized), mWidth(width), mHeight(height) {}

		uint32_t getWidth() { return mWidth; }
		uint32_t getHeight() { return mHeight; }

	private:
		uint32_t mWidth, mHeight;
	};

	class WindowMovedEvent : public MovedEvent
	{
	public:
		WindowMovedEvent(uint32_t x, uint32_t y) : MovedEvent(kEventWindowMoved, x, y) {}
	};

}

#endif
