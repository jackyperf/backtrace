/*
 * Copyright (C) lb.wang <wlb_dlut@163.com>
 * description: 
 */

#include <stdio.h>
#include <signal.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>

#define DEBUG
#define GET_RBP(addr) asm("movq %%rbp, %0" : "=r"(addr))

static void print_fun_name(void *addr)
{
	Dl_info dl_info;

	dladdr(addr, &dl_info);
	printf("function : %s\n", dl_info.dli_sname);
}

static void print_backtrace()
{
	print_fun_name((void *)(print_backtrace - 1));	
}

static void signal_handle(int signal_no)
{
#ifdef DEBUG
	printf("catch signal %d\n", signal_no);
#endif
	print_backtrace();
}

int main(int argc, char **argv)
{
	if (signal(SIGUSR1, signal_handle) == SIG_ERR) {
		printf("install signal fail!");
		exit(-1);
	}

	for ( ; ; ) {
		pause();
	}

	return 0;
}
