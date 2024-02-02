
//CHEEN : file added

#ifdef TCP_DEBUG_ACT
#define MAX_TCP_REOPEN_FILE_COUNT       500
#endif

#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
#ifdef OPERATOR_SIDE
#define MAX_TCP_CHECK_LINK_COUNT        8
#endif
#endif

#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
#define MAX_TCP_CONNECTION                              100
#define NULL_TCP                                                (unsigned int)(-1)

#define OAM_TCP_GROUP                                   0
#ifdef SYSTEM_SIDE
#define HTBY_LINK_TCP_GROUP                             1
#endif

#define BACKLOG                                                 10                      // how many pending connections queue will hold
#define NULL_TCP_LINK                                   (unsigned int)(-1)

#define OAM_TCP_TYPE            0
#define HTBY_LINK_TCP_TYPE      1

#define NULL_TCP_TYPE           0xff


#define MAX_TCP_DATA_SIZE                               (4096)  // max number of bytes we can get at once
#define TCP_NETBUF_SIZE                                 (4096)

typedef struct{
        int sockfd;
        BYTE state;
        unsigned int reconnect_flag:1;
        unsigned int tx_fail_flag:1;
        struct timeval start_time;
        struct timeval connect_time;
        struct sockaddr_in remote_addr; // connector's address information
        char *remote_ip_string;
        BUF  netrx_st;
        BUF  nettx_st;
        BYTE tx[TCP_NETBUF_SIZE];
        BYTE rx[TCP_NETBUF_SIZE];
        BYTE tx_pckt_arr[MAX_TCP_DATA_SIZE];
        BYTE rx_pckt_arr[MAX_TCP_DATA_SIZE];
        unsigned int tx_pckt_indx;
        unsigned int rx_pckt_indx;
        int tx_pckt_tot_size;
        int rx_pckt_data_size;
        BYTE no_size_bytes;
        unsigned int grp_no;
        unsigned int link_no;
        BYTE type;
        unsigned int wtfr_reconnection_cnt;
        unsigned int rp;
        unsigned int lp;
}TCP_STRUCT;

typedef struct{
        unsigned int grp_no;
#ifdef TCP_ON_SERVER_SIDE
        unsigned int first_server_tcp;
        unsigned int last_server_tcp;
        fd_set server_rd_sockset;
        fd_set server_wr_sockset;
        int server_rd_fdmax;
        int server_wr_fdmax;
        unsigned int server_port_no;
        int listener_sockfd;
#endif
#ifdef TCP_ON_CLIENT_SIDE
        unsigned int first_inprogress_tcp;
        unsigned int last_inprogress_tcp;
        unsigned int first_wtfr_reconnection_tcp;
        unsigned int last_wtfr_reconnection_tcp;
        unsigned int first_client_tcp;
        unsigned int last_client_tcp;
        fd_set client_rw_sockset;
        fd_set inprogress_client_wr_sockset;
        int client_fdmax;
    int client_rw_sockset_cnt;
        unsigned int client_port_no;
#endif
}TCP_GROUP_STRUCT;

#define NULL_TCP_GROUP                                  (unsigned int)(-1)

#define CONNECTION_FAILED                               0
#define CONNECTION_OK                                   1
#define CONNECTION_CLOSED                               2

#define TCP_IN_QUE                                      0
#define TCP_OUT_OF_QUE                                  1
#define TCP_INPROGRESS_STATE                            2
#define TCP_WTFR_RECONNECTION_STATE                     3
#define TCP_CONNECTED_STATE                             4


#define MAX_INPROGRESS_CONNECT_TIME             10000 // 10 sec
#define MAX_SELECT_TIMEOUT_COUNT                10      // 10 sec
#define MAX_WTFR_TCP_RECONNECTION               2000    // 20 sec

#define SYS_OAM_PORT                                    3491            // the port client will be connecting to
#define HTBY_LINK_TCP_PORT                              3492            // the port client will be connecting to


