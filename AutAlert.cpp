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
//  AutAlert.cpp
//

#include "AutAlert.h"
#include <iostream>

namespace Aut
{
    namespace
    {
        static std::function<void(const std::string&)> warningFunc = 0;
        static std::function<void(const std::string&)> errorFunc = 0;
        static std::function<void(const std::string&)> fatalErrorFunc = 0;
    }
    
    void warning(const std::string& text)
    {
        if (warningFunc)
            warningFunc(text);
        else
            std::cerr << "Warning: " << text << "\n";
    }
    
    void error(const std::string& text)
    {
        if (errorFunc)
            errorFunc(text);
        else
            std::cerr << "Error: " << text << "\n";
    }

    void fatalError(const std::string& text)
    {
        if (fatalErrorFunc)
        {
            fatalErrorFunc(text);
        }
        else
        {
            std::cerr << "Fatal Error: " << text << "\n";
            abort();
        }
    }
    
    void setWarningFunction(std::function<void(const std::string&)> f)
    {
        warningFunc = f;
    }
    
    void setErrorFunction(std::function<void(const std::string&)> f)
    {
        errorFunc = f;
    }
    void setFatalErrorFunction(std::function<void(const std::string&)> f)
    {
        fatalErrorFunc = f;
    }
    
    std::function<void(const std::string&)> warningFunction()
    {
        return warningFunc;
    }
    
    std::function<void(const std::string&)> errorFunction()
    {
        return errorFunc;
    }
    
    std::function<void(const std::string&)> fatalErrorFunction()
    {
        return fatalErrorFunc;
    }
}
