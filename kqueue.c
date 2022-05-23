#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/event.h>
#include <err.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

/*
struct kevent {
    uintptr_t       ident;          identifier for this event
    int16_t         filter;         filter for event
    uint16_t        flags;          general flags
    uint32_t        fflags;         filter-specific flags
    intptr_t        data;           filter-specific data
    void            *udata;         opaque user data identifier
};
*/

int main(void) {
    int kq = kqueue();

    struct kevent evt;
    struct kevent *list = malloc(sizeof(*list) * 1024);

    memset(list, 0x42, sizeof(*list) * 1024);

    EV_SET(&evt, SIGUSR2, EVFILT_SIGNAL, EV_ADD, 0, 0, NULL);

    int ret = kevent(kq, &evt, 1, NULL, 0, NULL);
    if (ret == -1)
        err(EXIT_FAILURE, "kevent");
    if (evt.flags & EV_ERROR)
        err(EXIT_FAILURE, "kqueue fail: %s", strerror(evt.data));

    printf("%d\n", getpid());
    while (1) {
        int ret = kevent(kq, NULL, 0, list, 1024, NULL);
        if (ret > 0) {
            printf("===START===\n");
            printf("ident : %#lx\n", list[ret].ident);
            printf("filter: %#x\n", list[ret].filter);
            printf("flags : %#x\n", list[ret].flags);
            printf("flags : %#x\n", list[ret].fflags);
            printf("data  : %#lx\n", list[ret].data);
            printf("udata : %p\n", list[ret].udata);
            printf("===end===\n");
        }
        else
            err(EXIT_FAILURE, "error");
    }

    return 0;
}