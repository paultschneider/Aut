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
//  AutRunningAverageImp.h
//
// The template definitions in the this header file should be considered
// private implementation details.
//

#ifndef _AutRunningAverageImp_h
#define _AutRunningAverageImp_h

#include <deque>

namespace Aut
{
    template <typename T>
    class RunningAverage<T>::Imp
    {
    public:
        Imp(size_t cap, std::chrono::milliseconds win) : capacity(cap), window(win) {}
        std::chrono::milliseconds                           window;
        size_t                                              capacity;
        std::chrono::time_point<std::chrono::system_clock>  time;
        std::deque<T>                                       deque;
    };

    template <typename T>
    RunningAverage<T>::RunningAverage(size_t cap, std::chrono::milliseconds win) :
        _m(new Imp(cap, win))
    {
    }

    template <typename T>
    RunningAverage<T>::~RunningAverage()
    {
    }

    template <typename T>
        size_t RunningAverage<T>::capacity() const
    {
        return _m->capacity;
    }

    template <typename T>
    void RunningAverage<T>::setCapacity(size_t cap)
    {
        _m->capacity = cap;
        while (_m->deque.size() > _m->capacity)
            _m->deque.pop_front();
    }

    template <typename T>
    std::chrono::milliseconds RunningAverage<T>::window() const
    {
        return _m->window;
    }

    template <typename T>
    void RunningAverage<T>::setWindow(std::chrono::milliseconds win)
    {
        _m->window = win;
    }

    template <typename T>
    void RunningAverage<T>::add(const T& x)
    {
        std::chrono::time_point<std::chrono::system_clock> now =
            std::chrono::system_clock::now();

        if (now - _m->time > _m->window)
            _m->deque.clear();
        
        _m->time = now;
        
        _m->deque.push_back(x);
        if (_m->deque.size() > _m->capacity)
            _m->deque.pop_front();
    }

    template <typename T>
    T RunningAverage<T>::operator()()
    {
        T sum = T();
        for (auto x : _m->deque)
            sum += x;
        return sum / _m->deque.size();
    }

}

#endif
