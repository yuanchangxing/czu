#include "clog.h"


#include <sys/inotify.h>


void monitor_py(){
    int fd = inotify_init ();
    int wd = inotify_add_watch (fd, "./entry/", IN_DELETE|IN_MOVED_FROM|IN_MOVED_TO|IN_CREATE|IN_CLOSE_WRITE);

    if (wd < 0)
        perror ("inotify_add_watch");

    int buffer_len = 1024;
    char buffer[buffer_len] = {'\0'};
    int event_size = sizeof(struct inotify_event);

    while (true){
        int i = 0;

        size_t len = read (fd, buffer, buffer_len);
        if (len <0){
            LOGE("inotify read error");
        }
        LOGD("inotify:%s", buffer);
        while(i < buffer_len ) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];

            if(event->len) {
                if(event->mask & IN_CREATE) {
                    if(event->mask & IN_ISDIR) {
                        LOGD("The directory %s was created.", event->name);
                    } else {
                        LOGD("The file %s was created.", event->name);
                    }
                } else if(event->mask & IN_DELETE) {
                    if(event->mask & IN_ISDIR) {
                        LOGD("The directory %s was delete.", event->name);
                    } else {
                        LOGD("The file %s was delete.", event->name);
                    }
                } else if(event->mask & IN_MODIFY) {
                    if(event->mask & IN_ISDIR) {
                        LOGD("The directory %s was modify.", event->name);
                    } else {
                        LOGD("The file %s was modify.", event->name);
                    }
                } else if(event->mask & IN_OPEN) {
                    if(event->mask & IN_ISDIR) {
                        LOGD("The directory %s was open.", event->name);
                    } else {
                        LOGD("The file %s was open.", event->name);
                    }
                } else if(event->mask & IN_CLOSE) {
                    if(event->mask & IN_ISDIR) {
                        LOGD("The directory %s was close.", event->name);
                    }
                }
            }
            i += event_size + event->len;
        }

    }
    int ret = inotify_rm_watch (fd, wd);
    close(fd);
}
