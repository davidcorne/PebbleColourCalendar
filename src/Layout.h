// Layout constants
#define DEVICE_WIDTH             144
#define DEVICE_HEIGHT            168

#define CONNECTION_LEFT             2
#define CONNECTION_TOP              2
#define CONNECTION_WIDTH           22
#define CONNECTION_HEIGHT          22

#define BATTERY_CHARGING_LEFT     78
#define BATTERY_CHARGING_TOP       0
#define BATTERY_CHARGING_WIDTH    22
#define BATTERY_CHARGING_HEIGHT   22

#define STATUS_BATTERY_LABEL_LEFT 50
#define STATUS_BATTERY_LABEL_TOP   2
#define STATUS_BATTERY_LABEL_WIDTH 45 
#define STATUS_BATTERY_LABEL_HEIGHT 14

#define STATUS_BATTERY_LEFT       96
#define STATUS_BATTERY_TOP         4
#define STATUS_BATTERY_WIDTH      43
// STATUS_BATTERY_WIDTH should be divisible by 10, after subtracting 
// STATUS_BATTERY_NUB_WIDTH, and 4 (2 pixels/side for the 'border')
#define STATUS_BATTERY_HEIGHT     15
#define STATUS_BATTERY_NUB_WIDTH   3 // >= 3
#define STATUS_BATTERY_NUB_HEIGHT  5 // >= 3

#define DATE_DISPLAY_LEFT          2
#define DATE_DISPLAY_TOP          19
#define DATE_DISPLAY_WIDTH  (DEVICE_WIDTH - DATE_DISPLAY_LEFT - 2)
#define DATE_DISPLAY_HEIGHT       26

#define TIME_DISPLAY_LEFT          0
#define TIME_DISPLAY_TOP          45
#define TIME_DISPLAY_WIDTH  (DEVICE_WIDTH - TIME_DISPLAY_LEFT)
#define TIME_DISPLAY_HEIGHT       45

#define CALENDAR_LEFT              2
#define CALENDAR_TOP              88
#define CALENDAR_WIDTH  (DEVICE_WIDTH - 4)
#define CALENDAR_HEIGHT (DEVICE_HEIGHT - CALENDAR_TOP)
