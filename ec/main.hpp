#ifndef MAIN_H
#define MAIN_H

#include <map>
#include <string>
#include <windows.h>

#ifdef EC_EXPORTS
#define EC_API __declspec(dllexport)
#else
#define EC_API __declspec(dllimport)
#endif

#endif
