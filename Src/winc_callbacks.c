#include "winc_callbacks.h"

static void set_rtc_time(tstrSystemTime *pstrSystemTime);
static int8_t get_GMT_from_settings(void);

/*  WIFI STUFF*/
/** Index of scan list to request scan result. */
uint8_t scan_request_index = 0;
/** Number of APs found. */
uint8_t num_founded_ap = 0;

tenuConState gconnected;
uint8_t gscancomplete;

/* SOCKET STUFF */
/** Socket for client */
SOCKET tcp_client_socket = -1;
/** Receive buffer definition. */
static uint8_t gau8SocketTestBuffer[MAIN_WIFI_M2M_BUFFER_SIZE];

/** Message format definitions. */
typedef struct s_msg_wifi_product
{
  uint8_t name[9];
} t_msg_wifi_product;

/** Message format declarations. */
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

  static uint8 u8ScanResultIdx = 0;
  printf("(wifi_cb) --- MSG %d \r\n", u8MsgType);

  switch (u8MsgType)
  {

  case M2M_WIFI_RESP_SCAN_DONE:
  {
    tstrM2mScanDone *pstrInfo = (tstrM2mScanDone *)pvMsg;

    printf("Num of AP found %d \r\n", pstrInfo->u8NumofCh);
    if (pstrInfo->s8ScanState == M2M_SUCCESS)
    {
      u8ScanResultIdx = 0;
      if (pstrInfo->u8NumofCh >= 1)
      {
        m2m_wifi_req_scan_result(u8ScanResultIdx);
        u8ScanResultIdx++;
      }
      else
      {
        printf("No AP Found Rescan \r\n");
        m2m_wifi_request_scan(M2M_WIFI_CH_ALL);
      }
    }
    else
    {
      printf("(ERR) Scan fail with error <%d> \r\n", pstrInfo->s8ScanState);
    }
    break;
  }
  case M2M_WIFI_RESP_SCAN_RESULT:
  {
    tstrM2mWifiscanResult *pstrScanResult = (tstrM2mWifiscanResult *)pvMsg;
    uint8 u8NumFoundAPs = m2m_wifi_get_num_ap_found();

    printf(">>%02d RI %d SEC %d CH %02d BSSID %02X:%02X:%02X:%02X:%02X:%02X SSID %s \r\n",
           pstrScanResult->u8index,
           pstrScanResult->s8rssi,
           pstrScanResult->u8AuthType,
           pstrScanResult->u8ch,
           pstrScanResult->au8BSSID[0], pstrScanResult->au8BSSID[1], pstrScanResult->au8BSSID[2],
           pstrScanResult->au8BSSID[3], pstrScanResult->au8BSSID[4], pstrScanResult->au8BSSID[5],
           pstrScanResult->au8SSID);

    if (u8ScanResultIdx < u8NumFoundAPs)
    {
      // Read the next scan result
      m2m_wifi_req_scan_result(u8ScanResultIdx);
      u8ScanResultIdx++;
    }
    else
    {
      gscancomplete = 1;
    }
    break;
  }

  case M2M_WIFI_RESP_CON_STATE_CHANGED:
  {
    tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;
    if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED)
    {
      printf("Wi-Fi connected\r\n");
      gconnected = CONNECTED;
    }
    else
    {
      gconnected = NOT_CONNECTED;
      printf("Wi-Fi NOT connected\r\n");
    }
    break;
  }
  case M2M_WIFI_REQ_DHCP_CONF:
  {
    uint8_t *pu8IPAddress = (uint8_t *)pvMsg;
    printf("Wi-Fi IP is %u.%u.%u.%u\r\n", pu8IPAddress[0],
           pu8IPAddress[1], pu8IPAddress[2], pu8IPAddress[3]);
    break;
  }
  case M2M_WIFI_RESP_GET_SYS_TIME:
  {
    tstrSystemTime *pstrSystemTime = (tstrSystemTime *)pvMsg;
    set_rtc_time(pstrSystemTime);
    printf("AP-time : %04d-%02d-%02d %02d:%02d:%02d \r\n", pstrSystemTime->u16Year,
           pstrSystemTime->u8Month,
           pstrSystemTime->u8Day,
           pstrSystemTime->u8Hour,
           pstrSystemTime->u8Minute,
           pstrSystemTime->u8Second);
    break;
  }
  }
} /* WIFI_CB  END*/

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

  printf("\nsocket_cb:\r\n");
  switch (u8Msg)
  {
  /* Socket connected */
  case SOCKET_MSG_CONNECT:
  {
    tstrSocketConnectMsg *pstrConnect = (tstrSocketConnectMsg *)pvMsg;
    if (pstrConnect && pstrConnect->s8Error >= 0)
    {
      printf("socket_cb: connect success!\r\n");
      send(tcp_client_socket, &msg_wifi_product, sizeof(t_msg_wifi_product), 0);
    }
    else
    {
      printf("socket_cb: connect error!\r\n");
      close(tcp_client_socket);
      tcp_client_socket = -1;
    }
  }
  break;

  /* Message send */
  case SOCKET_MSG_SEND:
  {
    printf("socket_cb: send success!\r\n");
    recv(tcp_client_socket, gau8SocketTestBuffer, sizeof(gau8SocketTestBuffer), 0);
  }
  break;

  /* Message receive */
  case SOCKET_MSG_RECV:
  {
    tstrSocketRecvMsg *pstrRecv = (tstrSocketRecvMsg *)pvMsg;
    if (pstrRecv && pstrRecv->s16BufferSize > 0)
    {
      printf("socket_cb: recv success!\r\n");
      printf(" %c \r\n", pstrRecv->pu8Buffer[0]);
      char buff[] = "hej";
      send(tcp_client_socket, buff, 3, 5);
      // printf("TCP Client Test Complete!\r\n");
    }
    else
    {
      printf("socket_cb: recv error!\r\n");
      close(tcp_client_socket);
      tcp_client_socket = -1;
    }
  }

  break;

  default:
    break;
  }
}

/**
 * @brief @todo Get the GMT from eeprom-settings
 *
 * @return int8_t
 */
static int8_t get_GMT_from_settings(void)
{
  return 1;
}

static void set_rtc_time(tstrSystemTime *pstrSystemTime)
{
  if (pstrSystemTime->u16Year > 0)
  {
    int8_t gmt = get_GMT_from_settings();
    RTC_TimeTypeDef time = {0};
    RTC_DateTypeDef date = {0};

    time.Hours = pstrSystemTime->u8Hour + gmt;
    time.Minutes = pstrSystemTime->u8Minute;
    time.Seconds = pstrSystemTime->u8Second;
    date.Year = (pstrSystemTime->u16Year - 2000);
    date.Month = pstrSystemTime->u8Month;
    date.Date = pstrSystemTime->u8Day;

    HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
    HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
  }
}