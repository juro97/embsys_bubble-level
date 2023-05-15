/*
 * fake_clk_wifi.h
 *
 *  Created on: May 15, 2023
 *      Author: juro
 */

#ifndef INC_FAKE_CLK_WIFI_H_
#define INC_FAKE_CLK_WIFI_H_


#include <stdbool.h>
#include <stdlib.h>

#define WIFIESP_CHECK						"AT"
#define WIFIESP_RST							"AT+RST"
#define WIFIESP_CHECK_FIRMWARE				"AT+GMR"
#define WIFIESP_SET_MODE					"AT+CWMODE"
#define WIFIESP_CONNECT						"AT+CWJAP"
#define WIFIESP_IS_CONNECTED				"AT+CWSTATE"
#define WIFIESP_LIST_AP						"AT+CWLAP"
#define WIFIESP_QUIT_AP						"AT+CWQAP"
#define WIFIESP_SET_AP_PARAMETERS			"AT+CWSAP"
#define WIFIESP_JOIN_DEV_IP					"AT+CWLIF"
#define WIFIESP_CONNESTION_STATUS			"AT+CIPSTATUS"
#define WIFIESP_START						"AT+CIPSTART"
#define WIFIESP_MODE						"AT+CIPMODE"
#define WIFIESP_SEND						"AT+CIPSEND"
#define WIFIESP_CLOSE						"AT+CIPCLOSE"
#define WIFIESP_GET_IP						"AT+CIFSR"
#define WIFIESP_SET_MULTIPLE_CONNECTION		"AT+CIPMUX"
#define WIFIESP_CHECK_SERVER_TIMEOUT      	"AT+CIPSTO?"
#define WIFIESP_RECEIVE                     "+IPD"


/* initialize random number generator */
void fakeWifi_init(void);

/* returns a random pattern to print on led matrix */
unsigned char fakeWifi_readCodePattern(void);

/* fakes the sending of an at cmd and saves the answer into response array */
bool fakeWifi_send(const char *command, char *response, uint8_t response_size);



#endif /* INC_FAKE_CLK_WIFI_H_ */
