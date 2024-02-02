
//APOLO : some changes,also some questions

//LAST TCP_ERR: 0x3189
#define COMPILER_NULL_IS_USED
//#define       DEBUG_ACT

#ifdef CENTRAL_EXTERN
#include "dsv.ext"
#else  // CENTRAL_EXTERN
#include "hdr.h"
#include "/ec/work/sys/lintcp.var"
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
extern BYTE hot_link;
extern BYTE sync_led[];
extern BYTE link_up_led[];
#endif
#endif
#if (OS == LINUX_COMPATIBLE)
extern pthread_mutex_t  start_mutex_flag;
extern pthread_cond_t   start_cond_flag;
#endif

/* ============================================================================= */
/* ============================================================================= */
/* start asynchronous non-blocking connection */
#ifdef TCP_ON_CLIENT_SIDE
int start_client_connection(unsigned int tcp_no)
{

        TCP_STRUCT *tcp_pt;
        int optval;
        socklen_t optlen;
        int res;
        TCP_GROUP_STRUCT *grp_pt;
        unsigned int grp_no;

//#ifdef DEBUG_ACT
        prnt_string("start_client_connection.\n");
        prnt_int(tcp_no);prnt_char(CR);
//#endif
        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3100);
                return -1;
        }
        tcp_pt=&tcp_st[tcp_no];
        grp_no=tcp_pt->grp_no;
        if(grp_no >= max_tcp_group){
                prnt_msg(TCP_ERR,0x3101);
                return -1;
        }
        grp_pt=&tcp_grp_st[grp_no];
        // Create socket
        if(tcp_pt->sockfd != -1){
                prnt_msg(TCP_ERR,0x3102);
                prnt_int(tcp_pt->sockfd);prnt_char(CR);
                close(tcp_pt->sockfd);
        }
        tcp_pt->sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (tcp_pt->sockfd < 0) {
                prnt_msg(TCP_ERR,0x3103);
                add_to_wtfr_reconnection_tcp_list(tcp_no);
                return -1;
        }
        nonblock_socket(tcp_pt->sockfd);
        gettimeofday(&tcp_pt->start_time,0);
        // Trying to connect with timeout
        //      FD_CLR(tcp_pt->sockfd,&(grp_pt->client_rw_sockset)); // remove from sockset
        //      FD_CLR(tcp_pt->sockfd,&(grp_pt->inprogress_client_wr_sockset));
        res = connect(tcp_pt->sockfd,(struct sockaddr *)&(tcp_pt->remote_addr),sizeof(struct sockaddr));
        if (res < 0) {
                if (errno == EINPROGRESS) {
                        prnt_msg(TCP_ERR,0x3104);
//#ifdef DEBUG_ACT
                        prnt_string("connection in progress\n");
                        prnt_int(tcp_no);prnt_char('-');
                        prnt_int(tcp_pt->sockfd);prnt_char('-');
                        prnt_int(grp_pt->client_fdmax);prnt_char(CR);
//#endif
                        FD_SET(tcp_pt->sockfd,&(grp_pt->inprogress_client_wr_sockset));
                        fd_added_set_fdmax(&(grp_pt->client_fdmax),tcp_pt->sockfd);
                        add_to_inprogress_tcp_list(tcp_no);
                        // I don't know if I should change its mode to blocking now ,or later
                        block_socket(tcp_pt->sockfd);
#ifdef DEBUG_ACT
                        prnt_string("0x3104 cont: ");
                        prnt_int(grp_pt->client_fdmax);prnt_char(CR);
#endif
                        return 0;
                }
                else {
                        prnt_msg(TCP_ERR,0x3105);
                        prnt_int(tcp_no);prnt_char(CR);
                        close(tcp_pt->sockfd);
                        tcp_pt->sockfd=-1;
                        add_to_wtfr_reconnection_tcp_list(tcp_no);
                        return -1;
                }
        }
        else{
#ifdef DEBUG_ACT
                prnt_string("client connected. socket no.:");
                prnt_int(tcp_pt->sockfd);prnt_char(CR);
#endif
                // Socket selected for write
                optlen = sizeof(int);
                if (getsockopt(tcp_pt->sockfd, SOL_SOCKET, SO_ERROR, (void*)(&optval), &optlen) < 0) {
                        prnt_msg(TCP_ERR,0x3106);
                        close(tcp_pt->sockfd);
                        return -1;
                }
                FD_SET(tcp_pt->sockfd,&(grp_pt->client_rw_sockset));
                grp_pt->client_rw_sockset_cnt++;
//              FD_SET(tcp_pt->sockfd,&inprogress_client_wr_sockset);
                fd_added_set_fdmax(&(grp_pt->client_fdmax),tcp_pt->sockfd);
                block_socket(tcp_pt->sockfd);
                add_to_client_tcp_list(tcp_no);
//APOLO start : block added
                switch(tcp_pt->grp_no){
#ifdef OPERATOR_SIDE
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
                case OAM_TCP_GROUP:
                        if(tcp_pt->link_no < max_sys_oam_tcp_link)
                                tcp_link_st[tcp_pt->link_no].flag=SET;
                        break;
#endif
#endif
#ifdef SYSTEM_SIDE
#ifdef HTBY_LINK_ON_TCPIP
                case HTBY_LINK_TCP_GROUP :
                        htby_link_connected_rtn();
                        break;
#endif
#endif
                default :
                        prnt_msg(TCP_ERR,0x3175);
                        break;
                }
//APOLO end
                return 0;
        }
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef TCP_ON_CLIENT_SIDE
void *tcp_trx_client_thread_program(void *threadarg)
{

        int optval,res;
        socklen_t optlen;
        unsigned int tcp_no,tmp_tcp_no;
        fd_set rd_socksel,wr_socksel/*,except_socksel*/;
        int n;
        TCP_STRUCT *tcp_pt;
        TCP_GROUP_STRUCT *grp_pt;
        struct timeval timeout;
        /*
        pthread_mutex_lock(&start_mutex_flag);
        pthread_cond_wait (&start_cond_flag, &start_mutex_flag);
        pthread_mutex_unlock(&start_mutex_flag);
        */
        /* it seems that we can not have two threads for inprogress connect             */
        /* and connected tcps(to prevent having timer for select in connected           */
        /* tcps thread.) because when I want to move one sockfd from inprogress         */
        /* to connected thread ,if connected thread is blocked on its select line       */
        /* its sockset will not be updated in select.(its variable is updated ,but      */
        /* its value in select routine will be updated when we return from select       */
        /* and continue with the first line in forever loop)                                            */

#ifdef DEBUG_ACT
        prnt_char('s');prnt_char('t');prnt_char('a');prnt_char('r');prnt_char('t');prnt_char(' ');
        prnt_char('c');prnt_char('l');prnt_char('i');prnt_char('e');prnt_char('n');prnt_char('t');prnt_char(' ');
        prnt_char('t');prnt_char('h');prnt_char('r');prnt_char('e');prnt_char('a');prnt_char('d');prnt_char(CR);
#endif
        grp_pt=(TCP_GROUP_STRUCT *)threadarg;
        for(;;){
//APOLOAPOLO:to be asked
 //grp_pt=(TCP_GROUP_STRUCT *)(threadarg);
//CHEENCHEEN : to be asked
                if(grp_pt->client_fdmax == -1) {sleep(1);continue;}
//APOLOAPOLO:to be asked
//grp_pt=(TCP_GROUP_STRUCT *)(threadarg);
                /* setup bit arrays */
                memcpy(&wr_socksel ,&(grp_pt->inprogress_client_wr_sockset), sizeof(fd_set));
                memcpy(&rd_socksel ,&(grp_pt->client_rw_sockset), sizeof(fd_set));
//              memcpy(&except_socksel ,&client_rw_sockset, sizeof(client_rw_sockset));
                /* Timeout of 2 seconds. */
                timeout.tv_sec =  MAX_SELECT_TIMEOUT_COUNT;
                timeout.tv_usec = 0;

                /* wr_socksel is only used for inprogress client tcp */
//APOLOAPOLO:to be asked
//grp_pt=(TCP_GROUP_STRUCT *)(threadarg);
                n = select((grp_pt->client_fdmax)+1, &rd_socksel, &wr_socksel, /*&except_socksel*/NULL, &timeout);
//APOLOAPOLO:to be asked
//grp_pt=(TCP_GROUP_STRUCT *)(threadarg);
                if(n == 0){             // Timeout
#ifdef DEBUG_ACT
                        prnt_msg(TCP_ERR,0x3107);
                        prnt_int(grp_pt->client_fdmax);prnt_char(CR);
#endif
                        for(tcp_no=grp_pt->first_inprogress_tcp;tcp_no<max_tcp_connection;){
                                tcp_pt=&tcp_st[tcp_no];
                                gettimeofday(&tcp_pt->connect_time,0);
                                if(time_diff(tcp_pt->connect_time,tcp_pt->start_time) > MAX_INPROGRESS_CONNECT_TIME){
                                        tmp_tcp_no=tcp_pt->rp;
                                        release_inprogress_client_tcp_socket(tcp_no);
                                        tcp_no=tmp_tcp_no;
                                        continue;
                                }
                                else{
                                        tcp_no=tcp_pt->rp;
                                        continue;
                                }
                        }
                        for(tcp_no=grp_pt->first_client_tcp;tcp_no<max_tcp_connection;){
                                tcp_pt=&tcp_st[tcp_no];
                                if(tcp_pt->tx_fail_flag == SET){
                                        prnt_msg(TCP_ERR,0x3108);
                                        tmp_tcp_no=tcp_pt->rp;
                                        release_client_tcp_socket(tcp_no);
                                        tcp_no=tmp_tcp_no;
                                        continue;
                                }
                                tcp_no=tcp_pt->rp;
                        }
                }
                else if (n < 0) {
                        switch(errno){ // to be completed
                          case EBADF    :
                          case EINTR    :
                          case EINVAL   :
                          case ENOMEM   :
                          default               :
                                  prnt_msg(TCP_ERR,0x3109);
                                  break;
                        }
                        continue;
                }
                else if (n > 0) {
                        /* getsockopt after a non-blocking connect returns -1 if something    */
                        /* is really messed up(bad descriptor,usually).Otherwise , it returns */
                        /* 0 and error holds what connect would have returned if it blocked   */
                        /* until now.thus ,error == 0 is success ,error == EINPROGRESS means  */
                        /* "try again" , and anything else is a real error.                               */
                        optlen = sizeof(int);
                        for(tcp_no=grp_pt->first_inprogress_tcp;((tcp_no<max_tcp_connection)/* && (n > 0)*/);){
                                tcp_pt=&tcp_st[tcp_no];
                                if(FD_ISSET(tcp_pt->sockfd,&wr_socksel)){ // we got one !!
                                  /*n--;*/
                                        if (getsockopt(tcp_pt->sockfd, SOL_SOCKET, SO_ERROR, (void*)(&optval), &optlen) < 0) {
                                                prnt_msg(TCP_ERR,0x3110);
                                                prnt_int(tcp_no);prnt_char('-');
                                                prnt_string(strerror(errno));prnt_char(CR);
                                                tmp_tcp_no=tcp_pt->rp;
                                                release_inprogress_client_tcp_socket(tcp_no);
                                                tcp_no=tmp_tcp_no;
                                                continue;
                                        }
                                        // Check the value returned...
                                        if (optval) {
                                                prnt_msg(TCP_ERR,0x3111);
                                                prnt_int(tcp_no);prnt_char('-');
                                                prnt_int(tcp_pt->sockfd);prnt_char('-');
                                                prnt_int(optval);prnt_char('-');
                                                prnt_string(strerror(optval));prnt_char(CR);
                                                tmp_tcp_no=tcp_pt->rp;
#ifdef DEBUG_ACT
                                                prnt_string("0x3111 before client_fdmax:");
                                                prnt_int(grp_pt->client_fdmax);prnt_char(CR);
#endif
                                                release_inprogress_client_tcp_socket(tcp_no);
#ifdef DEBUG_ACT
                                                prnt_string("0x3111 after remove client_fdmax:");
                                                prnt_int(grp_pt->client_fdmax);prnt_char(CR);
#endif
                                                tcp_no=tmp_tcp_no;
                                                continue;
                                        }
//#ifdef DEBUG_ACT
                                        prnt_string("inprogress to active: ");
                                        prnt_int(tcp_no);prnt_char('-');
                                        prnt_int(tcp_pt->sockfd);prnt_char(CR);
                                        prnt_string("ok client_fdmax:");
                                        prnt_int(grp_pt->client_fdmax);prnt_char(CR);
//#endif
                                        tmp_tcp_no=tcp_pt->rp;
                                        remove_from_inprogress_tcp_list(tcp_no);
                                        add_to_client_tcp_list(tcp_no);
//APOLO start : block added
                                        switch(tcp_pt->grp_no){
#ifdef OPERATOR_SIDE
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
                                        case OAM_TCP_GROUP:
                                                if(tcp_pt->link_no < max_sys_oam_tcp_link)
                                                        tcp_link_st[tcp_pt->link_no].flag=SET;
                                                break;
#endif
#endif
#ifdef SYSTEM_SIDE
#ifdef HTBY_LINK_ON_TCPIP
                                        case HTBY_LINK_TCP_GROUP :
                                                htby_link_connected_rtn();
                                                break;
#endif
#endif
                                        default :
                                                prnt_msg(TCP_ERR,0x3176);
                                                break;
                                        }
//APOLO end
                                        FD_CLR(tcp_pt->sockfd,&(grp_pt->inprogress_client_wr_sockset));
                                        FD_SET(tcp_pt->sockfd,&(grp_pt->client_rw_sockset));
                                        grp_pt->client_rw_sockset_cnt++;
                                        //                                      fd_added_set_fdmax(&(grp_pt->client_fdmax),tcp_pt->sockfd);
                                        //                                      block_socket (tcp_pt->sockfd);
                                        tcp_no=tmp_tcp_no;
                                        continue;
                                }
                                tcp_no=tcp_pt->rp;
                        }
                        for(tcp_no=grp_pt->first_client_tcp;tcp_no<max_tcp_connection;){
                                tcp_pt=&tcp_st[tcp_no];
                                if(tcp_pt->tx_fail_flag == SET){
                                        prnt_msg(TCP_ERR,0x3112);
                                        tmp_tcp_no=tcp_pt->rp;
                                        release_client_tcp_socket(tcp_no);
                                        tcp_no=tmp_tcp_no;
                                        continue;
                                }
                                if(FD_ISSET(tcp_pt->sockfd,&rd_socksel)){
                                        res=save_rcvd_tcp_pckt(tcp_no);
                                        if((res == CONNECTION_FAILED) ||
                                           (res == CONNECTION_CLOSED)){
                                                prnt_msg(TCP_ERR,0x3113);
                                                prnt_int(tcp_no);prnt_char('-');
                                                prnt_int(tcp_pt->sockfd);prnt_char(CR);
                                                tmp_tcp_no=tcp_pt->rp;
                                                release_client_tcp_socket(tcp_no);
                                                tcp_no=tmp_tcp_no;
                                                continue;
                                        }
                                        else{
#ifdef DEBUG_ACT
                                                prnt_string("recv ok\n");
#endif
                                                tcp_no=tcp_pt->rp;
                                                continue;
                                        }
                                }
                                tcp_no=tcp_pt->rp;
                        } // for
                }
        }
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef TCP_ON_CLIENT_SIDE
void tcp_tx_client_hndlr(void)
{
        fd_set wr_socksel;
        unsigned int tcp_no,tmp_tcp_no,grp_no;
        TCP_STRUCT *tcp_pt;
        int cnt;
        struct timeval timeout;
        TCP_GROUP_STRUCT *grp_pt;

        for(grp_no=0,grp_pt= &tcp_grp_st[0];grp_no<max_tcp_group;grp_no++,grp_pt++){
                for(tcp_no=grp_pt->first_wtfr_reconnection_tcp;tcp_no<max_tcp_connection;){
                        tcp_pt=&tcp_st[tcp_no];
                        if(tcp_pt->wtfr_reconnection_cnt-- == 0){
                                tmp_tcp_no=tcp_pt->rp;
                                remove_from_wtfr_reconnection_tcp_list(tcp_no);
                                start_client_connection(tcp_no);
                                tcp_no=tmp_tcp_no;
                        }
                        else{
                                tcp_no=tcp_pt->rp;
                        }
                }
                if(grp_pt->client_fdmax == -1){
                        continue;
                }
                if(grp_pt->client_rw_sockset_cnt <= 0){
                        continue;
                }
                memcpy(&wr_socksel ,&(grp_pt->client_rw_sockset), sizeof(fd_set));
                /* timeout=0 (select returns immediately) */
                timeout.tv_sec =  0;
                timeout.tv_usec = 0;
#ifdef DEBUG_ACT
                /*
                {
                        struct timeval time;

                        gettimeofday(&time,0);
                        prnt_int((((int)time.tv_sec)>>16)&0xffff);prnt_int(((int)time.tv_sec)&0xffff);prnt_char('-');
                        prnt_int((((int)time.tv_usec)>>16)&0xffff);prnt_int(((int)time.tv_usec)&0xffff);prnt_char(CR);
                }
                */
#endif
                cnt = select((grp_pt->client_fdmax)+1, NULL, &wr_socksel, NULL, &timeout);
#ifdef DEBUG_ACT
                /*
                {
                        struct timeval time;

                        gettimeofday(&time,0);
                        prnt_int((((int)time.tv_sec)>>16)&0xffff);prnt_int(((int)time.tv_sec)&0xffff);prnt_char('-');
                        prnt_int((((int)time.tv_usec)>>16)&0xffff);prnt_int(((int)time.tv_usec)&0xffff);prnt_char(CR);
                }
                */
#endif
                for(tcp_no=grp_pt->first_client_tcp;((tcp_no<max_tcp_connection)/* && (cnt > 0)*/);){
                        tcp_pt=&tcp_st[tcp_no];
                        if(check_variable_size_buf_is_empty(&(tcp_pt->nettx_st)) == OK){
                                tcp_no=tcp_pt->rp;
                                continue;
                        }
                        if(FD_ISSET(tcp_pt->sockfd,&wr_socksel)){ // we got one !!
 //                             cnt--;
                                if(send_all_tcp_pckts(tcp_no) == -1){
                                        prnt_msg(TCP_ERR,0x3114);
                                        tcp_pt->tx_fail_flag=SET;
                                }
                                tcp_no=tcp_pt->rp;
                                continue;
                        }
                        tcp_no=tcp_pt->rp;
                }
        }
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef TCP_ON_SERVER_SIDE
void *tcp_rx_server_thread_program(void *threadarg)
{

//MOHAJER : int->socklen_t
        socklen_t addrlen;
        int yes=1;
        fd_set  rd_socksel/*,except_socksel*/;
        int n,res;
        unsigned int tcp_no,new_tcp_no,tmp_tcp_no;
        TCP_STRUCT *tcp_pt;
        TCP_GROUP_STRUCT *grp_pt;
        struct timeval timeout;
        struct sockaddr_in remote_addr; // connector's address information
        int new_sockfd;
        struct sockaddr_in my_addr;             // server address

        /*
        pthread_mutex_lock(&start_mutex_flag);
        pthread_cond_wait (&start_cond_flag, &start_mutex_flag);
        pthread_mutex_unlock(&start_mutex_flag);
        */

        grp_pt=(TCP_GROUP_STRUCT *)(threadarg);

#ifdef DEBUG_ACT
        prnt_char('s');prnt_char('t');prnt_char('a');prnt_char('r');prnt_char('t');prnt_char(' ');
        prnt_char('s');prnt_char('e');prnt_char('r');prnt_char('v');prnt_char('e');prnt_char('r');prnt_char(' ');
        prnt_char('t');prnt_char('h');prnt_char('r');prnt_char('e');prnt_char('a');prnt_char('d');prnt_char(CR);
        prnt_int(grp_pt->grp_no);prnt_char(CR);
#endif

        FD_ZERO(&(grp_pt->server_rd_sockset));                          // clear the server_rd_sockset and temp sets
        FD_ZERO(&(grp_pt->server_wr_sockset));
        // get the sockfd
        if((grp_pt->listener_sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
                prnt_msg(TCP_ERR,0x3115);
                goto end;
        }
        grp_pt->server_rd_fdmax=grp_pt->listener_sockfd;
        grp_pt->server_wr_fdmax=-1;
        // lose the pesky "address already in use" error message
        if(setsockopt(grp_pt->listener_sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1){
                prnt_msg(TCP_ERR,0x3116);
                goto end;
        }
        my_addr.sin_family=AF_INET;             // host byte order
        my_addr.sin_port=htons(grp_pt->server_port_no);         // short,network byte order
        my_addr.sin_addr.s_addr=INADDR_ANY; // automatically fill with my IP
        memset(&(my_addr.sin_zero),'\0',8);     // zero the rest of the struct

        if(bind(grp_pt->listener_sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr)) == -1){
                prnt_msg(TCP_ERR,0x3117);
                prnt_int(errno);prnt_char('-');
                prnt_string(strerror(errno));prnt_char(CR);
                goto end;
        }
        if(listen(grp_pt->listener_sockfd,BACKLOG) == -1){
                prnt_msg(TCP_ERR,0x3118);
                prnt_int(errno);prnt_char('-');
                prnt_string(strerror(errno));prnt_char(CR);
                goto end;
        }
        FD_SET(grp_pt->listener_sockfd,&(grp_pt->server_rd_sockset));
        fd_added_set_fdmax(&(grp_pt->server_rd_fdmax),grp_pt->listener_sockfd);
        update_server_wr_sockset(grp_pt->grp_no);
        tcp_no=find_free_tcp_rec();
        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3119);
                goto end;
        }
        tcp_pt=&tcp_st[tcp_no];
        tcp_pt->grp_no=grp_pt->grp_no;
        tcp_pt->sockfd=grp_pt->listener_sockfd;
//#ifdef DEBUG_ACT
        prnt_string("server listener :");
        prnt_int(grp_pt->grp_no);prnt_char('-');
        prnt_int(tcp_no);prnt_char('-');
        prnt_int(tcp_pt->sockfd);prnt_char(CR);
//#endif
        add_to_server_tcp_list(tcp_no);
        // main loop
        for(;;){
select_again:
                memcpy(&rd_socksel,&(grp_pt->server_rd_sockset),sizeof(fd_set)); // copy it
//              memcpy(&except_socksel ,&server_rd_sockset,sizeof(server_rd_sockset));
//              memcpy(&wr_socksel,&server_rd_sockset,sizeof(server_rd_sockset)); // copy it

                timeout.tv_sec =  MAX_SELECT_TIMEOUT_COUNT;
                timeout.tv_usec = 0;
//APOLOAPOLO:to be asked
//grp_pt=(TCP_GROUP_STRUCT *)(threadarg);
                n = select((grp_pt->server_rd_fdmax)+1, &rd_socksel,NULL,/*&except_socksel*/NULL,/*NULL*/ &timeout);
//APOLOAPOLO:to be asked
//grp_pt=(TCP_GROUP_STRUCT *)(threadarg);
#ifdef DEBUG_ACT
                //              prnt_msg(TCP_ERR,0x3120);
                //              prnt_int(n);prnt_char('-');
                //              prnt_int(grp_pt->server_rd_fdmax);prnt_char(CR);
#endif
                if(n == 0){
#ifdef DEBUG_ACT
                        prnt_msg(TCP_ERR,0x3121);
#endif
                        for(tcp_no=grp_pt->first_server_tcp;tcp_no<max_tcp_connection;){
                                tcp_pt=&tcp_st[tcp_no];
                                if(tcp_pt->tx_fail_flag == SET){
//#ifdef DEBUG_ACT
                                        prnt_string("server_thread removed socket no. :");
                                        prnt_int(tcp_pt->sockfd);prnt_char(CR);
//#endif
                                        tmp_tcp_no=tcp_pt->rp;
                                        release_server_tcp_socket(tcp_no);
                                        tcp_no=tmp_tcp_no;
                                }
                                else{
                                        tcp_no=tcp_pt->rp;
                                }
                        }
                        continue;
                }
                else if(n == -1){
                        prnt_msg(TCP_ERR,0x3122);
                        goto select_again;
                }
                // run through the existing connections looking for data to read
                for(tcp_no=grp_pt->first_server_tcp;((tcp_no<max_tcp_connection) /*&& (n > 0)*/);){
#ifdef DEBUG_ACT
                        prnt_string(" server_thread something happend.\n");
                        prnt_int(grp_pt->grp_no);prnt_char('-');
                        prnt_int(tcp_no);prnt_char(CR);
#endif
                        tcp_pt=&tcp_st[tcp_no];
                        if(FD_ISSET(tcp_pt->sockfd,&rd_socksel)){ // we got one !!
                                //n--;
                                if(tcp_pt->sockfd == grp_pt->listener_sockfd){
//#ifdef DEBUG_ACT
                                        prnt_string(" server_thread new tcp connection requested.\n");
                                        prnt_int(grp_pt->grp_no);prnt_char('-');
                                        prnt_int(tcp_no);prnt_char('-');
                                        prnt_int(tcp_pt->sockfd);prnt_char(CR);
//#endif
                                        // handle new connections
                                        addrlen=sizeof(struct sockaddr_in);
                                        if((new_sockfd = accept(grp_pt->listener_sockfd,(struct sockaddr *)&(remote_addr),&addrlen)) == -1){
                                                prnt_msg(TCP_ERR,0x3124);
                                                tcp_no=tcp_pt->rp;
                                                continue;
                                        }
                                        else{
//#ifdef DEBUG_ACT
                                                prnt_string("server_thread added socket no. :");
                                                prnt_int(grp_pt->grp_no);prnt_char('-');
                                                prnt_int(tcp_no);prnt_char('-');
                                                prnt_int(grp_pt->listener_sockfd);prnt_char('-');
                                                prnt_int(new_sockfd);prnt_char(CR);
//#endif
#ifdef TCPIP_ON_SYS_OAM_SYSTEM_SIDE
                                                switch(grp_pt->grp_no){
                                                case OAM_TCP_GROUP:
                                                        {
                                                                struct hostent *remote0,*remote1;
                                                                struct in_addr remote_sin_addr;
                                                                unsigned long s_addr;
//#ifdef DEBUG_ACT
                                                                prnt_string("server_thread oam socket is requested\n");
                                                                prnt_int(grp_pt->grp_no);prnt_char('-');
                                                                prnt_int(tcp_no);prnt_char(CR);
//#endif
                                                                if((remote0=gethostbyname(OAM0_IP_STRING)) == NULL_POINTER){ // get the host info
                                                                        prnt_msg(TCP_ERR,0x3166);
                                                                        tcp_no=tcp_pt->rp;
                                                                        continue;
                                                                }
                                                                s_addr=remote_addr.sin_addr.s_addr;
                                                                remote_sin_addr=*((struct in_addr *)remote0->h_addr);
#ifdef DEBUG_ACT
                                                                prnt_string("server_thread oam remote0:");
                                                                prnt_int(remote_sin_addr.s_addr>>16);prnt_int(remote_sin_addr.s_addr);prnt_char('-');
                                                                prnt_int(s_addr>>16);prnt_int(s_addr);prnt_char(CR);
#endif
                                                                if(remote_sin_addr.s_addr == s_addr){
                                                                        new_tcp_no=tcp_link_st[tcp_link_0].tcp_no;
                                                                        if(new_tcp_no >= max_tcp_connection){
                                                                                prnt_msg(TCP_ERR,0x3168);
                                                                                tcp_no=tcp_pt->rp;
                                                                                continue;
                                                                        }
#ifdef DEBUG_ACT
                                                                        prnt_string("server_thread oam remote0 tcp_no:");
                                                                        prnt_int(new_tcp_no);prnt_char(CR);
#endif
#if (NO_ACTIVE_OPR > 0)
                                                                        opr[0].link_no=new_tcp_no;
#endif
#if (NO_OPRTR > 0)
                                                                        oam[0].link_no=new_tcp_no;
#endif
                                                                        tcp_st[new_tcp_no].remote_addr=remote_addr;
                                                                        tcp_st[new_tcp_no].sockfd=new_sockfd;
                                                                        break;
                                                                }
                                                                else{
                                                                        if((remote1=gethostbyname(OAM1_IP_STRING)) == NULL_POINTER){ // get the host info
                                                                                prnt_msg(TCP_ERR,0x3167);
                                                                                tcp_no=tcp_pt->rp;
                                                                                continue;
                                                                        }
                                                                        remote_sin_addr=*((struct in_addr *)remote1->h_addr);
#ifdef DEBUG_ACT
                                                                        prnt_string("server_thread oam remote1:");
                                                                        prnt_int(remote_sin_addr.s_addr>>16);prnt_int(remote_sin_addr.s_addr);prnt_char('-');
                                                                        prnt_int(s_addr>>16);prnt_int(s_addr);prnt_char(CR);
#endif
                                                                        if(remote_sin_addr.s_addr == s_addr){
                                                                                new_tcp_no=tcp_link_st[tcp_link_1].tcp_no;
                                                                                if(new_tcp_no >= max_tcp_connection){
                                                                                        prnt_msg(TCP_ERR,0x3169);
                                                                                        tcp_no=tcp_pt->rp;
                                                                                        continue;
                                                                                }
#ifdef DEBUG_ACT
                                                                                prnt_string("server_thread oam remote1 tcp_no:");
                                                                                prnt_int(new_tcp_no);prnt_char(CR);
#endif
#if (NO_ACTIVE_OPR > 1)
                                                                                opr[1].link_no=new_tcp_no;
#endif
#if (NO_OPRTR > 1)
                                                                                oam[1].link_no=new_tcp_no;
#endif
                                                                                tcp_st[new_tcp_no].remote_addr=remote_addr;
                                                                                tcp_st[new_tcp_no].sockfd=new_sockfd;
                                                                                break;
                                                                        }
                                                                        else{
                                                                                prnt_msg(TCP_ERR,0x3172);
                                                                                tcp_no=tcp_pt->rp;
                                                                                continue;
                                                                        }
                                                                }
                                                        }
#endif // TCPIP_ON_SYS_OAM_SYSTEM_SIDE

#ifdef SYSTEM_SIDE
#ifdef HTBY_LINK_ON_TCPIP
                                                case HTBY_LINK_TCP_GROUP :
                                                        {
                                                                struct hostent *remote0;
                                                                struct in_addr remote_sin_addr;
                                                                unsigned long s_addr;

//#ifdef DEBUG_ACT
                                                                prnt_string("server_thread htby socket is requested\n");
                                                                prnt_int(grp_pt->grp_no);prnt_char('-');
                                                                prnt_int(tcp_no);prnt_char(CR);
//#endif
                                                                if((remote0=gethostbyname(SLAVE_IP_STRING)) == NULL_POINTER){ // get the host info
                                                                        prnt_msg(TCP_ERR,0x3177);
                                                                        tcp_no=tcp_pt->rp;
                                                                        continue;
                                                                }
                                                                s_addr=remote_addr.sin_addr.s_addr;
                                                                remote_sin_addr=*((struct in_addr *)remote0->h_addr);
#ifdef DEBUG_ACT
                                                                prnt_string("server_thread htby link remote0:");
                                                                prnt_int(remote_sin_addr.s_addr>>16);prnt_int(remote_sin_addr.s_addr);prnt_char('-');
                                                                prnt_int(s_addr>>16);prnt_int(s_addr);prnt_char(CR);
#endif
                                                                if(remote_sin_addr.s_addr == s_addr){
                                                                        new_tcp_no=tcp_htby_link.tcp_no;
                                                                        if(new_tcp_no >= max_tcp_connection){
                                                                                prnt_msg(TCP_ERR,0x3178);
                                                                                tcp_no=tcp_pt->rp;
                                                                                continue;
                                                                        }
#ifdef DEBUG_ACT
                                                                        prnt_string("server_thread htby link remote0 tcp_no:");
                                                                        prnt_int(new_tcp_no);prnt_char(CR);
#endif
                                                                        tcp_st[new_tcp_no].remote_addr=remote_addr;
                                                                        tcp_st[new_tcp_no].sockfd=new_sockfd;
                                                                        htby_link_connected_rtn();
                                                                        break;
                                                                }
                                                                else{
                                                                        prnt_msg(TCP_ERR,0x3179);
                                                                        tcp_no=tcp_pt->rp;
                                                                        continue;
                                                                }
                                                        }
#endif // HTBY_LINK_ON_TCPIP
#endif // SYSTEM_SIDE
                                                default:
                                                        prnt_msg(TCP_ERR,0x3180);
                                                        tcp_no=tcp_pt->rp;
                                                        continue;
                                                }
                                                FD_SET(new_sockfd,&(grp_pt->server_rd_sockset));        // add to server_rd_sockset set
                                                fd_added_set_fdmax(&(grp_pt->server_rd_fdmax),new_sockfd);
                                                update_server_wr_sockset(tcp_pt->grp_no);
                                                tmp_tcp_no=tcp_pt->rp;
                                                add_to_server_tcp_list(new_tcp_no);
                                                tcp_no=tmp_tcp_no;
                                                continue;
                                        }
                                }
                                else{
                                        res=save_rcvd_tcp_pckt(tcp_no);
                                        if(res == CONNECTION_CLOSED){
                                                // connection closed
//#ifdef DEBUG_ACT
                                                prnt_string("socket closed up:");
                                                prnt_int(tcp_pt->sockfd);prnt_char(CR);
//#endif
                                                tmp_tcp_no=tcp_pt->rp;
                                                release_server_tcp_socket(tcp_no);
                                                tcp_no=tmp_tcp_no;
                                                continue;
                                        }
                                        else if (res == CONNECTION_FAILED){
                                                prnt_msg(TCP_ERR,0x3125);
                                                tmp_tcp_no=tcp_pt->rp;
                                                release_server_tcp_socket(tcp_no);
                                                tcp_no=tmp_tcp_no;
                                                continue;
                                        }
                                        else{
#ifdef DEBUG_ACT
                                                prnt_string("recv ok\n");
#endif
                                                tcp_no=tcp_pt->rp;
                                                continue;
                                        }
                                }
                        }
                        tcp_no=tcp_pt->rp;
                        continue;
                }
        }
        // to be corrected
    // pthread_mutex_unlock(&tcp_rx_mutex_flag);
end:
        pthread_exit(NULL);
        return 0;
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef TCP_ON_SERVER_SIDE
void update_server_wr_sockset(unsigned int grp_no)
{
        TCP_GROUP_STRUCT *grp_pt;

        if(grp_no >= max_tcp_group){
                prnt_msg(TCP_ERR,0x3126);
                return;
        }
        grp_pt=&tcp_grp_st[grp_no];
        memcpy(&(grp_pt->server_wr_sockset) ,&(grp_pt->server_rd_sockset), sizeof(fd_set));
        grp_pt->server_wr_fdmax=grp_pt->server_rd_fdmax;
        fd_removed_set_fdmax(&(grp_pt->server_wr_fdmax),grp_pt->listener_sockfd,grp_pt->server_wr_sockset);
        FD_CLR(grp_pt->listener_sockfd,&(grp_pt->server_wr_sockset)); // remove from sockset
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef TCP_ON_SERVER_SIDE
/* this thread_program runs in a thread */
void tcp_tx_server_hndlr(void)
{

        fd_set wr_socksel;
        unsigned int tcp_no,grp_no;
        TCP_GROUP_STRUCT *grp_pt;
        TCP_STRUCT *tcp_pt;
        int cnt;

        for(grp_no=0,grp_pt=&tcp_grp_st[0];grp_no<max_tcp_group;grp_no++,grp_pt++){
                if(grp_pt->server_wr_fdmax == -1) continue;

                memcpy(&wr_socksel ,&(grp_pt->server_wr_sockset), sizeof(fd_set));

                /* grp_pt->listener_sockfd should be removed from wr_socksel,if not     */
                /* select does not return.                                                                      */
                /* timeout=0 (select returns immediately) */

                cnt = select(grp_pt->server_wr_fdmax+1, NULL, &wr_socksel, NULL, 0);
                for(tcp_no=grp_pt->first_server_tcp;((tcp_no<max_tcp_connection) && (cnt > 0));){
                        tcp_pt=&tcp_st[tcp_no];
                        if(check_variable_size_buf_is_empty(&(tcp_pt->nettx_st)) == OK){
                                tcp_no=tcp_pt->rp;
                                continue;
                        }
                        if(FD_ISSET(tcp_pt->sockfd,&wr_socksel)){
                                cnt--;
                                if(send_all_tcp_pckts(tcp_no) == -1){
                                        tcp_pt->tx_fail_flag=SET;
                                }
                        }
                        tcp_no=tcp_pt->rp;
                }
        }
}
#endif
/* ============================================================================= */
/* ============================================================================= */
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
void fd_added_set_fdmax(int *fdmax,int sockfd){
        *fdmax=(*fdmax > sockfd) ? *fdmax:sockfd;
}
#endif
/* ============================================================================= */
/* ============================================================================= */
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
void fd_removed_set_fdmax(int *fdmax,int sockfd,fd_set sockset)
{
        int new_fdmax,i;

        if(*fdmax < 0){
                return;
        }
        if(sockfd < *fdmax)     return;
        else{
                new_fdmax=-1;
                for(i=0;i<=*fdmax;i++){
                        if(i != sockfd){
                                if(FD_ISSET(i,&sockset)){
                                        if(i > new_fdmax){
                                                new_fdmax=i;
                                        }
                                }
                        }
                }
                *fdmax=new_fdmax;
        }
}
#endif
/* ============================================================================= */
/* ============================================================================= */
#ifdef TCP_ON_CLIENT_SIDE
void fd_removed_set_client_fdmax(unsigned int grp_no,int sockfd,fd_set *sockset_pt)
{

        int old_fdmax,i;
        fd_set *other_sockset_pt;
        TCP_GROUP_STRUCT *grp_pt;

        grp_pt=&tcp_grp_st[grp_no];
        old_fdmax=grp_pt->client_fdmax;

        fd_removed_set_fdmax(&(grp_pt->client_fdmax),sockfd,*sockset_pt);
        if(old_fdmax != sockfd){
                return;
        }
        if (sockset_pt == &(grp_pt->inprogress_client_wr_sockset)){
                other_sockset_pt=&(grp_pt->client_rw_sockset);
        }
        else if (sockset_pt == &(grp_pt->client_rw_sockset)){
                other_sockset_pt=&(grp_pt->inprogress_client_wr_sockset);
        }
        else{
                prnt_msg(TCP_ERR,0x3173);
                return;
        }
        for(i=0;i<=old_fdmax;i++){
                if(FD_ISSET(i,other_sockset_pt)){
                        if(i > grp_pt->client_fdmax){
                                grp_pt->client_fdmax=i;
                        }
                }
        }

}
#endif
/* ============================================================================= */
/* ============================================================================= */
#ifdef TCP_ON_CLIENT_SIDE
int time_diff(struct timeval a, struct timeval b)
{
        register int us,s;

        us=a.tv_usec - b.tv_usec;
        us /= 1000;
        s = a.tv_sec - b.tv_sec;
        s *= 1000;
        return s + us;
}
#endif
/* ============================================================================= */
/* ============================================================================= */
// BOZ :#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))->
#if (OS == LINUX_COMPATIBLE)
int block_socket(int sockfd)
{
        long arg;

        // Set to blocking mode again...
        if( (arg = fcntl(sockfd, F_GETFL, NULL)) < 0) {
                prnt_string("Error fcntl(..., F_GETFL) :");
                prnt_string(strerror(errno));prnt_char(CR);
                return -1;
        }
        arg &= (~O_NONBLOCK);
        if( fcntl(sockfd, F_SETFL, arg) < 0) {
                prnt_string("Error fcntl(..., F_SETFL) :");
                prnt_string(strerror(errno));prnt_char(CR);
                return -1;
        }
        return 0;
}
#endif
/* ============================================================================= */
/* ============================================================================= */
// BOZ :#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))->
#if (OS == LINUX_COMPATIBLE)
int nonblock_socket (int sockfd)
{
        long arg;
        // set non-blocking
        if( (arg = fcntl(sockfd, F_GETFL, NULL)) < 0) {
                prnt_string("Error fcntl(..., F_GETFL):");
                prnt_string(strerror(errno));prnt_char(CR);
                return -1;
        }
        arg |= O_NONBLOCK;
        if( fcntl(sockfd, F_SETFL, arg) < 0) {
                prnt_string("Error fcntl(..., F_SETFL):");
                prnt_string(strerror(errno));prnt_char(CR);
                return -1 ;
        }
        return 0;
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
BYTE save_rcvd_tcp_pckt(unsigned int tcp_no){

        TCP_STRUCT *tcp_pt;
        char dat[MAX_TCP_DATA_SIZE];
        int i,j,no_remained_size_bytes,remained_data_size,indx;
        int size;

        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3127);
                return CONNECTION_FAILED;
        }
        tcp_pt=&tcp_st[tcp_no];
#ifdef DEBUG_ACT
        prnt_string("save_rcvd_tcp_pckt\n");
#endif
        size=recv(tcp_pt->sockfd,dat,sizeof(dat),0);
        /* PEEK but not read from receive buffer list */
        if(size == 0){ // connection closed
                return CONNECTION_CLOSED;
        }
        else if (size < 0){
                return CONNECTION_FAILED;
        }
        indx=0;
        remained_data_size=0;
        while(size > 0){
                if(tcp_pt->rx_pckt_indx == 0){
                        if(size == 1){ // we do not know pckt_size.
                                tcp_pt->no_size_bytes=dat[indx];
                                tcp_pt->rx_pckt_arr[0]=dat[indx];
                                tcp_pt->rx_pckt_indx++;
                                return CONNECTION_OK;
                        }
                        size-=1;
                        tcp_pt->no_size_bytes=dat[indx];
                        tcp_pt->rx_pckt_arr[tcp_pt->rx_pckt_indx++]=dat[indx++];
                }
                no_remained_size_bytes=tcp_pt->no_size_bytes-(tcp_pt->rx_pckt_indx - 1);
                if(no_remained_size_bytes > 0){
                        if(size < tcp_pt->no_size_bytes){ // we do not have received complete size bytes.
                                for(i=0;i<size;i++){
                                        tcp_pt->rx_pckt_arr[tcp_pt->rx_pckt_indx++]=dat[indx++];
                                }
                                return CONNECTION_OK;
                        }
                        else{
                                for(i=0;i<no_remained_size_bytes;i++){
                                        tcp_pt->rx_pckt_arr[tcp_pt->rx_pckt_indx++]=dat[indx++];
                                }
                                tcp_pt->rx_pckt_data_size=0; // complete packet size
                                for(i=0,j=1;i<tcp_pt->no_size_bytes;i++,j++){
                                        tcp_pt->rx_pckt_data_size |= tcp_pt->rx_pckt_arr[j] << (i*8);
                                }
                                size-=no_remained_size_bytes;
                        }
                }
                remained_data_size=(tcp_pt->rx_pckt_data_size+(1+tcp_pt->no_size_bytes))-tcp_pt->rx_pckt_indx;
                if(size >= remained_data_size){
                        // size is also copied in tcp_pt->rx_pckt_arr[]
                        // for compatibility when we rcv only one byte of size
                        // and need a place to hold it.
                        memcpy(&tcp_pt->rx_pckt_arr[tcp_pt->rx_pckt_indx],&dat[indx],remained_data_size);
                        put_pckt_to_variable_size_buf(&(tcp_pt->netrx_st),tcp_pt->rx_pckt_data_size,&(tcp_pt->rx_pckt_arr[1+tcp_pt->no_size_bytes]));
#ifdef DEBUG_ACT
                        prnt_string("no of saved data bytes :");
                        prnt_int(tcp_pt->rx_pckt_data_size);prnt_char(CR);
#endif
                        tcp_pt->rx_pckt_indx=0;
                        tcp_pt->rx_pckt_data_size=0;
                        tcp_pt->no_size_bytes=0;
                        size-=remained_data_size;
                        indx+=remained_data_size;
                        remained_data_size=0;
                        continue;
                }
                else{
                        remained_data_size=size;
                        memcpy(&tcp_pt->rx_pckt_arr[tcp_pt->rx_pckt_indx],&dat[indx],remained_data_size);
                        tcp_pt->rx_pckt_indx+=remained_data_size;
                        return CONNECTION_OK;
                }
        }
        return CONNECTION_OK;
}
#endif
/* ============================================================================= */
/* send returns the number of bytes actually sent out ,this might be less                */
/* than the number you told it to send.sometimes you tell it to send a whole     */
/* gob of data and it just can't handle it.It will fire off as much of the data  */
/* as it can ,and trust you to send the rest later.if the value returned by send()*/
/* doen't match the value in len,it is up to you to send the rest of the string  */
/* if the packet is small(less than 1k or so) it will "probably" manage to send  */
/* the whole thing all in one go.                                                */
/* ============================================================================= */
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
int send_all_tcp_pckts(unsigned int tcp_no)
{
        unsigned int s,data_size,i,j;
        int size,sent_size;
        int remained_size;
        TCP_STRUCT *tcp_pt;
        BYTE dat[MAX_TCP_DATA_SIZE],no_size_bytes;

#ifdef DEBUG_ACT
        /*
        {
                struct sockaddr_in rmt_addr;    // connector's address information

                rmt_addr=tcp_st[tcp_no].remote_addr;

                prnt_string("send_all_tcp_pckts1: ");
                prnt_int(tcp_no);prnt_char('-');
                prnt_int(rmt_addr.sin_addr.s_addr>>16);prnt_int(rmt_addr.sin_addr.s_addr);prnt_char('-');
                prnt_int(ntohs(rmt_addr.sin_port));prnt_char(CR);
        }
        */
#endif
        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3128);
                return -1;
        }
        tcp_pt=&tcp_st[tcp_no];
        if(tcp_pt->tx_pckt_tot_size > 0){
                remained_size=tcp_pt->tx_pckt_tot_size-tcp_pt->tx_pckt_indx;
                if(remained_size > 0){
                        if(tcp_pt->tx_pckt_indx > MAX_TCP_DATA_SIZE){
                                prnt_msg(TCP_ERR,0x3188);
                                return -1;
                        }
                        sent_size=send(tcp_pt->sockfd,&tcp_pt->tx_pckt_arr[tcp_pt->tx_pckt_indx],remained_size,0);
                        if(sent_size == -1){
                                prnt_msg(TCP_ERR,0x3189);
                                return -1;
                        }
                        if(sent_size != remained_size){
                                tcp_pt->tx_pckt_indx+=sent_size;
                                return 0;
                        }
                        else{
                                tcp_pt->tx_pckt_tot_size=0;
                        }
                }
        }
        while(get_pckt_from_variable_size_buf(&(tcp_pt->nettx_st),&data_size,&dat[0]) == OK){
#ifdef DEBUG_ACT
                /*
                {
                        unsigned int i;
                        prnt_string("send_all_tcp_pckts2: ");
                        prnt_int(data_size);prnt_char(CR);
                        for(i=0;i<data_size;i++){
                                prnt_byte(dat[i]);prnt_char('-');
                        }
                        prnt_char(CR);
                }
                */
#endif
                if(data_size <= 0xff){
                        no_size_bytes=1;
                }
                else if(data_size <= 0xffff){
                        no_size_bytes=2;
                }
                else if(data_size <= 0xffffff){
                        no_size_bytes=3;
                }
                else {
                        no_size_bytes=4;
                }
                tcp_pt->tx_pckt_arr[0]=no_size_bytes;
                s=data_size;
                for(i=0,j=1;i<no_size_bytes;i++,j++,s>>=8){
                        tcp_pt->tx_pckt_arr[j]=(BYTE)s;
                }
                memcpy(&tcp_pt->tx_pckt_arr[j],dat,data_size);
                size=(int)(data_size+1+no_size_bytes);
                sent_size=send(tcp_pt->sockfd,tcp_pt->tx_pckt_arr,size,0);
#ifdef DEBUG_ACT
                /*
                {
                        unsigned int i;
                        prnt_string("send_all_tcp_pckts3: ");
                        prnt_int(size);prnt_char('-');
                        prnt_int(sent_size);prnt_char('-');
                        prnt_int(tcp_pt->sockfd);prnt_char(CR);
                        for(i=0;i<size;i++){
                                prnt_byte(tcp_pt->tx_pckt_arr[i]);prnt_char('-');
                        }
                        prnt_char(CR);
                }
                */
#endif
                if(sent_size == -1){
                        prnt_msg(TCP_ERR,0x3129);
                        return -1;
                }
                if(sent_size != size){
                        prnt_msg(TCP_ERR,0x3130);
                        tcp_pt->tx_pckt_indx=sent_size;
                        tcp_pt->tx_pckt_tot_size=size;
                        return 0;
                }
        }
        return 0;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
void init_tcp_st(void)
{

        unsigned int i;
        TCP_STRUCT *tcp_pt;

        for(i=0,tcp_pt=&tcp_st[0];i < max_tcp_connection;i++,tcp_pt++){

                tcp_pt->sockfd=-1;
                tcp_pt->state=TCP_IN_QUE;
                tcp_pt->reconnect_flag=SET;
                tcp_pt->tx_fail_flag=RESET;
//              tcp_pt->start_time;
//              tcp_pt->connect_time;
//              tcp_pt->remote_addr=
                tcp_pt->remote_ip_string=NULL_POINTER;
                tcp_pt->nettx_st.arr = &tcp_pt->tx[0];
                tcp_pt->nettx_st.front = tcp_pt->nettx_st.rear = 0;
                tcp_pt->nettx_st.size = TCP_NETBUF_SIZE;

                tcp_pt->netrx_st.arr = &tcp_pt->rx[0];
                tcp_pt->netrx_st.front = tcp_pt->netrx_st.rear = 0;
                tcp_pt->netrx_st.size = TCP_NETBUF_SIZE;
//              tx[TCP_NETBUF_SIZE];
//              rx[TCP_NETBUF_SIZE];
//              tx_pckt_arr[MAX_TCP_DATA_SIZE];
//              rx_pckt_arr[MAX_TCP_DATA_SIZE];
                tcp_pt->tx_pckt_indx=0;
                tcp_pt->rx_pckt_indx=0;
                tcp_pt->tx_pckt_tot_size=0;
                tcp_pt->rx_pckt_data_size=0;
                tcp_pt->no_size_bytes=0;
                tcp_pt->grp_no=NULL_TCP_GROUP;
                tcp_pt->link_no=NULL_TCP_LINK;
                tcp_pt->type=NULL_TCP_TYPE;
                tcp_pt->wtfr_reconnection_cnt=MAX_WTFR_TCP_RECONNECTION;

                tcp_pt->rp=i+1;
                tcp_pt->lp=i-1;
        }
        first_tcp=0;
        last_tcp=max_tcp_connection-1;
        tcp_st[max_tcp_connection-1].rp=NULL_TCP;
        tcp_st[0].lp=NULL_TCP;

        return;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
void init_tcp_groups(void)
{
        unsigned int grp_no;
        TCP_GROUP_STRUCT *grp_pt;

        for(grp_no=0,grp_pt=&tcp_grp_st[0];grp_no<max_tcp_group;grp_no++,grp_pt++){
                grp_pt->grp_no=grp_no;
#ifdef TCP_ON_CLIENT_SIDE
                grp_pt->first_inprogress_tcp=grp_pt->last_inprogress_tcp=NULL_TCP;
                grp_pt->first_wtfr_reconnection_tcp=grp_pt->last_wtfr_reconnection_tcp=NULL_TCP;
                grp_pt->first_client_tcp=grp_pt->last_client_tcp=NULL_TCP;
                memset(&(grp_pt->client_rw_sockset),0,sizeof(fd_set));
                memset(&(grp_pt->inprogress_client_wr_sockset),0,sizeof(fd_set));
                grp_pt->client_fdmax=-1;
                grp_pt->client_rw_sockset_cnt=0;
                //              grp_pt->client_port_no=
#endif
#ifdef TCP_ON_SERVER_SIDE
                grp_pt->first_server_tcp=grp_pt->last_server_tcp=NULL_TCP;
                memset(&(grp_pt->server_rd_sockset),0,sizeof(fd_set));
                memset(&(grp_pt->server_wr_sockset),0,sizeof(fd_set));
                grp_pt->server_rd_fdmax=grp_pt->server_wr_fdmax=-1;
                //              grp_pt->server_port_no;
                grp_pt->listener_sockfd=-1;
#endif
        }
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
unsigned int find_free_tcp_rec(void)
{

        unsigned int a;
        TCP_STRUCT *tcp_pt;

        if(first_tcp >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3131);
                return NULL_TCP;
        }
        a=first_tcp;
        tcp_pt=&tcp_st[a];
        first_tcp=tcp_pt->rp;
        if(first_tcp < max_tcp_connection)
                tcp_st[first_tcp].lp=NULL_TCP;
        else
                last_tcp=NULL_TCP;
        tcp_pt->rp=NULL_TCP;
        tcp_pt->state=TCP_OUT_OF_QUE;
        return a;
}
#endif
/* ======================================================================== */
/* ======================================================================== */
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
void rls_tcp_rec(unsigned int tcp_no)
{

        TCP_STRUCT *tcp_pt;

        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3152);
                prnt_int(tcp_no);prnt_char(CR);
                return;
        }
        tcp_pt=&tcp_st[tcp_no];
        if(tcp_pt->state == TCP_IN_QUE){
                prnt_msg(TCP_ERR,0x3153);
                prnt_int(tcp_no);prnt_char(CR);
                return;
        }
        if(tcp_pt->sockfd != -1){
                prnt_msg(TCP_ERR,0x3132);
                close(tcp_pt->sockfd);
        }
        tcp_pt->sockfd=-1;
        tcp_pt->state=TCP_IN_QUE;
        tcp_pt->reconnect_flag=SET;
        tcp_pt->tx_fail_flag=RESET;
//      tcp_pt->start_time;
//      tcp_pt->connect_time;
//      tcp_pt->remote_addr=
        if(tcp_pt->remote_ip_string != NULL_POINTER){
                free(tcp_pt->remote_ip_string);
                tcp_pt->remote_ip_string=NULL_POINTER;
        }
//      tcp_pt->nettx_st.arr = &tcp_pt->tx[0];
        tcp_pt->nettx_st.front = tcp_pt->nettx_st.rear = 0;
//      tcp_pt->nettx_st.size = TCP_NETBUF_SIZE;

//      tcp_pt->netrx_st.arr = &tcp_pt->rx[0];
        tcp_pt->netrx_st.front = tcp_pt->netrx_st.rear = 0;
//  tcp_pt->netrx_st.size = TCP_NETBUF_SIZE;
//      tcp_pt->tx[TCP_NETBUF_SIZE];
//      tcp_pt->rx[TCP_NETBUF_SIZE];
//      tcp_pt->tx_pckt_arr[MAX_TCP_DATA_SIZE];
//      tcp_pt->rx_pckt_arr[MAX_TCP_DATA_SIZE];
        tcp_pt->tx_pckt_indx=0;
        tcp_pt->rx_pckt_indx=0;
        tcp_pt->tx_pckt_tot_size=0;
        tcp_pt->rx_pckt_data_size=0;
        tcp_pt->no_size_bytes=0;
        tcp_pt->grp_no=NULL_TCP_GROUP;
        tcp_pt->link_no=NULL_TCP_LINK;
        tcp_pt->type=NULL_TCP_TYPE;
        tcp_pt->wtfr_reconnection_cnt=MAX_WTFR_TCP_RECONNECTION;

        tcp_pt->lp=last_tcp;
        if(last_tcp < max_tcp_connection)
                tcp_st[last_tcp].rp=tcp_no;
        else
                first_tcp=tcp_no;
        last_tcp=tcp_no;
        tcp_pt->rp=NULL_TCP;
        return;
}
#endif
/* ============================================================================= */
/* ============================================================================= */
#ifdef TCP_ON_CLIENT_SIDE
BYTE add_to_inprogress_tcp_list(unsigned int tcp_no)
{

        TCP_STRUCT *tcp_pt;
        unsigned int grp_no;
        TCP_GROUP_STRUCT *grp_pt;

        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3154);
                prnt_int(tcp_no);prnt_char(CR);
                return FAIL;
        }
        tcp_pt=&tcp_st[tcp_no];
        grp_no=tcp_pt->grp_no;
        if(grp_no >= max_tcp_group){
                prnt_msg(TCP_ERR,0x3133);
                return FAIL;
        }
        grp_pt=&tcp_grp_st[grp_no];
        tcp_pt->state=TCP_INPROGRESS_STATE;
        tcp_pt->rp=NULL_TCP;
        tcp_pt->lp=grp_pt->last_inprogress_tcp;
        if(grp_pt->last_inprogress_tcp < max_tcp_connection)
                tcp_st[grp_pt->last_inprogress_tcp].rp=tcp_no;
        else
                grp_pt->first_inprogress_tcp=tcp_no;
        grp_pt->last_inprogress_tcp=tcp_no;
        return OK;
}
#endif
/* ============================================================================= */
/* ============================================================================= */
#ifdef TCP_ON_CLIENT_SIDE
BYTE remove_from_inprogress_tcp_list(unsigned int tcp_no)
{

        TCP_STRUCT *tcp_pt;
        unsigned int grp_no;
        TCP_GROUP_STRUCT *grp_pt;

        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3155);
                prnt_int(tcp_no);prnt_char(CR);
                return FAIL;
        }
        tcp_pt=&tcp_st[tcp_no];
        grp_no=tcp_pt->grp_no;
        if(grp_no >= max_tcp_group){
                prnt_msg(TCP_ERR,0x3134);
                return FAIL;
        }
        grp_pt=&tcp_grp_st[grp_no];
        if(tcp_pt->state == TCP_IN_QUE){
                prnt_msg(TCP_ERR,0x3156);
                prnt_int(tcp_no);prnt_char(CR);
                return FAIL;
        }
        if(tcp_pt->lp != NULL_TCP)
                tcp_st[tcp_pt->lp].rp=tcp_pt->rp;
        else{  /* first node */
                grp_pt->first_inprogress_tcp=tcp_pt->rp;
        }
        if(tcp_pt->rp != NULL_TCP)
                tcp_st[tcp_pt->rp].lp=tcp_pt->lp;
        else  /* last node */
                grp_pt->last_inprogress_tcp=tcp_pt->lp;
        return OK;
}
#endif
/* ======================================================================= */
/* ============================================================================= */
#ifdef TCP_ON_CLIENT_SIDE
BYTE add_to_wtfr_reconnection_tcp_list(unsigned int tcp_no)
{

        TCP_STRUCT *tcp_pt;
        unsigned int grp_no;
        TCP_GROUP_STRUCT *grp_pt;

        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3157);
                prnt_int(tcp_no);prnt_char(CR);
                return FAIL;
        }
        tcp_pt=&tcp_st[tcp_no];
        grp_no=tcp_pt->grp_no;
        if(grp_no >= max_tcp_group){
                prnt_msg(TCP_ERR,0x3135);
                return FAIL;
        }
