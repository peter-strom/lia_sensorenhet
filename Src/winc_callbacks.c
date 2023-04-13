#include "winc_callbacks.h"
#include "eeprom.h"
#include <stdbool.h>

static void set_rtc_time(tstrSystemTime *pstrSystemTime);
static uint8_t calc_weekday(uint16_t year, uint8_t month, uint8_t day);
static bool is_DST(uint8_t month, uint8_t day, uint8_t weekDay, uint8_t hour);
static bool is_leap_year(uint16_t year);
static bool adjus_time(tstrSystemTime *sysTime, int adjHours);

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
 * @brief Set the rtc time from the wifi-cips sysTime and adjusts timezone and DTS
 *
 * @param pstrSystemTime
 */
static void set_rtc_time(tstrSystemTime *pstrSystemTime)
{
  if (pstrSystemTime->u16Year > 0)
  {
    adjus_time(pstrSystemTime, eeprom.divided.utcH);
    RTC_TimeTypeDef time = {0};
    RTC_DateTypeDef date = {0};
    time.Hours = pstrSystemTime->u8Hour;
    time.Minutes = pstrSystemTime->u8Minute;
    time.Seconds = pstrSystemTime->u8Second;
    date.Year = (pstrSystemTime->u16Year - 2000);
    date.Month = pstrSystemTime->u8Month;
    date.Date = pstrSystemTime->u8Day;
    date.WeekDay = calc_weekday(pstrSystemTime->u16Year, pstrSystemTime->u8Month, pstrSystemTime->u8Day);

    HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
    HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
    if (is_DST(date.Month, date.Date, date.WeekDay, time.Hours))
    {
      HAL_RTC_DST_Add1Hour(&hrtc);
      printf("daylightsaving\r\n");
    }
  }
}

/**
 * @brief calculates the weekday from a date
 * @details using Michael Keith and Tom Craver's expression
 *          https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week#Keith
 * @param[in] year
 * @param[in] month
 * @param[in] day
 * @return uint8_t (0 = sunday, 1 = monday, ..)
 */
static uint8_t calc_weekday(uint16_t year, uint8_t month, uint8_t day)
{
  return (day += month < 3 ? year-- : year - 2, 23 * month / 9 + day + 4 + year / 4 - year / 100 + year / 400) % 7;
}

/**
 * @brief function to check if current date is daylight saving time.
 *
 * @param month
 * @param day
 * @param weekDay 0=sunday, 1=monday, ..
 * @param hour
 * @return true if daylight saving
 */
static bool is_DST(uint8_t month, uint8_t day, uint8_t weekDay, uint8_t hour)
{
  if (month > 3 && month < 10)
  {
    return true;
  }

  // after last Sunday 2:00
  if (day - weekDay >= 25 && (weekDay > 0 || hour >= 2))
  {
    if (month == 3)
    {
      return true;
    }
  }
  else // before last Sunday 2:00
  {
    if (month == 10)
    {
      return true;
    }
  }

  return false;
}

/**
 * @brief calculates if the year is a leap year or not
 *
 * @param[in] year the year to check
 * @return true if leap year
 */
static bool is_leap_year(uint16_t year)
{
  if (year % 400 == 0)
  {
    return true;
  }
  else if (year % 100 == 0)
  {
    return false;
  }
  else if (year % 4 == 0)
  {
    return true;
  }
  return false;
}

/**
 * @brief takes the system time from the wifi-chip and adjust it by maximum +-24h
 *
 * @param[out] sysTime time struct from the wifichip
 * @param[in] adjHours amount of time in hours to adjust
 * @return true if time is adjusted
 */
static bool adjus_time(tstrSystemTime *sysTime, int adjHours)
{
  uint8_t daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (is_leap_year(sysTime->u16Year))
  {
    daysInMonth[1] = 29;
  }

  if (adjHours == 0 || adjHours > 24 || adjHours < -24)
  {
    return false;
  }

  if (adjHours > 0)
  {
    sysTime->u8Hour += adjHours;
    if (sysTime->u8Hour >= 24)
    {
      sysTime->u8Hour = sysTime->u8Hour - 24;

      if (sysTime->u8Day == daysInMonth[sysTime->u8Month - 1])
      {
        sysTime->u8Day = 0;
        if (sysTime->u8Month == 12)
        {
          sysTime->u8Month = 0;
          sysTime->u16Year++;
        }
        sysTime->u8Month++;
      }
      sysTime->u8Day++;
    }
  }
  else
  {
    sysTime->u8Hour += adjHours;
    if (sysTime->u8Hour > 0)
    {
      sysTime->u8Hour = sysTime->u8Hour + 24;
      if (sysTime->u8Day == 1)
      {
        if (sysTime->u8Month == 1)
        {
          sysTime->u8Month = 12;
          sysTime->u16Year--;
        }
        else
        {
          sysTime->u8Month--;
        }
        sysTime->u8Day = daysInMonth[sysTime->u8Month - 1] + 1;
      }
      sysTime->u8Day--;
    }
  }

  return true;
}
