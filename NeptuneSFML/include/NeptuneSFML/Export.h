#ifndef EXPORT_H
#define EXPORT_H

#pragma warning(disable : 4251)

#ifdef NEPTUNE_EXPORT
#define NEPTUNE_API __declspec(dllexport)
#else
#define NEPTUNE_API __declspec(dllimport)
#endif

#endif // EXPORT_H
