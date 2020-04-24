#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#define LIBBIP_VERSION "1.2"

#define VIDEO_X 176
#define VIDEO_Y 176

#define NULL ((void *)0)

#ifndef min
#define min(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef max
#define max(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef abssub
#define abssub(x, y) ((x) > (y) ? (x) - (y) : (y) - (x))
#endif

#define MAX_NUM_BUTTONS 8
#define MAX_SIZE_BUTTON_LABEL 15
#define MAX_SIZE_TEXT_BOX 180

#define DEFAULT_BUTTON_WIDTH 81
#define DEFAULT_BUTTON_HEIGHT 45

#define MAX_SIZE_WINDOW_LABEL 30

#define MAX_NUM_WINDOWS 10

#define MAX_NUM_LAYERS 2

#define DEFAULT_BORDER_THICKNESS 4 // minimum reasonable distance of button edge to screen

#define DEFAULT_TEXT_HEIGHT 20

typedef struct Point_
{

    short x,
        y;
} Point_;

typedef enum Caffeine_t
{

    WEAK,  // 0 - screen backlight will go off
    STRONG // 1 - backlight always on

} Caffeine_t;

typedef enum Style_t
{

    BUTTON_STYLE_DEFAULT_SQUARED,
    BUTTON_STYLE_SQUARED_NOBORDER,
    BUTTON_STYLE_ROUNDED_NOBORDER

} Style_t;

typedef enum Way_
{

    UP,
    DOWN,
    LEFT,
    RIGHT,
    CENTER

} Way_;

typedef struct ButtonParams_
{
    Style_t style;

} ButtonParams_;

typedef struct Button_
{

    Point_ topLeft,
        bottomRight;

    char label[MAX_SIZE_BUTTON_LABEL];

    short border, // color of button features
        filling,
        textColour;

    char visible;

    void (*callbackFunction)();

    ButtonParams_ params; // style, state..

} Button_;

typedef struct TextBox_
{

    Point_ topLeft,
        bottomRight;

    char body[MAX_SIZE_TEXT_BOX];

    short colour,
        background;

    char visible;
    char centerText;        // 1: render text centered, 0: render normally
    
} TextBox_;

typedef struct LayerParams_
{

    short state;

} LayerParams_;

typedef struct Layer_
{

    Button_ *buttonArray[MAX_NUM_BUTTONS]; // all buttons
    unsigned short buttonIndex;            // current valid button, init=0

    short backgroundColour; // background for the current Layer
    char visible;           // is the layer visible?

    TextBox_ *textBox; // textbox for general usage

    LayerParams_ params; // holding state of the layer
    //void (*callbackFunction)();
} Layer_;

typedef struct Window_
{
    int neighbors[4]; //pointers to neighboring windows (up. down, left, right)

    char name[MAX_SIZE_WINDOW_LABEL];
    char nameVisible;

    Layer_ *layerArray[MAX_NUM_LAYERS];
    char layerIndex;

    void (*callbackFunction)();

} Window_;
typedef struct Viewport_
{

    Window_ *windowArray[MAX_NUM_WINDOWS];
    char windowIndex;

    Window_ *active;

} Viewport_;

typedef struct Game_
{

    unsigned short score;
    unsigned short record;
} Game_;

typedef struct app_data_t
{
    void *ret_f; //	the address of the return function

    Viewport_ vp;
} app_data_t;

// screen structure
struct regmenu_
{
    char curr_scr;        //	menu section
    char swipe_scr;       //  submenu section
    char overlay;         //
    void *dispatch_func,  //	screen gesture processing function
        *key_press,       //	side button click processing function
        *scr_job_func,    //	callback screen refresh timer function
        *scr_job_buff,    //	variable for callback screen refresh
        *show_menu_funct, //	screen forming function
        *show_menu_buff,  //	variable for screen forming function
        *long_key_press;  //	function of processing a long press on the side button
};

//	gesture structure when processing clicks on the screen and side button
struct gesture_
{
    char gesture;    // gesture type
    int touch_pos_x, // click coordinates	X
        touch_pos_y; //						Y
};

// gesture type
#define GESTURE_CLICK 1       // simple tap on the screen, click coordinatesв variables touch_pos_x touch_pos_y
#define GESTURE_SWIPE_UP 2    // swipe up
#define GESTURE_SWIPE_DOWN 3  // swipe down
#define GESTURE_SWIPE_LEFT 4  // swipe right to left
#define GESTURE_SWIPE_RIGHT 5 // swipe left to right

// time date structure
struct datetime_
{
    unsigned short year;
    unsigned char month,
        day,
        hour,
        min,
        sec,
        weekday;
    unsigned char h24;
};

// animation view for show_menu_animate function
#define ANIMATE_LEFT 0  // screen transition animation from right to left
#define ANIMATE_RIGHT 1 // screen transition animation from left to right
#define ANIMATE_UP 2    // screen transition animation from bottom to top
#define ANIMATE_DOWN 3  // top-down animation

