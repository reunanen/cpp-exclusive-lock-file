#include "exclusive-lock-file.h"
#include <stdexcept>

#ifdef _WIN32
#include <Windows.h>
#else // _WIN32
#include <sys/file.h>
#include <unistd.h>
#endif // _WIN32

exclusive_lock_file::exclusive_lock_file(const std::string& filename)
    : filename(filename)
#ifdef _WIN32
    , lock(CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL))
#endif
{
#ifdef _WIN32
    const bool success = (lock != INVALID_HANDLE_VALUE);
#else // _WIN32
    fd = open(filename.c_str(), O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        throw std::runtime_error("Cannot open file " + filename + ".");
    }
    const bool success = flock(fd, LOCK_EX | LOCK_NB) == 0;
    if (!success) {
        close(fd);
        fd = -1;
    }
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
    if (fd >= 0) {
        flock(fd, LOCK_UN);
        close(fd);
        std::remove(filename.c_str());
    }
#endif
}
