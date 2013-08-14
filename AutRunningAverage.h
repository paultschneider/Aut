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
//  AutRunningAverage.h
//
//  An Utility Toolkit
//  A template class to compute running averages.
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
        RunningAverage(size_t cap = 5,
                       std::chrono::milliseconds win = std::chrono::milliseconds(500));
        ~RunningAverage();
        
        size_t                      capacity() const;
        void                        setCapacity(size_t);
        
        std::chrono::milliseconds   window() const;
        void                        setWindow(std::chrono::milliseconds);
        
        void                        add(const T&);
        
        T                           operator()();
        
    private:
        class Imp;
        std::unique_ptr<Imp> _m;
    };
}

// The template definitions in the following header file should be considered
// private implementation details.

#include "AutRunningAverageImp.h"

#endif
