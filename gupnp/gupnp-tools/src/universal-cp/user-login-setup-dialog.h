/*
 * user-login-setup-dialog.h
 *
 *  Created on: Jun 4, 2009
 *      Author: vlillvis
 */

#ifndef USERLOGINSETUPDIALOG_H_
#define USERLOGINSETUPDIALOG_H_

void
init_user_login_dialog_fields (void);

void
init_user_login_setup_dialog (GladeXML *glade_xml);

void
deinit_user_login_setup_dialog (void);

void
continue_login_cb (GUPnPDeviceProxy       *proxy,
                   GUPnPDeviceProxyLogin  *logindata,
                   GError                **error,
                   gpointer                user_data);

void
continue_logout_cb (GUPnPDeviceProxy        *proxy,
					GUPnPDeviceProxyLogout  *logoutdata,
                    GError                 **error,
                    gpointer                 user_data);

void
get_current_username(GString *current_user);


#endif /* USERLOGINSETUPDIALOG_H_ */
