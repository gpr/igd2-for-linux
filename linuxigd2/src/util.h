/** 
 * This file is part of Nokia InternetGatewayDevice v2 reference implementation
 * Copyright © 2009 Nokia Corporation and/or its subsidiary(-ies).
 * Contact: mika.saaranen@nokia.com
 *  
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 2 of the License, or 
 * (at your option) any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see http://www.gnu.org/licenses/. 
 * 
 */
 
#ifndef _UTIL_H_
#define _UTIL_H_

#include <upnp/upnp.h>

int get_sockfd(void);
char *toUpperCase(const char * str);
int GetIpAddressStr(char *address, char *ifname);
int GetMACAddressStr(unsigned char *address, int addressSize, char *ifname);
int GetConnectionStatus(char *conStatus, char *ifname);
int ControlPointIP_equals_InternalClientIP(char *ICAddress, struct in_addr *);
int checkForWildCard(const char *str);
void addErrorData(struct Upnp_Action_Request *ca_event, int errorCode, char* message);
void trace(int debuglevel, const char *format, ...);
int setEthernetLinkStatus(char *ethLinStatus, char *iface);
int resolveBoolean(char *);
int releaseIP(char *iface);
int killDHCPClient(char *iface);
int startDHCPClient(char *iface);
int readIntFromFile(char *file);

char* GetFirstDocumentItem( IN IXML_Document * doc, const char *item );
char* GetDocumentItem(IXML_Document * doc, const char *item, int index);
int writeDocumentToFile(IXML_Document *doc, const char *file);

void ParseXMLResponse(struct Upnp_Action_Request *ca_event, const char *result);


// access level handling and parsing stuff
int initActionAccessLevels(const char *pathToFile);
void deinitActionAccessLevels();
char* getAccessLevel(const char *actionName, int manage);

// ACL handling stuff
char *ACL_getRolesOfUser(IXML_Document *doc, const char *username);
char *ACL_getRolesOfCP(IXML_Document *doc, const char *hash);
char *ACL_createRoleListXML(const char *csv_roles);
int ACL_addCP(IXML_Document *doc, const char *name, const char *alias, const char *hash, int introduced);
int ACL_addUser(IXML_Document *doc, const char *name);
int ACL_removeUser(IXML_Document *doc, const char *name);
int ACL_removeCP(IXML_Document *doc, const char *hash);

#endif //_UTIL_H_
