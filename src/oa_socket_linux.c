#include "oa_socket.h"
#include "oa_error_number.h"
#include "oa_time.h"

#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

#define OA_IPV4_STR_MAX_LENGTH      16
#define OA_MAX_PORT                 0xffff

typedef struct {
    int sp_fd;
    int sp_errno;
}oa_socket_pri_t;

int oa_socket_wsa_init() {}
int oa_socket_wsa_destroy() {}

int oa_socket_init(oa_socket_t* sock, oa_socket_family_t family) {
    oa_socket_pri_t* sp;
    unsigned int sock_family;
    int ret;

    if(sock == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    *sock = NULL;
    if(family == OA_TCP) {
        sock_family = SOCK_STREAM;
    }
    else if(family == OA_UDP) {
        sock_family = SOCK_DGRAM;
    }
    else {
        return OA_ERR_ILLEGAL_ARG;
    }
    sp = (oa_socket_pri_t*)malloc(sizeof(oa_socket_pri_t));
    if(sp == NULL) {
        return OA_ERR_OUT_OF_MEMORY;
    }
    sp->sp_fd = socket(AF_INET, family, 0);
    if(sp->sp_fd == -1) {
        sp->sp_errno = errno;
        return OA_ERR_SYSTEM_CALL_FAILED;
    }
    *sock = sp;

    return 0;
}

int oa_socket_init_by_fd(oa_socket_t* sock, int fd) {
    oa_socket_pri_t* sp;

    if (sock == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    *sock = NULL;
    sp = (oa_socket_pri_t*)malloc(sizeof(oa_socket_pri_t));
    if (sp == NULL) {
        return OA_ERR_OUT_OF_MEMORY;
    }
    sp->sp_fd = fd;
    *sock = sp;
    return 0;
}

int oa_socket_destroy(oa_socket_t* sock) {
    oa_socket_pri_t* sp;

    if(sock == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    sp = *sock;
    if(sp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    close(sp->sp_fd);
    free(sp);
    *sock = NULL;

    return 0;
}

int oa_socket_bind(oa_socket_t* sock, 
            const char* ip, unsigned port) {
    oa_socket_pri_t* sp;
    struct sockaddr_in addr;
    int ret;

    if(socket == NULL || port > OA_MAX_PORT) {
        return OA_ERR_ILLEGAL_ARG;
    }
    sp = (oa_socket_pri_t*)(*sock);
    if(sp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    if(ip != NULL) {
        ret = inet_pton(AF_INET, ip, &(addr.sin_addr));
        if(ret == 0) {
            return OA_ERR_ILLEGAL_ARG;
        }
        else if(ret == -1) {
            sp->sp_errno = errno;
            return OA_ERR_SYSTEM_CALL_FAILED;
        }
    }
    else {
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    addr.sin_port = htons((unsigned short)port);
    ret = bind(sp->sp_fd, (struct sockaddr*)&addr,
               sizeof(addr));
    if(ret != 0) {
        sp->sp_errno = errno;
        return OA_ERR_SYSTEM_CALL_FAILED;
    }

    return 0;
}

int oa_socket_listen(oa_socket_t* sock,
            unsigned int back_log) {
    oa_socket_pri_t* sp;
    int ret;

    if(socket == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    sp = *sock;
    if(sp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    ret = listen(sp->sp_fd, back_log);
    if(ret != 0) {
        sp->sp_errno = errno;
        return OA_ERR_SYSTEM_CALL_FAILED;
    }

    return 0;
}

int oa_socket_accept(oa_socket_t* sock, oa_socket_t* client_sock,
                     char* client_ip, unsigned int* client_port) {
    oa_socket_pri_t* sp;
    oa_socket_pri_t* csp;
    struct sockaddr_in client_addr;
    socklen_t client_addr_length = sizeof(client_addr);
    int ret;
    const char* retp;
    char buf[OA_IPV4_STR_MAX_LENGTH];

    // check args
    if(sock == NULL || client_sock == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    csp = *client_sock;
    sp = *sock;
    if(sp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }

    // call accept()
    ret = accept(sp->sp_fd, (struct sockaddr*)&client_addr, 
                 &client_addr_length);
    if(ret == -1) {
        csp = NULL;
        sp->sp_errno = errno;
        return OA_ERR_SYSTEM_CALL_FAILED;
    }
    csp = (oa_socket_pri_t*)malloc(sizeof(oa_socket_pri_t));
    if(csp == NULL) {
        return OA_ERR_OUT_OF_MEMORY;
    }
    csp->sp_fd = ret;

    // record client address
    if(client_ip != NULL) {
        retp = inet_ntop(AF_INET, &(client_addr.sin_addr),
                        buf, OA_IPV4_STR_MAX_LENGTH);
        if(retp == NULL) {
            sp->sp_errno = errno;
            return OA_ERR_SYSTEM_CALL_FAILED;
        }
        strcpy(client_ip, buf);
    }
    if(client_port != NULL) {
        *client_port = ntohs(client_addr.sin_port);
    }

    return 0;
}

int oa_socket_connect(oa_socket_t* sock, const char* ip, unsigned int port) {
    oa_socket_pri_t* sp;
    struct sockaddr_in addr;
    int ret;

    if(socket == NULL || ip == NULL || port > OA_MAX_PORT) {
        return OA_ERR_ILLEGAL_ARG;
    }
    sp = (oa_socket_pri_t*)(*sock);
    if(sp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)port);
    ret = inet_pton(AF_INET, ip, &(addr.sin_addr));
    if(ret == 0) {
        return OA_ERR_ILLEGAL_ARG;
    }
    if(ret == -1) {
        sp->sp_errno = errno;
        return OA_ERR_SYSTEM_CALL_FAILED;
    }
    ret = connect(sp->sp_fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret != 0) {
        sp->sp_errno = errno;
        return OA_ERR_SYSTEM_CALL_FAILED;
    }

    return 0;
}

int oa_socket_send(oa_socket_t* sock, const char* buf, unsigned int length, 
                    unsigned int* sent_length) {
    oa_socket_pri_t* sp;
    int ret;

    if(sock == NULL || buf == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    sp = (oa_socket_pri_t*)(*sock);
    if(sp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    ret = send(sp->sp_fd, buf, length, 0);
    if(ret == -1) {
        sp->sp_errno = errno;
        return OA_ERR_SYSTEM_CALL_FAILED;
    }
    if(sent_length != NULL) {
        *sent_length = (unsigned int)ret;
    }

    return 0;
}

int oa_socket_receive(oa_socket_t* sock, char* buf, unsigned int buf_size,
                      unsigned int* received_length) {
    oa_socket_pri_t* sp;
    int ret;

    if(sock == NULL || buf == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    sp = (oa_socket_pri_t*)(*sock);
    if(sp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    ret = recv(sp->sp_fd, buf, buf_size, 0);
    if(ret == -1) {
        sp->sp_errno = errno;
        return OA_ERR_SYSTEM_CALL_FAILED;
    }
    if(received_length != NULL) {
        *received_length = (unsigned int)ret;
    }

    return 0;
}

int oa_socket_send_to(oa_socket_t* sock,
            const char* ip, unsigned int port,
            const char* buf, unsigned int length,
            unsigned int* sent_length) {
    oa_socket_pri_t* sp;
    struct sockaddr_in addr;
    int ret;

    if (sock == NULL || ip == NULL || port > OA_MAX_PORT ||
        buf == NULL || length == 0) {
        return OA_ERR_ILLEGAL_ARG;
    }
    sp = *sock;
    if (sp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)port);
    ret = inet_pton(AF_INET, ip, &(addr.sin_addr));
    if (ret == 0) {
        return OA_ERR_ILLEGAL_ARG;
    }
    if (ret == -1) {
        sp->sp_errno = errno;
        return OA_ERR_SYSTEM_CALL_FAILED;
    }
    ret = sendto(sp->sp_fd, buf, length, 0,
                (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1) {
        sp->sp_errno = errno;
        return OA_ERR_SYSTEM_CALL_FAILED;
    }
    if (sent_length != 0) {
        *sent_length = (unsigned int)ret;
    }
    return 0;
}

int oa_socket_receive_from(oa_socket_t* sock,
            char* ip, unsigned int* port,
            char* buf, unsigned int buf_size,
            unsigned int* received_length) {
    oa_socket_pri_t* sp;
    struct sockaddr_in addr;
    int addr_length = sizeof(addr);
    int ret;
    const char* retp;
    char ip_buf[OA_IPV4_STR_MAX_LENGTH];

    if (sock == NULL || buf == NULL || buf_size == 0) {
        return OA_ERR_ILLEGAL_ARG;
    }
    sp = *sock;
    if (sp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    ret = recvfrom(sp->sp_fd, buf, buf_size, 0, 
                    (struct sockaddr*)&addr, &addr_length);
    if (ret == -1) {
        sp->sp_errno = errno;
        return OA_ERR_SYSTEM_CALL_FAILED;
    }
    if (received_length != NULL) {
        *received_length = (unsigned int)ret;
    }
    if (ip != NULL) {
        retp = inet_ntop(AF_INET, &(addr.sin_addr),
            ip_buf, OA_IPV4_STR_MAX_LENGTH);
        if (retp == NULL) {
            sp->sp_errno = errno;
            return OA_ERR_SYSTEM_CALL_FAILED;
        }
        strcpy(ip, ip_buf);
    }
    if (port != NULL) {
        *port = ntohs(addr.sin_port);
    }

    return 0;
}

int oa_socket_get_fd(oa_socket_t* sock, long long* fd) {
    oa_socket_pri_t* sp;

    if (sock == NULL || fd == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    sp = *sock;
    if (sp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    *fd = (long long)(sp->sp_fd);

    return 0;
}

int oa_socket_set_nonblock(oa_socket_t* sock, int flag) {
    oa_socket_pri_t* sp;
    unsigned long value;
    int ret;

    if (sock == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    sp = *sock;
    if (sp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    value = flag;
    ret = ioctl(sp->sp_fd, FIONBIO, &value);
    if (ret != 0) {
        sp->sp_errno = errno;
        return OA_ERR_SYSTEM_CALL_FAILED;
    }

    return 0;
}

int oa_socket_get_last_error(oa_socket_t* sock, int* error_number) {
    oa_socket_pri_t* sp;

    if (sock == NULL || error_number == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    sp = *sock;
    if (sp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    *error_number = sp->sp_errno;
    return 0;
}

typedef struct {
    fd_set fds;
    int maxfd;
}oa_ss_fdset_pri_t;

int oa_ss_fdset_init(oa_ss_fdset_t* fdset) {
    oa_ss_fdset_pri_t* ssp;

    if (fdset == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    ssp = malloc(sizeof(oa_ss_fdset_pri_t));
    if (ssp == NULL) {
        return OA_ERR_OUT_OF_MEMORY;
    }
    ssp->maxfd = 0;
    *fdset = ssp;

    return 0;
}

int oa_ss_fdset_destroy(oa_ss_fdset_t* fdset) {
    oa_ss_fdset_pri_t* ssp;
    if (fdset == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    ssp = (oa_ss_fdset_pri_t*)(*fdset);
    if (ssp == NULL) {
        return OA_ERR_OPERATION_FAILED;
    }
    free(ssp);
    *fdset = NULL;
    return 0;
}

int oa_ss_fdset_set(oa_ss_fdset_t fdset, int fd) {
    oa_ss_fdset_pri_t* ssp;
    if (fdset == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    ssp = (oa_ss_fdset_pri_t*)fdset;
    FD_SET(fd, &(ssp->fds));
    if (fd > ssp->maxfd) {
        ssp->maxfd = fd;
    }
    return 0;
}

int oa_ss_fdset_isset(oa_ss_fdset_t fdset, int fd, int* isset) {
    oa_ss_fdset_pri_t* ssp;
    if (fdset == NULL || isset == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    ssp = (oa_ss_fdset_pri_t*)fdset;
    *isset = FD_ISSET(fd, &(ssp->fds));
    return 0;
}

int oa_ss_fdset_clear(oa_ss_fdset_t fdset) {
    oa_ss_fdset_pri_t* ssp;
    if (fdset == NULL) {
        return OA_ERR_ILLEGAL_ARG;
    }
    ssp = (oa_ss_fdset_pri_t*)fdset;
    FD_ZERO((fd_set*)&(ssp->fds));
    return 0;
}

int oa_socket_select(oa_ss_fdset_t read_set,
                    oa_ss_fdset_t write_set,
                    oa_ss_fdset_t except_set,
                    unsigned long long timeout_ms,
                    unsigned int* okay_fd_count) {
    struct timeval tv;
    int ret;
    oa_ss_fdset_pri_t *rssp, *wssp, *essp;
    fd_set *rs, *ws, *es;
    int maxfd = 0;

    rssp = (oa_ss_fdset_pri_t*)read_set;
    wssp = (oa_ss_fdset_pri_t*)write_set;
    essp = (oa_ss_fdset_pri_t*)except_set;
    if (rssp != NULL) {
        rs = &(rssp->fds);
        maxfd = rssp->maxfd > maxfd ? rssp->maxfd : maxfd;
    }
    if (wssp != NULL) {
        rs = &(wssp->fds);
        maxfd = wssp->maxfd > maxfd ? wssp->maxfd : maxfd;
    }
    if (essp != NULL) {
        rs = &(essp->fds);
        maxfd = essp->maxfd > maxfd ? essp->maxfd : maxfd;
    }
    tv.tv_sec = (long)(timeout_ms / 1000);
    tv.tv_usec = (long)(timeout_ms % 1000 * 1000);
    ret = select(maxfd + 1, rs, ws, es, &tv);
    if (ret == -1) {
        ret = errno;
        return OA_ERR_SYSTEM_CALL_FAILED;
    }
    if (okay_fd_count != NULL) {
        *okay_fd_count = ret;
    }

    return 0;
}

