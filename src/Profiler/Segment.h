#ifndef NIMBUS_SEGMENT_H
#define NIMBUS_SEGMENT_H

#include <string>
#include <list>
#include <chrono>

namespace Nimbus
{
    namespace Profiler
    {
        /** The Segment class contains data about a specific execution period.
            It is identified by a Module and a Status value and contains the
            duration of execution time for the execution segment in microseconds.

            It is mostly an internal class used by the Profiler object. Read that
            documentation for more information on how to use the Profiler.
         */
        class Segment
        {
        private:
            // Private members

            /// The module of the segment
            std::string mModule;

            /// The status of the module during the segment
            std::string mStatus;

            /// The starting time of the segment
            std::chrono::time_point<std::chrono::high_resolution_clock> mStartPoint;

            /// The number of ticks of the segment
            int mTicks;

            /// The parent profiler segment
            Segment* mParent;

            /// The children profiler segments
            std::list<Segment*> mChildren;

            /// The clock keeping track of the profiler segment timing
            std::chrono::high_resolution_clock mClock;

        public:
            /** Creates a profiler segment.

                @param module The module of the segment.
                @param status The status of the module for the segment.
                @param parent The parent segment of the new segment.
             */
            Segment(std::string module, std::string status, Segment* parent);

            /** Destroys a profiler segment.
             */
            ~Segment();

            /** Starts the timer of the segment.
             */
            void start();

            /** Ends the timer of the segment.
             */
            void end();

            /** Adds a child profiler segment to this one.

                @param module The module of the new child segment.
                @param status The status of the module of the child segment.
                @return A pointer to the new child segment.
             */
            Segment* addChild(std::string module, std::string status);

            /** Gets the parent profiler segment of this one.

                @return A pointer to the parent segment.
             */
            Segment* getParent();

            /** Get the module the segment measures.

                @return A string containing the segment module.
             */
            std::string getModule();

            /** Get the status of the module during the segment.

                @return A string containing the status of the module.
             */
            std::string getStatus();

            /** Gets the microsecond duration of the profiler segment.

                @return An integer counting the microsecond duration.
             */
            int getMicroseconds();

            /** Gets the list of children for the segment.

                @return An STL list containing children segments.
             */
            std::list<Segment*>* getChildren();

            /** Builds a string summarizing the results of this segment.
                No child segments are included.

                @return A string containing a summary of the profile segment results.
             */
            std::string str();
        };
    }
}

#endif // NIMBUS_SEGMENT_H
