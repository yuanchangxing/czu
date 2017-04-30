
/***************************************************************
 * @author@xiesong.
 * songtzu@126.com
 * 4.29.2017
 */

#ifndef _CZU_H_NET_BASE_H_
#define _CZU_H_NET_BASE_H_
#include "pack_util.h"
#include "pack_base.h"
#include <string>
#include <sys/epoll.h>
#include <unistd.h>
#include <unordered_map>
#include <memory>   //shared_ptr
#include <mutex>
typedef unsigned char BYTE;

#define default_port 8080


#define TCP_MAX_BUFF 51200


namespace czu {


    class ConnectBuffer {
    public:
        std::shared_ptr<char> body;
        int length;

        ConnectBuffer() {
            length = 0;
        }
    };


    class NetBase : public PackUtil {
    public:

        NetBase();

        virtual  ~NetBase();

        virtual void OnRecv(int _sockid, PackBase &_pack) = 0;

        virtual void OnConn(const char *_ip, short _port) {}

        virtual void OnDisconn(int _sockid) {}

        virtual void OnTimeOut() {}

        int start_server(std::string _ip, short _port = default_port);

        bool send_pack(int _fd, PackBase &_data);

        bool send_buffer(int _fd, const char *_buff, int _len);

        virtual void OnSendFailed(int _fd, PackBase &_data) {
            //TODO
            LOGE("OnSendFailed fd:%d\n", _fd);
        }

        void close_connect(int _sockfd);


    public:
        std::unordered_map<int, ConnectBuffer> recv_buffers_;

        int epollfd_;
        int socket_listenfd_;
    protected:
        void start_poll(const char *_ip, const int _port);

        bool listen_socket(int _sock, int _conn_num);

        void set_linger(int _sockfd);

        void set_nodealy(int _sockfd);

        void set_reuse(int _sock);

        bool bind_socket(int _sock, char *_pAddr, int _port);

        void set_nonblocking(int _sock);

        void close_cb(int _listenfd, int _sockfd, struct epoll_event &_ev);

        int read_cb(  int _sockfd, struct epoll_event &_ev);

    protected:

        int listen_num_;        //监听的SOCKET数量
        std::string ip_;
        int port_;          //服务端监听端口

        std::mutex send_mutex_;

        int position_;//like position in java ByteBuffer.


    };


}
#endif