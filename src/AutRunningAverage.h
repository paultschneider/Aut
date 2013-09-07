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
// AutRunningAverage.h
//
// A template class to compute running averages, with a time window to prevent
// out-of-date values from skewing recent values.
//

#ifndef _AutRunningAverage_h
#define _AutRunningAverage_h

#include <chrono>
#include <memory>

namespace Aut
{
    
    template <typename T>
    class RunningAverage
    {
    public:
        
        // See the comments below for explanations of the capacity and
        // time window.
        
        RunningAverage(size_t capacity = 5,
                       std::chrono::milliseconds window =
                       std::chrono::milliseconds(500));
        ~RunningAverage();
        
        // Set and get the number of the most recent values that will be used in
        // the average, assuming that the time between values does not exceed the
        // time window.
        
        void                        setCapacity(size_t);
        size_t                      capacity() const;
        
        // Set and get the time window.  If a value is added and the time since
        // the previously added value exceeds the time window, then the average
        // is reset to just the most recently added value (so the older values
        // do not skew the average).
        
        void                        setWindow(std::chrono::milliseconds);
        std::chrono::milliseconds   window() const;
        
        // Add a value to be used in the running average.
        
        void                        add(const T&);
        
        // Return the running average.
        
        T                           operator()();
        
    private:

        // Details of the class' data are "hidden" in the Imp.h file.
        
        class Imp;
        std::unique_ptr<Imp> _m;
    };
    
}

// The template definitions in the following header file should be considered
// private implementation details.

#include "AutRunningAverageImp.h"

#endif
