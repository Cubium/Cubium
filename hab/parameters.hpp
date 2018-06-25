#ifndef PARAMETERS_CUBIUM_HPP
#define PARAMETERS_CUBIUM_HPP

// Global booleans
auto GLOBAL_30_MIN_SINCE_PPOD = false;
auto GLOBAL_45_MIN_SINCE_PPOD = false;
auto GLOBAL_BOOM_DEPLOYED     = false;
auto GLOBAL_ANTENNA_DEPLOYED  = false;


//***********************************
//*** Radio-modifiable parameters **
//*********************************

/* Boom deployment */
auto BOOM_MIN_LIGHT            = 1.0;
auto BOOM_MAX_ANGULAR_VELOCITY = 1.0;
auto BOOM_BATTERY_HEALTH       = 1.0;
auto BOOM_MIN_BOOMBOX_TEMP     = 1.0;
auto BOOM_MAX_BOOMBOX_TEMP     = 2.0;
auto BOOM_MIN_BOARD_TEMP       = 1.0;
auto BOOM_MAX_BOARD_TEMP       = 2.0;
auto BOOM_LIGHT_WINDOW         = 30.0;

/* Antenna deployment */
auto ANTENNA_BATTERY_HEALTH = 1.0;
auto ANTENNA_MIN_BOARD_TEMPERATURE = 1.0;
auto ANTENNA_MAX_BOARD_TEMPERATURE = 1.0;

/* Radio transmission */
auto RADIO_BATTERY_HEALTH = 1.0;
auto RADIO_MIN_BOARD_TEMPERATURE = 1.0;
auto RADIO_MAX_BOARD_TEMPERATURE = 1.0;
auto RADIO_TRANSMISSION_WINDOW = 30.0;
auto RADIO_PICTURE_COMMAND = "SEND_NUDES";

/* System restart */
auto RESTART_BATTERY_HEALTH = 0.1;


#endif