#ifdef DEBUG_ACT
        prnt_string("add_to_wtfr_reconnection_tcp_list\n");
        prnt_int(tcp_no);prnt_char(CR);
#endif
        grp_pt=&tcp_grp_st[grp_no];
        tcp_pt->state=TCP_WTFR_RECONNECTION_STATE;
        tcp_pt->wtfr_reconnection_cnt=MAX_WTFR_TCP_RECONNECTION;
        tcp_pt->rp=NULL_TCP;
        tcp_pt->lp=grp_pt->last_wtfr_reconnection_tcp;
        if(grp_pt->last_wtfr_reconnection_tcp < max_tcp_connection)
                tcp_st[grp_pt->last_wtfr_reconnection_tcp].rp=tcp_no;
        else
                grp_pt->first_wtfr_reconnection_tcp=tcp_no;
        grp_pt->last_wtfr_reconnection_tcp=tcp_no;
        return OK;
}
#endif
/* ============================================================================= */
/* ============================================================================= */
#ifdef TCP_ON_CLIENT_SIDE
BYTE remove_from_wtfr_reconnection_tcp_list(unsigned int tcp_no)
{

        TCP_STRUCT *tcp_pt;
        unsigned int grp_no;
        TCP_GROUP_STRUCT *grp_pt;

        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3158);
                prnt_int(tcp_no);prnt_char(CR);
                return FAIL;
        }
        tcp_pt=&tcp_st[tcp_no];
        grp_no=tcp_pt->grp_no;
        if(grp_no >= max_tcp_group){
                prnt_msg(TCP_ERR,0x3136);
                return FAIL;
        }
        grp_pt=&tcp_grp_st[grp_no];
        if(tcp_pt->state == TCP_IN_QUE){
                prnt_msg(TCP_ERR,0x3159);
                prnt_int(tcp_no);prnt_char(CR);
                return FAIL;
        }
        if(tcp_pt->lp != NULL_TCP)
                tcp_st[tcp_pt->lp].rp=tcp_pt->rp;
        else{  /* first node */
                grp_pt->first_wtfr_reconnection_tcp=tcp_pt->rp;
        }
        if(tcp_pt->rp != NULL_TCP)
                tcp_st[tcp_pt->rp].lp=tcp_pt->lp;
        else  /* last node */
                grp_pt->last_wtfr_reconnection_tcp=tcp_pt->lp;
        return OK;
}
#endif
/* ============================================================================= */
/* ============================================================================= */
#ifdef TCP_ON_CLIENT_SIDE
BYTE add_to_client_tcp_list(unsigned int tcp_no)
{

        TCP_STRUCT *tcp_pt;
        unsigned int grp_no;
        TCP_GROUP_STRUCT *grp_pt;

        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3160);
                prnt_int(tcp_no);prnt_char(CR);
                return FAIL;
        }
        tcp_pt=&tcp_st[tcp_no];
        grp_no=tcp_pt->grp_no;
        if(grp_no >= max_tcp_group){
                prnt_msg(TCP_ERR,0x3137);
                return FAIL;
        }
        grp_pt=&tcp_grp_st[grp_no];
        tcp_pt->state=TCP_CONNECTED_STATE;
        tcp_pt->rp=NULL_TCP;
        tcp_pt->lp=grp_pt->last_client_tcp;
        if(grp_pt->last_client_tcp < max_tcp_connection)
                tcp_st[grp_pt->last_client_tcp].rp=tcp_no;
        else
                grp_pt->first_client_tcp=tcp_no;
        grp_pt->last_client_tcp=tcp_no;
        return OK;
}
#endif
/* ============================================================================= */
/* ============================================================================= */
#ifdef TCP_ON_CLIENT_SIDE
BYTE remove_from_client_tcp_list(unsigned int tcp_no)
{

        TCP_STRUCT *tcp_pt;
        unsigned int grp_no;
        TCP_GROUP_STRUCT *grp_pt;

        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3161);
                prnt_int(tcp_no);prnt_char(CR);
                return FAIL;
        }
        if(tcp_st[tcp_no].state == TCP_IN_QUE){
                prnt_msg(TCP_ERR,0x3162);
                prnt_int(tcp_no);prnt_char(CR);
                return FAIL;
        }
        tcp_pt=&tcp_st[tcp_no];
        grp_no=tcp_pt->grp_no;
        if(grp_no >= max_tcp_group){
                prnt_msg(TCP_ERR,0x3138);
                return FAIL;
        }
        grp_pt=&tcp_grp_st[grp_no];
        if(tcp_pt->lp != NULL_TCP)
                tcp_st[tcp_pt->lp].rp=tcp_pt->rp;
        else{  /* first node */
                grp_pt->first_client_tcp=tcp_pt->rp;
        }
        if(tcp_pt->rp != NULL_TCP)
                tcp_st[tcp_pt->rp].lp=tcp_pt->lp;
        else  /* last node */
                grp_pt->last_client_tcp=tcp_pt->lp;
        return OK;
}
#endif
/* ============================================================================= */
/* ============================================================================= */
#ifdef TCP_ON_SERVER_SIDE
BYTE add_to_server_tcp_list(unsigned int tcp_no)
{

        TCP_STRUCT *tcp_pt;
        unsigned int grp_no;
        TCP_GROUP_STRUCT *grp_pt;

        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3163);
                prnt_int(tcp_no);prnt_char(CR);
                return FAIL;
        }
        tcp_pt=&tcp_st[tcp_no];
        grp_no=tcp_pt->grp_no;
        if(grp_no >= max_tcp_group){
                prnt_msg(TCP_ERR,0x3139);
                prnt_int(grp_no);prnt_char(CR);
                return FAIL;
        }
        grp_pt=&tcp_grp_st[grp_no];
        tcp_pt->state=TCP_CONNECTED_STATE;
        tcp_pt->rp=NULL_TCP;
        tcp_pt->lp=grp_pt->last_server_tcp;
        if(grp_pt->last_server_tcp < max_tcp_connection)
                tcp_st[grp_pt->last_server_tcp].rp=tcp_no;
        else
                grp_pt->first_server_tcp=tcp_no;
        grp_pt->last_server_tcp=tcp_no;
        return OK;
}
#endif
/* ============================================================================= */
/* ============================================================================= */
#ifdef TCP_ON_SERVER_SIDE
BYTE remove_from_server_tcp_list(unsigned int tcp_no)
{

        TCP_STRUCT *tcp_pt;
        unsigned int grp_no;
        TCP_GROUP_STRUCT *grp_pt;

        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3164);
                prnt_int(tcp_no);prnt_char(CR);
                return FAIL;
        }
        tcp_pt=&tcp_st[tcp_no];
        grp_no=tcp_pt->grp_no;
        if(grp_no >= max_tcp_group){
                prnt_msg(TCP_ERR,0x3140);
                return FAIL;
        }
        grp_pt=&tcp_grp_st[grp_no];
        if(tcp_pt->state == TCP_IN_QUE){
                prnt_msg(TCP_ERR,0x3165);
                prnt_int(tcp_no);prnt_char(CR);
                return FAIL;
        }
        if(tcp_pt->lp != NULL_TCP)
                tcp_st[tcp_pt->lp].rp=tcp_pt->rp;
        else{  /* first node */
                grp_pt->first_server_tcp=tcp_pt->rp;
        }
        if(tcp_pt->rp != NULL_TCP)
                tcp_st[tcp_pt->rp].lp=tcp_pt->lp;
        else  /* last node */
                grp_pt->last_server_tcp=tcp_pt->lp;
        return OK;
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef TCP_ON_CLIENT_SIDE
void release_inprogress_client_tcp_socket(unsigned int tcp_no){

        TCP_GROUP_STRUCT *grp_pt;
        TCP_STRUCT *tcp_pt;
        unsigned int grp_no;

        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3145);
                return;
        }
        tcp_pt=&tcp_st[tcp_no];
        grp_no=tcp_pt->grp_no;
        if(grp_no >= max_tcp_group){
                prnt_msg(TCP_ERR,0x3146);
                return;
        }
        grp_pt=&tcp_grp_st[grp_no];
