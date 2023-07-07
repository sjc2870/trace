#define _GNU_SOURCE
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <link.h>
#include <time.h>

// #define __always_inline inline __attribute__((always_inline))

// https://ubuntuforums.org/showthread.php?t=2418651
// https://stackoverflow.com/a/66193831/19784965
// https://codingrelic.geekhold.com/2010/09/gcc-function-instrumentation.html

//  gcc trace.c -finstrument-functions -ldl -g -rdynamic

void __cyg_profile_func_enter(void *func, void *caller)
                              __attribute__((no_instrument_function));
void __cyg_profile_func_enter(void *func, void *caller) {
            Dl_info a, b;
        struct link_map* link_mapa;
        struct link_map* link_mapb;
        int mypipe[2];
        char addr[20] = {0};
        char ret[100] = {0};

        if (pipe(mypipe) == -1) {
            perror("pipe");
            exit(1);
        }
        dladdr1((void*)func,&a,(void**)&link_mapa,RTLD_DL_LINKMAP);
        dladdr1((void*)caller,&b,(void**)&link_mapb,RTLD_DL_LINKMAP);
        sprintf(addr, "%p", func-link_mapa->l_addr);
        { // execute addr2line
            if(fork() == 0)            //first fork
            {
                close(1);  //closing stdout
                dup(mypipe[1]);         //replacing stdout with pipe write
                close(mypipe[0]);       //closing pipe read
                close(mypipe[1]);

                const char* myprog[] = { "addr2line", "-sfipe", "a.out", addr, 0};
                execvp(myprog[0], (char * const*)myprog);
                perror("error in execvp for ps command");
                exit(1);
            }
        }
        close(mypipe[1]);
        // printf("enter %p %p \n", func-link_mapa->l_addr, caller-link_mapb->l_addr);
        read(mypipe[0], ret, 100);
        printf("-> %s", ret);
} /* __cyg_profile_func_enter */

void __cyg_profile_func_exit(void *func, void *caller)
                             __attribute__((no_instrument_function));
void __cyg_profile_func_exit(void *func, void *caller) {
            Dl_info a, b;
        struct link_map* link_mapa;
        struct link_map* link_mapb;
        int mypipe[2];
        char addr[20] = {0};
        char ret[100] = {0};

        if (pipe(mypipe) == -1) {
            perror("pipe");
            exit(1);
        }
        dladdr1((void*)func,&a,(void**)&link_mapa,RTLD_DL_LINKMAP);
        dladdr1((void*)caller,&b,(void**)&link_mapb,RTLD_DL_LINKMAP);
        sprintf(addr, "%p", func-link_mapa->l_addr);
        { // execute addr2line
            if(fork() == 0)            //first fork
            {
                close(1);  //closing stdout
                dup(mypipe[1]);         //replacing stdout with pipe write
                close(mypipe[0]);       //closing pipe read
                close(mypipe[1]);

                const char* myprog[] = { "addr2line", "-sfipe", "a.out", addr, 0};
                execvp(myprog[0], (char * const*)myprog);
                perror("error in execvp for ps command");
                exit(1);
            }
        }
        close(mypipe[1]);
        // printf("exit %p %p \n", func-link_mapa->l_addr, caller-link_mapb->l_addr);
        read(mypipe[0], ret, 100);
        printf("<- /* end of %s", ret);
} /* __cyg_profile_func_enter */

__always_inline int foo() {
  return 2;
}

int bar() {
  foo();
  return 1;
}

int main(int argc, char** argv) {
  bar();
}
