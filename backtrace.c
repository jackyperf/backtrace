/*
 * This is a simple Backtrace Impl.
 *
 * Copyright (C) lb.wang <wlb_dlut@163.com>
 * 
 * Backtraceimpl:
 *  H |----------|
 *|-> |----------| <---- parent_rbp
 *|   |----------|
 *|   |----------|
 *|   |----------|
 *|   |----------| <---- parent_ret
 *|-- |parent_rbp| <---- child_rbp
 *	  |----------|
 *	  |----------|
 *	  |----------|
 *	L |----------| <---- child_rsp 
 *	
 *	1> get current rbp register
 *	2> cal parent fun return address
 *	3> use 'dladdr' to get parent fun info
 *	   --- according symbol table we can get
 *	   --- fun start addr and size, as we know
 *	   --- fun start_addr <= return <= start + size
 *	4> get parent fun rbp
 *	5> goto 1>
 *
 *	Note: how do we get stack end address?
 *	--- /proc/pid/
 *	--- __libc_stack_end ? recent GLIBC NOT support
 *	--- other ???
 */

#include <stdio.h>
#include <signal.h>
#include <stdint.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>

#define DEBUG
#define GET_RBP(addr) asm("movq %%rbp, %0":"=r"(addr))
#define GET_RSP(addr) asm("movq %%rsp, %0":"=r"(addr))
#define MAX_STACK_DEPTH 5

// Stack end addr
//extern void *_libc_stack_end;

static void print_fun_name(void *addr)
{
	Dl_info dl_info;

	dladdr(addr, &dl_info);
	printf("<%s> ---- %s\n", dl_info.dli_fname, dl_info.dli_sname);
}

static void print_backtrace()
{
	uint64_t *stack_top;
	uint64_t *stack_curr;
	int stack_depth = 0;
	//uint64_t *stack_end = (uint64_t *)_libc_stack_end;

	GET_RSP(stack_top);
	GET_RBP(stack_curr);
	while (stack_curr >= stack_top && stack_depth < MAX_STACK_DEPTH) {
		uint64_t * ret_addr = (uint64_t *)*(stack_curr + 1);

		print_fun_name(ret_addr);
		stack_curr = (uint64_t *)(*stack_curr);
		stack_depth++;
	}
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
