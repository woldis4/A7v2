#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

#include <iostream>
#include <string>

using namespace std;


class PermissionDenied
{
public:
    string out();
};

class NotFound
{
public:
    string out();
};

class BadRequest
{
public:
    string out();
};

class Empty
{
public:
    string out();
};

#endif