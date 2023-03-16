#ifndef __WINC_CALLBACKS_H__
#define __WINC_CALLBACKS_H__

#include "main.h"
#include "rtc.h"
#include "socket/include/socket.h"
#include "driver/include/m2m_wifi.h"

void wifi_cb(uint8_t u8MsgType, void *pvMsg);
void socket_cb(SOCKET sock, uint8_t u8Msg, void *pvMsg);

typedef enum
{
    NOT_CONNECTED,
    PENDING_CONNECT,
    CONNECTED
} tenuConState;

extern tenuConState gconnected;
extern uint8_t gscancomplete;
extern SOCKET tcp_client_socket;

#endif /*__WINC_CALLBACKS_H__ */