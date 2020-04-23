#include <stdlib.h>
#include "cJSON.h"
#include <ws.h>

#include <stdio.h>
#include <string.h>

#include <bipui.h>


int ID_client = 0;
app_data_t *app_data_global;

void setClientID(int clientID) {

    ID_client = clientID;

}

int getClientID(void) {

    return ID_client;
}

void text_out(char *text_in, int origin_x, int origin_y)
{

    cJSON *content = NULL;

    cJSON *x = NULL;
    cJSON *y = NULL;

    cJSON *message = cJSON_CreateObject();

    cJSON *from = cJSON_CreateString("app"); // from : app
    cJSON_AddItemToObject(message, "from", from);

    cJSON *action = cJSON_CreateString("text_out"); //action : text_out_center
    cJSON_AddItemToObject(message, "action", action);

    content = cJSON_CreateString(text_in); // content : text_in
    cJSON_AddItemToObject(message, "content", content);

    x = cJSON_CreateNumber(origin_x); // x : X
    cJSON_AddItemToObject(message, "x", x);

    y = cJSON_CreateNumber(origin_y); // y : Y
    cJSON_AddItemToObject(message, "y", y);

        char *string = cJSON_Print(message);
    printf("%s\n", string);

    ws_sendframe(getClientID(), (char *)string, true);
    free(message);
}

void text_out_center(char *text_in, int origin_x, int origin_y)
{

    cJSON *content = NULL;

    cJSON *x = NULL;
    cJSON *y = NULL;

    cJSON *message = cJSON_CreateObject();

    cJSON *from = cJSON_CreateString("app"); // from : app
    cJSON_AddItemToObject(message, "from", from);

    cJSON *action = cJSON_CreateString("text_out_center"); //action : text_out_center
    cJSON_AddItemToObject(message, "action", action);

    content = cJSON_CreateString(text_in); // content : text_in
    cJSON_AddItemToObject(message, "content", content);

    x = cJSON_CreateNumber(origin_x); // x : X
    cJSON_AddItemToObject(message, "x", x);

    y = cJSON_CreateNumber(origin_y); // y : Y
    cJSON_AddItemToObject(message, "y", y);

        char *string = cJSON_Print(message);
    printf("%s\n", string);

    ws_sendframe(getClientID(), (char *)string, true);
    free(message);
}

void set_bg_color(long color)
{

    int R = (color & 0xFF);         //extract first byte
    int G = ((color >> 8) & 0xFF);  //extract second byte
    int B = ((color >> 16) & 0xFF); //extract third byte

    cJSON *red_field = NULL;
    cJSON *blue_field = NULL;
    cJSON *green_field = NULL;

    cJSON *message = cJSON_CreateObject();

    cJSON *from = cJSON_CreateString("app"); // from : app
    cJSON_AddItemToObject(message, "from", from);

    cJSON *action = cJSON_CreateString("set_bg_color"); //action : set_bg_color
    cJSON_AddItemToObject(message, "action", action);

    cJSON *color_field = cJSON_CreateArray();

    red_field = cJSON_CreateNumber(R);
    cJSON_AddItemToArray(color_field, red_field);

    green_field = cJSON_CreateNumber(G);
    cJSON_AddItemToArray(color_field, green_field);

    blue_field = cJSON_CreateNumber(B);
    cJSON_AddItemToArray(color_field, blue_field);

    cJSON_AddItemToObject(message, "color", color_field); // color : {R, G, B}

        char *string = cJSON_Print(message);
    printf("%s\n", string);

    ws_sendframe(getClientID(), (char *)string, true);
    free(message);
}

