/* 
 * Stub implementations of system calls
 *
 */

#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include "stm32g0xx.h"

#undef errno
extern int errno;

register char *stack_ptr asm("sp");

char *__env[1] = {0};
char **environ = __env;

void _exit(int code) { while(1); }

int _close() {
	return -1;
}

int _execve(char *name, char **argv, char **env) {
	errno = ENOMEM;
	return -1;
}

int _fork(void) {
	errno = EAGAIN;
	return -1;
}

int _fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

int _getpid(void) {
	return -1;
}

int _isatty(int file) {
	return 1;
}

int _kill(int pid, int sig) {
	errno = EINVAL;
	return -1;
}

int _link(char *old, char *new) {
	errno = EMLINK;
	return -1;
}

int _lseek(int file, int ptr, int dir) {
	return 0;
}

int _open(const char *name, int flags, int mode) {
	return -1;
}

int _read(int file, char *ptr, int len) {
	return -1;
}

caddr_t _sbrk(int incr) {
	extern char __bss_end__;
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0) {
		heap_end = &__bss_end__;
	}
	prev_heap_end = heap_end;

	if (heap_end + incr > stack_ptr) {
		while(1);
	}

	heap_end += incr;
	return (caddr_t) prev_heap_end;
}

int _stat(const char *file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

int _times(struct tms *buf) {
	return -1;
}

int _unlink(char *name) {
	errno = ENOENT;
	return -1;
}

int _wait(int *status) {
	errno = ECHILD;
	return -1;
}

// override write
int _write(int handle, char* data, int size) {
	int count = size;
	while (count--) {
		while(!(USART2->ISR & USART_ISR_TXE_TXFNF));
		USART2->TDR = *data++;
		while(!(USART2->ISR & USART_ISR_TC));
	}
	return size;
}
