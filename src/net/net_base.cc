

#include "clog.h"
#include "net_base.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <cstring>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include<unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


using namespace std;

namespace czu {


    void NetBase::close_cb(int listenfd, int sockfd, struct epoll_event &ev) {
        LOGD("close_cb") ;
        close(sockfd);
        recv_buffers_.erase(sockfd);
        OnDisconn(sockfd);
    }

    int NetBase::read_cb( int sock_fd, struct epoll_event &ev) {

        bool needs_close = false;
        char sbuf[1024] = {0};
        char *buf = sbuf;
        while (true) {

            int val = (int)read(sock_fd, buf, 1024);
            if (val > 0) {

                PackBase pdu;
                std::unordered_map<int, ConnectBuffer>::iterator it = recv_buffers_.find(sock_fd);

                if (recv_buffers_.end() != it) {

                    if (it->second.length >= 0) {
                        std::shared_ptr<char> mergebuf(new char[it->second.length + val + 1]);
                        memcpy(mergebuf.get(), it->second.body.get(), it->second.length);
                        memcpy(mergebuf.get() + it->second.length, buf, val);
                        buf = mergebuf.get();
                        val += it->second.length;
                        it->second.length = val;
                        it->second.body = mergebuf;
                    }
                } else {
                    std::shared_ptr<char> newbuf(new char[val + 1]);
                    memcpy(newbuf.get(), buf, val);
                    ConnectBuffer connect_buffer;
                    connect_buffer.body = newbuf;
                    connect_buffer.length = val;
                    recv_buffers_[sock_fd] = connect_buffer;
                    it = recv_buffers_.find(sock_fd);
                }
                buf = sbuf;

                int result = 0;
                while ((result = OnParsePack(it->second.body.get(), it->second.length, pdu)) != 0) {
                    if (result < 0) {
                        recv_buffers_.erase(sock_fd);
                        if (epoll_ctl(epollfd_, EPOLL_CTL_DEL, sock_fd, &ev) == -1) {
                            exit(-1);
                        }
                        close_cb(socket_listenfd_, sock_fd, ev);

                        needs_close = true;
                        return -1;
                    } else if (result > 0) {
                        memmove(it->second.body.get(), it->second.body.get() + result, it->second.length - result);
                        it->second.length -= result;
                        val -= result;
                        OnRecv(sock_fd, pdu);
                    }
                }
            } else if (val == 0) {
                if (epoll_ctl(epollfd_, EPOLL_CTL_DEL, sock_fd, &ev) == -1) {
                    exit(-2);
                }
                close_cb(socket_listenfd_, sock_fd, ev);
                return -2;
            } else if (val < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                LOGI("error is %d; val is %d",errno, val);
                break;
            } else if (errno == EBADF) {
                if (epoll_ctl(epollfd_, EPOLL_CTL_DEL, sock_fd, &ev) == -1) {
                    exit(-2);
                }
                close_cb(socket_listenfd_, sock_fd, ev);
                return -3;
            } else {
                LOGD("else run");
                break;
            }
        }
        return 0;
    }

    void NetBase::close_connect(int sockfd) {
        close(sockfd);
    }

    void NetBase::set_nonblocking(int sock) {
        int opts;
        opts = fcntl(sock, F_GETFL);
        if (opts < 0) {
            exit(1);
        }

        opts = opts | O_NONBLOCK;

        if (fcntl(sock, F_SETFL, opts) < 0) {
            exit(1);
        }
    }


