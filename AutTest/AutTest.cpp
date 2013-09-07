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
// AutTest.cpp
//

#include "AutTest.h"

#include "AutRunningAverage.h"
#include "AutAnim.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <assert.h>

namespace Aut
{
    
    void testRunningAverage()
    {
        std::cerr << "Starting Aut::testRunningAverage()\n";
        
        RunningAverage<float> avg(3, std::chrono::milliseconds(750));
        assert (avg.capacity() == 3);
        assert (avg.window() == std::chrono::milliseconds(750));
        
        // Without assuming what kind of average Aut::RunningAverage implements
        // (e.g., it might be weighted or not) we can still test that the
        // running average of one number added multiple times is that number.
        
        avg.add(10.0);
        assert (avg() == 10.0);
        avg.add(10.0);
        assert (avg() == 10.0);
        avg.add(10.0);
        assert (avg() == 10.0);
        avg.add(10.0);
        assert (avg() == 10.0);
        avg.add(11.0);
        assert (avg() != 10.0);
        
        // But if we wait longer than time window and add a different number,
        // then the running average should ignore all the previous additions
        // and return just the new number.
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        avg.add(12.0);
        assert (avg() == 12.0);
        
        std::cerr << "ok\n";
    }
    
    void testAnim()
    {
        std::cerr << "Starting Aut::testAnim()\n";
        
        Aut::Anim<double> animD;
        double D = 0.0;
        
        std::vector<Aut::Anim<double>::Segment> segmentsD;
        segmentsD.push_back(Aut::Anim<double>::Segment(&D, 0.0, 10.0,
                                                       std::chrono::seconds(1)));
        segmentsD.push_back(Aut::Anim<double>::Segment(&D, 10.0, 5.0,
                                                       std::chrono::seconds(1)));
        animD.set(segmentsD);
        
        Aut::Anim<int> animI;
        int I = 0;
        
        std::vector<Aut::Anim<int>::Segment> segmentsI;
        segmentsI.push_back(Aut::Anim<int>::Segment(&I, 0, 100,
                                                    std::chrono::seconds(1)));
        segmentsI.push_back(Aut::Anim<int>::Segment(&I, 100, 300,
                                                    std::chrono::seconds(1)));
        animI.set(segmentsI);
        
        assert (!animD.running());
        animD.start();
        assert (animD.running());
        
        assert (!animI.running());
        animI.start();
        assert (animI.running());
        
        // Given the uncertainty about the timing of confidence test, it is easiest
        // to just verify that if we evaluate after the animations are over then
        // D and I are set to the ending values for their animations.
        
        std::this_thread::sleep_for(std::chrono::seconds(3));
        
        animD.eval(std::chrono::steady_clock::now(),
                   Aut::Anim<double>::StopAfterEnd);
        animI.eval(std::chrono::steady_clock::now(),
                   Aut::Anim<int>::StopAfterEnd);
        
        assert (!animD.running());
        assert (!animI.running());
        
        assert (D == 5.0);
        assert (I == 300);
        
        std::cerr << "ok\n";
    }
    
}
