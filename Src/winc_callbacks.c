#include "winc_callbacks.h"
#include "eeprom.h"
#include <stdbool.h>
#include "rtc_functions.h"

/* WIFI STUFF */
tenuConState gconnected;
uint8_t gscancomplete;

/* SOCKET STUFF */
SOCKET tcp_client_socket = -1;
/* Receive buffer definition. */
static uint8_t gau8SocketTestBuffer[MAIN_WIFI_M2M_BUFFER_SIZE];

/* Message format declarations. */
static t_msg_wifi_product msg_wifi_product = {
    .name = MAIN_WIFI_M2M_PRODUCT_NAME,
};

/**
 * @brief https://www.microchip.com/forums/m1124053.aspx
 *
 * @param u8MsgType
 * @param pvMsg
 */
void wifi_cb(uint8_t u8MsgType, void *pvMsg)
{
#ifdef DEBUG_MODE
  printf("(wifi_cb) --- MSG %d \r\n", u8MsgType);
#endif

  switch (u8MsgType)
  {

  case M2M_WIFI_RESP_CON_STATE_CHANGED:
  {
    tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;
    if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED)
    {
      gconnected = CONNECTED;
#ifdef DEBUG_MODE
      printf("Wi-Fi connected\r\n");
#endif
    }
    else
    {
      gconnected = NOT_CONNECTED;
#ifdef DEBUG_MODE
      printf("Wi-Fi NOT connected\r\n");
#endif
    }
    break;
  }

  case M2M_WIFI_REQ_DHCP_CONF:
  {
    uint8_t *pu8IPAddress = (uint8_t *)pvMsg;
#ifdef DEBUG_MODE
    printf("Wi-Fi IP is %u.%u.%u.%u\r\n", pu8IPAddress[0],
           pu8IPAddress[1], pu8IPAddress[2], pu8IPAddress[3]);
#endif
    break;
  }
  case M2M_WIFI_RESP_GET_SYS_TIME:
  {
    tstrSystemTime *pstrSystemTime = (tstrSystemTime *)pvMsg;
    set_rtc_time(&hrtc, pstrSystemTime);
#ifdef DEBUG_MODE
    print_time(&hrtc);
#endif
    break;
  }
  }
} /* WIFI_CB  END */

/**
 * @brief Callback to get the Data from socket.
 *
 * @param[in] sock socket handler.
 * @param[in] u8Msg socket event type. Possible values are:
 *  - SOCKET_MSG_BIND
 *  - SOCKET_MSG_LISTEN
 *  - SOCKET_MSG_ACCEPT
 *  - SOCKET_MSG_CONNECT
 *  - SOCKET_MSG_RECV
 *  - SOCKET_MSG_SEND
 *  - SOCKET_MSG_SENDTO
 *  - SOCKET_MSG_RECVFROM
 * @param[in] pvMsg is a pointer to message structure. Existing types are:
 *  - tstrSocketBindMsg
 *  - tstrSocketListenMsg
 *  - tstrSocketAcceptMsg
 *  - tstrSocketConnectMsg
 *  - tstrSocketRecvMsg
 */
void socket_cb(SOCKET sock, uint8_t u8Msg, void *pvMsg)
{
#ifdef DEBUG_MODE
  printf("\nsocket_cb:\r\n");
#endif
  switch (u8Msg)
  {
  /* Socket connected */
  case SOCKET_MSG_CONNECT:
  {
    tstrSocketConnectMsg *pstrConnect = (tstrSocketConnectMsg *)pvMsg;
    if (pstrConnect && pstrConnect->s8Error >= 0)
    {
#ifdef DEBUG_MODE
      printf("socket_cb: connect success!\r\n");
#endif
      send(tcp_client_socket, &msg_wifi_product, sizeof(t_msg_wifi_product), 0);
    }
    else
    {
#ifdef DEBUG_MODE
      printf("socket_cb: connect error!\r\n");
#endif
      close(tcp_client_socket);
      tcp_client_socket = -1;
    }
  }
  break;

  /* Message send */
  case SOCKET_MSG_SEND:
  {
#ifdef DEBUG_MODE
    printf("socket_cb: send success!\r\n");
#endif
    recv(tcp_client_socket, gau8SocketTestBuffer, sizeof(gau8SocketTestBuffer), 0);
  }
  break;

  /* Message receive */
  case SOCKET_MSG_RECV:
  {
    tstrSocketRecvMsg *pstrRecv = (tstrSocketRecvMsg *)pvMsg;
    if (pstrRecv && pstrRecv->s16BufferSize > 0)
    {
#ifdef DEBUG_MODE
      printf("socket_cb: recv success!\r\n");
      printf(" %c \r\n", pstrRecv->pu8Buffer[0]);
#endif
      char buff[] = "hej";
      send(tcp_client_socket, buff, 3, 5);
      // printf("TCP Client Test Complete!\r\n");
    }
    else
    {
#ifdef DEBUG_MODE
      printf("socket_cb: recv error!\r\n");
#endif
      close(tcp_client_socket);
      tcp_client_socket = -1;
    }
  }

  break;

  default:
    break;
  }
}
