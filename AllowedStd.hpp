#ifndef ALLOWEDSTD_H
#define ALLOWEDSTD_H

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <algorithm>
#include <exception>
#include <ctime>
#include <cstring>
#include <fstream>
#include <sstream>


// Write external functions as "::func()"
// It will be easier to search for them as " ::func"

#include <sys/types.h>

// strerror
#include <stdio.h>

// exit
#include <stdlib.h>

// strptime, strftime ??
#include <time.h>

// write, read, close, rmdir, unlink, fork, getcwd,
// chdir, lseek, execve, dup, dup2, pipe, usleep
#include <unistd.h>

// open
#include <fcntl.h>

// mkdir, stat, lstat, fstat
#include <sys/stat.h>

// opendir, readdir, closedir
#include <dirent.h>

// wait, waitpid, wait3, wait4
#include <sys/wait.h>

// signal, kill
#include <signal.h>

// errno
#include <sys/errno.h>

// gettimeofday
#include <sys/time.h>

// htons, htonl, ntohs, ntohl, inet_addr, setsockopt, getsockname
#include <arpa/inet.h>

// socket, listen, accept, listen, send, recv, bind, connect
#include <sys/socket.h>

// select
#include <sys/select.h>

// fcntl
#include <fcntl.h>

#include <fstream>

#endif
