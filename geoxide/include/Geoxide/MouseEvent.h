
#ifndef __GX_MOUSE_EVENT_H__
#define __GX_MOUSE_EVENT_H__

#include "Event.h"

namespace Geoxide {

	enum MouseButton
	{
		kMouseButtonUnknown,
		kMouseButtonLeft,
		kMouseButtonMiddle,
		kMouseButtonRight,
	};

	class MouseButtonEvent : public Event
	{
	public:
		MouseButton getButton() { return mButton; }

	protected:
		MouseButtonEvent(EventType type, MouseButton button) : Event(type), mButton(button) {}

	private:
		MouseButton mButton;
	};

	class MouseButtonUpEvent : public MouseButtonEvent
	{
	public:
		MouseButtonUpEvent(MouseButton button) : MouseButtonEvent(kEventMouseButtonUp, button) {}
	};

	class MouseButtonDownEvent : public MouseButtonEvent
	{
	public:
		MouseButtonDownEvent(MouseButton button) : MouseButtonEvent(kEventMouseButtonDown, button) {}
	};

	class MouseWheelEvent : public Event
	{
	public:
		MouseWheelEvent(uint32_t delta) : Event(kEventMouseWheel), mDelta(delta) {}

		uint32_t getDelta() { return mDelta; }

	private:
		uint32_t mDelta;
	};

	class MouseMovedEvent : public MovedEvent
	{
	public:
		MouseMovedEvent(uint32_t x, uint32_t y, uint32_t xRel, uint32_t yRel) : MovedEvent(kEventMouseMoved, x, y), mXRel(xRel), mYRel(yRel) {}

		uint32_t getXRel() { return mXRel; }
		uint32_t getYRel() { return mYRel; }

	private:
		MouseButton mMouseButton;
		uint32_t mXRel, mYRel;
	};

}

#endif