    void NetBase::set_nodealy(int sockfd) {
        int enable = 1;
        setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (void *) &enable, sizeof(enable));
    }

    void NetBase::set_linger(int sockfd) {
        struct linger ling;
        ling.l_onoff = 0;
        ling.l_linger = 0;
        setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
    }

    void NetBase::set_reuse(int sock) {
        int opt = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt));
    }

    bool NetBase::bind_socket(int sock, char *pAddr, int port) {
        struct sockaddr_in sock_addr;
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_addr.s_addr = inet_addr(pAddr);
        sock_addr.sin_port = htons(port);

        if (::bind(sock, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0) {
            exit(-1);
        }
        return 0;
    }


    bool NetBase::listen_socket(int sock, int conn_num) {
        if (::listen(sock, conn_num) < 0) {
            exit(-2);
        }
        return 0;
    }


/**
 * Function to start the event loop which monitors all fds and callbacks accordingly
 * @note event loop runs indefinitely and can only be stopped by timeout callback, so to process the events only once use poll_event_process
 */
    void NetBase::start_poll(const char *_ip, const int _port) {
        struct epoll_event ev;
        struct epoll_event events_[SOMAXCONN];
        this->epollfd_ = epoll_create(SOMAXCONN);
        struct sockaddr_in serveraddr;
        this->socket_listenfd_ = socket(AF_INET, SOCK_STREAM, 0);

        set_linger(this->socket_listenfd_);
        set_reuse(this->socket_listenfd_);
        set_nodealy(this->socket_listenfd_);
        set_nonblocking(this->socket_listenfd_);

        ev.data.fd = this->socket_listenfd_;
        ev.events = EPOLLIN;

        if (epoll_ctl(this->epollfd_, EPOLL_CTL_ADD,
                      this->socket_listenfd_, &ev) == -1) {
            exit(-1);
        }

        bzero(&serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        char *local_addr = (char *) _ip;
        inet_aton(local_addr, &(serveraddr.sin_addr));
        serveraddr.sin_port = htons(_port);

        bind_socket(this->socket_listenfd_, (char *) _ip, _port);
        listen_socket(this->socket_listenfd_, SOMAXCONN * 1000);

        while (true) {
            struct sockaddr_in client_addr;
            socklen_t clilen = sizeof(sockaddr_in);
            int fds = epoll_wait(this->epollfd_, events_,
                                 SOMAXCONN, 10);

            for (int i = 0; i < fds; i++) {
//                LOGD("poll loop; events_[i].data.fd:%d; %d",events_[i].data.fd, socket_listenfd_);
                if (events_[i].data.fd == this->socket_listenfd_) {
                    int connfd = accept(this->socket_listenfd_,
                                        (struct sockaddr *) &client_addr,
                                        &clilen);
                    if (connfd < 0) {
                        exit(-2);
                    }
                    set_nonblocking(connfd);

                    char *ip = inet_ntoa(client_addr.sin_addr);
                    short port = ntohs(client_addr.sin_port);
                    ev.data.fd = connfd;
                    ev.events = EPOLLIN | EPOLLET;
                    if (epoll_ctl(this->epollfd_, EPOLL_CTL_ADD,
                                  connfd, &ev) == -1) {
                        exit(-3);
                    }
                    OnConn(ip, port);
                } else if ((events_[i].events & EPOLLHUP) ||
                           (events_[i].events & EPOLLERR) ||
                           (events_[i].events & EPOLLRDHUP)) {
                    struct sockaddr_in peeraddr;
                    socklen_t len;
                    getpeername(events_[i].data.fd,
                                (struct sockaddr *) &peeraddr, &len);

                    if (epoll_ctl(this->epollfd_, EPOLL_CTL_DEL,
                                  events_[i].data.fd,
                                  &(events_[i])) == -1) {
                        exit(-4);
                    }
                    close_cb(this->socket_listenfd_,
                             events_[i].data.fd, events_[i]);
                } else if (events_[i].events & EPOLLIN ||
                           events_[i].events & EPOLLPRI) {
                    int result = read_cb(events_[i].data.fd, events_[i]);
                    if (!result) {
                        events_[i].events = EPOLLOUT | EPOLLET |
                                            EPOLLPRI | EPOLLIN;
                        if (epoll_ctl(this->epollfd_, EPOLL_CTL_MOD,
                                      events_[i].data.fd,
                                      &(events_[i])) == -1) {
                            exit(-5);
                        }
                    }
                } else if (events_[i].events & EPOLLOUT) {
                    events_[i].events = EPOLLIN | EPOLLET | EPOLLPRI;
                    if (epoll_ctl(this->epollfd_, EPOLL_CTL_MOD,
                                  events_[i].data.fd, &(events_[i])) == -1) {
                        exit(-6);
                    }
                } else if ((events_[i].events & EPOLLPRI) ||
                           (events_[i].events & EPOLLERR) ||
                           (events_[i].events & EPOLLRDHUP)) {
                    if (epoll_ctl(this->epollfd_, EPOLL_CTL_DEL,
                                  events_[i].data.fd, NULL)) {
                        exit(-7);
                    }
                    close_cb(this->socket_listenfd_,
                             events_[i].data.fd, events_[i]);
                }
            } // for
        }

    }


    NetBase::NetBase() {
        listen_num_ = 1;
        port_ = default_port;
        ip_ = "127.0.0.1";
    }

    NetBase::~NetBase() {
        ::close(this->socket_listenfd_);
    }


    int NetBase::start_server(std::string _ip, int _port) {
        ip_ = _ip;
        port_ = _port;
        start_poll(this->ip_.c_str(), this->port_);
    }


    bool NetBase::send_pack(int _fd, PackBase &_data) {
        std::lock_guard<std::mutex> lk1(send_mutex_);
        char *buf = NULL;
        int len = OnSerializePack(_data, buf);
        if (len > 0) {
            int ret = (int)send(_fd, buf, len, MSG_NOSIGNAL );
//            int ret = write(_fd, buf, len);
            if (ret == -1) {
                char *mesg = strerror(errno);
                LOGE("SEND ERROR;%s", mesg);
                OnSendFailed(_fd, _data);
            }
            free(buf);  //释放内存
            return ret > 0;
        }
        return len > 0;
    }

    bool NetBase::send_buffer(int _fd, const char *buff, int len) {
        if (len > 0) {
//        int ret = send(_fd, buff, len, MSG_NOSIGNAL );
            int ret = write(_fd, buff, len);
            if (ret == -1) {
                char *mesg = strerror(errno);
                return false;
            }
            return true;
        }
        return false;
    }

}