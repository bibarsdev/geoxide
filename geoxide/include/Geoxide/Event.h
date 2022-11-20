
#ifndef __GX_EVENT_H__
#define __GX_EVENT_H__

namespace Geoxide {

	enum EventType
	{
		kEventUnknown,
		kEventQuit,
		kEventWindowClose,
		kEventWindowResized,
		kEventWindowFocus,
		kEventWindowLostFocus,
		kEventWindowMoved,
		kEventKeyUp,
		kEventKeyDown,
		kEventMouseButtonUp,
		kEventMouseButtonDown,
		kEventMouseWheel,
		kEventMouseMoved,
	};

	class Event
	{
	public:
		Event(EventType type) : mType(type) {}

		EventType getType() { return mType; };

	private:
		EventType mType;
	};

	class MovedEvent : public Event
	{
	public:
		uint32_t getX() { return mX; }
		uint32_t getY() { return mY; }

	protected:
		MovedEvent(EventType type, uint32_t x, uint32_t y) :
			Event(type), mX(x), mY(y) {}

	private:
		uint32_t mX, mY;
	};

}

#endif
