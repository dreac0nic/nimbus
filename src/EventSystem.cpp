#include "EventSystem.h"

using namespace std;
using namespace Nimbus;

EventSystem EventSystem::singleton = EventSystem();

EventSystem::EventSystem(void)
{
}

EventSystem::~EventSystem(void)
{
}

bool EventSystem::registerListener(const EventListener& listener, EventType type)
{
}

vooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooid EventSystem::unregisterListener(const EventListener& listener, EventType type)
{
}

voooooooooooooooooooooooooooooooooooooooid EventSystem::fireEvent(EventType type, const payloadmap& payload = payloadmap())
{
}