//      fd_removed_set_fdmax(&(grp_pt->client_fdmax),tcp_pt->sockfd,(grp_pt->inprogress_client_wr_sockset));
        fd_removed_set_client_fdmax(grp_no,tcp_pt->sockfd,&(grp_pt->inprogress_client_wr_sockset));
        FD_CLR(tcp_pt->sockfd,&(grp_pt->inprogress_client_wr_sockset)); // remove from sockset
        close(tcp_pt->sockfd);
        tcp_pt->sockfd=-1;
        remove_from_inprogress_tcp_list(tcp_no);
#ifdef DEBUG_ACT
        prnt_string("release_inprogress_client_tcp_socket\n");
        prnt_int(tcp_no);prnt_char(CR);
#endif
        if(tcp_pt->reconnect_flag == SET){
                        add_to_wtfr_reconnection_tcp_list(tcp_no);
        }
        else{
                //              rls_tcp_rec(tcp_no);
        }
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef TCP_ON_CLIENT_SIDE
void release_client_tcp_socket(unsigned int tcp_no){

        TCP_GROUP_STRUCT *grp_pt;
        TCP_STRUCT *tcp_pt;
        unsigned int grp_no;

        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3147);
                return;
        }
        tcp_pt=&tcp_st[tcp_no];
        grp_no=tcp_pt->grp_no;
        if(grp_no >= max_tcp_group){
                prnt_msg(TCP_ERR,0x3148);
                return;
        }
        grp_pt=&tcp_grp_st[grp_no];
