Better String
============

After reading [this](https://groups.google.com/a/chromium.org/forum/#!msg/chromium-dev/EUqoIz2iFU4/kPZ5ZK0K3gEJ) post in the Chromium-dev group I thought it was about time to take a crack at creating a better C++ string library for average use. This is the result.

The better string library contains a whole host of improvements! Delayed transfer of ownership, Delayed memory allocation and copying, extended feature set built in (conversions, parsers, constructers), and a number of optimizations built in.

Better Strings contain all of the behavior of std::string and more! Preform common operations like converting a string to an integer, preforming an insensitive comparision, or even checking if a string is a palindrome right out of the box with a single function call.

Usage
============

Better string is ready for out of the box use in any C++ project. Simply include BetterString.cpp and BetterString.h in your project and you're ready to replace all those nasty std::strings.
