#include "winc.h"

void winc_init(void)
{
  tstrWifiInitParam initParams;
  m2m_memset((uint8 *)&initParams, 0, sizeof(tstrWifiInitParam));
  initParams.pfAppWifiCb = wifi_cb;
  int8_t ret = m2m_wifi_init(&initParams);
  if (M2M_SUCCESS != ret)
  {
    M2M_ERR("Driver Init Failed <%d>\r\n", ret);
    while (1)
      ;
  }
  gconnected = NOT_CONNECTED;
}

void socket_init(struct sockaddr_in *pSockAddr)
{
  struct sockaddr_in addr;
  (*pSockAddr).sin_family = AF_INET;
  (*pSockAddr).sin_port = _htons(MAIN_WIFI_M2M_SERVER_PORT);
  (*pSockAddr).sin_addr.s_addr = _htonl(MAIN_WIFI_M2M_SERVER_IP);
  socketInit();
  registerSocketCallback(socket_cb, NULL);
}
void connect_to_ap(void)
{
  if (m2m_wifi_connect((char *)MAIN_WLAN_SSID, strlen(MAIN_WLAN_SSID), MAIN_WLAN_AUTH, (char *)MAIN_WLAN_PASS, M2M_WIFI_CH_ALL) == M2M_SUCCESS)
  {
    printf("\nWi-Fi trying to connect \r\n");
    gconnected = PENDING_CONNECT;
  }
  else
  {
    printf("doh! \r\n");
  }
}

void connect_to_server(struct sockaddr_in *pSockAddr)
{
  /* Open client socket. */
  if (tcp_client_socket < 0)
  {
    if ((tcp_client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      printf("main: failed to create TCP client socket error!\r\n");
    }
    else if ((connect(tcp_client_socket, (struct sockaddr *)pSockAddr, sizeof(struct sockaddr_in))) < 0)
    {
      close(tcp_client_socket);
      tcp_client_socket = -1;
      m2m_wifi_get_system_time();
    }
  }
}

void get_mac(uint8_t *pu8mac, tenuMacOpt opt)
{
  if (opt == OPT_TRUE)
  {
    uint8 valid;
    m2m_wifi_get_otp_mac_address(pu8mac, &valid);
  }
  else
  {
    m2m_wifi_get_mac_address(pu8mac);
  }
}

void print(uint8_t *pu8mac)
{
  printf("%02x:%02x:%02x:%02x:%02x:%02x \r\n",
         pu8mac[0], pu8mac[1], pu8mac[2], pu8mac[3], pu8mac[4], pu8mac[5]);
}

void print_mac(void)
{
  uint8_t mac[6];
  get_mac(&mac, OPT_TRUE);
  print(&mac);
  get_mac(&mac, OPT_FALSE);
  print(&mac);
}

void send_socket_message(char *msg, uint16_t msgSize)
{
  send(tcp_client_socket, msg, msgSize, 0);
}