// colors
#define COLOR_BLACK 0x000000  //	black
#define COLOR_RED 0x0000FF    //	red
#define COLOR_GREEN 0x00FF00  //	green
#define COLOR_BLUE 0xFF0000   //	blue
#define COLOR_YELLOW 0x00FFFF //	yellow
#define COLOR_AQUA 0xFFFF00   //	aquamarine
#define COLOR_PURPLE 0xFF00FF //	lilac
#define COLOR_WHITE 0xFFFFFF  //	white
#define COLOR_MASK 0xFFFFFF   //	mask colors

// colors в short format
#define COLOR_SH_BLACK 0b0000  //	black
#define COLOR_SH_RED 0b0001    //	red
#define COLOR_SH_GREEN 0b0010  //	green
#define COLOR_SH_BLUE 0b0100   //	blue
#define COLOR_SH_YELLOW 0b0011 //	yellow
#define COLOR_SH_AQUA 0b0110   //	aquamarine
#define COLOR_SH_PURPLE 0b0101 //	lilac
#define COLOR_SH_WHITE 0b0111  //	white
#define COLOR_SH_MASK 0b1111   //	маска colors

// text languages (locales)
#define locale_ru_RU 1049 //	Russian
#define locale_en_US 1033 //	English
#define locale_it_IT 1040 //	Italian
#define locale_es_ES 3082 //	Spanish
#define locale_fr_FR 1036 //	French
#define locale_de_DE 1031 //	Deutsch

// language set for the mod menu returned by get_stored_lang function
#define OPT_LANG_COUNT 6   //	number of languages
#define OPT_LANG_AUTO 0x00 //	auto locale phone
#define OPT_LANG_RUS 0x01  //	Russian
#define OPT_LANG_ENG 0x02  //	English
#define OPT_LANG_ITA 0x03  //	Italian
#define OPT_LANG_SPA 0x04  //	Spanish
#define OPT_LANG_FRA 0x05  //	French
#define OPT_LANG_DEU 0x06  //	Deutsch

// system settings data ( function get_generic_data / set_generic_data )
#define GENERIC_DATA_LOCALE 36 //	phone locale

//  menu structure
//	settings menu
#define MAX_MENU_ITEM_NAME_LEN 64
#define MAX_MENU_ENCLOSURE 15 //	maximum menu nesting
#define ITEMS_ON_PAGE 3
#define MAX_MENU_ITEMS_COUNT 15
#define MAX_LINES_PER_ITEM 3
//	menu item display options
#define MENU_H_MARGIN 10

// menu item statuses
#define MENU_ITEM_STYLE_NORMAL 0     //	regular menu item	the ">" icon. Procedure start through show_animate with return function parameter
#define MENU_ITEM_STYLE_LIST 1       //	regular menu item	the ">" icon.	Starting a procedure with menu and index parameters
#define MENU_ITEM_STYLE_UNCHECKED 2  //	checkbox not checked	the 0 icon.	Starting a procedure with menu and index parameters
#define MENU_ITEM_STYLE_CHECKED 3    //	checkbox checked		v icon.	Starting a procedure with menu and index parameters
#define MENU_ITEM_STYLE_TOGGLE_ON 4  //	switch on	badge (o).	Starting a procedure with menu and index parameters
#define MENU_ITEM_STYLE_TOGGLE_OFF 5 //	switch on	badge (o).	Starting a procedure with menu and index parameters
#define MENU_ITEM_STYLE_LOCKED 6     //	blocked		lock icon.
#define MENU_ITEM_STYLE_DISABLED 7   //	the item is currently not available	X icon

// "Quick" application (launch when entering the side menu)
#define LEFT_SIDE_APP_COUNT 4
#define LEFT_SIDE_APP_NONE 0
#define LEFT_SIDE_APP_CALC 1
#define LEFT_SIDE_APP_CALEND 2
#define LEFT_SIDE_APP_FLASH 3

struct menu_item_struct
{
    //unsigned char 	index;		//	menu item number
    char name[MAX_MENU_ITEM_NAME_LEN + 1]; //	menu item name
    void *show_func;                       //      start function
    int item_color;                        //	menu item color
    int item_style;                        //	the status of the menu bar
    int item_param0;                       //	custom menu item parameter
    //unsigned char	premium;		//	premium version item
};

struct menu_struct
{
    struct menu_item_struct items[MAX_MENU_ITEMS_COUNT]; //	menu structure
    unsigned char page;                                  //      current page number
    unsigned char item_count;                            //	number of menu items
    void *show_func;                                     //	display function address of the current screen
    void *ret_f[MAX_MENU_ENCLOSURE];                     //	return to previous screen
    int menu_level;                                      //	current nesting level
};