//      fd_removed_set_fdmax(&(grp_pt->client_fdmax),tcp_pt->sockfd,grp_pt->client_rw_sockset);
        fd_removed_set_client_fdmax(grp_no,tcp_pt->sockfd,&(grp_pt->client_rw_sockset));
        FD_CLR(tcp_pt->sockfd,&(grp_pt->client_rw_sockset)); // remove from sockset
        if(grp_pt->client_rw_sockset_cnt > 0){
                grp_pt->client_rw_sockset_cnt--;
        }
        close(tcp_pt->sockfd);
        switch(tcp_pt->grp_no){
#ifdef OPERATOR_SIDE
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
        case OAM_TCP_GROUP:
                if(tcp_pt->link_no < max_sys_oam_tcp_link)
                        tcp_link_st[tcp_pt->link_no].flag=RESET;
                break;
#endif
#endif
//APOLO start : block added
#ifdef SYSTEM_SIDE
#ifdef HTBY_LINK_ON_TCPIP
        case HTBY_LINK_TCP_GROUP :
                htby_link_disconnected_rtn();
                break;
#endif
#endif
//APOLO end
        default :
                prnt_msg(TCP_ERR,0x3181);
                break;
        }
        tcp_pt->sockfd=-1;
        remove_from_client_tcp_list(tcp_no);
        if(tcp_pt->reconnect_flag == SET){
                add_to_wtfr_reconnection_tcp_list(tcp_no);
        }
        else{
                //              rls_tcp_rec(tcp_no);
        }
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef TCP_ON_SERVER_SIDE
void release_server_tcp_socket(unsigned int tcp_no){

        TCP_GROUP_STRUCT *grp_pt;
        TCP_STRUCT *tcp_pt;
        unsigned int grp_no;

        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3149);
                return;
        }
        tcp_pt=&tcp_st[tcp_no];
        grp_no=tcp_pt->grp_no;
        if(grp_no >= max_tcp_group){
                prnt_msg(TCP_ERR,0x3150);
                return;
        }
        grp_pt=&tcp_grp_st[grp_no];
        fd_removed_set_fdmax(&(grp_pt->server_rd_fdmax),tcp_pt->sockfd,grp_pt->server_rd_sockset);
        FD_CLR(tcp_pt->sockfd,&(grp_pt->server_rd_sockset)); // remove from server_rd_sockset
        update_server_wr_sockset(tcp_pt->grp_no);
        close(tcp_pt->sockfd);  // bye!
        tcp_pt->sockfd=-1;
