* What is the signale?
A system message sent from one process to another, not ususally to transfer data.
but instead remotely command the partnered process.
E.g. umpire signaling OUT to the batsman.

* Singal size
1 or 2 bytes.

* Signaling comibiations
Kernel-Space/OS may signal user space process
User-Space process p1 may signal User-Space process p2.
But, US process can not signal KS or OS.

* What happens when a process receives the signal?
1. It may execute default action associated with that signal.
e.g. OS sends SIGTERM to process, and process dies as this is the default action associated with SIGTERM.
2. Process may choose to execute something before executing default action. customised processing.
e.g. printing 'GoodBye!' in signal handler routine before terminating on reception of SIGTERM from OS.
3. Process can ignore the signal.

* *Signal handler routine*.
A special function which is supposed to be registered against the signal type.
It executes at the highest priority, preempting the normal excution flow.

* Types of signals
30 signals are supported in linux
SIGINT - Ctrl+C - interrupt
SIGUSR1 or SIGUSR2 - user defined signal -- p1 sends to p2 -- left for user to use.
SIGKILL - OS sends and terminate the user space process -- this signal can not be caught by the process. kill -9 PID.
SIGABRT - Raised by abort() by process itself. can not be blocked and process gets terminated.
SIGTERM - SIGTERM can be caught and customized processing can be done - Ctrl+Z or kill PID.
SIGSEGV - OS sends to user space process on memory violation. -- aggressive signal like SIGKILL.
SIGCHILD - Whenever child is terminated, this signal is send by os to the parent. this will make parent pass wait() blocking call.

* Ways to generate signals in linux
- Rasing a signal from OS to process (e.g. Ctrl-C)
- Sending a signal form process A to itself. (achived by raise() system call)
- Sending a signal from process A to process B (e.g. kill())

* How trapping an OS signal works in a process.
// #Include<signal.h>
// 
// signal(SIGINT, cb_sigint_sig_handler);
// 
// static void
// cb_sigint_sig_handler(int sig){
//     do something
//      ...
//     exit(0);
// }

* A process can send signal to iteslf by raise()
int raise(int signo); //prototype.

* Process can send a signal to another process. e.g. kill()
int kill(int pid, int signo);
