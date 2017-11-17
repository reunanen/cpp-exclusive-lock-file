#include "exclusive-lock-file.h"
#include <stdexcept>

#ifdef _WIN32
#include <Windows.h>
#else // _WIN32
#error "Not implemented yet"
#endif // _WIN32

exclusive_lock_file::exclusive_lock_file(const std::string& filename)
    : filename(filename)
#ifdef _WIN32
    , lock(CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL))
#else
    // TODO
#endif
{
#ifdef _WIN32
    const bool success = (lock != INVALID_HANDLE_VALUE);
#endif // _WIN32
    if (!success) {
        throw std::runtime_error("Cannot open file " + filename + " for exclusive access.");
    }
}

exclusive_lock_file::~exclusive_lock_file()
{
#ifdef _WIN32
    if (lock != INVALID_HANDLE_VALUE) {
        CloseHandle((HANDLE) lock);
        std::remove(filename.c_str());
    }
#else
    // TODO
#endif
}