//APOLO start : block added
#ifdef SYSTEM_SIDE
#ifdef HTBY_LINK_ON_TCPIP
        switch(tcp_pt->grp_no){
        case HTBY_LINK_TCP_GROUP :
                htby_link_disconnected_rtn();
                break;
        default:
                break;
        }
#endif
#endif
//APOLO end
        remove_from_server_tcp_list(tcp_no);
        //      rls_tcp_rec(tcp_no);
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
void init_tcp(void){

#ifdef TCP_DEBUG_ACT
        open_tcp_err_file();
#endif
        max_tcp_connection=MAX_TCP_CONNECTION;
        max_sys_oam_tcp_link=MAX_SYS_OAM_TCP_LINK;
        max_tcp_group=MAX_TCP_GROUP;
#if (MAX_SYS_OAM_TCP_LINK > 0)
        tcp_link_0=0;
#endif
#if (MAX_SYS_OAM_TCP_LINK > 1)
        tcp_link_1=1;
#endif
        init_tcp_groups();
        init_tcp_st();
#ifdef OPERATOR_SIDE
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
        init_sys_oam_tcp_client();
#endif
#endif
#ifdef SYSTEM_SIDE
#ifdef TCPIP_ON_SYS_OAM_SYSTEM_SIDE
        init_sys_oam_tcp_server();
#endif
//APOLO start
#ifdef HTBY_LINK_ON_TCPIP
        if (rd_master_swch() == SET){
#ifdef TCP_ON_SERVER_SIDE
                init_htby_link_tcp_server();
#endif
        }
        else{
#ifdef TCP_ON_CLIENT_SIDE
                init_htby_link_tcp_client();
#endif
        }
#endif
//APOLO end
#endif // SYSTEM_SIDE

}
#endif
/* ======================================================================= */
/* ======================================================================= */
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
void init_tcp_threads(void){

#ifdef OPERATOR_SIDE
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
        init_sys_oam_tcp_client_thread();
#endif
#endif
#ifdef SYSTEM_SIDE
#ifdef TCPIP_ON_SYS_OAM_SYSTEM_SIDE
        init_sys_oam_tcp_server_thread();
#endif
//APOLO start
#ifdef HTBY_LINK_ON_TCPIP
        if (rd_master_swch() == SET){
#ifdef TCP_ON_SERVER_SIDE
                init_htby_link_tcp_server_thread();
#endif
        }
        else{
#ifdef TCP_ON_CLIENT_SIDE
                init_htby_link_tcp_client_thread();
#endif
        }
#endif
//APOLO end
#endif // SYSTEM_SIDE
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef OPERATOR_SIDE
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
int init_sys_oam_tcp_client(void){

        unsigned int tcp_no,grp_no;
        struct hostent *remote;
        TCP_STRUCT *tcp_pt;
        TCP_GROUP_STRUCT *grp_pt;

#ifdef DEBUG_ACT
        prnt_char('i');prnt_char('n');prnt_char('i');prnt_char('t');prnt_char(' ');
        prnt_char('o');prnt_char('a');prnt_char('m');prnt_char(' ');
        prnt_char('c');prnt_char('l');prnt_char('i');prnt_char('e');prnt_char('n');prnt_char('t');prnt_char(CR);
#endif
#if (MAX_SYS_OAM_TCP_LINK > 0)
        tcp_link_st[0].tcp_no=NULL_TCP;
        tcp_link_st[0].flag=RESET;
        tcp_link_st[0].count=MAX_TCP_CHECK_LINK_COUNT;
#endif
#if (MAX_SYS_OAM_TCP_LINK > 1)
        tcp_link_st[1].tcp_no=NULL_TCP;
        tcp_link_st[1].flag=RESET;
        tcp_link_st[1].count=MAX_TCP_CHECK_LINK_COUNT;
#endif
#ifdef HOT_STANDBY
        hot_link=UNKNOWN_LINK;
#else
        hot_link=tcp_link_0;
#endif
        grp_no=OAM_TCP_GROUP;
        grp_pt=&tcp_grp_st[grp_no];
        grp_pt->client_port_no=SYS_OAM_PORT;
        tcp_no=find_free_tcp_rec();
        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3141);
                return -1;
        }
        tcp_pt=&tcp_st[tcp_no];
        tcp_pt->grp_no=grp_no;
        tcp_pt->remote_ip_string=malloc(sizeof(MASTER_IP_STRING));
        memcpy(tcp_pt->remote_ip_string,MASTER_IP_STRING,sizeof(MASTER_IP_STRING));
        if((remote=gethostbyname(tcp_pt->remote_ip_string)) == NULL){ // get the host info
                prnt_msg(TCP_ERR,0x3142);
                rls_tcp_rec(tcp_no);
        }
        else{
                tcp_pt->remote_addr.sin_family=AF_INET;                 // host byte order
                tcp_pt->remote_addr.sin_port=htons(grp_pt->client_port_no);             // short,network byte order
                tcp_pt->remote_addr.sin_addr=*((struct in_addr *)remote->h_addr);
                memset(&(tcp_pt->remote_addr.sin_zero),'\0',8); // zero the rest of the struct
                tcp_link_st[tcp_link_0].tcp_no=tcp_no;
                tcp_pt->link_no=tcp_link_0;
                tcp_pt->type=OAM_TCP_TYPE;
        }
