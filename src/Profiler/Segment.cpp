#include "Segment.h"
#include <iostream>
#include <sstream>

using namespace Nimbus::Profiler;

Segment::Segment(std::string module, std::string status, Segment* parent) :
    mModule(module),
    mStatus(status),
    mParent(parent),
    mTicks(0),
    mClock()
{
}

Segment::~Segment()
{
    for(std::list<Segment*>::iterator it = this->mChildren.begin(); it != this->mChildren.end(); ++it)
    {
        delete *it;
    }

    this->mChildren.clear();
}

void Segment::start()
{
    // Get the start point
    this->mStartPoint = this->mClock.now();
}

void Segment::end()
{
    // The length definition of a microsecond duration
    typedef std::chrono::duration<int, std::ratio<1, 1000000> > microsecondDuration;

    // Get the end point
    std::chrono::time_point<std::chrono::high_resolution_clock> endPoint = this->mClock.now();

    // Calculate the duration of the profiler segment
    std::chrono::time_point<std::chrono::high_resolution_clock>::duration diff = endPoint - this->mStartPoint;

    // Store the ticks of this duration (one tick = one microsecond)
    this->mTicks = std::chrono::duration_cast<microsecondDuration>(diff).count();
}

Segment* Segment::addChild(std::string module, std::string status)
{
    Segment* child = new Segment(module, status, this);

    this->mChildren.push_back(child);

    return child;
}

Segment* Segment::getParent()
{
    return this->mParent;
}

std::string Segment::getModule()
{
    return this->mModule;
}

std::string Segment::getStatus()
{
    return this->mStatus;
}

int Segment::getMicroseconds()
{
    return this->mTicks;
}

std::list<Segment*>* Segment::getChildren()
{
    return &this->mChildren;
}

std::string Segment::str()
{
    std::stringstream output;

    output << this->mModule << " (" << this->mStatus << "): " << this->mTicks << " microseconds";

    return output.str();
}
