Aut: A Utility Toolkit
======================

By Philip M. Hubbard, 2013


Overview
--------

Aut is a C++ library that provides some useful utility functions.  While the library was designed to be relatively general, it was implemented to meet the needs of the Facetious application.  It has classes to define animations as sequences of ease-in-ease-out interpolations with specific durations.  It also has code for a running-average computation (which Facetious uses to stabilize the sometimes-jittery results of the face tracker).  Its most generally useful functionality is a set of functions for reporting warnings, errors and fatal errors, with a way for an application to define how these reports are presented.


Implementation
--------------

`Aut::Anim<T>` is a template class for animating changes to a variable of type T.  The animation is defined as a series of segments, each with a beginning value, ending value and time duration, represented by the `Aut::Anim<T>::Segment` class.  The animation uses an ease-in-ease-out form of interpolation based on the cosine function.

`Aut::RunningAverage<T>` is a template class for computing running averages of values of type T.  Values are added to an instance of `Aut::RunningAverage<T>`, and `operator()` returns the average of the last N values added, where N is the capacity specified for the instance.  The instance has a time window, and if the time between adding values exceeds the window then older values are dropped so they do not affect the average returned by operator().  The time window thus prevents out-of-date values from skewing recent values (which is useful for how Facetious uses the running average to stabilize the face tracker).

`Aut::warning()`, `Aut::error()` and `Aut::fatalError()` allow code to report warnings and errors (as strings) without worrying about how they will be reported.  `Aut::setWarningFunction()`, `Aut::setErrorFunction()` and `Aut::setFatalErrorFunction()` allow an application to specify the functions that will handle the reporting.  Although there are distinct functions for errors and fatal errors, it is up the the application-specified functions to treat fatal errors differently (e.g., by calling `abort()`).


Testing
-------

AutTest is a set of confidence tests for (parts of) Aut.

The test for `Aut::RunningAverage<T>` does not assume what kind of average is being performed, so the implementation of `Aut::RunningAverage<T>` could be changed to use some sort of weighting in the average.  The test mainly confirms that values outside the time window do not affect the average.

The test for `Aut::Anim<T>` is complicated by the way the results to be tested could change with the specific timing of how the test runs.  So the test is limited to waiting until well after the end of a non-cycling animation and then checking that evaluating the animation produces the ending value.  The test does also verify that the template can be instituted for several types.


Building
--------

Aut does not depend on any other libraries, other than system libraries that appear by default in Xcode.  The specific version of Xcode used to develop Aut was 4.6.3.

AutTest is set up as another target in the Xcode project for Aut.  This setting and others are stored in Aut.xcodeproj/project.pbxproj.

The project has build setting of "Installation Directory" to "@rpath".  This setting allows the library to be found when it is embedded in an application bundle.  The application should have a build setting of "Runpath Search Paths" to "@loader_path/../Frameworks" and a "Copy Files" build phase to copy the library into the Frameworks section of its bundle.

Aut uses a few C++11 features, like the `std::function` templates and the `std::chrono` classes for representing time.  It should not be difficult to remove those features, assuming that an appropriate version of Boost is available to provide replacements for those features.
