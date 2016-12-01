/* ESP8266.h
 * Trevor Eggenberger - tae498
 * Joshua Dong - jid295
 */

#ifndef __ESP8266__
#define __ESP8266__

void ESP8266_Init(void);
int ESP8266_Reset();
int ESP8266_SetWifiMode(uint8_t mode);
int ESP8266_JoinAccessPoint(const char* ssid, const char* password);
int ESP8266_ConfigureAccessPoint(const char* ssid, const char* password, 
									uint8_t channel, uint8_t encryptMode);
int ESP8266_GetIPAddress(void);
int ESP8266_MakeTCPConnection(char *IPaddress);
int ESP8266_SendTCP(char* fetch);
int ESP8266_CloseTCPConnection(void);

#endif /* __ESP8266__ */
