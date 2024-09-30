// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SETTINGSKEYS_H_
#define MUMBLE_MUMBLE_SETTINGSKEYS_H_

#include <initializer_list>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

class SettingsKey {
public:
	SettingsKey(std::initializer_list< std::string > keyIDs);

	operator const char *() const;
	operator nlohmann::json::object_t::key_type() const;

	nlohmann::json selectFrom(const nlohmann::json &json) const;

protected:
	std::vector< std::string > m_ids;
};

namespace SettingsKeys {

/*
 * The idea of these instances is to act as constants for keys that are used in Mumble's settings. Each SettingsKey is
 * initialized by an array of keys. This is meant to facilitate renaming of keys in the settings. The first name is
 * always the most recent (the one used for saving new data) and the other ones are tested for consecutively, when
 * loading settings.
 */

// Audio settings
const SettingsKey UNMUTE_ON_UNDEAF_KEY                        = { "unmute_on_undeaf" };
const SettingsKey MUTE_KEY                                    = { "mute" };
const SettingsKey DEAF_KEY                                    = { "deaf" };
const SettingsKey TRANSMIT_MODE_KEY                           = { "transmit_mode" };
const SettingsKey DOUBLE_PUSH_DELAY_KEY                       = { "double_push_delay" };
const SettingsKey PTT_HOLD_KEY                                = { "ptt_hold" };
const SettingsKey TRANSMIT_CUE_WHEN_PTT_KEY                   = { "transmit_cue_when_ptt" };
const SettingsKey TRANSMIT_CUE_WHEN_VAD_KEY                   = { "transmit_cue_when_vad" };
const SettingsKey TRANSMIT_CUE_START_KEY                      = { "transmit_cue_start" };
const SettingsKey TRANSMIT_CUE_STOP_KEY                       = { "transmit_cue_stop" };
const SettingsKey PLAY_MUTE_CUE_KEY                           = { "play_mute_cue" };
const SettingsKey MUTE_CUE_KEY                                = { "mute_cue" };
const SettingsKey MUTE_CUE_POPUP_SHOWN                        = { "mute_cue_popup_shown" };
const SettingsKey AUDIO_QUALITY_KEY                           = { "audio_quality" };
const SettingsKey LOUDNESS_KEY                                = { "loudness" };
const SettingsKey VOLUME_KEY                                  = { "volume" };
const SettingsKey EXTERNAL_APPLICATIONS_VOLUME_KEY            = { "external_applications_volume" };
const SettingsKey ATTENUATE_EXTERNAL_APPLICATIONS_KEY         = { "attenuate_external_applications" };
const SettingsKey ATTENUATE_EXTERNAL_APPLICATIONS_ON_TALK_KEY = { "attenuate_external_applications_on_talk" };
const SettingsKey ATTENUATE_USERS_ON_PRIORITY_SPEAKER_KEY     = { "attenuate_users_on_priority_speaker" };
const SettingsKey ATTENUATE_ONLY_SAME_OUTPUT_KEY              = { "attenuate_only_same_output" };
const SettingsKey ATTENUATE_LOOPBACK_KEY                      = { "attenuate_loopback" };
const SettingsKey VAD_MODE_KEY                                = { "vad_mode" };
const SettingsKey VAD_MIN_KEY                                 = { "vad_min" };
const SettingsKey VAD_MAX_KEY                                 = { "vad_max" };
const SettingsKey NOISE_CANCEL_MODE_KEY                       = { "noise_cancel_mode" };
const SettingsKey SPEEX_NOISE_CANCEL_STRENGTH_KEY             = { "speex_noise_cancel_strength" };
const SettingsKey INPUT_CHANNEL_MASK_KEY                      = { "input_channel_mask" };
const SettingsKey ALLOW_LOW_DELAY_MODE_KEY                    = { "allow_low_delay_mode" };
const SettingsKey VOICE_HOLD_KEY                              = { "voice_hold" };
const SettingsKey OUTPUT_DELAY_KEY                            = { "output_delay" };
const SettingsKey ECHO_CANCEL_MODE_KEY                        = { "echo_cancel_mode" };
const SettingsKey EXCLUSIVE_INPUT_KEY                         = { "exclusive_input" };
const SettingsKey EXCLUSIVE_OUTPUT_KEY                        = { "exclusive_output" };
const SettingsKey INPUT_SYSTEM_KEY                            = { "input_system" };
const SettingsKey OUTPUT_SYSTEM_KEY                           = { "output_system" };
const SettingsKey NOTIFICATION_VOLUME_KEY                     = { "notification_volume" };
const SettingsKey CUE_VOLUME_KEY                              = { "cue_volume" };
const SettingsKey RESTRICT_WHISPERS_TO_FRIENDS_KEY            = { "restrict_whispers_to_friends" };
const SettingsKey NOTIFICATION_USER_LIMIT_KEY                 = { "notification_user_limit" };

// Idle settings
const SettingsKey IDLE_TIME_KEY                  = { "idle_time" };
const SettingsKey IDLE_ACTION_KEY                = { "idle_action" };
const SettingsKey UNDO_IDLE_ACTION_UPON_ACTIVITY = { "undo_idle_action_upon_activity" };

// Positional audio
const SettingsKey ENABLE_POSITIONAL_AUDIO_KEY      = { "enable_positional_audio" };
const SettingsKey POSITIONAL_HEADPHONE_MODE_KEY    = { "use_headphone_mode" };
const SettingsKey POSITIONAL_MIN_DISTANCE_KEY      = { "minimum_distance" };
const SettingsKey POSITIONAL_MAX_DISTANCE_KEY      = { "maximum_distance" };
const SettingsKey POSITIONAL_MIN_VOLUME_KEY        = { "minimum_volume" };
const SettingsKey POSITIONAL_BLOOM_KEY             = { "bloom" };
const SettingsKey POSITIONAL_TRANSMIT_POSITION_KEY = { "transmit_position" };

// Network
const SettingsKey JITTER_BUFFER_SIZE_KEY            = { "jitter_buffer_size" };
const SettingsKey FRAMES_PER_PACKET_KEY             = { "frames_per_packet" };
const SettingsKey RESTRICT_TO_TCP_KEY               = { "restrict_to_tcp" };
const SettingsKey USE_QUALITY_OF_SERVICE_KEY        = { "use_quality_of_service" };
const SettingsKey AUTO_RECONNECT_KEY                = { "reconnect_automatically" };
const SettingsKey AUTO_CONNECT_LAST_SERVER_KEY      = { "auto_connect_to_last_server" };
const SettingsKey PROXY_TYPE_KEY                    = { "proxy_type" };
const SettingsKey PROXY_HOST_KEY                    = { "proxy_host" };
const SettingsKey PROXY_PORT_KEY                    = { "proxy_port" };
const SettingsKey PROXY_USERNAME_KEY                = { "proxy_username" };
const SettingsKey PROXY_PASSWORD_KEY                = { "proxy_password" };
const SettingsKey MAX_IMAGE_WIDTH_KEY               = { "max_image_width" };
const SettingsKey MAX_IMAGE_HEIGHT_KEY              = { "max_image_height" };
const SettingsKey SERVICE_PREFIX_KEY                = { "service_prefix" };
const SettingsKey MAX_IN_FLIGHT_TCP_PINGS_KEY       = { "max_in_flight_tcp_pings" };
const SettingsKey PING_INTERVAL_KEY                 = { "ping_interval" };
const SettingsKey CONNECTION_TIMEOUT_KEY            = { "connection_timeout" };
const SettingsKey FORCE_UDP_BIND_TO_TCP_ADDRESS_KEY = { "force_udp_bind_to_tcp_address" };
const SettingsKey SSL_CIPHERS_KEY                   = { "ssl_ciphers" };

// ASIO
const SettingsKey ASIO_ENABLE_KEY     = { "enable_asio" };
const SettingsKey ASIO_CLASS_KEY      = { "asio_class" };
const SettingsKey ASIO_MICROPHONE_KEY = { "asio_microphone" };
const SettingsKey ASIO_SPEAKER_KEY    = { "asio_speaker" };

// WASAPI
const SettingsKey WASAPI_INPUT_KEY  = { "wasapi_input" };
const SettingsKey WASAPI_OUTPUT_KEY = { "wasapi_output" };
const SettingsKey WASAPI_ROLE_KEY   = { "wasapi_role" };

// ALSA
const SettingsKey ALSA_INPUT_KEY  = { "alsa_input" };
const SettingsKey ALSA_OUTPUT_KEY = { "alsa_output" };

// PipeWire
const SettingsKey PIPEWIRE_INPUT_KEY  = { "pipewire_input" };
const SettingsKey PIPEWIRE_OUTPUT_KEY = { "pipewire_output" };

// PulseAudio
const SettingsKey PULSEAUDIO_INPUT_KEY  = { "pulseaudio_input" };
const SettingsKey PULSEAUDIO_OUTPUT_KEY = { "pulseaudio_output" };

// Jack Audio
const SettingsKey JACK_OUTPUT_KEY       = { "jack_output" };
const SettingsKey JACK_START_SERVER_KEY = { "jack_start_server" };
const SettingsKey JACK_AUTOCONNECT_KEY  = { "jack_autoconnect" };
const SettingsKey JACK_CLIENT_NAME_KEY  = { "jack_client_name" };

// OSS
const SettingsKey OSS_INPUT_KEY  = { "oss_input" };
const SettingsKey OSS_OUTPUT_KEY = { "oss_output" };

// CoreAudio
const SettingsKey COREAUDIO_INPUT_KEY  = { "coreaudio_input" };
const SettingsKey COREAUDIO_OUTPUT_KEY = { "coreaudio_output" };

// PortAudio
const SettingsKey PORTAUDIO_INPUT_KEY  = { "portaudio_input" };
const SettingsKey PORTAUDIO_OUTPUT_KEY = { "portaudio_output" };

// TTS
const SettingsKey TTS_ENABLE_KEY        = { "enable_tts" };
const SettingsKey TTS_VOLUME_KEY        = { "tts_volume" };
const SettingsKey TTS_THRESHOLD_KEY     = { "tts_threshold" };
const SettingsKey TTS_READBACK_KEY      = { "tts_readback" };
const SettingsKey TTS_IGNORE_SCOPE_KEY  = { "tts_ignore_scope" };
const SettingsKey TTS_IGNORE_AUTHOR_KEY = { "tts_ignore_author" };
const SettingsKey TTS_LANGAGE_KEY       = { "tts_language" };

// Privacy
const SettingsKey HIDE_OS_FROM_SERVER_KEY = { "hide_os_from_server" };

// UI
const SettingsKey LANGUAGE_KEY                         = { "language" };
const SettingsKey THEME_KEY                            = { "theme" };
const SettingsKey THEME_STYLE_KEY                      = { "theme_style" };
const SettingsKey CHANNEL_EXPANSION_MODE_KEY           = { "channel_expansion_mode" };
const SettingsKey CHANNEL_DRAG_MODE_KEY                = { "channel_drag_mode" };
const SettingsKey USER_DRAG_MODE_KEY                   = { "user_drag_mode" };
const SettingsKey ALWAYS_ON_TOP_KEY                    = { "always_on_top" };
const SettingsKey QUIT_BEHAVIOR_KEY                    = { "quit_behavior" };
const SettingsKey SHOW_DEVELOPER_MENU_KEY              = { "show_developer_menu" };
const SettingsKey LOCK_LAYOUT_KEY                      = { "lock_layout" };
const SettingsKey MINIMAL_VIEW_KEY                     = { "minimal_view" };
const SettingsKey HIDE_FRAME_KEY                       = { "hide_frame" };
const SettingsKey DISPLAY_USERS_BEFORE_CHANNELS        = { "display_users_before_channels" };
const SettingsKey WINDOW_GEOMETRY_KEY                  = { "window_geometry" };
const SettingsKey WINDOW_GEOMETRY_MINIMAL_VIEW_KEY     = { "minimal_view_window_geometry" };
const SettingsKey WINDOW_STATE_KEY                     = { "window_state" };
const SettingsKey WINDOW_STATE_MINIMAL_VIEW_KEY        = { "minimal_view_window_state" };
const SettingsKey CONFIG_GEOMETRY_KEY                  = { "config_geometry" };
const SettingsKey WINDOW_LAYOUT_KEY                    = { "window_layout" };
const SettingsKey OVERLAY_HEADER_STATE                 = { "overlay_header_state" };
const SettingsKey SERVER_FILTER_MODE_KEY               = { "server_filter_mode" };
const SettingsKey HIDE_IN_TRAY_KEY                     = { "hide_in_tray" };
const SettingsKey DISPLAY_TALKING_STATE_IN_TRAY_KEY    = { "display_talking_state_in_tray" };
const SettingsKey SEND_USAGE_STATISTICS_KEY            = { "send_usage_statistics" };
const SettingsKey DISPLAY_USER_COUNT_KEY               = { "display_user_count" };
const SettingsKey DISPLAY_VOLUME_ADJUSTMENTS_KEY       = { "display_volume_adjustments" };
const SettingsKey DISPLAY_NICKNAMES_ONLY_KEY           = { "display_nicknames_only" };
const SettingsKey SELECTED_ITEM_AS_CHATBAR_TARGET_KEY  = { "use_selected_item_as_chatbar_target" };
const SettingsKey FILTER_HIDES_EMPTY_CHANNEL_KEY       = { "filter_hides_empty_channel" };
const SettingsKey FILTER_ACTIVE_KEY                    = { "filter_active" };
const SettingsKey CONTEXT_MENU_ENTRIES_IN_MENU_BAR_KEY = { "display_context_menu_entries_in_menu_bar" };
const SettingsKey CONNECT_DIALOG_GEOMETRY_KEY          = { "connect_dialog_geometry" };
const SettingsKey CONNECT_DIALOG_HEADER_STATE_KEY      = { "connect_dialog_header_state" };
const SettingsKey DISPLAY_TRANSMIT_MODE_COMBOBOX_KEY   = { "display_transmit_mode_combobox" };
const SettingsKey HIGH_CONTRAST_MODE_KEY               = { "high_contrast_mode" };
const SettingsKey MAX_LOG_LENGTH_KEY                   = { "max_log_length" };
const SettingsKey USE_24H_CLOCK_KEY                    = { "use_24h_clock_format" };
const SettingsKey LOG_MESSAGE_MARGINS_KEY              = { "log_message_margins" };
const SettingsKey DISABLE_PUBLIC_SERVER_LIST_KEY       = { "disable_public_server_list" };

// Last connection
const SettingsKey LAST_USERNAME_KEY    = { "username" };
const SettingsKey LAST_SERVER_NAME_KEY = { "server_name" };

// Updates
const SettingsKey CHECK_FOR_UPDATES_KEY        = { "check_for_updates" };
const SettingsKey CHECK_FOR_PLUGIN_UPDATES_KEY = { "check_for_plugin_updates" };
const SettingsKey AUTO_UPDATE_PLUGINS_KEY      = { "auto_update_plugins" };

// Misc
const SettingsKey DATABASE_LOCATION_KEY                  = { "database_location" };
const SettingsKey IMAGE_DIRECTORY_KEY                    = { "image_directory" };
const SettingsKey SERVER_PING_CONSENT_MESSAGE_VIEWED_KEY = { "viewed_server_ping_consent_message" };
const SettingsKey AUDIO_WIZARD_SHOWN_KEY                 = { "audio_wizard_has_been_shown" };
const SettingsKey CRASH_EMAIL_ADDRESS_KEY                = { "crash_report_email_address" };


// TalkingUI
const SettingsKey TALKINGUI_POSITION_KEY                   = { "talkingui_position" };
const SettingsKey SHOW_TALKINGUI_KEY                       = { "display_talkingui" };
const SettingsKey TALKINGUI_USERS_ALWAYS_VISIBLE_KEY       = { "users_always_visible" };
const SettingsKey TALKINGUI_LOCAL_USER_STAYS_VISIBLE_KEY   = { "local_user_stays_visible" };
const SettingsKey TALKINGUI_ABBREVIATE_CHANNEL_NAMES_KEY   = { "abbreviate_channel_names" };
const SettingsKey TALKINGUI_ABBREVIATE_CURRENT_CHANNEL_KEY = { "abbreviate_current_channel_name" };
const SettingsKey TALKINGUI_DISPLAY_LOCAL_LISTENERS_KEY    = { "display_local_listeners" };
const SettingsKey TALKINGUI_RELATIVE_FONT_SIZE_KEY         = { "relative_font_size" };
const SettingsKey TALKINGUI_SILENT_USER_LIFETIME_KEY       = { "silent_user_lifetime" };
const SettingsKey TALKINGUI_CHANNEL_HIERARCHY_DEPTH_KEY    = { "channel_hierarchy_depth" };
const SettingsKey TALKINGUI_MAX_CHANNEL_NAME_LENGTH_KEY    = { "max_channel_name_length" };
const SettingsKey TALKINGUI_NAME_PREFIX_COUNT_KEY          = { "name_prefix_count" };
const SettingsKey TALKINGUI_NAME_POSTFIX_COUNT_KEY         = { "name_postfix_count" };
const SettingsKey TALKINGUI_ABBREVIATION_REPLACEMENT_KEY   = { "abbreviation_replacement" };

// Channel hierarchy
const SettingsKey CHANNEL_NAME_SEPARATOR_KEY = { "channel_name_separator" };

// Manual plugin
const SettingsKey MANUALPLUGIN_SILENT_USER_LIFETIME_KEY = { "silent_user_lifetime" };

// PTT button window
const SettingsKey DISPLAY_PTTWINDOW_KEY  = { "display_ptt_window" };
const SettingsKey PTTWINDOW_GEOMETRY_KEY = { "ptt_window_geometry" };

// Recording
const SettingsKey RECORDING_PATH_KEY   = { "recording_path" };
const SettingsKey RECORDING_FILE_KEY   = { "recording_file" };
const SettingsKey RECORDING_MODE_KEY   = { "recording_mode" };
const SettingsKey RECORDING_FORMAT_KEY = { "recording_format" };

// Hidden
const SettingsKey DISABLE_CONNECT_DIALOG_EDITING_KEY = { "disable_connect_dialog_editing" };

// Overlay (win)
const SettingsKey OVERLAY_WIN_HELPER_ENABLE_x86_KEY            = { "enable_win_overlay_helper_x86" };
const SettingsKey OVERLAY_WIN_HELPER_ENABLE_x64_KEY            = { "enable_win_overlay_helper_x64" };
const SettingsKey OVERLAY_WIN_HELPER_RESTART_COOLDOWN_TIME_KEY = { "win_overlay_helper_restart_cooldown_time" };

// LCD
const SettingsKey LCD_USERVIEW_MIN_COLUMN_WIDTH_KEY = { "lcd_userview_min_column_width" };
const SettingsKey LCD_USERVIEW_SPLITTER_WIDTH_KEY   = { "lcd_userview_splitter_width" };

// Shortcuts
const SettingsKey ENABLE_GLOBAL_SHORTCUTS_KEY              = { "enable_global_shortcuts" };
const SettingsKey SUPPRESS_MACOS_EVENT_TAPPING_WARNING_KEY = { "suppress_macos_event_tapping_message" };
const SettingsKey ENABLE_EVDEV_KEY                         = { "enable_evdev" };
const SettingsKey ENABLE_XINPUT2_KEY                       = { "enable_xinput2" };
const SettingsKey ENABLE_GKEY_KEY                          = { "enable_gkey" };
const SettingsKey ENABLE_XBOX_WIN_KEY                      = { "enable_xbox_win" };
const SettingsKey WIN_UIACCESS_KEY                         = { "win_uiaccess" };

// Search
const SettingsKey SEARCH_FOR_USERS_KEY       = { "search_for_users" };
const SettingsKey SEARCH_FOR_CHANNELS_KEY    = { "search_for_channels" };
const SettingsKey SEARCH_CASE_SENSITIVE_KEY  = { "case_sensitive" };
const SettingsKey SEARCH_REGEX_KEY           = { "regex" };
const SettingsKey DISPLAY_SEARCH_OPTIONS_KEY = { "display_search_options" };
const SettingsKey SEARCH_USER_ACTION_KEY     = { "user_action" };
const SettingsKey SEARCH_CHANNEL_ACTION_KEY  = { "channel_action" };
const SettingsKey SEARCH_WINDOW_POSITION_KEY = { "search_window_position" };

// Overlay
const SettingsKey OVERLAY_ENABLE_KEY                = { "enable_overlay" };
const SettingsKey OVERLAY_STYLE_KEY                 = { "style" };
const SettingsKey OVERLAY_SHOW_MODE_KEY             = { "show_mode" };
const SettingsKey OVERLAY_ALWAYS_SELF_KEY           = { "always_self" };
const SettingsKey OVERLAY_ACTIVE_TIME_KEY           = { "active_time" };
const SettingsKey OVERLAY_SORT_MODE_KEY             = { "sort_mode" };
const SettingsKey OVERLAY_X_POS_KEY                 = { "x_position" };
const SettingsKey OVERLAY_Y_POS_KEY                 = { "y_position" };
const SettingsKey OVERLAY_ZOOM_KEY                  = { "zoom" };
const SettingsKey OVERLAY_COLUMNS_KEY               = { "columns" };
const SettingsKey OVERLAY_USERNAME_COLORS_KEY       = { "username_colors" };
const SettingsKey OVERLAY_USERNAME_FONT_KEY         = { "username_font" };
const SettingsKey OVERLAY_CHANNEL_COLOR_KEY         = { "channel_color" };
const SettingsKey OVERLAY_CHANNEL_FONT_KEY          = { "channel_font" };
const SettingsKey OVERLAY_FPS_COUNTER_COLOR_KEY     = { "fps_counter_color" };
const SettingsKey OVERLAY_FPS_COUNTER_FONT_KEY      = { "fps_counter_font" };
const SettingsKey OVERLAY_BOX_PAD_KEY               = { "box_pad" };
const SettingsKey OVERLAY_BOX_STROKE_WIDTH_KEY      = { "box_stroke_width" };
const SettingsKey OVERLAY_BOX_COLOR_KEY             = { "box_color" };
const SettingsKey OVERLAY_BOX_FILL_COLOR_KEY        = { "box_fill_color" };
const SettingsKey OVERLAY_SHOW_USERNAME_KEY         = { "show_username" };
const SettingsKey OVERLAY_SHOW_CHANNEL_KEY          = { "show_channel" };
const SettingsKey OVERLAY_SHOW_MUTEDEAF_KEY         = { "show_mute_deaf" };
const SettingsKey OVERLAY_SHOW_AVATAR_KEY           = { "show_avatar" };
const SettingsKey OVERLAY_SHOW_BOX_KEY              = { "show_box" };
const SettingsKey OVERLAY_SHOW_FPS_KEY              = { "show_fps_counter" };
const SettingsKey OVERLAY_SHOW_TIME_KEY             = { "show_time" };
const SettingsKey OVERLAY_USERNAME_OPACITY_KEY      = { "username_opacity" };
const SettingsKey OVERLAY_CHANNEL_OPACITY_KEY       = { "channel_opacity" };
const SettingsKey OVERLAY_MUTEDEAF_OPACITY_KEY      = { "mute_deaf_opacity" };
const SettingsKey OVERLAY_AVATAR_OPACITY_KEY        = { "avatar_opacity" };
const SettingsKey OVERLAY_USERS_OPACITIES_KEY       = { "user_opacities" };
const SettingsKey OVERLAY_FPS_COUNTER_OPACITY_KEY   = { "fps_counter_opacity" };
const SettingsKey OVERLAY_USERNAME_RECTANGLE_KEY    = { "username_rectangle" };
const SettingsKey OVERLAY_CHANNEL_RECTANGLE_KEY     = { "channel_rectangle" };
const SettingsKey OVERLAY_MUTEDEAF_RECTANGLE_KEY    = { "mute_deaf_rectangle" };
const SettingsKey OVERLAY_AVATAR_RECTANGLE_KEY      = { "avatar_rectangle" };
const SettingsKey OVERLAY_FPS_COUNTER_RECTANGLE_KEY = { "fps_counter_rectangle" };
const SettingsKey OVERLAY_TIME_RECTANGLE_KEY        = { "time_rectangle" };
const SettingsKey OVERLAY_USERNAME_ALIGNMENT_KEY    = { "username_alignment" };
const SettingsKey OVERLAY_CHANNEL_ALIGNMENT_KEY     = { "channel_alignment" };
const SettingsKey OVERLAY_MUTEDEAF_ALIGNMENT_KEY    = { "mute_deaf_alignment" };
const SettingsKey OVERLAY_AVATAR_ALIGNMENT_KEY      = { "avatar_alignment" };
const SettingsKey OVERLAY_EXCLUSION_MODE_KEY        = { "exclusion_mode" };
const SettingsKey OVERLAY_LAUNCHERS_KEY             = { "launchers" };
const SettingsKey OVERLAY_LAUNCHERS_EXCLUDE_KEY     = { "launchers_exclude" };
const SettingsKey OVERLAY_WHITELIST_KEY             = { "whitelist" };
const SettingsKey OVERLAY_WHITELIST_EXCLUDE_KEY     = { "whitelist_exclude" };
const SettingsKey OVERLAY_PATHS_KEY                 = { "paths" };
const SettingsKey OVERLAY_PATHS_EXCLUDE_KEY         = { "paths_exclude" };
const SettingsKey OVERLAY_BLACKLIST_KEY             = { "blacklist" };
const SettingsKey OVERLAY_BLACKLIST_EXCLUDE_KEY     = { "blacklist_exclude" };


const SettingsKey SETTINGS_VERSION_KEY     = { "settings_version" };
const SettingsKey CERTIFICATE_KEY          = { "certificate" };
const SettingsKey MUMBLE_QUIT_NORMALLY_KEY = { "mumble_has_quit_normally" };

} // namespace SettingsKeys

#endif // MUMBLE_MUMBLE_SETTINGSKEYS_H_
