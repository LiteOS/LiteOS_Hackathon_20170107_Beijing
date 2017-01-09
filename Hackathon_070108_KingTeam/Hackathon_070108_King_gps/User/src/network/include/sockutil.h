/*
*
@file		sockutil.h
@brief	Implementation of useful function of iinChip
*
*/

#ifndef __SOCKUTIL_H
#define __SOCKUTIL_H


#define NO_USE_SOCKUTIL_FUNC

char* inet_ntoa(unsigned long addr);			/* Convert 32bit Address into Dotted Decimal Format */
char* inet_ntoa_pad(unsigned long addr);

//unsigned long inet_addr(unsigned char* addr);		/* Converts a string containing an (Ipv4) Internet Protocol decimal dotted address into a 32bit address */

char VerifyIPAddress_orig(char* src);			/* Verify dotted notation IP address string */
char VerifyIPAddress(char* src, uint8 * ip);

unsigned long GetDestAddr(SOCKET s);			/* Output destination IP address of appropriate channel */

unsigned int GetDestPort(SOCKET s);			/* Output destination port number of appropriate channel */

unsigned short htons( unsigned short hostshort);	/* htons function converts a unsigned short from host to TCP/IP network byte order (which is big-endian).*/

unsigned long htonl(unsigned long hostlong);		/* htonl function converts a unsigned long from host to TCP/IP network byte order (which is big-endian). */

unsigned long ntohs(unsigned short netshort);		/* ntohs function converts a unsigned short from TCP/IP network byte order to host byte order (which is little-endian on Intel processors). */

unsigned long ntohl(unsigned long netlong);		/* ntohl function converts a u_long from TCP/IP network order to host byte order (which is little-endian on Intel processors). */

u_char CheckDestInLocal(u_long destip);			/* Check Destination in local or remote */

SOCKET getSocket(unsigned char status, SOCKET start); 	/* Get handle of socket which status is same to 'status' */
unsigned short checksum(unsigned char * src, unsigned int len);		/* Calculate checksum of a stream */

#ifndef NO_USE_SOCKUTIL_FUNC

u_long GetIPAddress(void);					/* Get Source IP Address of iinChip. */

u_long GetGWAddress(void);					/* Get Source IP Address of iinChip. */

u_long GetSubMask(void);					/* Get Source Subnet mask of iinChip. */

void GetMacAddress(unsigned char* mac);		/* Get Mac address of iinChip. */

void GetDestMacAddr(SOCKET s, u_char* mac);

void GetNetConfig(void);				/* Read established network information(G/W, IP, S/N, Mac) of iinChip and Output that through Serial.*/

void dump_iinchip(void);					/* dump the 4 channel status of iinChip */

#endif

#endif
