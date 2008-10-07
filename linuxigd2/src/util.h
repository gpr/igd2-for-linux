#ifndef _UTIL_H_
#define _UTIL_H_

#include <upnp/upnp.h>

int get_sockfd(void);
int GetIpAddressStr(char *address, char *ifname);
int ControlPointIP_equals_InternalClientIP(char *ICAddress);
int checkForWildCard(const char *str);
void addErrorData(struct Upnp_Action_Request *ca_event, int errorCode, char* message);
void trace(int debuglevel, const char *format, ...);

int resolveBoolean(char *);

#endif //_UTIL_H_
