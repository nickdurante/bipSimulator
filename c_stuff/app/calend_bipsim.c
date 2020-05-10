/*
	Calendar - BipSIM application
	v.0.1

	https://github.com/josehhh/bip_dev/tree/master/Calend
	
*/

#define ONE_DAY 86400
#define ONE_HOUR 3600
#define ONE_MINUTE 60

#include "template_bipsim.h"

#ifdef __SIMULATION__
#include <stdio.h>
#include <string.h>
#endif

//	calendar screen menu structure
struct regmenu_ screen_data = {
    55,                   //	curr_screen - main screen number, value 0-255, for custom windows it is better to take from 50 and above
    1,                    //	swipe_scr - auxiliary screen number (usually 1)
    0,                    //	overlay - 0
    interactionHandler,   //          - pointer to the handler of interaction (touch, swipe, long press)
    shortKeyPressHandler, //	        - handler of short button press
    refreshScreen,        //	        - timer callback function and..
    0,                    //	            ..the variable passed to it
    show_screen,          //	        - function writing to video buffer and..
    0,                    //              ..the variable passed to it
    longKeyPressHandler   //	        - handler of long button press
};

#ifdef __SIMULATION__
int main_app(int param0)
{
#else
int main(int param0, char **argv)
{ //	here the variable argv is not defined
#endif
    show_screen((void *)param0);
}

void swap_event_(struct event_ *event1, struct event_ *event2)
{
    struct event_ tmp = *event2;
    *event2 = *event1;
    *event1 = tmp;
}
void show_screen(void *param0)
{
    struct calend_ **calend_p = get_ptr_temp_buf_2(); //	pointer to screen data pointer
    struct calend_ *calend;                           //	pointer to screen data
    struct calend_opt_ calend_opt;                    //	calendar options

    if ((param0 == *calend_p) && get_var_menu_overlay())
    { // return from the overlay screen (incoming call, notification, alarm, target, etc.)

        calend = *calend_p; //	 pointer to data must be saved for exception
                            //	memory release function reg_menu
        *calend_p = NULL;   //	nullify the pointer to pass to the reg_menu function

        // 	create a new screen, while the pointers temp_buf_1 and temp_buf_2 were 0 and the memory was not freed
        reg_menu(&screen_data, 0); // 	menu_overlay=0

        *calend_p = calend; //	we restore the pointer to the data after the reg_menu function
        draw_calendar(calend);
    }
    else
    { // if the function started from the menu,

        // create a screen
        reg_menu(&screen_data, 0);

        // allocate the necessary memory and place data in it
        *calend_p = (struct calend_ *)pvPortMalloc(sizeof(struct calend_));
        calend = *calend_p; //	data pointer

        // clear memory for data
        _memclr(calend, sizeof(struct calend_));

        calend->proc = param0;

        // remember the address of the pointer to the function you need to return to after finishing this screen
        if (param0 && calend->proc->elf_finish) //	if the return pointer is passed, then return to it
            calend->ret_f = calend->proc->elf_finish;
        else //	if not, then on the dial
            calend->ret_f = show_watchface;

        struct datetime_ datetime;
        _memclr(&datetime, sizeof(struct datetime_));

        // get the current date
        get_current_date_time(&datetime);

        // generate test events. This funtion should read events from smartphone or memory.
        read_all_events();

        calend->day = datetime.day;
        calend->month = datetime.month;
        calend->year = datetime.year;

        // we consider options from flash memory, if the value in flash memory is incorrect then we take the first scheme
        // current color scheme is stored about offset 0
        ElfReadSettings(calend->proc->index_listed, &calend_opt, OPT_OFFSET_CALEND_OPT, sizeof(struct calend_opt_));

        if (calend_opt.color_scheme < COLOR_SCHEME_COUNT)
            calend->color_scheme = calend_opt.color_scheme;
        else
            calend->color_scheme = 0;

        calend->calendar_screen_view = CALENDAR_VIEW_MONTHLY;
        calend->event_list_page = 0;
        draw_calendar(calend);
    }

    // when idle, turn off the backlight and do not exit
    set_display_state_value(8, 1);
    set_display_state_value(2, 1);

    // timer on job for 20s where is the output.
    set_update_period(1, INACTIVITY_PERIOD);
}

void draw_month(unsigned int day, unsigned int month, unsigned int year)
{
    struct calend_ **calend_p = get_ptr_temp_buf_2(); //	pointer to screen data pointer
    struct calend_ *calend = *calend_p;               //	pointer to screen data

    // black theme without highlighting the weekend with today's highlight frame*/
    static unsigned char short_color_scheme[15] = {
        COLOR_SH_BLACK,            // 0: CALEND_COLOR_BG calendar background
        COLOR_SH_WHITE,            // 1: CALEND_COLOR_MONTH current month name color
        COLOR_SH_WHITE,            // 2: CALEND_COLOR_YEAR current year color
        COLOR_SH_WHITE,            // 3: CALEND_COLOR_WORK_NAME weekday color names
        COLOR_SH_BLACK,            // 4: CALEND_COLOR_HOLY_NAME_BG weekend day names background
        COLOR_SH_WHITE,            // 5: CALEND_COLOR_HOLY_NAME_FG weekend name color
        COLOR_SH_WHITE,            // 6: CALEND_COLOR_SEPAR calendar separator color
        COLOR_SH_GREEN,            // 7: CALEND_COLOR_NOT_CUR_WORK color of numbers NOT the current weekday
        COLOR_SH_BLACK,            // 8: CALEND_COLOR_NOT_CUR_HOLY_BG the background of numbers NOT the current month weekend
        COLOR_SH_GREEN,            // 9: CALEND_COLOR_NOT_CUR_HOLY_FG the color of the numbers NOT the current month weekend
        COLOR_SH_WHITE,            //10: CALEND_COLOR_CUR_WORK the color of the numbers of the current month of the week
        COLOR_SH_BLACK,            //11: CALEND_COLOR_CUR_HOLY_BG the background of the current month
        COLOR_SH_WHITE,            //12: CALEND_COLOR_CUR_HOLY_FG the color of the numbers of the current month weekend
        COLOR_SH_WHITE | (1 << 7), //13: CALEND_COLOR_TODAY_BG the background of the numbers of the current day; bit 31 - fill: = 0 fill with the background color, = 1 only the frame, the background is like the date of a non-current month
        COLOR_SH_WHITE             //14: CALEND_COLOR_TODAY_FG the color of the numbers of the current day
    };

    int color_scheme[15];

    for (unsigned char j = 0; j < 15; j++)
    {
        color_scheme[j] = (((unsigned int)short_color_scheme[j] & (unsigned char)COLOR_SH_MASK) & COLOR_SH_RED) ? COLOR_RED : 0;      //  red component
        color_scheme[j] |= (((unsigned int)short_color_scheme[j] & (unsigned char)COLOR_SH_MASK) & COLOR_SH_GREEN) ? COLOR_GREEN : 0; //	green component
        color_scheme[j] |= (((unsigned int)short_color_scheme[j] & (unsigned char)COLOR_SH_MASK) & COLOR_SH_BLUE) ? COLOR_BLUE : 0;   //	blue component
        color_scheme[j] |= (((unsigned int)short_color_scheme[j] & (unsigned char)(1 << 7))) ? (1 << 31) : 0;                         //	for the frame
    }

    char text_buffer[24];
    char *weekday_string_ru[] = {"??", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Вс"};
    char *weekday_string_en[] = {"??", "Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"};
    char *weekday_string_it[] = {"??", "Lu", "Ma", "Me", "Gi", "Ve", "Sa", "Do"};
    char *weekday_string_fr[] = {"??", "Lu", "Ma", "Me", "Je", "Ve", "Sa", "Di"};
    char *weekday_string_es[] = {"??", "Lu", "Ma", "Mi", "Ju", "Vi", "Sá", "Do"};

    char *weekday_string_short_ru[] = {"?", "П", "В", "С", "Ч", "П", "С", "В"};
    char *weekday_string_short_en[] = {"?", "M", "T", "W", "T", "F", "S", "S"};
    char *weekday_string_short_it[] = {"?", "L", "M", "M", "G", "V", "S", "D"};
    char *weekday_string_short_fr[] = {"?", "M", "T", "W", "T", "F", "S", "S"};
    char *weekday_string_short_es[] = {"?", "L", "M", "X", "J", "V", "S", "D"};

    char *monthname_ru[] = {
        "???",
        "Январь", "Февраль", "Март", "Апрель",
        "Май", "Июнь", "Июль", "Август",
        "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};

    char *monthname_en[] = {
        "???",
        "January", "February", "March", "April",
        "May", "June", "July", "August",
        "September", "October", "November", "December"};

    char *monthname_it[] = {
        "???",
        "Gennaio", "Febbraio", "Marzo", "Aprile",
        "Maggio", "Giugno", "Luglio", "Agosto",
        "Settembre", "Ottobre", "Novembre", "Dicembre"};

    char *monthname_fr[] = {
        "???",
        "Janvier", "Février", "Mars", "Avril",
        "Mai", "Juin", "Juillet", "Août",
        "Septembre", "Octobre", "Novembre", "Décembre"};
    char *monthname_es[] = {
        "???",
        "Enero", "Febrero", "Marzo", "Abril",
        "Mayo", "Junio", "Julio", "Agosto",
        "Septiembre", "Octubre", "Noviembre", "Diciembre"};

    char **weekday_string;
    char **weekday_string_short;
    char **monthname;

    switch (get_selected_locale())
    {
    case locale_ru_RU:
    {
        weekday_string = weekday_string_ru;
        weekday_string_short = weekday_string_short_ru;
        monthname = monthname_ru;
        break;
    }
    case locale_it_IT:
    {
        weekday_string = weekday_string_it;
        weekday_string_short = weekday_string_short_it;
        monthname = monthname_it;
        break;
    }
    case locale_fr_FR:
    {
        weekday_string = weekday_string_fr;
        weekday_string_short = weekday_string_short_fr;
        monthname = monthname_fr;
        break;
    }
    case locale_es_ES:
    {
        weekday_string = weekday_string_es;
        weekday_string_short = weekday_string_short_es;
        monthname = monthname_es;
        break;
    }
    default:
    {
        weekday_string = weekday_string_en;
        weekday_string_short = weekday_string_short_en;
        monthname = monthname_en;
        break;
    }
    }

    unsigned char day_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    _memclr(&text_buffer, 24);

    set_bg_color(color_scheme[CALEND_COLOR_BG]); //	calendar background
    fill_screen_bg();

    set_graph_callback_to_ram_1();
    load_font(); // load fonts

    _sprintf(&text_buffer[0], " %d", year);
    int month_text_width = text_width(monthname[month]);
    int year_text_width = text_width(&text_buffer[0]);

    int pos_x1_frame = 0;
    int pos_y1_frame = 0;
    int pos_x2_frame = 0;
    int pos_y2_frame = 0;

    set_fg_color(color_scheme[CALEND_COLOR_MONTH]);                                //	color of the month
    text_out(monthname[month], (176 - month_text_width - year_text_width) / 2, 0); // 	display the name of the month

    set_fg_color(color_scheme[CALEND_COLOR_YEAR]);                                //	color of the year
    text_out(&text_buffer[0], (176 + month_text_width - year_text_width) / 2, 0); // 	conclusion of the year

    //text_out("←", 5		 ,2); 		// left arrow output
    //text_out("→", 176-5-text_width("→"),2); 		// right arrow output

    int calend_name_height = get_text_height();

    set_fg_color(color_scheme[CALEND_COLOR_SEPAR]);

    // Names of the days of the week
    for (unsigned i = 1; (i <= 7); i++)
    {
        if (i > 5)
        { //	weekends
            set_bg_color(color_scheme[CALEND_COLOR_HOLY_NAME_BG]);
            set_fg_color(color_scheme[CALEND_COLOR_HOLY_NAME_FG]);
        }
        else
        { //	work days
            set_bg_color(color_scheme[CALEND_COLOR_BG]);
            set_fg_color(color_scheme[CALEND_COLOR_WORK_NAME]);
        }

        //  drawing the background of the weekend names
        int pos_x1 = H_MARGIN + (i - 1) * (WIDTH + H_SPACE);
        int pos_y1 = CALEND_Y_BASE + V_MARGIN - 2;

        // background for each day of the week name
        //draw_filled_rect_bg(pos_x1, pos_y1, pos_x2, pos_y2);

        // display the names of the days of the week. if the width of the name is greater than the width of the field, print short names
        //if (text_width(weekday_string[1]) <= (WIDTH - 2))
        //text_out_center(weekday_string[i], pos_x1 + WIDTH/2, pos_y1 + (calend_name_height-get_text_height())/2 );
        //else
        text_out_center(weekday_string_short[i], pos_x1 + WIDTH / 2, pos_y1 + (calend_name_height - get_text_height()) / 2);
    }

    //draw_horizontal_line(175, H_MARGIN, 176-H_MARGIN);	// Bottom month separator

    int calend_days_y_base = CALEND_Y_BASE + 1 + V_MARGIN + calend_name_height + V_MARGIN + 1;

    if (isLeapYear(year) > 0)
        day_month[2] = 29;

    unsigned char d = wday(1, month, year);
    unsigned char m = month;
    if (d > 1)
    {
        m = (month == 1) ? 12 : month - 1;
        d = day_month[m] - d + 2;
    }

    struct datetime_ datetime;
    get_current_date_time(&datetime); // get the current date

    // day of the month
    for (unsigned i = 1; (i <= 7 * 6); i++)
    {

        unsigned char row = (i - 1) / 7;
        unsigned char col = (i - 1) % 7 + 1;

        _sprintf(&text_buffer[0], "%2.0d", d);

        int bg_color = 0;
        int fg_color = 0;
        int frame = 0; // 1-frame; 0 - fill

        // if the current day of the current month
        if ((m == datetime.month) && (d == datetime.day) && (year == datetime.year))
        {
            frame = 1;
        }

        if (month == m)
        {
            bg_color = (color_scheme[CALEND_COLOR_BG]);
            fg_color = (color_scheme[CALEND_COLOR_CUR_WORK]);
        }
        else
        {
            bg_color = (color_scheme[CALEND_COLOR_BG]);
            fg_color = (color_scheme[CALEND_COLOR_NOT_CUR_WORK]);
        }

        //  line: from 7 to 169 = 162 px in width 7 numbers by 24 px per number 7+(22+2)*6+22+3
        //  lines: from 57 to 174 = 117px in height 6 lines at 19px per line 1+(17+2)*5+18

        // drawing the number background
        int pos_x1 = H_MARGIN + (col - 1) * (WIDTH + H_SPACE);
        int pos_y1 = calend_days_y_base + V_MARGIN + row * (HEIGHT + V_SPACE) - 9;

        set_bg_color(bg_color);
        set_fg_color(fg_color);

        text_out_center(&text_buffer[0], pos_x1 + WIDTH / 2, pos_y1 + (HEIGHT - get_text_height()) / 2);

        if (frame)
        { // TODO make a function out of this
            // with frame

            pos_x1_frame = H_MARGIN + (col - 1) * (WIDTH + H_SPACE);
            pos_y1_frame = calend_days_y_base + V_MARGIN + row * (HEIGHT + V_SPACE) - 6;
            pos_x2_frame = pos_x1_frame + WIDTH - 1;
            pos_y2_frame = pos_y1_frame + HEIGHT - 1;
        };

        if (d < day_month[m])
        {
            d++;
        }
        else
        {
            d = 1;
            m = (m == 12) ? 1 : (m + 1);
        }
    }

    set_fg_color(color_scheme[CALEND_COLOR_SEPAR]);
    draw_horizontal_line(CALEND_Y_BASE + V_MARGIN, H_MARGIN, 176 - H_MARGIN);               // Upper weekday separator
    draw_horizontal_line(CALEND_Y_BASE + calend_name_height - 2, H_MARGIN, 176 - H_MARGIN); // Bottom day separator
    set_bg_color(color_scheme[CALEND_COLOR_BG]);
    set_fg_color(color_scheme[CALEND_COLOR_SEPAR]);
    draw_rect(pos_x1_frame, pos_y1_frame, pos_x2_frame, pos_y2_frame); // add today frame
    draw_all_events_in_monthly(day, month, year);
    repaint_screen();
};

unsigned char wday(unsigned int day, unsigned int month, unsigned int year)
{
    signed int a = (14 - month) / 12;
    signed int y = year - a;
    signed int m = month + 12 * a - 2;
    return 1 + (((day + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12) % 7) + 6) % 7;
}

unsigned char isLeapYear(unsigned int year)
{
    unsigned char result = 0;
    if ((year % 4) == 0)
        result++;
    if ((year % 100) == 0)
        result--;
    if ((year % 400) == 0)
        result++;
    return result;
}

void shortKeyPressHandler()
{
    struct calend_ **calend_p = get_ptr_temp_buf_2(); //	pointer to screen data pointer
    struct calend_ *calend = *calend_p;               //	pointer to screen data

    show_menu_animate(calend->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
};

void longKeyPressHandler(void) {} // blank but present

void refreshScreen()
{
    struct calend_ **calend_p = get_ptr_temp_buf_2(); //	pointer to screen data pointer
    struct calend_ *calend = *calend_p;               //	pointer to screen data

    // when the update timer is reached, exit
    show_menu_animate(calend->ret_f, (unsigned int)show_screen, ANIMATE_LEFT);
}

#define N_EVENTS_PAGE 4
#define VERT_SPACE_FOR_EVENT_IN_LIST VIDEO_Y / N_EVENTS_PAGE
void draw_list_of_future_events(int page_number)
{

    // char buf_list_pos_string[10];
    // int list_pos = 0;
    // _sprintf(buf_list_pos_string, "%d", page_number);

    // _debug_print(buf_list_pos_string);

    set_bg_color(COLOR_BLACK);
    set_fg_color(COLOR_WHITE);
    fill_screen_bg();
    int now_unix = get_now_in_unix_time();

    for (int i = 0; i < N_EVENTS_PAGE; i++)
    {

        int item_index = N_EVENTS_PAGE * page_number + i;
        if (item_index >= all_events.number_of_events)
        {
            page_number = all_events.number_of_events / N_EVENTS_PAGE;
            break;
        }

        struct datetime_ tmp_date;
        struct datetime_ now_datime;
        from_unix_time_to_datetime_(all_events.array_of_events[item_index].start, &tmp_date);
        get_current_date_time(&now_datime);

        int upper_pos_item = VERT_SPACE_FOR_EVENT_IN_LIST * i;
        int date_pos = upper_pos_item + 0;
        int square_y_pos = date_pos + 8;
        int event_text_pos = upper_pos_item + get_text_height() - 1;

        char date_string[30];
        _strcpy(date_string, "");
        char buf[15];
        _strcpy(buf, "");

        if (abs(now_unix - all_events.array_of_events[item_index].start) < ONE_DAY * 2 && (tmp_date.day == now_datime.day + 1))
        {
            _strcat(date_string, "tomorrow at ");
            _sprintf(buf, "%.2d", tmp_date.hour);
            _strcat(date_string, buf);
            _strcat(date_string, ":");
            _sprintf(buf, "%.2d", tmp_date.min);
            _strcat(date_string, buf);
            _strcpy(buf, "");
        }
        else
        {
            get_char_string_from_date_and_duration_with_now_unix(now_unix, all_events.array_of_events[item_index].start, all_events.array_of_events[item_index].end - all_events.array_of_events[item_index].start, date_string);
        }

        // _sprintf(date_string, "%d", tmp_date.year);
        // _strcat(date_string, "-");
        // _sprintf(buf, "%d", tmp_date.month);
        // _strcat(date_string, buf);
        // _strcat(date_string, "-");
        // _sprintf(buf, "%d", tmp_date.day);
        // _strcat(date_string, buf);
        // _strcat(date_string, " ");
        // _sprintf(buf, "%.2d", tmp_date.hour);
        // _strcat(date_string, buf);
        // _strcat(date_string, ":");
        // _sprintf(buf, "%.2d", tmp_date.min);
        // _strcat(date_string, buf);
        // _strcat(date_string, ":");
        // _sprintf(buf, "%.2d", tmp_date.sec);
        // _strcat(date_string, buf);

        set_fg_color(all_events.array_of_events[item_index].color);
        draw_filled_rect(4, square_y_pos, 4 + 4, square_y_pos + 4);
        set_fg_color(COLOR_WHITE);

        text_out(date_string, 13, date_pos);
        text_out(all_events.array_of_events[item_index].name, 15, event_text_pos);
        draw_horizontal_line(upper_pos_item + VERT_SPACE_FOR_EVENT_IN_LIST - 1, 0, VIDEO_X);
    }

    repaint_screen();
    //sort_events_by_start_time_and_discard_expired_events();
}

void div_integers_as_floats_to_res_char(int dividend, int divisor, char *res)
{
    dividend *= 10;
    int res_int = dividend / divisor;
    char buf[2];
    _sprintf(res, "%d", res_int / 10);
    _sprintf(buf, "%d", res_int % 10);
    _strcat(res, ".");
    _strcat(res, buf);
}

void _aux_cat_event_duration_from_seconds_to_string(int duration_seconds, char *res)
{
    char buf[4];
    if (duration_seconds < 0)
    {
        _strcat(res, "negative duration ");
    }
    else if (duration_seconds < ONE_MINUTE)
    {
        _strcat(res, "1 min");
    }
    else if (duration_seconds < ONE_HOUR)
    {
        int num_mins = duration_seconds / ONE_MINUTE;
        _sprintf(buf, "%d", num_mins);
        _strcat(res, buf);
        _strcat(res, " min");
    }
    else if (duration_seconds < ONE_DAY)
    {
        div_integers_as_floats_to_res_char(duration_seconds, ONE_HOUR, buf);
        _strcat(res, buf);
        _strcat(res, " h");
    }
    else
    {
        div_integers_as_floats_to_res_char(duration_seconds, ONE_DAY, buf);
        _strcat(res, buf);
        _strcat(res, " days");
    }
}

void get_char_string_from_date_and_duration(int event_unix_time, int event_duration, char *res)
{
    int now_unix = get_now_in_unix_time();
    get_char_string_from_date_and_duration_with_now_unix(now_unix, event_unix_time, event_duration, res);
}

void get_char_string_from_date_and_duration_with_now_unix(int now_time_unix, int event_unix_time, int event_duration, char *res)
{
#ifdef DEBUG_LOG

    if (now_time_unix > event_unix_time + event_duration)
    {
        _debug_print("error, event expired in get_char_string_from_date_and_duration() ");
    }
#endif

    char buf[510];
    _strcpy(buf, "");

    int delta_start = event_unix_time - now_time_unix;
    if (delta_start <= 0)
    {
        _strcat(res, "now, for ");
        int duration_left = (event_unix_time + event_duration) - now_time_unix;
        _aux_cat_event_duration_from_seconds_to_string(duration_left, buf);
        _strcat(res, buf);
    }
    else
    {
        _strcat(res, "in ");
        _aux_cat_event_duration_from_seconds_to_string(event_unix_time - now_time_unix, buf);
        _strcat(res, buf);
    }
}

int interactionHandler(void *param)
{
    struct calend_ **calend_p = get_ptr_temp_buf_2(); //	pointer to screen data pointer
    struct calend_ *calend = *calend_p;               //	pointer to screen data

    struct calend_opt_ calend_opt; //	calendar options

    struct datetime_ datetime;
    // get the current date

    get_current_date_time(&datetime);
    unsigned int day;

    //	char text_buffer[32];
    struct gesture_ *gest = param;
    int result = 0;

    switch (gest->gesture)
    {
    case GESTURE_CLICK:
    {
        if (calend->calendar_screen_view == CALENDAR_VIEW_MONTHLY)
        {

            // if (gest->touch_pos_y < CALEND_Y_BASE)
            // { // clicked on the top line
            // if (gest->touch_pos_x < 44)
            calend->day = datetime.day;
            calend->month = datetime.month;
            calend->year = datetime.year;
        }

        draw_calendar(calend);
        // 	calend_opt.color_scheme = calend->color_scheme;

        // 	// write the settings to flash memory
        // 	ElfWriteSettings(calend->proc->index_listed, &calend_opt, OPT_OFFSET_CALEND_OPT, sizeof(struct calend_opt_));

        // extend inactivity exit timer through INACTIVITY_PERIOD with
        set_update_period(1, INACTIVITY_PERIOD);
        break;
    };

    case GESTURE_SWIPE_RIGHT: //	from left to right
    {
        // action when starting from the menu and further swipe left
        calend->calendar_screen_view++;
        calend->calendar_screen_view %= NUMBER_OF_CALEND_VIEWS;
        draw_calendar(calend);
        set_update_period(1, INACTIVITY_PERIOD);
        break;
    }
    case GESTURE_SWIPE_LEFT:
    { // from right to left

        if (get_left_side_menu_active())
        {
            set_update_period(0, 0);

            void *show_f = get_ptr_show_menu_func();

            // run dispatch_left_side_menu with param parameter as a result, the corresponding side screen will start
            // this will upload the data of the current application and deactivate it.
            dispatch_left_side_menu(param);

            if (get_ptr_show_menu_func() == show_f)
            {
                // if dispatch_left_side_menu worked unsuccessfully (there is nowhere to scroll) then in show_menu_func it will still be
                // our show_calend_screen function is contained, then just ignore this gesture

                // extend the inactivity exit timer through INACTIVITY_PERIOD with
                set_update_period(1, INACTIVITY_PERIOD);
                return 0;
            }

            // if dispatch_left_side_menu worked, then we finish our application, because screen data has already been uploaded
            // at this stage, a new screen is already running (the one where you swiped)

            Elf_proc_ *proc = get_proc_by_addr(main_app);
            proc->ret_f = NULL;

            elf_finish(main_app); //	unload Elf from memory
            return 0;
        }
        else
        { //	if the launch is not from the quick menu, we process swipes separately
            switch (gest->gesture)
            {
            case GESTURE_SWIPE_RIGHT:
            { //	swap right
                return show_menu_animate(calend->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
                break;
            }
            case GESTURE_SWIPE_LEFT:
            { // swipe left
                // action when starting from the menu and further swipe left
                calend->calendar_screen_view++;
                calend->calendar_screen_view %= NUMBER_OF_CALEND_VIEWS;
                draw_calendar(calend);
                set_update_period(1, INACTIVITY_PERIOD);
                break;
            }
            } /// switch (gest->gesture)
        }

        break;
    }; //	case GESTURE_SWIPE_LEFT:

    case GESTURE_SWIPE_UP:
    { // swipe up

        if (calend->calendar_screen_view == CALENDAR_VIEW_MONTHLY)
        {
            if (calend->month < 12)
                calend->month++;
            else
            {
                calend->month = 1;
                calend->year++;
            }

            if ((calend->year == datetime.year) && (calend->month == datetime.month))
                day = datetime.day;
            else
                day = 0;
        }
        else if (calend->calendar_screen_view == CALENDAR_VIEW_LIST)
        {
            calend->event_list_page = min(calend->event_list_page + 1, all_events.number_of_events / N_EVENTS_PAGE);
        }
        draw_calendar(calend);
        // extend inactivity exit timer through INACTIVITY_PERIOD
        set_update_period(1, INACTIVITY_PERIOD);
        break;
    };
    case GESTURE_SWIPE_DOWN:
    { // swipe down

        if (calend->calendar_screen_view == CALENDAR_VIEW_MONTHLY)
        {
            if (calend->month > 1)
                calend->month--;
            else
            {
                calend->month = 12;
                calend->year--;
            }

            if ((calend->year == datetime.year) && (calend->month == datetime.month))
                day = datetime.day;
            else
                day = 0;
        }
        else if (calend->calendar_screen_view == CALENDAR_VIEW_LIST)
        {
            calend->event_list_page = max(calend->event_list_page - 1, 0);
        }
        draw_calendar(calend);
        // extend inactivity exit timer through INACTIVITY_PERIOD
        set_update_period(1, INACTIVITY_PERIOD);
        break;
    };
    default:
    { // Something went wrong...
        break;
    };
    }

    return result;
};

void from_unix_time_to_datetime_(int unix_time, struct datetime_ *res)
{

    int t = unix_time + 37;

    //Retrieve hours, minutes and seconds
    (*res).sec = t % 60;
    t /= 60;
    (*res).min = t % 60;
    t /= 60;
    (*res).hour = t % 24;
    t /= 24;

    int a;
    int b;
    int c;
    int d;
    int e;
    int f;

    //Negative Unix time values are not supported
    if (t < 1)
        t = 0;

    //Convert Unix time to date
    a = (int)((4 * t + 102032) / 146097 + 15);
    b = (int)(t + 2442113 + a - (a / 4));
    c = (20 * b - 2442) / 7305;
    d = b - 365 * c - (c / 4);
    e = d * 1000 / 30601;
    f = d - e * 30 - e * 601 / 1000;

    //January and February are counted as months 13 and 14 of the previous year
    if (e <= 13)
    {
        c -= 4716;
        e -= 1;
    }
    else
    {
        c -= 4715;
        e -= 13;
    }

    //Retrieve year, month and day
    (*res).year = c;
    (*res).month = e;
    (*res).day = f;
}

// Return 1 if datetime2 goes after datetime 1, else, return -1. Return 0 if datetime1 = datetime2
int compare_datetimes(struct datetime_ *datetime1, struct datetime_ *datetime2)
{
    if (datetime2->year > datetime1->year)
    {
        return 1;
    }
    else if (datetime2->year < datetime1->year)
    {
        return -1;
    }

    if (datetime2->month > datetime1->month)
    {
        return 1;
    }
    else if (datetime2->month < datetime1->month)
    {
        return -1;
    }

    if (datetime2->day > datetime1->day)
    {
        return 1;
    }
    else if (datetime2->day < datetime1->day)
    {
        return -1;
    }

    if (datetime2->hour > datetime1->hour)
    {
        return 1;
    }
    else if (datetime2->hour < datetime1->hour)
    {
        return -1;
    }

    if (datetime2->min > datetime1->min)
    {
        return 1;
    }

    else if (datetime2->min < datetime1->min)
    {
        return -1;
    }

    if (datetime2->sec > datetime1->sec)
    {
        return 1;
    }

    else if (datetime2->sec < datetime1->sec)
    {
        return -1;
    }

    return 0;
}

int from_datetime_to_unix_time(struct datetime_ *datetime)
{

    int before_unix_time = 1546304401;
    int after_unix_time = 2147483645; // this triggers an overflow error

    struct datetime_ pivot_time;
    int pivot_time_unix;

    int found_unix_time = false;

    int counter = 0;
    while (found_unix_time == false && counter < 1000)
    {
        counter++;
        pivot_time_unix = (before_unix_time / 2 + after_unix_time / 2);
        if (before_unix_time % 2 == 1 && after_unix_time % 2 == 1)
        {
            pivot_time_unix++;
        }

        //_debug_print_integer(pivot_time_unix);

        if (after_unix_time - before_unix_time == 1)
        {
            from_unix_time_to_datetime_(pivot_time_unix, &pivot_time);
            int comp_res_when_diff_is_1 = compare_datetimes(datetime, &pivot_time);
            if (comp_res_when_diff_is_1 == 0)
            {
                return pivot_time_unix;
            }
            pivot_time_unix++;
        }

        from_unix_time_to_datetime_(pivot_time_unix, &pivot_time);
        int comp_res = compare_datetimes(datetime, &pivot_time);

        switch (comp_res)
        {
        case 0:
            return pivot_time_unix;
            break;

        case 1:
            after_unix_time = pivot_time_unix;
            break;

        case -1:
            before_unix_time = pivot_time_unix;
            break;

        default:
            _debug_print("Error, to_unix_time, default case");
            break;
        }
    }
    //_debug_print_integer(pivot_time_unix);
    _debug_print("Error,to_unix_time (max counter reached)");
    return 1;
}

void _debug_print_integer(int number)
{
    char buf[20];
    _sprintf(buf, "%d", number);
    _debug_print(buf);
}

void _debug_print(char *str)
{
// repaint_screen_lines(1, 176);
// set_bg_color(COLOR_BLACK);
// set_fg_color(COLOR_WHITE);
// draw_filled_rect_bg(5, 50, 100, 170);
// text_out_center(str, 75, 75);
// repaint_screen_lines(1, 176);
#ifdef DEBUG_LOG
    log_printf(5, "%s", str);
#endif
}

void _strcat(char *destination, const char *source)
{
    int len_dest = _strlen(destination);
    _strcpy(destination + len_dest, source);
}

// djb2: http://www.cse.yorku.ca/~oz/hash.html
int hash(const char *str)
{
    int hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c;
    return abs(hash);
}

struct event_ create_event(int unix_time_start, int unix_time_duration, char *event_name, char *event_type)
{
    struct event_ res;

#define n_colors 6
    int colors[n_colors] = {COLOR_BLUE, COLOR_AQUA, COLOR_GREEN, COLOR_PURPLE, COLOR_RED, COLOR_YELLOW};
    int color_index = hash(event_type) % n_colors;
    int color = colors[color_index];

    res.color = color;
    res.start = unix_time_start;
    res.end = unix_time_start + unix_time_duration;

    _strcpy(res.name, event_name);
    _strcpy(res.type_of_event, event_type);

    return res;
}

void draw_event_in_monthly_view(struct event_ ev, unsigned int month, unsigned int year)
{
    struct datetime_ start_datetime;
    from_unix_time_to_datetime_(ev.start, &start_datetime);

    int pos_x = -1;
    int pos_y = -1;

    if (month == start_datetime.month && year == start_datetime.year)
    {
        get_pos_day_in_monthly(start_datetime.day, start_datetime.month, start_datetime.year, &pos_x, &pos_y);
        set_bg_color(ev.color);
        pos_y += get_text_height() + 2;
        draw_filled_rect_bg(pos_x, pos_y, pos_x + 5, pos_y + 5);
        set_bg_color(COLOR_BLACK);
    }
    return;
}

#define X_CENTERING_OFFSET (WIDTH / 2 - 4)
#define Y_CENTERING_OFFSET -3
void get_pos_day_in_monthly(unsigned int day, unsigned int month, unsigned int year, int *pos_x, int *pos_y)
{
    unsigned char day_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year) > 0)
    {
        day_month[2] = 29;
    }
    int calend_days_y_base = CALEND_Y_BASE + 1 + V_MARGIN + get_text_height() + V_MARGIN + 1;
    unsigned char d = wday(1, month, year);
    unsigned char m = month;
    if (d > 1)
    {
        m = (month == 1) ? 12 : month - 1;
        d = day_month[m] - d + 2;
    }

    // char deb_chars[40];
    // char deb_chars_tmp[40];
    // _sprintf(deb_chars, "%0.3d", d);
    // _sprintf(deb_chars_tmp, "%0.3d", m);
    // _strcat(deb_chars, " :d - m: ");
    // _strcat(deb_chars, deb_chars_tmp);
    // _debug_print(deb_chars);

    // day of the month
    for (unsigned i = 1; (i <= 7 * 6); i++)
    {
        unsigned char row = (i - 1) / 7;
        unsigned char col = (i - 1) % 7 + 1;

        // if the current day of the current month
        if ((m == month) && (d == day))
        {
            *pos_x = H_MARGIN + (col - 1) * (WIDTH + H_SPACE) + X_CENTERING_OFFSET;
            *pos_y = calend_days_y_base + V_MARGIN + row * (HEIGHT + V_SPACE) - 9 + Y_CENTERING_OFFSET;
            return;
        }
        if (d < day_month[m])
        {
            d++;
        }
        else
        {
            d = 1;
            m = (m == 12) ? 1 : (m + 1);
        }
    }
}

int get_now_in_unix_time()
{
    struct datetime_ now;
    get_current_date_time(&now);
    return from_datetime_to_unix_time(&now);
}

void draw_all_events_in_monthly(unsigned int day, unsigned int month, unsigned int year)
{
    struct calend_ **calend_p = get_ptr_temp_buf_2(); //	pointer to screen data pointer
    struct calend_ *calend = *calend_p;               //	pointer to screen data

    set_graph_callback_to_ram_1();
    load_font(); // load fonts

    for (int i = 0; i < all_events.number_of_events; i++)
    {
        draw_event_in_monthly_view(all_events.array_of_events[i], month, year);
    }
}

void read_all_events()
{
    int NOW_EPOCH = get_now_in_unix_time();

    all_events.array_of_events[0] = create_event(NOW_EPOCH - ONE_HOUR, ONE_HOUR, "Pick up poster", "work");
    all_events.array_of_events[6] = create_event(NOW_EPOCH - 3, ONE_HOUR, "Meeting with boss", "work");
    all_events.array_of_events[2] = create_event(NOW_EPOCH + ONE_DAY * 6, ONE_HOUR, "Jhon's BD", "birthday");
    all_events.array_of_events[3] = create_event(NOW_EPOCH + ONE_DAY * 4, ONE_HOUR, "Laura's BD", "birthday");
    all_events.array_of_events[4] = create_event(NOW_EPOCH + ONE_DAY * 5, ONE_HOUR, "Bar with friends", "social_life");
    all_events.array_of_events[5] = create_event(NOW_EPOCH + ONE_DAY, ONE_HOUR, "Hockey with Donald", "social_life");
    all_events.array_of_events[1] = create_event(NOW_EPOCH + ONE_DAY * 7, ONE_HOUR, "Iron the clothes", "other_stuff");

    all_events.number_of_events = 7;

    discard_finished_events_and_sort(&all_events);
}

void BubbleSort_all_events(struct event_ *array_of_events, int n_of_events)
{
    int sorted = false;
    int i = 0;
    while ((i < n_of_events - 1) && sorted == false)
    {

        sorted = true;
        for (int j = 0; j < n_of_events - i - 1; j++)
        {
            if (array_of_events[j].start > array_of_events[j + 1].start) // For decreasing order use <
            {
                sorted = false;
                swap_event_(&array_of_events[j], &array_of_events[j + 1]);
            }
        }
        i++;
    }
}

void discard_finished_events_and_sort(struct all_events_ *all_events_struct)
{

    struct datetime_ now;
    get_current_date_time(&now);

    int current_time_unix = from_datetime_to_unix_time(&now);

    _debug_print("CUrrent epoch: ");
    _debug_print_integer(current_time_unix);

    for (int i = 0; i < all_events_struct->number_of_events; i++)
    {
        if (all_events_struct->array_of_events[i].end <= current_time_unix)
        {

            swap_event_(&all_events_struct->array_of_events[i], &all_events_struct->array_of_events[all_events.number_of_events - 1]);
            all_events_struct->number_of_events--;
        }
    }

    BubbleSort_all_events(all_events_struct->array_of_events, all_events_struct->number_of_events);
}

void draw_calendar(struct calend_ *calend_p)
{

    if (calend_p->calendar_screen_view == CALENDAR_VIEW_MONTHLY)
    {
        draw_month(calend_p->day, calend_p->month, calend_p->year);
    }
    else if (calend_p->calendar_screen_view == CALENDAR_VIEW_LIST)
    {
        draw_list_of_future_events(calend_p->event_list_page);
    }
    else
    {
        _debug_print("calend_p->cal_screen_view not recognn.");
    }

    return;
}
