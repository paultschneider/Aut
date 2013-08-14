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
// AutAnimImp.h
//
// The template definitions in the this header file should be considered
// private implementation details.
//

#ifndef __AutAnimImp_h
#define __AutAnimImp_h

#include "AutAnim.h"
#include <math.h>

namespace Aut
{
    template <typename T>
    class Anim<T>::Segment::Imp
    {
    public:
        Imp(T* v = 0, const T& v0 = 0, const T& v1 = 0,
            std::chrono::seconds d = std::chrono::seconds(0)) :
            val(v), val0(v0), val1(v1), duration(d) {}
        
        void setStart(std::chrono::steady_clock::time_point t0);
        bool contains(std::chrono::steady_clock::time_point t) const;
        void eval(std::chrono::steady_clock::time_point t) const;

        T*                                    val;
        T                                     val0;
        T                                     val1;
        std::chrono::seconds                  duration;
        std::chrono::steady_clock::time_point t0;
    };
    
    template <typename T>
    void Anim<T>::Segment::Imp::setStart(std::chrono::steady_clock::time_point t)
    {
        t0 = t;
    }
    
    template <typename T>
    bool Anim<T>::Segment::Imp::contains(std::chrono::steady_clock::time_point t) const
    {
        return ((t0 <= t) && (t < t0 + duration));
    }
    
    template <typename T>
    void Anim<T>::Segment::Imp::eval(std::chrono::steady_clock::time_point t) const
    {
        std::chrono::steady_clock::time_point t1 = t0 + duration;
        if (t < t0)
        {
            *val = val0;
        }
        else if (t >= t1)
        {
            *val = val1;
        }
        else
        {
            float tt0 = std::chrono::duration_cast<std::chrono::milliseconds>(t - t0).count();
            float t1t0 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
            *val = val0 + ((1 - cosf(tt0 / t1t0 * M_PI)) / 2.0f) * (val1 - val0);
        }
    }

    template <typename T>
    Anim<T>::Segment::Segment(T* val, const T& val0, const T& val1,
                              std::chrono::seconds duration) :
        _m(new Imp(val, val0, val1, duration))
    {
    }
    
    template <typename T>
    Anim<T>::Segment::~Segment()
    {
    }
    
    template <typename T>
    Anim<T>::Segment::Segment(const Segment& o) :
        _m(new Imp(o._m->val, o._m->val0, o._m->val1, o._m->duration))
    {
    }
    
    template <typename T>
    typename Anim<T>::Segment& Anim<T>::Segment::operator=(const Segment&o)
    {
        if (this != &o)
        {
            _m->val = o._m->val;
            _m->val0 = o._m->val0;
            _m->val1 = o._m->val1;
            _m->duration = o._m->duration;
            _m->t0 = o._m->t0;
        }
        return *this;
    }
    
    template <typename T>
    std::chrono::seconds Anim<T>::Segment::duration() const
    {
        return _m->duration;
    }
    
    template <typename T>
    T Anim<T>::Segment::value0() const
    {
        return _m->val0;
    }
    
    template <typename T>
    T Anim<T>::Segment::value1() const
    {
        return _m->val1;
    }
    
    //
    
    template <typename T>
    class Anim<T>::Imp
    {
    public:
        Imp() : running(false) {}
        
        void restart(std::chrono::steady_clock::time_point);
        bool eval(std::chrono::steady_clock::time_point);
        
        std::vector<Segment>                  segments;
        std::chrono::steady_clock::time_point t0;
        bool                                  running;
    };
    
    template <typename T>
    void Anim<T>::Imp::restart(std::chrono::steady_clock::time_point t)
    {
        for (Segment& segment : segments)
        {
            segment._m->setStart(t);
            t += segment.duration();
        }
    }
    
    template <typename T>
    bool Anim<T>::Imp::eval(std::chrono::steady_clock::time_point t)
    {
        for (const Segment& segment : segments)
        {
   
            if (segment._m->contains(t))
            {
                segment._m->eval(t);
                return true;
            }
        }
        
        const Segment& segment = segments[segments.size()-1];
        segment._m->eval(t);
        return false;
    }
    
    template <typename T>
    Anim<T>::Anim() :
        _m(new Imp)
    {
    }
    
    template <typename T>
    Anim<T>::~Anim()
    {
    }
    
    template <typename T>
    void Anim<T>::set(const std::vector<Segment>& segments)
    {
        _m->segments = segments;
    }
    
    template <typename T>
    void Anim<T>::start(std::chrono::steady_clock::time_point t0)
    {
        if (_m->segments.size() > 0)
        {
            _m->running = true;
            _m->restart(t0);
        }
    }
    
    template <typename T>
    void Anim<T>::stop()
    {
        _m->running = false;
    }
    
    template <typename T>
    bool Anim<T>::running() const
    {
        return _m->running;
    }
    
    template <typename T>
    void Anim<T>::eval(std::chrono::steady_clock::time_point t,
                       EvalAfterEnd afterEnd)
    {
        if (_m->running)
        {
            if (!_m->eval(t))
            {
                if (afterEnd == RestartAfterEnd)
                {
                    _m->restart(t);
                    (void) _m->eval(t);
                }
                else
                {
                    _m->running = false;
                }
            }
        }
    }
}

#endif