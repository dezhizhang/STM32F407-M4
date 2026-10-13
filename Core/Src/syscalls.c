/**
 * syscalls.c — newlib 系统调用桩
 * printf 最终走到 _write，重定向到 USART1。
 * 堆管理 _sbrk 基于链接脚本导出的 _end 符号向上生长。
 */
#include <stdint.h>
#include <stddef.h>     /* ptrdiff_t */
#include <sys/stat.h>   /* struct stat / S_IFCHR */
#include <sys/time.h>   /* struct timeval */
#include <errno.h>

#undef errno
extern int errno;
extern int _end;                 /* 链接脚本 ._user_heap_stack 段起始 */

/* printf 输出重定向到 USART1（115200） */
extern int fputc_target_usart1(const char *buf, int len);

int _write(int fd, const char *buf, int len)
{
  (void)fd;
  return fputc_target_usart1(buf, len);
}

int _read(int fd, char *buf, int len)
{
  (void)fd; (void)buf; (void)len;
  return -1;    /* 暂不支持标准输入 */
}

int _close(int fd)   { (void)fd; return -1; }
int _fstat(int fd, struct stat *st) { (void)fd; st->st_mode = S_IFCHR; return 0; }
int _isatty(int fd)  { (void)fd; return 1; }
int _lseek(int fd, int off, int whence) { (void)fd; (void)off; (void)whence; return -1; }

void *_sbrk(ptrdiff_t incr)
{
  /* 堆指针，静态记录；栈从 RAM 顶向下长，堆从 _end 向上长 */
  static unsigned char *heap = NULL;
  unsigned char *prev;

  if (heap == NULL)
    heap = (unsigned char *)&_end;

  prev = heap;
  heap += incr;

  /* 简单溢出检查：撞到栈区（留 512B 余量）就报内存不足 */
  extern char _estack;   /* 链接脚本：RAM 顶 */
  if ((uintptr_t)heap > ((uintptr_t)&_estack - 512))
  {
    heap = prev;
    errno = ENOMEM;
    return (void *)-1;
  }
  return prev;
}

void _kill(int pid, int sig)   { (void)pid; (void)sig; errno = EINVAL; }
int  _getpid(void)             { return 1; }
void _exit(int status)         { (void)status; while (1) { } }
int  _gettimeofday(struct timeval *tv, void *tz) { (void)tv; (void)tz; return -1; }

/* _init/_fini：-nostartfiles 时 crti.o/crtn.o 不参与链接，
 * __libc_init_array 会引用它们，这里提供空实现（等价 crti 的行为） */
void _init(void) { }
void _fini(void) { }
