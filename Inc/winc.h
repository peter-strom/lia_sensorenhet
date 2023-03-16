#ifndef __WINC_H__
#define __WINC_H__

#include "main.h"
#include "socket/include/socket.h"
#include "driver/include/m2m_wifi.h"
#include "driver/source/nmbus.h"
#include "winc_callbacks.h"

typedef enum
{
    OPT_TRUE,
    OPT_FALSE
} tenuMacOpt;

void winc_init(void);
void socket_init(struct sockaddr_in * pSockAddr);
void connect_to_ap(void);
void connect_to_server(struct sockaddr_in *pSockAddr);
void print_mac(void);


#endif /*__WINC_H__ */