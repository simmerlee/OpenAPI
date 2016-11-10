#ifndef _OPENAPI_SOCKET_H_
#define _OPENAPI_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

typedef void* oa_socket_t;
typedef enum {
    OA_TCP = 1,
    OA_UDP = 2
}oa_socket_family_t;

int oa_socket_wsa_init();
int oa_socket_wsa_destroy();
int oa_socket_init(oa_socket_t* sock, oa_socket_family_t family);
int oa_socket_init_by_fd(oa_socket_t* sock, int fd);
int oa_socket_destroy(oa_socket_t* sock);
int oa_socket_bind(oa_socket_t* sock, 
            const char* ip, unsigned port);
int oa_socket_listen(oa_socket_t* sock,
            unsigned int back_log);
int oa_socket_accept(oa_socket_t* sock, oa_socket_t* client_sock,
                     char* client_ip, unsigned int* client_port);
int oa_socket_connect(oa_socket_t* sock, const char* ip, unsigned int port);
int oa_socket_send(oa_socket_t* sock, const char* buf, unsigned int length, 
                    unsigned int* sent_length);
int oa_socket_receive();
int oa_socket_send_to(oa_socket_t* sock,
            const char* ip, unsigned int port,
            const char* buf, unsigned int length,
            unsigned int* sent_length);
int oa_socket_receive_from(oa_socket_t* sock,
            char* ip, unsigned int* port,
            char* buf, unsigned int buf_size,
            unsigned int* received_length);
int oa_socket_get_fd(oa_socket_t* sock, long long* fd);

int oa_socket_set_nonblock(oa_socket_t* sock, int flag);

#define OA_SS_EMPTY_FDSET              0

typedef void* oa_ss_fdset_t;
int oa_ss_fdset_init(oa_ss_fdset_t* fdset);
int oa_ss_fdset_destroy(oa_ss_fdset_t* fdset);
int oa_ss_fdset_set(oa_ss_fdset_t fdset,int fd);
int oa_ss_fdset_isset(oa_ss_fdset_t fdset, int fd, int* isset);
int oa_ss_fdset_clear(oa_ss_fdset_t fdset);

int oa_socket_select(oa_ss_fdset_t read_set, 
                     oa_ss_fdset_t write_set,
                     oa_ss_fdset_t except_set,
                     unsigned long long timeout_ms,
                     unsigned int* okay_fd_count);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//_OPENAPI_SOCKET_H_

