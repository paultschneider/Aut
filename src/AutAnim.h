// Copyright (c) 2013 Philip M. Hubbard
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// http://opensource.org/licenses/MIT

//
// AutAnim.h
//
// A template class for animating changes to a variable whose type is the template
// parameter.  The animation uses ease-in-ease-out interpolation.

#ifndef __AutAnim__
#define __AutAnim__

#include <chrono>
#include <vector>
#include <memory>

namespace Aut
{
    
    template <typename T>
    class Anim
    {
    public:
        
        Anim();
        ~Anim();
        
        // The animation is specified as a sequence of segments, each having a
        // beginning value and ending value for the variable being animated,
        // and a duration for the animation between those two values.
        
        class Segment
        {
        public:
            
            Segment(T* val = 0, const T& val0 = 0, const T& val1 = 0,
                    std::chrono::seconds duration = std::chrono::seconds(0));
            ~Segment();
            
            // A copy constructor and copy assigment operator are necessary
            // so segments can be put in STL containers.
            
            Segment(const Segment&);
            Segment& operator=(const Segment&);
            
            // Access the beginning and ending values for this segment.
            
            T                    value0() const;
            T                    value1() const;
            
            // Access the duration of this segment's animation.
            
            std::chrono::seconds duration() const;
            
        private:
            
            // Details of the class' data are "hidden" in the Imp.h file.

            class Imp;
            std::unique_ptr<Imp> _m;
            
            friend class Anim;
        };

        // Set the sequence of segments that specify this animation.
        
        void    set(const std::vector<Segment>& segments);
        
        // Start the animation as of the specified time.
        
        void    start(std::chrono::steady_clock::time_point t0 =
                      std::chrono::steady_clock::now());
        
        // Stop the animation.
        
        void    stop();
        
        // Return whether the animation is running or not.
        
        bool    running() const;
        
        // Evaluate the animation as of the specified time.  If theEvalAfterEnd
        // argument is RestartAfterEnd, the animation will cycle; otherwise,
        // evaluating after the summed durations of the segments will return the
        // ending value of the last segment.
        
        enum EvalAfterEnd { RestartAfterEnd, StopAfterEnd };
        
        void    eval(std::chrono::steady_clock::time_point t =
                     std::chrono::steady_clock::now(),
                     EvalAfterEnd afterEnd = RestartAfterEnd);
        
    private:

        // Details of the class' data are "hidden" in the Imp.h file.
        
        class Imp;
        std::unique_ptr<Imp> _m;
    };
}

// The template definitions in the following header file should be considered
// private implementation details.

#include "AutAnimImp.h"

#endif
