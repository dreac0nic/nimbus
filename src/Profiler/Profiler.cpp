#include "Profiler.h"
#include <iostream>
#include <sstream>

using namespace Nimbus::Profiler;

Profiler::Profiler()
{
    this->mRootSegment = new Segment("Profiler", "Run", NULL);
    this->mCurrentSegment = this->mRootSegment;
}

Profiler::Profiler(std::string module, std::string status)
{
    this->mRootSegment = new Segment(module, status, NULL);
    this->mCurrentSegment = this->mRootSegment;
}

Profiler::~Profiler()
{
    // Delete the root segment
    delete this->mRootSegment;

    // Reset all segment pointers
    this->mCurrentSegment = NULL;
    this->mRootSegment = NULL;
}

void Profiler::start()
{
    this->mCurrentSegment->start();
}

void Profiler::end()
{
    this->mCurrentSegment->end();
}

void Profiler::startSegment(std::string module, std::string status)
{
    this->mCurrentSegment = this->mCurrentSegment->addChild(module, status);

    this->mCurrentSegment->start();
}

void Profiler::endSegment(std::string module, std::string status)
{
    if(this->mCurrentSegment->getModule().compare(module) == 0 && this->mCurrentSegment->getStatus().compare(status) == 0)
    {
        this->mCurrentSegment->end();

        this->mCurrentSegment = this->mCurrentSegment->getParent();
    }
}

Segment* Profiler::getRoot()
{
    return this->mRootSegment;
}

std::string Profiler::str()
{
    return strTree(this->mRootSegment);
}

std::string Profiler::sqlOut()
{
    std::stringstream output;

    this->mSqlId = 0;

    output << "create database profile;" << std::endl
           << "create table segment_table (id INT, module TEXT, status TEXT, duration INT, parent_id INT);" << std::endl;

    output << sqlTree(this->mRootSegment, this->mSqlId);

    return output.str();
}

std::string Profiler::strTree(Segment *printNode)
{
    std::stringstream output;
    std::list<Segment*>* childList = printNode->getChildren();

    output << printNode->str();

    if(childList->size() > 0)
    {
        output << std::endl << "-----";

        for(std::list<Segment*>::iterator segment = childList->begin(); segment != childList->end(); ++segment)
        {
            output << std::endl << strTree(*segment);
        }

        output << std::endl << "-----";
    }

    return output.str();
}

std::string Profiler::sqlTree(Segment *printNode, int parentId)
{
    std::stringstream output;
    std::list<Segment*>* children = printNode->getChildren();

    output << "insert into segment_table (id, module, status, duration, parent_id) values ("
           << ++this->mSqlId << ", '"
           << printNode->getModule() << "', '"
           << printNode->getStatus() << "', "
           << printNode->getMicroseconds() << ", "
           << parentId << ");";

    if(children->size() > 0)
    {
        int parentId = this->mSqlId;

        for(std::list<Segment*>::iterator segment = children->begin(); segment != children->end(); ++segment)
        {
            output << std::endl << sqlTree(*segment, parentId);
        }
    }

    return output.str();
}
