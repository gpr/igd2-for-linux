#ifndef PKI_H_
#define PKI_H_

#include <gnutls/gnutls.h>

/* default file for client certificate storing */
#ifndef UPNP_X509_CLIENT_CERT_FILE
#define UPNP_X509_CLIENT_CERT_FILE      "libupnpX509.pem"
#endif

/* default file for client private key storing */
#ifndef UPNP_X509_CLIENT_PRIVKEY_FILE
#define UPNP_X509_CLIENT_PRIVKEY_FILE      "libupnpX509.pem"
#endif

/* default file for server certificate storing */
#ifndef UPNP_X509_SERVER_CERT_FILE
#define UPNP_X509_SERVER_CERT_FILE      "libupnpX509server.pem"
#endif

/* default file for server private key storing */
#ifndef UPNP_X509_SERVER_PRIVKEY_FILE
#define UPNP_X509_SERVER_PRIVKEY_FILE      "libupnpX509server.pem"
#endif

/* Used X.509 certificate version */
#ifndef UPNP_X509_CERT_VERSION
#define UPNP_X509_CERT_VERSION           3
#endif

/* default bit size of used modulus in created certificate (key size) */
#ifndef UPNP_X509_CERT_MODULUS_SIZE
#define UPNP_X509_CERT_MODULUS_SIZE      2048
#endif

/* how many seconds created certificate should last. Lets use 100 years to make sure that
 * no need for certificate renewal exists
 */
#ifndef UPNP_X509_CERT_LIFETIME   //(100*365*24*60*60)
#define UPNP_X509_CERT_LIFETIME   3153600000UL
#endif

/* This tries to solve Year 2038 problem with "too big" unix timestamps, for which
 * gnutls seems to be vulnerable.
 * http://en.wikipedia.org/wiki/Year_2038_problem 
 * 
 * Remove this definition and UPNP_X509_CERT_LIFETIME value will be used for
 * expiration time calculation.
 */
#ifndef UPNP_X509_CERT_ULTIMATE_EXPIRE_DATE   //Thu Dec 31 23:59:59 UTC 2037
#define UPNP_X509_CERT_ULTIMATE_EXPIRE_DATE   2145916799
#endif 

/************************************************************************
*   Function :  init_crypto_libraries
*
*   Description :   Initialize libgcrypt for gnutls. Not sure should this rather 
*        be done in final program using this UPnP library?
*        Makes gcrypt thread save, and disables usage of blocking /dev/random.
*        Initialize also gnutls.
*
*   Return : int ;
*       0 on succes, gnutls error else
*
*   Note : assumes that libupnp uses pthreads.
************************************************************************/
int init_crypto_libraries(); 

/************************************************************************
*   Function :  clientCertCallback
*
*   Description :   Callback function which is called by gnutls when 
*         server asks client certificate at the tls handshake.
*         Function sets certificate and private key used by client for 
*         response.
*
*   Return : int
*
*   Note : Don't call this.
************************************************************************/
int clientCertCallback(gnutls_session_t session, const gnutls_datum_t* req_ca_dn, int nreqs, gnutls_pk_algorithm_t* pk_algos, int pk_algos_length, gnutls_retr_st* st);


/************************************************************************
*   Function :  init_x509_certificate_credentials
*
*   Parameters :
*       OUT gnutls_certificate_credentials_t *x509_cred     ;  Pointer to gnutls_certificate_credentials_t where certificate credentials are inserted
*       IN const char *directory       ;  Path to directory where files locate or where files are created
*       IN const char *CertFile        ;  Selfsigned certificate file of client
*       IN const char *PrivKeyFile     ;  Private key file of client.
*       IN const char *TrustFile       ;  File containing trusted certificates. (PEM format)
*       IN const char *CRLFile         ;  Certificate revocation list. Untrusted certificates. (PEM format)
*
*   Description :   Init gnutls_certificate_credentials_t structure for use with 
*       input from given parameter files. All files may be NULL
*
*   Return : int ;
*       UPNP or gnutls error code.
*
*   Note :
************************************************************************/
int init_x509_certificate_credentials(gnutls_certificate_credentials_t *x509_cred, const char *directory, const char *CertFile, const char *PrivKeyFile, const char *TrustFile, const char *CRLFile);


/************************************************************************
*   Function :  load_x509_self_signed_certificate
*
*   Parameters :
*       OUT gnutls_x509_crt_t *crt     ;  Pointer to gnutls_x509_crt_t where certificate is created
*       OUT gnutls_x509_privkey_t *key ;  Pointer to gnutls_x509_privkey_t where private key is created
*       IN const char *directory       ;  Path to directory where files locate or where files are created
*       IN const char *certfile        ;  Name of file where certificate is exported in PEM format
*       IN const char *privkeyfile     ;  Name of file where private key is exported in PEM format
*       IN char *CN                    ;  Common Name velue in certificate
*       IN int modulusBits             ;  Size of modulus in certificate
*       IN unsigned long lifetime      ;  How many seconds until certificate will expire. Counted from now.
* 
*   Description :   Create self signed certificate. For this private key is also created.
*           If certfile already contains certificate and privkeyfile contains privatekey,
*           function uses that certificate. If only other is defined, then both will be created.
*
*   Return : int ;
*       UPNP or gnutls error code.
*
*   Note :
************************************************************************/
int load_x509_self_signed_certificate(gnutls_x509_crt_t *crt, gnutls_x509_privkey_t *key, const char *directory, const char *certfile, const char *privkeyfile, const char *CN, const int modulusBits, const unsigned long lifetime);


/************************************************************************
*   Function :  validate_x509_certificate
*
*   Parameters :
*       IN const gnutls_x509_crt_t *crt  ;  Pointer to certificate which is validated
*       IN const char *hostname          ;  Hostname to compare with certificates subject
*       IN const char *commonname        ;  CN value which is compared with subject CN value of certificate 
* 
*   Description :   Check that given certificate is activated (not before > now), certificate 
*       has not expired (not after < now). If hostname or commonname are defined check that
*       those values match values found from certificate. Hostname check is "a basic implementation 
*       of the matching described in RFC2818 (HTTPS), which takes into account wildcards, and the 
*       DNSName/IPAddress subject alternative name PKIX extension." (gnutls)
*       Commonname check just checks if commonname value equals CN found from certificates subject.
*
*   Return : int ;
*       UPNP or gnutls error code.
*
*   Note :
************************************************************************/
int validate_x509_certificate(const gnutls_x509_crt_t *crt, const char *hostname, const char *commonname);

#endif /*PKI_H_*/