struct res_struct
{                             // sizeof=32+4
    char res_type[5];         //	NERES, HMRES
    char version;             //
    char magic[4];            //	E8 09 1A D7 = 0xD71A09E8
    char reserved_0[4];       //	03 00 00 00
    char reserved_1[2];       //	FF FF
    unsigned int reserved[4]; //  FF..FF
    unsigned int count;       //
};

#define RES_MAGIC 0xD71A09E8
#define NERES_ADDRESS 0x2A0000

// parameters of graphic resources
struct res_params_
{
    short width;  // width in px
    short height; // height in px
};

#define INDEX_MAIN_RES 0xFFFF0000

typedef struct
{                                      // structure of the running process
    unsigned int process_id;           // process identifier assigned by the loader
    int index_listed;                  // elf index in the bootloader list
    void *base;                        // pointer to the memory allocated for the process
    unsigned int size;                 // amount of memory allocated for the process
    void *ret_f;                       // process return point
    unsigned int ret_param0;           // return function parameter
    void (*elf_finish)(void *param0);  // pointer to the elf completion procedure, here you need to pass
    void (*entry_point)(void *param0); // pointer to the elf launch procedure, entry point. Param0 = pointer to the proc structure of the running process
    void *param;                       // user parameter, you can store anything, for example pointers instead of temp_buf_2 for background processes
    int argc;                          // number of parameters when running the elf
    void **argv;                       // parameters when starting the elf
} Elf_proc_;

// firmware version
#define LATIN_1_1_5_12 11512
#define LATIN_1_1_5_16 11516
#define LATIN_1_1_5_36 11536
#define LATIN_1_1_5_56 11556
#define NOT_LATIN_1_1_2_05 11205
#define UNI_LATIN 55555

// function status of get_app_state
#define APP_STATE_BT_CON 0x200

// data structure containing alarm information
struct alarm_struct
{
    unsigned char index;   //  bit_7 - included/switched off; bit_0..4 - номер alarm clock
    unsigned char hours;   //  clock
    unsigned char minutes; //  minutes
    unsigned char repeat;  //  bit_0 - bit_7 - days of the week monday to friday
                           //  значение 	128 - once; 127 - everyday;
                           //	31 - on workers; 0   - not created;   60 - on weekends
};

// data structure containing information about the selected page number and data from 10 alarms
struct alarm_10
{
    unsigned char page;             // current page for setting alarms
    unsigned char field_0;          //	unknown structure field, possibly not used
    struct alarm_struct alarms[10]; //	Array of 10 alarms
};

// data structs for heart rate sensing
// 1.1.5.12, 1.1.5.36
typedef struct
{
    int t_start;
    short last_hr;
    unsigned char field_2;
    unsigned char field_3;
    unsigned char field_4;
    unsigned char heart_rate; //	HR in beats / min; if >200 then value not available
    unsigned char ret_code;   //	status of current sample - 0:finished, >0 not yet finished
    unsigned char field_5;
} hrm_data_struct;

// 1.1.2.05
typedef struct
{
    int t_start;
    short last_hr;
    unsigned char heart_rate; //	HR in beats / min; if >200 then value not available
    unsigned char ret_code;   //	status of current sample - 0:finished, >0 not yet finished
} hrm_data_struct_legacy;

void text_out(char *text_in, int origin_x, int origin_y);
void text_out_center(char *text_in, int origin_x, int origin_y);
void set_bg_color(long color);
void set_fg_color(long color);
void fill_screen_bg(void);
void repaint_screen(void);
void repaint_screen_lines(int from_line, int to_line);
void draw_horizontal_line(int pos_y, int from_x, int to_x);
void draw_vertical_line(int pos_x, int from_y, int to_y);
void draw_rect(int from_x, int from_y, int to_x, int to_y);
void draw_filled_rect(int from_x, int from_y, int to_x, int to_y);
void draw_filled_rect_bg(int from_x, int from_y, int to_x, int to_y);
char *create_monitor(void);

void *pvPortMalloc(int size);
void vibrate(int count, int on_ms, int off_ms);
int _memclr(void *buf, int len);
void vPortFree(void *pv);
int _strcpy(char *destptr, const char *srcptr);
int _strncpy(char *destptr, const char *srcptr, int num);
void set_display_state_value(int state_1, int state);
int set_graph_callback_to_ram_1(void);
int show_watchface(void);
void set_update_period(int cmd, int period);
void reg_menu(void *regmenu_, int param);
int get_var_menu_overlay(void);

app_data_t *get_app_data_ptr(void);
int set_app_data_ptr(app_data_t *app_data);
int _sprintf(char *buf, const char *format, ...);

int _rand();                  
void _srand(unsigned int seed);

int show_menu_animate(void *show_menu_function, int param, int animate);
int get_tick_count(void);

#endif