#ifdef HOT_STANDBY
        tcp_no=find_free_tcp_rec();
        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3143);
                return -1;
        }
        tcp_pt=&tcp_st[tcp_no];
        tcp_pt->grp_no=grp_no;
        tcp_pt->remote_ip_string=malloc(sizeof(SLAVE_IP_STRING));
        memcpy(tcp_pt->remote_ip_string,SLAVE_IP_STRING,sizeof(SLAVE_IP_STRING));
        if((remote=gethostbyname(tcp_pt->remote_ip_string)) == NULL){ // get the host info
                prnt_msg(TCP_ERR,0x3144);
                rls_tcp_rec(tcp_no);
        }
        else{
                tcp_pt->remote_addr.sin_family=AF_INET;                 // host byte order
                tcp_pt->remote_addr.sin_port=htons(grp_pt->client_port_no);             // short,network byte order
                tcp_pt->remote_addr.sin_addr=*((struct in_addr *)remote->h_addr);
                memset(&(tcp_pt->remote_addr.sin_zero),'\0',8); // zero the rest of the struct
                tcp_link_st[tcp_link_1].tcp_no=tcp_no;
                tcp_pt->link_no=tcp_link_1;
                tcp_pt->type=OAM_TCP_TYPE;
        }
#endif // HOT_STANDBY
        return 0;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef OPERATOR_SIDE
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
int init_sys_oam_tcp_client_thread(void){

    pthread_attr_t attr1;
        unsigned int grp_no;
        TCP_GROUP_STRUCT *grp_pt;

//#ifdef DEBUG_ACT
        prnt_char('i');prnt_char('n');prnt_char('i');prnt_char('t');prnt_char(' ');
        prnt_char('o');prnt_char('a');prnt_char('m');prnt_char(' ');
        prnt_char('c');prnt_char('l');prnt_char('i');prnt_char('e');prnt_char('n');prnt_char('t');prnt_char(' ');
        prnt_char('t');prnt_char('h');prnt_char('r');prnt_char('e');prnt_char('a');prnt_char('d');prnt_char(CR);
//#endif

        grp_no=OAM_TCP_GROUP;
        grp_pt=&tcp_grp_st[grp_no];
        grp_pt->client_fdmax=-1;
        FD_ZERO(&(grp_pt->inprogress_client_wr_sockset));       // clear the sockset and temp sets
        FD_ZERO(&(grp_pt->client_rw_sockset));                          // clear the sockset and temp sets
        if( tcp_link_st[tcp_link_0].tcp_no < max_tcp_connection){
                start_client_connection(tcp_link_st[tcp_link_0].tcp_no);
        }
//#ifdef DEBUG_ACT
        prnt_int(tcp_link_st[tcp_link_0].tcp_no);prnt_char(CR);
//#endif
#ifdef HOT_STANDBY
        if( tcp_link_st[tcp_link_1].tcp_no < max_tcp_connection){
                start_client_connection(tcp_link_st[tcp_link_1].tcp_no);
        }
//#ifdef DEBUG_ACT
        prnt_int(tcp_link_st[tcp_link_1].tcp_no);prnt_char(CR);
//#endif
#endif // HOT_STANDBY
        pthread_attr_init(&attr1);
        pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);
        pthread_create(&tcp_trx_client_thread[grp_no], &attr1,tcp_trx_client_thread_program, (void *)grp_pt);
        return 0;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef SYSTEM_SIDE
