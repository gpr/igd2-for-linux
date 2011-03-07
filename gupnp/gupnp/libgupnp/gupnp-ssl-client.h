/**
 * This file is part of Nokia DeviceProtection v1 reference implementation
 * Copyright © 2010 Nokia Corporation and/or its subsidiary(-ies).
 * Contact:mika.saaranen@nokia.com
 * Developer(s): jaakko.pasanen@tieto.com, opensource@tieto.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 2.1 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program. If not, see http://www.gnu.org/licenses/.
 */

#ifndef SSLCLIENT_H_
#define SSLCLIENT_H_

#include <gnutls/gnutls.h>
#include <gnutls/x509.h>

#include <glib-object.h>

G_BEGIN_DECLS

#define GUPNP_SSL_PORT     443
#define GUPNP_CERT_STORE  ".gupnp" // this is concatenated with users home dir, currently accepted directory depth is 1
#define GUPNP_CERT_CN      "GUPNP Client"

/*
GType
gupnp_ssl_client_get_type (void) G_GNUC_CONST;

#define GUPNP_TYPE_SSL_CLIENT \
                (gupnp_ssl_client_get_type ())
#define GUPNP_SSL_CLIENT(obj) \
                (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                 GUPNP_TYPE_SSL_CLIENT, \
                 GUPnPSSLClient))
#define GUPNP_SSL_CLIENT_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_CAST ((obj), \
                 GUPNP_TYPE_SSL_CLIENT, \
                 GUPnPSSLClientClass))
#define GUPNP_IS_SSL_CLIENT(obj) \
                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                 GUPNP_TYPE_SSL_CLIENT))
#define GUPNP_IS_SSL_CLIENT_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_TYPE ((obj), \
                 GUPNP_TYPE_SSL_CLIENT))
#define GUPNP_SSL_CLIENT_GET_CLASS(obj) \
                (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                 GUPNP_TYPE_SSL_CLIENT, \
                 GUPnPSSLClientClass))

typedef struct {
        GUPnPContextClass parent_class;

        // future padding 
        void (* _gupnp_reserved1) (void);
        void (* _gupnp_reserved2) (void);
        void (* _gupnp_reserved3) (void);
        void (* _gupnp_reserved4) (void);
} GUPnPSSLClientClass;
*/

typedef struct {   
    // creadentials for ssl clients
    gnutls_certificate_credentials_t xcred;

    // ssl session
    gnutls_session_t session;

    GThreadPool *thread_pool;

} GUPnPSSLClient;


typedef struct _GUPnPSSLThreadData GUPnPSSLThreadData;
// callback funciotn which is called when action returns
typedef void (* GUPnPSSLClientCallback) (
                                     GUPnPSSLClient          **client,
                                     SoupMessage             *msg,
                                     gpointer                user_data //GUPnPServiceProxyAction
                                     );

void ssl_create_https_url(const char *http_url, int port, char **https_url);


int
ssl_init_client(  GUPnPSSLClient **client,
                  const char *directory,
                  const char *CertFile,
                  const char *PrivKeyFile,
                  const char *TrustFile,
                  const char *CRLFile,
                  const char *devName);
                 
int
ssl_finish_client( GUPnPSSLClient **client );

int
ssl_create_client_session(  GUPnPSSLClient **client,
                            const char *ActionURL_const,
                            void *SSLSessionData,
                            size_t *DataSize);
                            
int
ssl_close_client_session( GUPnPSSLClient **client );                                             


int ssl_client_send_and_receive(  GUPnPSSLClient **client,
                                  char *message,
                                  SoupMessage *msg,
                                  GUPnPSSLClientCallback callback,
                                  gpointer userdata);

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
 * Function: ssl_client_export_cert. From pupnp.
 *
 * Parameters:
 *  unsigned char *data - Certificate is returned in DER format here
 *  int *data_size - Pointer to integer which represents length of certificate
 *
 * Description:
 *  Get X.509 certificate that HTTPS server uses in DER format.
 *
 * Return: int
 *      0 on success, gnutls error else. 
 ************************************************************************/
int ssl_client_export_cert (unsigned char *data, int *data_size);

G_END_DECLS

#endif /*SSLCLIENT_H_*/
