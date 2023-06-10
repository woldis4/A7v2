#include "Exception.hpp"

string PermissionDenied::out()
{
    return "Permission Denied";
}

string NotFound::out()
{
    return "Not Found";
}

string BadRequest::out()
{
    return "Bad Request";
}

string Empty::out()
{
    return "Empty";
}