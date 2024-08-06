### POSIX API
File operators
- `open(filename)`, `read(fd,buf,count)`, `write(fd,buf,count)`, `close(fd)`
Process control
- `fork()`, `exec()`, `wait()`, `exit()`
Thread Management
- `pthread_create()`, `pthread_join()`,  `pthread_mutex_lock()`,  `pthread_mutex_unlock()` 
Memory Management
- `malloc()`, `free()`
- Shared memory object: `shm_open()` returns FD
Scheduling 
- `schedule()`
Networking
- `socket()`, `connect()`, `bind()`, `listen()`, `accept()`
### Services
- `systemd` - first running process (init service)
	- first process to start up, last to exit
	- to start services we use `systemctl start service_name`
- `kswapd` - manages page swaps / eviction

### Kernel
- `uname -a` - get kernel info
- `sysctl` (system control) - get or set kernel **runtime** parameters 
	- to make changes permanent, update `/etc/sysctl.conf`
- update compile / boot params in `/etc/default/grub` 
	- `GRUB` = Boot loader
#### Kernel Versions
- 6.10 - latest release
- 6.9 - improvements to startup time with huge pages
	- huge pages improve page table lookup (larger pages mean u can use less layers = faster lookup)
- 6.8 - Google released 40% speed up on TCP performance
	- cache line optimization particularly for AMD chips
- 6.1 - latest Debian release (bookworm) (released in 2022)
	- `Multi-Generational Least-Recently-Used` - Reduced Cache Thrashing
		- **Simple Age-Based Eviction**: The item least recently used gets evicted, regardless of how often it was accessed in the past. This can lead to issues in certain access patterns.
		- MG-LRU works by:
			1. divides cache into generations
			2. entries promoted / demoted based on access patterns
			3. youngest generation evicted first
- 5.12 - 2021
	- Introduced `PREEMPT_DYNAMIC` which shifts preemption model to run time (not when compiling kernel)
			- can choose between `NONE`, `VOLUNTARY` and `FULL`
	- Preemption only affects how kernel code is preempted (userspace you cant change directly)

### Networking
- `netstat -tuln` (network statistics) - check for open ports
	- `netstat -a` - all current network connections
	- `netstat -r` - routing table - tells you where to route packets (gateway) 
- `nmap` - network scanner - scan an IP address
	- `nmap -p 80 ip_address` see what process is using a specific port
- `ssh` - secure shell (port 22)
	- uses tcp, exchanges keys, and then gives you a session
	- encryption either password or public key / private key based
- `scp` - secure copy
- `tcpdump` to capture and analyze packets

### Inter-process communication
- Pipes - A way for processes to communicate with each other on the same machine
	- `ANON Pipe` - Typical for parent / child relationship where child inherits pipe
		- Unidirectional, temporary
		- `pipe()`
	- `NAMED Pipe` - Persistent, bidirectional queues (FIFO)
		- Access defined by filesystem
		- `mkfifo()`
- Sockets - The fundamental component of networking in Linux
### Filesystem
- files are represented by  `inodes` 
	- hold metadata + pointer to the data
- `hardlink` - shares the same `inode` and `data blocks` as original file
	- points to the `inode` of the file
	- remain valid as long as at least one link to the `inode` exists.
	- `ln original.txt hard_link.txt`
- `softlink` - creates new file that contains a pathname reference to the target file (another `inode` to a pathname)
	- `ln -s original.txt softlink.txt`
	- points to the pathname of the target file
	- could become invalid
- `lsof` - list open files
	- what processes are currently using file descriptors?
- `/proc/PID` - virtual filesystem that provides interface to kernel data structures 
	- presents system and process information in a hierarchical file-like structure
	- data generated on the file as requested
	- **CPU Info**: `/proc/cpuinfo`
	- **Memory Info**: `/proc/meminfo`
	- **Process status** `cat /proc/[PID]/status`
	- Get network buffer size: `cat /proc/sys/net/core/rmem_max`

#### Interrupts
- Exceptional situations which "interrupt" regular process execution
	- E.g. page faults, hardware inputs
	- Provide a way to divert processor 
- `PIC` - Programmable Interrupt Controller (hardware component of interrupts)
- IRQ Line - specific line for each type
	- 0 - System timer, 1 - Keyboard
- Interrupt handling
	- interrupt vector table - stores function pointers to execute if you receive `IRQ`
	- May come at any time
		- `IRQ_DISABLE` / `IRQ_SAVE` - Interrupt Request (and maybe save interrupt state which is `EFLAGS register`)
			- `IRQ` - disable interrupts so we aren't interrupt while running atomic or mutually exclusive code
				- E.g. in kernel its common
			- `IRQ_SAVE` - disable interrupts + save interrupts flags somewhere
		- Save context
		- `ISR` - Interrupt Service Routine
		- Restore with `IRQ_RESTORE` or re-enable with `IRQ_ENABLE` 
- Registers
	- `eip` register - Extended instruction pointer 
		- points to next instruction to be executed
	- `esp` register - extended stack pointer
		- points to the top of the stack
#### Profiling 
- `perf` - event monitoring

![[Pasted image 20240805101337.png]]

git/svn.  
posix (more than just, ive used it for multi-threading)  
strace and how to use it.  
gdb.  
perf.  
gcc and glibc.  
lsof.  
proc file descriptors.  
proc network settings (buffer sizes, initcwd, etc)  
netcat.  
tcpdump and or wireshark.

