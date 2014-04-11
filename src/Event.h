#ifndef VORONOI_EVENT_H
#define VORONOI_EVENT_H

#include "Site.h"

namespace Nimbus 
{
	namespace Voronoi
	{
		enum EventType { SITE_EVENT, CIRCLE_EVENT };

		class Event {
		public:
			/** This constructor takes only a point at which an event will occur.
				@param siteOfEvent The point of a site in our initial set of points.
			*/
			Event(Site siteOfEvent);
			
			/** This constructor creates a circle event using a parabola that may be
				'squeezed' and the bottom point of the theoretical circle that will
				trigger the event.
			*/
			Event(Site siteOfEvent, Parabola parabolaOfEvent);
			~Event(void);

		private:
			Site siteOfOccurrence;
			Parabola parabolaOfEvent;
			EventType eventType;

		public:
			/** Get the site that this event will occur at. */
			Site getSiteOfOccurrence(void) { return this->siteOfOccurrence; }

			/** Get whether this is a site event or a circle event. */
			EventType getEventType(void) { return this->eventType; }

			/** Get the parabola of the circle event.
				@return A Parabola that may be squeezed out of the graph
				to create an edge. NULL if this is a site event.
			*/
			Parabola getParabolaAboveEvent(void) { return this->parabolaOfEvent; }
		};
	}
}

#endif VORONOI_EVENT_H