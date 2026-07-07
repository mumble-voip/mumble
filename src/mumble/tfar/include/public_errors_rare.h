#ifndef PUBLIC_ERRORS__RARE_H
#define PUBLIC_ERRORS__RARE_H

//The idea here is: the values are 2 bytes wide, the first byte identifies the group, the second the count within that group

//client
const unsigned int ERROR_client_invalid_password                = 0x0208;
const unsigned int ERROR_client_too_many_clones_connected       = 0x0209;
const unsigned int ERROR_client_is_online                       = 0x020b;

//channel
const unsigned int ERROR_channel_is_private_channel             = 0x030e;
//note 0x030f is defined in public_errors;

//database
const unsigned int ERROR_database                               = 0x0500;
const unsigned int ERROR_database_empty_result                  = 0x0501;
const unsigned int ERROR_database_duplicate_entry               = 0x0502;
const unsigned int ERROR_database_no_modifications              = 0x0503;
const unsigned int ERROR_database_constraint                    = 0x0504;
const unsigned int ERROR_database_reinvoke                      = 0x0505;

//permissions
const unsigned int ERROR_permission_invalid_group_id            = 0x0a00;
const unsigned int ERROR_permission_duplicate_entry             = 0x0a01;
const unsigned int ERROR_permission_invalid_perm_id             = 0x0a02;
const unsigned int ERROR_permission_empty_result                = 0x0a03;
const unsigned int ERROR_permission_default_group_forbidden     = 0x0a04;
const unsigned int ERROR_permission_invalid_size                = 0x0a05;
const unsigned int ERROR_permission_invalid_value               = 0x0a06;
const unsigned int ERROR_permissions_group_not_empty            = 0x0a07;
const unsigned int ERROR_permissions_insufficient_group_power   = 0x0a09;
const unsigned int ERROR_permissions_insufficient_permission_power = 0x0a0a;
const unsigned int ERROR_permission_template_group_is_used      = 0x0a0b;
//0x0a0c is in public_errors.h

//server
const unsigned int ERROR_server_deployment_active               = 0x0405;
const unsigned int ERROR_server_unable_to_stop_own_server       = 0x0406;
const unsigned int ERROR_server_wrong_machineid                 = 0x0408;
const unsigned int ERROR_server_modal_quit                      = 0x040c;

//messages
const unsigned int ERROR_message_invalid_id                     = 0x0c00;

//ban
const unsigned int ERROR_ban_invalid_id                         = 0x0d00;
const unsigned int ERROR_connect_failed_banned                  = 0x0d01;
const unsigned int ERROR_rename_failed_banned                   = 0x0d02;
const unsigned int ERROR_ban_flooding                           = 0x0d03;

//tts
const unsigned int ERROR_tts_unable_to_initialize               = 0x0e00;

//privilege key
const unsigned int ERROR_privilege_key_invalid                  = 0x0f00;

//voip
const unsigned int ERROR_voip_pjsua                             = 0x1000;
const unsigned int ERROR_voip_already_initialized               = 0x1001;
const unsigned int ERROR_voip_too_many_accounts                 = 0x1002;
const unsigned int ERROR_voip_invalid_account                   = 0x1003;
const unsigned int ERROR_voip_internal_error                    = 0x1004;
const unsigned int ERROR_voip_invalid_connectionId              = 0x1005;
const unsigned int ERROR_voip_cannot_answer_initiated_call      = 0x1006;
const unsigned int ERROR_voip_not_initialized                   = 0x1007;

#endif
