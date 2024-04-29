/* An extremely minimalist syscalls.c for newlib
 * Based on riscv newlib libgloss/riscv/sys_*.c
 *
 * Copyright 2019 Clifford Wolf
 * Copyright 2019 ETH Zürich and University of Bologna
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <computer.h>
#ifdef __cplusplus
extern "C"
{
#endif

#include <sys/stat.h>
#include <newlib.h>
#include <unistd.h>
#include <errno.h>
#undef errno
    extern int errno;

/* It turns out that older newlib versions use different symbol names which goes
 * against newlib recommendations. Anyway this is fixed in later version.
 */
#if __NEWLIB__ <= 2 && __NEWLIB_MINOR__ <= 5
#define _sbrk sbrk
#define _write write
#define _close close
#define _lseek lseek
#define _read read
#define _fstat fstat
#define _isatty isatty
#endif

    void unimplemented_syscall()
    {
    }

    int nanosleep(const struct timespec *rqtp, struct timespec *rmtp)
    {
        errno = ENOSYS;
        return -1;
    }

    int _access(const char *file, int mode)
    {
        errno = ENOSYS;
        return -1;
    }

    int _chdir(const char *path)
    {
        errno = ENOSYS;
        return -1;
    }

    int _chmod(const char *path, mode_t mode)
    {
        errno = ENOSYS;
        return -1;
    }

    int _chown(const char *path, uid_t owner, gid_t group)
    {
        errno = ENOSYS;
        return -1;
    }

    int _close(int file)
    {
        return -1;
    }

    int _execve(const char *name, char *const argv[], char *const env[])
    {
        errno = ENOMEM;
        return -1;
    }

    __attribute__((noreturn)) void _exit(int exit_status)
    {
        asm volatile("ryan_loop_dont_touch:");
        __builtin_unreachable();
    }

    int _faccessat(int dirfd, const char *file, int mode, int flags)
    {
        errno = ENOSYS;
        return -1;
    }

    int _fork(void)
    {
        errno = EAGAIN;
        return -1;
    }

    int _fstat(int file, struct stat *st)
    {
        st->st_mode = S_IFCHR;
        return 0;
        // errno = -ENOSYS;
        // return -1;
    }

    int _fstatat(int dirfd, const char *file, struct stat *st, int flags)
    {
        errno = ENOSYS;
        return -1;
    }

    // int _ftime(struct timeb *tp)
    // {
    //     errno = ENOSYS;
    //     return -1;
    // }

    char *_getcwd(char *buf, size_t size)
    {
        errno = -ENOSYS;
        return NULL;
    }

    int _getpid()
    {
        return 1;
    }

    int _gettimeofday(struct timeval *tp, void *tzp)
    {
        errno = -ENOSYS;
        return -1;
    }

    int _isatty(int file)
    {
        return (file == STDOUT_FILENO);
    }

    int _kill(int pid, int sig)
    {
        errno = EINVAL;
        return -1;
    }

    int _link(const char *old_name, const char *new_name)
    {
        errno = EMLINK;
        return -1;
    }

    off_t _lseek(int file, off_t ptr, int dir)
    {
        return 0;
    }

    int _lstat(const char *file, struct stat *st)
    {
        errno = ENOSYS;
        return -1;
    }

    int _open(const char *name, int flags, int mode)
    {
        return -1;
    }

    int _openat(int dirfd, const char *name, int flags, int mode)
    {
        errno = ENOSYS;
        return -1;
    }

    ssize_t _read(int file, void *ptr, size_t len)
    {
        return 0;
    }

    int _stat(const char *file, struct stat *st)
    {
        st->st_mode = S_IFCHR;
        return 0;
        // errno = ENOSYS;
        // return -1;
    }

    long _sysconf(int name)
    {

        return -1;
    }

    // clock_t _times(struct tms *buf)
    // {
    //     return -1;
    // }

    int _unlink(const char *name)
    {
        errno = ENOENT;
        return -1;
    }

    // int _utime(const char *path, const struct utimbuf *times)
    // {
    //     errno = ENOSYS;
    //     return -1;
    // }

    int _wait(int *status)
    {
        errno = ECHILD;
        return -1;
    }

    ssize_t _write(int file, const char *ptr, size_t len)
    {
        print_uart(ptr, len);
        return len;
    }

    extern char __heap_start[];
    extern char __heap_end[];
    static char *brk = __heap_start;

    int _brk(void *addr)
    {
        brk = (char *)addr;
        return 0;
    }

    void *_sbrk(ptrdiff_t incr)

    {
        char *old_brk = brk;

        if (&__heap_start[0] == &__heap_end[0])
        {
            return NULL;
        }

        if ((brk += incr) < __heap_end)
        {
            brk += incr;
        }
        else
        {
            brk = __heap_end;
        }
        return old_brk;
    }
#ifdef __cplusplus
}
#endif