void set_fg_color(long color)
{

    int R = (color & 0xFF);         //extract first byte
    int G = ((color >> 8) & 0xFF);  //extract second byte
    int B = ((color >> 16) & 0xFF); //extract third byte

    cJSON *red_field = NULL;
    cJSON *blue_field = NULL;
    cJSON *green_field = NULL;

    cJSON *message = cJSON_CreateObject();

    cJSON *from = cJSON_CreateString("app"); // from : app
    cJSON_AddItemToObject(message, "from", from);

    cJSON *action = cJSON_CreateString("set_fg_color"); //action : set_fg_color
    cJSON_AddItemToObject(message, "action", action);

    cJSON *color_field = cJSON_CreateArray();

    red_field = cJSON_CreateNumber(R);
    cJSON_AddItemToArray(color_field, red_field);

    green_field = cJSON_CreateNumber(G);
    cJSON_AddItemToArray(color_field, green_field);

    blue_field = cJSON_CreateNumber(B);
    cJSON_AddItemToArray(color_field, blue_field);

    cJSON_AddItemToObject(message, "color", color_field); // color : {R, G, B}

        char *string = cJSON_Print(message);
    printf("%s\n", string);

    ws_sendframe(getClientID(), (char *)string, true);
    free(message);
}


void fill_screen_bg(void)
{

    cJSON *message = cJSON_CreateObject();

    cJSON *from = cJSON_CreateString("app"); // from : app
    cJSON_AddItemToObject(message, "from", from);

    cJSON *action = cJSON_CreateString("fill_screen_bg"); //action : fill_screen_bg
    cJSON_AddItemToObject(message, "action", action);

        char *string = cJSON_Print(message);
    printf("%s\n", string);

    ws_sendframe(getClientID(), (char *)string, true);
    free(message);
}

void repaint_screen(void)
{

    cJSON *message = cJSON_CreateObject();

    cJSON *from = cJSON_CreateString("app"); // from : app
    cJSON_AddItemToObject(message, "from", from);

    cJSON *action = cJSON_CreateString("repaint_screen"); //action : repaint_screen
    cJSON_AddItemToObject(message, "action", action);

        char *string = cJSON_Print(message);
    printf("%s\n", string);

    ws_sendframe(getClientID(), (char *)string, true);
    free(message);
}

void repaint_screen_lines(int from_line, int to_line)
{

    cJSON *content = NULL;

    cJSON *from = NULL;
    cJSON *to = NULL;

    cJSON *message = cJSON_CreateObject();

    from = cJSON_CreateString("app"); // from : app
    cJSON_AddItemToObject(message, "from", from);

    cJSON *action = cJSON_CreateString("repaint_screen_lines"); //action : repaint_screen_lines
    cJSON_AddItemToObject(message, "action", action);

    from = cJSON_CreateNumber(from_line); // x : X
    cJSON_AddItemToObject(message, "from", from);

    to = cJSON_CreateNumber(to_line); // y : Y
    cJSON_AddItemToObject(message, "to", to);

        char *string = cJSON_Print(message);
    printf("%s\n", string);

    ws_sendframe(getClientID(), (char *)string, true);
    free(message);
}

void draw_horizontal_line(int pos_y, int from_x, int to_x)
{

    cJSON *y = NULL;
    cJSON *from = NULL;
    cJSON *to = NULL;

    cJSON *message = cJSON_CreateObject();

    from = cJSON_CreateString("app"); // from : app
    cJSON_AddItemToObject(message, "from", from);

    cJSON *action = cJSON_CreateString("draw_horizontal_line"); //action : draw_horizontal_line
    cJSON_AddItemToObject(message, "action", action);

    y = cJSON_CreateNumber(pos_y); // y: Y
    cJSON_AddItemToObject(message, "y", y);

    from = cJSON_CreateNumber(from_x); // x1 : X1
    cJSON_AddItemToObject(message, "x1", from);

    to = cJSON_CreateNumber(to_x); // x2 : X2
    cJSON_AddItemToObject(message, "x2", to);

        char *string = cJSON_Print(message);
    printf("%s\n", string);

    ws_sendframe(getClientID(), (char *)string, true);
    free(message);
}

