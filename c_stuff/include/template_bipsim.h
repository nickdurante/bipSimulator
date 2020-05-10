/*
	(С) Волков Максим 2019 ( mr-volkov+bip@yandex.ru )
	Календарь для Amazfit Bip
	
*/
#ifndef __CALEND_H__
#define __CALEND_H__

// Значения цветовой схемы 
#define CALEND_COLOR_BG 0 // calendar background
#define CALEND_COLOR_MONTH 1 // current month name color
#define CALEND_COLOR_YEAR 2 // current year color
#define CALEND_COLOR_WORK_NAME 3 // color of weekday names
#define CALEND_COLOR_HOLY_NAME_BG 4 // weekend day name background
#define CALEND_COLOR_HOLY_NAME_FG 5 // color of the names of the days of the weekend
#define CALEND_COLOR_SEPAR 6 // color of calendar separators
#define CALEND_COLOR_NOT_CUR_WORK 7 // the color of the numbers is NOT the current weekday
#define CALEND_COLOR_NOT_CUR_HOLY_BG 8 // the background of the numbers is NOT the current month, the weekend
#define CALEND_COLOR_NOT_CUR_HOLY_FG 9 // color of numbers NOT of the current month; weekend
#define CALEND_COLOR_CUR_WORK 10 // the color of the numbers of the current weekday
#define CALEND_COLOR_CUR_HOLY_BG 11 // weekend background of the current month
#define CALEND_COLOR_CUR_HOLY_FG 12 // color of the numbers of the current month weekend
#define CALEND_COLOR_TODAY_BG 13 // color of the numbers of the current day
#define CALEND_COLOR_TODAY_FG 14 // background of the numbers of the current day

// number of color schemes
#define COLOR_SCHEME_COUNT	5

// address offset for storing calendar settings
#define OPT_OFFSET_CALEND_OPT		0

#if FW_VERSION==latin_1_1_5_12 || FW_VERSION==latin_1_1_5_36
// options for drawing calendar numbers
// line: from 7 to 169 = 162 px in the width of 7 numbers by 24 px for the number 7+ (24) * 6 + 22 + 3
// lines: from 57 to 174 = 117px in height 6 lines of 22px per line 1+ (22) * 5 + 22

#define CALEND_Y_BASE 18 // base height of the start of calendar rendering
// # define CALEND_NAME_HEIGHT 19 // line height of the names of the days of the week
// # define CALEND_DAYS_Y_BASE CALEND_Y_BASE + 1 + V_MARGIN + CALEND_NAME_HEIGHT + V_MARGIN + 1 // month base height
#define WIDTH 24 // number digits width
#define HEIGHT 23 // digits height of a number
#define V_SPACE 0 // vertical indent between lines of numbers
#define H_SPACE 0 // horizontal indent between columns of numbers
#define H_MARGIN 4 // horizontal indent from the edge of the screen
#define V_MARGIN 1 // vertical indent from the header (base)

#elif	FW_VERSION==not_latin_1_1_2_05
// options for drawing calendar numbers
// line: from 7 to 169 = 162 px in the width of 7 numbers by 24 px for the number 7+ (24) * 6 + 24 + 3
// lines: from 57 to 174 = 117px in height 6 lines in 22px per line 1+ (22) * 5 + 20

#define CALEND_Y_BASE 18 // base height of the start of calendar rendering
// # define CALEND_NAME_HEIGHT FONT_HEIGHT + 2 // line height of the names of the days of the week
// # define CALEND_DAYS_Y_BASE CALEND_Y_BASE + 1 + V_MARGIN + CALEND_NAME_HEIGHT + V_MARGIN + 1 // month base height is 56
#define WIDTH 24 // number digits width
#define HEIGHT 23 // digits height of a number
#define V_SPACE 0 // vertical indent between lines of numbers
#define H_SPACE 0 // horizontal indent between columns of numbers
#define H_MARGIN 4 // horizontal indent from the edge of the screen
#define V_MARGIN 1 // vertical indent from the header (base)
#endif

#define MAX_NUMBER_OF_EVENTS 10
#define INACTIVITY_PERIOD		300000		//	time after which we leave

#include <bipui.h>


struct event_{
	char name[30];
	int start; // start and end are in unix time, to save memory and be easyer to transmit in the future
	int end;
	char type_of_event[20];
	long color;
};

// stored calendar options
struct calend_opt_ {
		unsigned char	color_scheme;	//	color scheme
};

// current data of the viewed / edited calendar
struct calend_ {
	Elf_proc_* 	proc;				//	pointer to the data of the running process
		void* 	ret_f;				//	return function address
unsigned char	color_scheme;		//	color scheme
									//	displayed month
unsigned int 	day;				//	day
unsigned int 	month;				//	month
unsigned int 	year;				//	year

int calendar_screen_view; // CALENDAR_VIEW_MONTHLY or CALENDAR_VIEW_LIST

int event_list_page;
};



#define NUMBER_OF_CALEND_VIEWS 2
#define CALENDAR_VIEW_MONTHLY 0
#define CALENDAR_VIEW_LIST 1

struct all_events_{
int number_of_events;
struct event_ array_of_events[MAX_NUMBER_OF_EVENTS];
} all_events;

// sort array of events from first starting to last starting.
void sort_events_(struct all_events_ all_events);

// return the index of the first event in a day sorted by starting time. If no events that day, return -1.
int get_index_of_first_event_in_day(unsigned int year, unsigned int month, unsigned int day);

// list view of future events
void draw_list_of_future_events(int page_number);

// This function should read future events from the smartphone
void read_all_events();

void show_screen(void *return_screen);
void shortKeyPressHandler();

void longKeyPressHandler(void);
int interactionHandler(void *param);
void refreshScreen();

void draw_month(unsigned int day, unsigned int month, unsigned int year);
unsigned char wday(unsigned int day,unsigned int month,unsigned int year);
unsigned char isLeapYear(unsigned int year);
void from_unix_time_to_datetime_(int unix_time, struct datetime_ * datetime);
int from_datetime_to_unix_time(struct datetime_ *datetime);
int get_now_in_unix_time();
void get_char_string_from_date_and_duration(int event_unix_time, int event_duration, char* res);
void get_char_string_from_date_and_duration_with_now_unix(int now_unix, int event_unix_time, int event_duration, char* res);


struct event_ create_event(int unix_time_start, int unix_time_end, char* event_name,  char* event_type);
void draw_event_in_monthly_view(struct event_ ev, unsigned int month, unsigned int year);
void _debug_print();
void _debug_print_integer(int number);
void _strcat(char * destination, const char * source );
void draw_all_events(unsigned int month, unsigned int year);
void get_pos_day_in_monthly(unsigned int day, unsigned int month, unsigned int year, int* pos_x, int* pos_y);
void draw_all_events_in_monthly(unsigned int day, unsigned int month, unsigned int year);

void BubbleSort_all_events(struct event_ *array_of_events, int n_of_events);
void discard_finished_events_and_sort(struct all_events_ * all_events_struct);

#endif