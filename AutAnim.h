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
//  AutAnim.h
//

#ifndef __AutAnim__
#define __AutAnim__

#include <chrono>
#include <vector>
#include <memory>

namespace Aut
{
    
    //
    
    template <typename T>
    class Anim
    {
    public:
        Anim();
        ~Anim();
        
        class Segment
        {
        public:
            Segment(T* val = 0, const T& val0 = 0, const T& val1 = 0,
                    std::chrono::seconds duration = std::chrono::seconds(0));
            Segment(const Segment&);
            ~Segment();
            
            Segment& operator=(const Segment&);
            
            std::chrono::seconds duration() const;
            T                    value0() const;
            T                    value1() const;
            
        private:
            class Imp;
            std::unique_ptr<Imp> _m;
            
            friend class Anim;
        };

        void    set(const std::vector<Segment>& segments);
        
        void    start(std::chrono::steady_clock::time_point t0 =
                      std::chrono::steady_clock::now());
        void    stop();
        
        bool    running() const;
        
        enum EvalAfterEnd { RestartAfterEnd, StopAfterEnd };
        
        void    eval(std::chrono::steady_clock::time_point t =
                     std::chrono::steady_clock::now(),
                     EvalAfterEnd afterEnd = RestartAfterEnd);
        
    private:
        class Imp;
        std::unique_ptr<Imp> _m;
    };
}

// The template definitions in the following header file should be considered
// private implementation details.

#include "AutAnimImp.h"

#endif