#ifdef TCPIP_ON_SYS_OAM_SYSTEM_SIDE
int init_sys_oam_tcp_server(void){

        TCP_GROUP_STRUCT *grp_pt;
        unsigned int grp_no,tcp_no;
        TCP_STRUCT *tcp_pt;

#ifdef DEBUG_ACT
        prnt_char('i');prnt_char('n');prnt_char('i');prnt_char('t');prnt_char(' ');
        prnt_char('o');prnt_char('a');prnt_char('m');prnt_char(' ');
        prnt_char('s');prnt_char('e');prnt_char('r');prnt_char('v');prnt_char('e');prnt_char('r');prnt_char(CR);
#endif

        max_sys_oam_tcp_link=MAX_SYS_OAM_TCP_LINK;

        grp_no=OAM_TCP_GROUP;
        grp_pt=&tcp_grp_st[grp_no];
        grp_pt->server_port_no=SYS_OAM_PORT;

#if (MAX_SYS_OAM_TCP_LINK > 0)
        tcp_link_st[tcp_link_0].oam_no=0;
        tcp_no=find_free_tcp_rec();
        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3170);
                return -1;
        }
        tcp_pt=&tcp_st[tcp_no];
        tcp_pt->grp_no=grp_no;
        tcp_link_st[tcp_link_0].tcp_no=tcp_no;
        tcp_pt->link_no=tcp_link_0;
        tcp_pt->type=OAM_TCP_TYPE;
