#pragma once
// 👻 FAKE UNISTD FOR WINDOWS
#include <io.h>
#include <process.h>

// Map standard Unix file access to Windows
#define F_OK 0
#define R_OK 4
#define W_OK 2
#define X_OK 1 // Windows doesn't really have executable permission, but 1 works for parsing

// Map sleep (seconds) to Windows Sleep (milliseconds)
#define sleep(x) Sleep((x)*1000)