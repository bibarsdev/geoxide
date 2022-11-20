
#ifndef __GX_KEY_EVENT_H__
#define __GX_KEY_EVENT_H__

#include "KeyCode.h"
#include "Event.h"

namespace Geoxide {

	class KeyEvent : public Event
	{
	public:
		KeyCode getKeyCode() { return mKey; }

	protected:
		KeyEvent(EventType type, KeyCode k) : Event(type), mKey(k) {}

	private:
		KeyCode mKey;
	};

	class KeyUpEvent : public KeyEvent
	{
	public:
		KeyUpEvent(KeyCode k) : KeyEvent(kEventKeyUp, k) {}
	};

	class KeyDownEvent : public KeyEvent
	{
	public:
		KeyDownEvent(KeyCode k, bool repeat) : KeyEvent(kEventKeyDown, k), mRepeat(repeat) { }

		bool isRepeat() { return mRepeat; }

	private:
		bool mRepeat;
	};

}

#endif