void draw_vertical_line(int pos_x, int from_y, int to_y)
{

    cJSON *x = NULL;
    cJSON *from = NULL;
    cJSON *to = NULL;

    cJSON *message = cJSON_CreateObject();

    from = cJSON_CreateString("app"); // from : app
    cJSON_AddItemToObject(message, "from", from);

    cJSON *action = cJSON_CreateString("draw_vertical_line"); //action : draw_vertical_line
    cJSON_AddItemToObject(message, "action", action);

    x = cJSON_CreateNumber(pos_x); // x: X
    cJSON_AddItemToObject(message, "x", x);

    from = cJSON_CreateNumber(from_y); // y1 : Y1
    cJSON_AddItemToObject(message, "y1", from);

    to = cJSON_CreateNumber(to_y); // y2 : Y2
    cJSON_AddItemToObject(message, "y2", to);

        char *string = cJSON_Print(message);
    printf("%s\n", string);

    ws_sendframe(getClientID(), (char *)string, true);
    free(message);
}

void draw_rect(int from_x, int from_y, int to_x, int to_y)
{

    cJSON *from_x_out = NULL;
    cJSON *from_y_out = NULL;

    cJSON *to_x_out = NULL;
    cJSON *to_y_out = NULL;

    cJSON *message = cJSON_CreateObject();

    cJSON *from = cJSON_CreateString("app"); // from : app
    cJSON_AddItemToObject(message, "from", from);

    cJSON *action = cJSON_CreateString("draw_rect"); //action : draw_rect
    cJSON_AddItemToObject(message, "action", action);

    from_x_out = cJSON_CreateNumber(from_x); // x1: X1
    cJSON_AddItemToObject(message, "x1", from_x_out);

    from_y_out = cJSON_CreateNumber(from_y); // y1 : Y1
    cJSON_AddItemToObject(message, "y1", from_y_out);

    to_x_out = cJSON_CreateNumber(to_x); // x1: X1
    cJSON_AddItemToObject(message, "x2", to_x_out);

    to_y_out = cJSON_CreateNumber(to_y); // y2 : Y2
    cJSON_AddItemToObject(message, "y2", to_y_out);

        char *string = cJSON_Print(message);
    printf("%s\n", string);

    ws_sendframe(getClientID(), (char *)string, true);
    free(message);
}

void draw_filled_rect(int from_x, int from_y, int to_x, int to_y)
{

    cJSON *from_x_out = NULL;
    cJSON *from_y_out = NULL;

    cJSON *to_x_out = NULL;
    cJSON *to_y_out = NULL;

    cJSON *message = cJSON_CreateObject();

    cJSON *from = cJSON_CreateString("app"); // from : app
    cJSON_AddItemToObject(message, "from", from);

    cJSON *action = cJSON_CreateString("draw_filled_rect"); //action : draw_filled_rect
    cJSON_AddItemToObject(message, "action", action);

    from_x_out = cJSON_CreateNumber(from_x); // x1: X1
    cJSON_AddItemToObject(message, "x1", from_x_out);

    from_y_out = cJSON_CreateNumber(from_y); // y1 : Y1
    cJSON_AddItemToObject(message, "y1", from_y_out);

    to_x_out = cJSON_CreateNumber(to_x); // x1: X1
    cJSON_AddItemToObject(message, "x2", to_x_out);

    to_y_out = cJSON_CreateNumber(to_y); // y2 : Y2
    cJSON_AddItemToObject(message, "y2", to_y_out);

        char *string = cJSON_Print(message);
    printf("%s\n", string);

    ws_sendframe(getClientID(), (char *)string, true);
    free(message);
}

void draw_filled_rect_bg(int from_x, int from_y, int to_x, int to_y)
{

    cJSON *from_x_out = NULL;
    cJSON *from_y_out = NULL;

    cJSON *to_x_out = NULL;
    cJSON *to_y_out = NULL;

    cJSON *message = cJSON_CreateObject();

    cJSON *from = cJSON_CreateString("app"); // from : app
    cJSON_AddItemToObject(message, "from", from);

    cJSON *action = cJSON_CreateString("draw_filled_rect_bg"); //action : draw_filled_rect_bg
    cJSON_AddItemToObject(message, "action", action);

    from_x_out = cJSON_CreateNumber(from_x); // x1: X1
    cJSON_AddItemToObject(message, "x1", from_x_out);

    from_y_out = cJSON_CreateNumber(from_y); // y1 : Y1
    cJSON_AddItemToObject(message, "y1", from_y_out);

    to_x_out = cJSON_CreateNumber(to_x); // x1: X1
    cJSON_AddItemToObject(message, "x2", to_x_out);

    to_y_out = cJSON_CreateNumber(to_y); // y2 : Y2
    cJSON_AddItemToObject(message, "y2", to_y_out);

        char *string = cJSON_Print(message);
    printf("%s\n", string);

    ws_sendframe(getClientID(), (char *)string, true);
    free(message);
}