#endif

#if ((defined TCPIP_ON_SYS_OAM_OPERATOR_SIDE) || (defined TCPIP_ON_SYS_OAM_SYSTEM_SIDE))
int init_sys_oam_tcp_client(void);
int init_sys_oam_tcp_client_thread(void);

#ifdef SYSTEM_SIDE
//APOLO start
#ifdef HTBY_LINK_ON_TCPIP
typedef struct{
        unsigned int count;
        BYTE flag;
        unsigned int tcp_no;
}TCP_HTBY_LINK_STRUCT;
#endif
//APOLO end
#endif
typedef struct{
        unsigned int count;
        BYTE flag;
        unsigned int tcp_no;
#ifdef SYSTEM_SIDE
        unsigned int oam_no;
#endif
}TCP_LINK_STRUCT;
#endif

#ifdef TCP_ON_CLIENT_SIDE
int time_diff(struct timeval a, struct timeval b);
int start_client_connection(unsigned int tcp_no);
void *tcp_trx_client_thread_program(void *threadarg);
void tcp_tx_client_hndlr(void);
BYTE add_to_inprogress_tcp_list(unsigned int tcp_no);
BYTE remove_from_inprogress_tcp_list(unsigned int tcp_no);
BYTE add_to_client_tcp_list(unsigned int tcp_no);
BYTE remove_from_client_tcp_list(unsigned int tcp_no);
BYTE remove_from_wtfr_reconnection_tcp_list(unsigned int tcp_no);
BYTE add_to_wtfr_reconnection_tcp_list(unsigned int tcp_no);
void release_client_tcp_socket(unsigned int tcp_no);
void release_inprogress_client_tcp_socket(unsigned int tcp_no);
#endif

#ifdef TCP_ON_SERVER_SIDE
#ifdef TCPIP_ON_SYS_OAM_SYSTEM_SIDE
int init_sys_oam_tcp_server(void);
int init_sys_oam_tcp_server_thread(void);
#endif
BYTE add_to_server_tcp_list(unsigned int tcp_no);
BYTE remove_from_server_tcp_list(unsigned int tcp_no);
void *tcp_rx_server_thread_program(void *threadarg);
void tcp_tx_server_hndlr(void);
void update_server_wr_sockset(unsigned int grp_no);
void release_server_tcp_socket(unsigned int tcp_no);
#endif

#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
BYTE save_rcvd_tcp_pckt(unsigned int tcp_no);
int send_all_tcp_pckts(unsigned int tcp_no);
void fd_added_set_fdmax(int *fdmax,int sockfd);
void fd_removed_set_fdmax(int *fdmax,int sockfd,fd_set sockset);
void init_tcp(void);
void init_tcp_threads(void);
//BOZ start
#if 0
int block_socket(int sockfd);
int nonblock_socket (int sockfd);
#endif // 0
//BOZ end
void init_tcp_st(void);
unsigned int find_free_tcp_rec(void);
void rls_tcp_rec(unsigned int tcp_no);
void initialize_tcp_network_buf(unsigned int tcp_no);
void initialize_tcp_network_tx_buf(unsigned int tcp_no);
#ifdef TCP_DEBUG_ACT
void close_tcp_err_file(void);
void open_tcp_err_file(void);
void reopen_tcp_err_file(void);
#endif
#endif
//BOZ start : oomad inja
#if (OS == LINUX_COMPATIBLE)
int block_socket(int sockfd);
int nonblock_socket (int sockfd);
#endif
//BOZ end
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
#ifdef OPERATOR_SIDE
void check_tcp_link_handler(void);
#endif
#endif
#ifdef SYSTEM_SIDE
#ifdef HTBY_LINK_ON_TCPIP
int init_htby_link_tcp_client(void);
int init_htby_link_tcp_client_thread(void);
int init_htby_link_tcp_server(void);
int init_htby_link_tcp_server_thread(void);
#endif
#endif




