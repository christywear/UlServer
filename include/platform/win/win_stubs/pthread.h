#pragma once
// 👻 FAKE PTHREAD FOR WINDOWS COMPILATION
// We interpret "pthread_t" as a generic handle so the legacy headers parse.

typedef void* pthread_t;
typedef void* pthread_mutex_t;
typedef void* pthread_cond_t;
typedef int pthread_attr_t;

// Dummy macros to kill function call errors if they sneak through
#define pthread_create(a,b,c,d) 0
#define pthread_join(a,b) 0
#define pthread_mutex_init(a,b) 0
#define pthread_mutex_lock(a) 0
#define pthread_mutex_unlock(a) 0
#define pthread_cond_signal(a) 0
#define pthread_cond_wait(a,b) 0
#define pthread_self() 0