char *create_monitor(void)
{
    const unsigned int resolution_numbers[3][2] = {
        {1280, 720},
        {1920, 1080},
        {3840, 2160}};
    char *string = NULL;
    cJSON *name = NULL;
    cJSON *resolutions = NULL;
    cJSON *resolution = NULL;
    cJSON *width = NULL;
    cJSON *height = NULL;
    size_t index = 0;

    cJSON *monitor = cJSON_CreateObject();
    if (monitor == NULL)
    {
        goto end;
    }

    name = cJSON_CreateString("Awesome 4K");
    if (name == NULL)
    {
        goto end;
    }
    /* after creation was successful, immediately add it to the monitor,
     * thereby transferring ownership of the pointer to it */
    cJSON_AddItemToObject(monitor, "name", name);

    resolutions = cJSON_CreateArray();
    if (resolutions == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(monitor, "resolutions", resolutions);

    for (index = 0; index < (sizeof(resolution_numbers) / (2 * sizeof(int))); ++index)
    {
        resolution = cJSON_CreateObject();
        if (resolution == NULL)
        {
            goto end;
        }
        cJSON_AddItemToArray(resolutions, resolution);

        width = cJSON_CreateNumber(resolution_numbers[index][0]);
        if (width == NULL)
        {
            goto end;
        }
        cJSON_AddItemToObject(resolution, "width", width);

        height = cJSON_CreateNumber(resolution_numbers[index][1]);
        if (height == NULL)
        {
            goto end;
        }
        cJSON_AddItemToObject(resolution, "height", height);
    }

    /* 	string = cJSON_Print(monitor);
	if (string == NULL)
	{
		fprintf(stderr, "Failed to print monitor.\n");
	} */

end:
    cJSON_Delete(monitor);
    return string;
}

void *pvPortMalloc(int size) {

    return malloc((size_t) size);
}

void vibrate(int count, int on_ms, int off_ms) {

    //don't do anything for now
}

int _memclr(void *buf, int len) {

    return memset(buf, 0, len);
}

void vPortFree(void *pv) {

    free(pv);
}

int _strcpy(char *destptr, const char *srcptr) {

    return strcpy(destptr, srcptr);
}

int _strncpy(char *destptr, const char *srcptr, int num) {

    return strncpy(destptr, srcptr, num);
}

void set_display_state_value(int state_1, int state) {

    // don't do anything for now
}

int set_graph_callback_to_ram_1(void) {

    return 1;
}

int show_watchface(void) {

    return 0;       // big nono, yolo
}

void set_update_period(int cmd, int period) {

    cJSON *message = cJSON_CreateObject();

    cJSON *from = cJSON_CreateString("app"); // from : app
    cJSON_AddItemToObject(message, "from", from);

    cJSON *action = cJSON_CreateString("set_redraw_time"); //action : set_redraw_time
    cJSON_AddItemToObject(message, "action", action);

    cJSON *time = cJSON_CreateNumber(period);
    cJSON_AddItemToObject(message, "time", time);

    cJSON *cmd_out = cJSON_CreateBool(cmd);
    cJSON_AddItemToObject(message, "cmd", cmd_out);
        char *string = cJSON_Print(message);
    printf("%s\n", string);

    ws_sendframe(getClientID(), (char *)string, true);
    free(message);
}

void reg_menu(void *regmenu_, int param) {

    // nothing
}

int get_var_menu_overlay(void) {

    return 0; // ignoring call/notification overlay for now
}

app_data_t *get_app_data_ptr(void) {

    return app_data_global;
}

int set_app_data_ptr(app_data_t *app_data) {

    app_data_global = app_data;
    return 1;
}

int show_menu_animate(void *show_menu_function, int param, int animate) {

    exit(7);
}