#endif
#if (MAX_SYS_OAM_TCP_LINK > 1)
        tcp_link_st[tcp_link_1].oam_no=1;
        tcp_no=find_free_tcp_rec();
        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3171);
                return -1;
        }
        tcp_pt=&tcp_st[tcp_no];
        tcp_pt->grp_no=grp_no;
        tcp_link_st[tcp_link_1].tcp_no=tcp_no;
        tcp_pt->link_no=tcp_link_1;
        tcp_pt->type=OAM_TCP_TYPE;
#endif
        return 0;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef SYSTEM_SIDE
#ifdef TCPIP_ON_SYS_OAM_SYSTEM_SIDE
int init_sys_oam_tcp_server_thread(void){

        TCP_GROUP_STRUCT *grp_pt;
    pthread_attr_t attr1;
        unsigned int grp_no;

#ifdef DEBUG_ACT
        prnt_char('i');prnt_char('n');prnt_char('i');prnt_char('t');prnt_char(' ');
        prnt_char('o');prnt_char('a');prnt_char('m');prnt_char(' ');
        prnt_char('s');prnt_char('e');prnt_char('r');prnt_char('v');prnt_char('e');prnt_char('r');prnt_char(' ');
        prnt_char('t');prnt_char('h');prnt_char('r');prnt_char('e');prnt_char('a');prnt_char('d');prnt_char(CR);
#endif

        grp_no=OAM_TCP_GROUP;
        grp_pt=&tcp_grp_st[grp_no];
        grp_pt->server_rd_fdmax=-1;
        grp_pt->server_wr_fdmax=-1;
        FD_ZERO(&(grp_pt->server_rd_sockset));  // clear the server_rd_sockset and temp sets
        FD_ZERO(&(grp_pt->server_wr_sockset));  // clear the server_rd_sockset and temp sets

        pthread_attr_init(&attr1);
        pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);
        pthread_create(&tcp_rx_server_thread[grp_no], &attr1, tcp_rx_server_thread_program, (void *)grp_pt);
        return 0;
}
#endif
#endif
/* ====================================================================== */
/* ====================================================================== */
#ifdef TCP_DEBUG_ACT
void close_tcp_err_file(void){
//MOHAJER start : some changes
        if(tcp_fer != NULL_POINTER){
                fclose(tcp_fer);
        }
//MOHAJER end
}
#endif
/* ====================================================================== */
/* ====================================================================== */
#ifdef TCP_DEBUG_ACT
void open_tcp_err_file(void){
        tcp_reopen_cnt=MAX_TCP_REOPEN_FILE_COUNT;
        tcp_fer=fopen("tcp.dbg","ab");
}
#endif
/* ====================================================================== */
/* ====================================================================== */
#ifdef TCP_DEBUG_ACT
void reopen_tcp_err_file(void){

//MOHAJER start
        if(tcp_reopen_cnt-- == 0){
                tcp_reopen_cnt=MAX_TCP_REOPEN_FILE_COUNT;
                if(tcp_fer != NULL_POINTER){
                        fclose(tcp_fer);
                }
                tcp_fer=fopen("tcp.dbg","ab");
        }
//MOHAJER end
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef OPERATOR_SIDE
#ifdef TCPIP_ON_SYS_OAM_OPERATOR_SIDE
void check_tcp_link_handler(void){

        unsigned int i,tcp_no;

        for(i=0;i<max_sys_oam_tcp_link;i++){
                tcp_no=tcp_link_st[i].tcp_no;
                if(tcp_no >= max_tcp_connection){
                        led_reset(sync_led[i]);
                        led_reset(link_up_led[i]);
                        continue;
                }
                if (tcp_link_st[i].count-- == 0){
                        if (tcp_link_st[i].flag == SET){
                                led_set(sync_led[i]);
                                led_set(link_up_led[i]);
                        }
                        else{
                                led_reset(sync_led[i]);
                                led_reset(link_up_led[i]);
                        }
                        tcp_link_st[i].count=MAX_TCP_CHECK_LINK_COUNT;
                }
        }
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
void initialize_tcp_network_buf(unsigned int tcp_no)
{
        TCP_STRUCT *tcp_pt;

        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3174);
                return;
        }
        tcp_pt=&tcp_st[tcp_no];
        tcp_pt->nettx_st.front = tcp_pt->nettx_st.rear = 0;
        tcp_pt->netrx_st.front = tcp_pt->netrx_st.rear = 0;
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#if ((defined TCP_ON_CLIENT_SIDE) || (defined TCP_ON_SERVER_SIDE))
void initialize_tcp_network_tx_buf(unsigned int tcp_no)
{
        TCP_STRUCT *tcp_pt;

        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3185);
                return;
        }
        tcp_pt=&tcp_st[tcp_no];
        tcp_pt->nettx_st.front = tcp_pt->nettx_st.rear = 0;
}
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef SYSTEM_SIDE
#ifdef HTBY_LINK_ON_TCPIP
int init_htby_link_tcp_client(void){

        unsigned int tcp_no,grp_no;
        struct hostent *remote;
        TCP_STRUCT *tcp_pt;
        TCP_GROUP_STRUCT *grp_pt;

#ifdef DEBUG_ACT
        prnt_char('i');prnt_char('n');prnt_char('i');prnt_char('t');prnt_char(' ');
        prnt_char('h');prnt_char('t');prnt_char('b');prnt_char('y');prnt_char(' ');
        prnt_char('c');prnt_char('l');prnt_char('i');prnt_char('e');prnt_char('n');prnt_char('t');prnt_char(CR);
#endif
        tcp_htby_link.tcp_no=NULL_TCP;
        tcp_htby_link.flag=RESET;
        //      tcp_htby_link.count=MAX_TCP_CHECK_LINK_COUNT;

        grp_no=HTBY_LINK_TCP_GROUP;
        if(grp_no >= MAX_TCP_GROUP){
                prnt_msg(TCP_ERR,0x3186);
                return -1;
        }
        grp_pt=&tcp_grp_st[grp_no];
        grp_pt->client_port_no=HTBY_LINK_TCP_PORT;
        tcp_no=find_free_tcp_rec();
        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3182);
                return -1;
        }
        tcp_pt=&tcp_st[tcp_no];
        tcp_pt->grp_no=grp_no;
        tcp_pt->remote_ip_string=malloc(sizeof(MASTER_IP_STRING));
        memcpy(tcp_pt->remote_ip_string,MASTER_IP_STRING,sizeof(MASTER_IP_STRING));
        if((remote=gethostbyname(tcp_pt->remote_ip_string)) == NULL){ // get the host info
                prnt_msg(TCP_ERR,0x3183);
                rls_tcp_rec(tcp_no);
        }
        else{
                tcp_pt->remote_addr.sin_family=AF_INET;                 // host byte order
                tcp_pt->remote_addr.sin_port=htons(grp_pt->client_port_no);             // short,network byte order
                tcp_pt->remote_addr.sin_addr=*((struct in_addr *)remote->h_addr);
                memset(&(tcp_pt->remote_addr.sin_zero),'\0',8); // zero the rest of the struct
                tcp_htby_link.tcp_no=tcp_no;
                tcp_pt->link_no=0;
                tcp_pt->type=HTBY_LINK_TCP_TYPE;
        }
        init_htby_link_rtn();
        return 0;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef SYSTEM_SIDE
#ifdef HTBY_LINK_ON_TCPIP
int init_htby_link_tcp_client_thread(void){

    pthread_attr_t attr1;
        unsigned int grp_no;
        TCP_GROUP_STRUCT *grp_pt;

//#ifdef DEBUG_ACT
        prnt_char('i');prnt_char('n');prnt_char('i');prnt_char('t');prnt_char(' ');
        prnt_char('h');prnt_char('t');prnt_char('b');prnt_char('y');prnt_char(' ');
        prnt_char('c');prnt_char('l');prnt_char('i');prnt_char('e');prnt_char('n');prnt_char('t');prnt_char(' ');
        prnt_char('t');prnt_char('h');prnt_char('r');prnt_char('e');prnt_char('a');prnt_char('d');prnt_char(CR);
//#endif

        grp_no=HTBY_LINK_TCP_GROUP;
        grp_pt=&tcp_grp_st[grp_no];
        grp_pt->client_fdmax=-1;
        FD_ZERO(&(grp_pt->inprogress_client_wr_sockset));       // clear the sockset and temp sets
        FD_ZERO(&(grp_pt->client_rw_sockset));                          // clear the sockset and temp sets
        if( tcp_htby_link.tcp_no < max_tcp_connection){
                start_client_connection(tcp_htby_link.tcp_no);
        }
//#ifdef DEBUG_ACT
        prnt_int(tcp_htby_link.tcp_no);prnt_char(CR);
//#endif
    pthread_attr_init(&attr1);
    pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);
    pthread_create(&tcp_trx_client_thread[grp_no], &attr1,tcp_trx_client_thread_program, (void *)grp_pt);
        return 0;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef SYSTEM_SIDE
#ifdef HTBY_LINK_ON_TCPIP
int init_htby_link_tcp_server(void){

        TCP_GROUP_STRUCT *grp_pt;
        unsigned int grp_no,tcp_no;
        TCP_STRUCT *tcp_pt;

#ifdef DEBUG_ACT
        prnt_char('i');prnt_char('n');prnt_char('i');prnt_char('t');prnt_char(' ');
        prnt_char('h');prnt_char('t');prnt_char('b');prnt_char('y');prnt_char(' ');
        prnt_char('s');prnt_char('e');prnt_char('r');prnt_char('v');prnt_char('e');prnt_char('r');prnt_char(CR);
#endif

        grp_no=HTBY_LINK_TCP_GROUP;
        grp_pt=&tcp_grp_st[grp_no];
        if(grp_no >= max_tcp_group){
                prnt_msg(TCP_ERR,0x3187);
                return -1;
        }
        grp_pt->server_port_no=HTBY_LINK_TCP_PORT;

        tcp_no=find_free_tcp_rec();
        if(tcp_no >= max_tcp_connection){
                prnt_msg(TCP_ERR,0x3184);
                return -1;
        }
        tcp_pt=&tcp_st[tcp_no];
        tcp_pt->grp_no=grp_no;
        tcp_htby_link.tcp_no=tcp_no;
        tcp_pt->type=HTBY_LINK_TCP_TYPE;

        init_htby_link_rtn();
        return 0;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
#ifdef SYSTEM_SIDE
#ifdef HTBY_LINK_ON_TCPIP
int init_htby_link_tcp_server_thread(void){

        TCP_GROUP_STRUCT *grp_pt;
    pthread_attr_t attr1;
        unsigned int grp_no;

#ifdef DEBUG_ACT
        prnt_char('i');prnt_char('n');prnt_char('i');prnt_char('t');prnt_char(' ');
        prnt_char('h');prnt_char('t');prnt_char('b');prnt_char('y');prnt_char(' ');
        prnt_char('s');prnt_char('e');prnt_char('r');prnt_char('v');prnt_char('e');prnt_char('r');prnt_char(' ');
        prnt_char('t');prnt_char('h');prnt_char('r');prnt_char('e');prnt_char('a');prnt_char('d');prnt_char(CR);
#endif

        grp_no=HTBY_LINK_TCP_GROUP;
        grp_pt=&tcp_grp_st[grp_no];
        grp_pt->server_rd_fdmax=-1;
        grp_pt->server_wr_fdmax=-1;
        FD_ZERO(&(grp_pt->server_rd_sockset));  // clear the server_rd_sockset and temp sets
        FD_ZERO(&(grp_pt->server_wr_sockset));  // clear the server_rd_sockset and temp sets

    pthread_attr_init(&attr1);
    pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);
    pthread_create(&tcp_rx_server_thread[grp_no], &attr1, tcp_rx_server_thread_program, (void *)grp_pt);
        return 0;
}
#endif
#endif
/* ======================================================================= */
/* ======================================================================= */
