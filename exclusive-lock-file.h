#ifndef EXCLUSIVE_LOCK_FILE_H
#define EXCLUSIVE_LOCK_FILE_H

#include <string>

class exclusive_lock_file
{
public:
    // Throws if unable to acquire the lock
    exclusive_lock_file(const std::string& filename);

    virtual ~exclusive_lock_file();

private:
    exclusive_lock_file(const exclusive_lock_file&) = delete; // not construction-copyable
    exclusive_lock_file& operator=(const exclusive_lock_file&) = delete; // not assignment-copyable

    const std::string filename;
#ifdef _WIN32
    const void* lock;
#else
    int fd;
#endif
};

#endif // EXCLUSIVE_LOCK_FILE_H
