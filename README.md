This is a simple Backtrace Impl.


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

