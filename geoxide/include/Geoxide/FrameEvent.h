
#ifndef __GX_FRAME_EVENT_H__
#define __GX_FRAME_EVENT_H__

#include "Event.h"

namespace Geoxide {

	class FrameEvent : public Event
	{
	public:
		FrameEvent(float elapsedTime) : Event(kEventFrame), mElapsedTime(elapsedTime) {}

		// Time since last frame in seconds
		float getElapsedTime() { return mElapsedTime; }

	private:
		float mElapsedTime;

		friend class Application;
	};
}

#endif
