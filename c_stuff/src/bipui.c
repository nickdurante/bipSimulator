
#ifdef __SIMULATION__
#include <simulator.h>
#endif

#include "bipui.h"

void blank_screen(void)
{

    set_bg_color(getLongColour(COLOR_SH_BLACK));
    fill_screen_bg();
}

Viewport_ *getCurrentViewport(app_data_t *app_data)
{

    return &app_data->vp;
}

void destroyViewport(Viewport_ *vp)
{

    short i;

    for (i = 0; i < vp->windowIndex; i++)
    {
        destroyWindow(vp->windowArray[i]);
    }
}

void initializeWindow(Window_ *window)
{

    window->layerIndex = -1;
}

TextBox_ *createTextbox(void)
{

    TextBox_ *temp = (TextBox_ *)pvPortMalloc(sizeof(TextBox_));

    if (temp == NULL)
        vibrate(2, 50, 50);
    else
    {
        _memclr(temp, sizeof(TextBox_));
    }

    return temp;
}

void drawTextBox(TextBox_ *box)
{

    set_bg_color(getLongColour(box->background));
    set_fg_color(getLongColour(box->colour));

    draw_filled_rect_bg(box->topLeft.x, box->topLeft.y,
                        box->bottomRight.x, box->bottomRight.y);

    if(box->centerText) {

        text_out_center(box->body,                                      // the text
                    (int)(box->topLeft.x + box->bottomRight.x) / 2, // median
                    (int)box->topLeft.y );                       // slightly up
    } else {

        text_out(box->body, (int) box->topLeft.x, (int) box->topLeft.y);
    }
}

void refreshWindow(Window_ *window, char repaint)
{
    short i;

    // render layers progressively
    for (i = 0; i < window->layerIndex; i++)
    {

        if (i == 0)
        { //hack to have transparency
            set_bg_color(getLongColour(window->layerArray[i]->backgroundColour));
            fill_screen_bg();
        }

        if (window->layerArray[i]->visible)
            refreshLayer(window->layerArray[i], 0); // do not repaint since we're drawing more stuff on top of this layer
    }

    // render window name
    if (window->nameVisible)
    {

        set_bg_color(getLongColour(window->layerArray[0]->backgroundColour)); // for legibility
        if (window->layerArray[0]->backgroundColour == COLOR_SH_WHITE || window->layerArray[0]->backgroundColour == COLOR_SH_YELLOW)
            set_fg_color(getLongColour(COLOR_SH_BLACK));
        else
            set_fg_color(getLongColour(COLOR_SH_WHITE));

        text_out_center(window->name, VIDEO_X / 2, 2);
    }

    //set_graph_callback_to_ram_1();
    //repaint_screen_lines(0, VIDEO_Y);

    if (repaint)
        repaint_screen();
}

Window_ *getWindowByIndex(short index) {

    short currentIndex = getAppData()->vp.windowIndex;

    if(index < currentIndex)
        return getAppData()->vp.windowArray[index];

}

short setWindowName(char *name, Window_ *window)
{

    window->nameVisible = 1;

    return (short)_strcpy(window->name, name);
}

Window_ *createWindow(void)
{

    Window_ *temp = (Window_ *)pvPortMalloc(sizeof(Window_));

    if (temp == NULL)
        vibrate(2, 50, 50);
    else
    {
        _memclr(temp, sizeof(Window_));
    }

    return temp;
}

void destroyWindow(Window_ *window)
{
    short i;

    for (i = 0; i < window->layerIndex; i++)
    {

        destroyLayer(window->layerArray[i]);
    }
}

Window_ *addWindowToViewport(Viewport_ *vp)
{

    Window_ *window;

    if (vp->windowIndex >= MAX_NUM_WINDOWS)
    {

        printErrorText("VIEWPORT FULL");
    }
    else
    {
        window = createWindow();
        vp->windowArray[(short)vp->windowIndex] = window;
        vp->windowIndex++;

        return window;
    }

    return 0; // should never get here
}

short removeWindowFromViewport(Viewport_ *vp)
{

    if (vp->windowIndex == 0)
    {

        printErrorText("NO WINDOWS");
        return 1;
    }
    else
    {
        destroyWindow(vp->windowArray[(short)vp->windowIndex]);
        vp->windowIndex--;
        return 0;
    }
}

void linkWindows(Window_ *windowReference, Way_ way, Window_ *windowToLink)
{
    // it's not wrong, if you swipe "UP", you should go to the "DOWN" layer
    switch (way)
    {
    case UP:
        windowReference->neighbors[DOWN] = (int)windowToLink;
        windowToLink->neighbors[UP] = (int)windowReference;
        break;

    case DOWN:
        windowReference->neighbors[UP] = (int)windowToLink;
        windowToLink->neighbors[DOWN] = (int)windowReference;
        break;

    case LEFT:
        windowReference->neighbors[RIGHT] = (int)windowToLink;
        windowToLink->neighbors[LEFT] = (int)windowReference;
        break;

    case RIGHT:
        windowReference->neighbors[LEFT] = (int)windowToLink;
        windowToLink->neighbors[RIGHT] = (int)windowReference;
        break;
    default:

        break;
    }
}

void processTap(Layer_ *layer, int x, int y)
{

    short i;
    Button_ *temp;
    //Layer_ *layer = &window->layerArray[window->layerIndex];

    for (i = 0; i < layer->buttonIndex; i++)
    {
        temp = layer->buttonArray[i];
        // was the tap inside the button?
        if (temp->topLeft.x < x && temp->bottomRight.x > x && temp->topLeft.y < y && temp->bottomRight.y > y)
        {
            vibrate(1, 50, 0); // vibrate if successful
            if (temp->callbackFunction != 0)
                temp->callbackFunction(layer, i);
            else
                printErrorText("CALLBACK UNDEFINED");
        }
    }
}

void processSwipe(Window_ *window, char gesture)
{
    Way_ way = (Way_)gesture - 2; // casting the gesture to way type

    if (window->callbackFunction != 0)
    {
        window->callbackFunction(window, way);
    }
    else
    {
        printErrorText("CALLBACK UNDEFINED");
    }
}

Layer_ *createLayer(void)
{

    Layer_ *temp = (Layer_ *)pvPortMalloc(sizeof(Layer_));

    if (temp == NULL)
        vibrate(2, 50, 50);
    else
    {
        _memclr(temp, sizeof(Layer_));
        //temp->params.refreshDelay = 0; // initializing the params for the layer
    }

    return temp;
}

void destroyLayer(Layer_ *layer)
{

    short i = 0;

    for (i = 0; i < layer->buttonIndex; i++)
        vPortFree(layer->buttonArray[i]);

    vPortFree(layer->textBox);

    vPortFree(layer);
}

void refreshLayer(Layer_ *layer, short repaint)
{

    short i;
    for (i = 0; i < layer->buttonIndex; i++)
    {
        if (layer->buttonArray[i]->visible)
            drawButton(layer->buttonArray[i]);
    }

    if (layer->textBox) // uninitialized pointers == 0
        if (layer->textBox->visible)
            drawTextBox(layer->textBox);

    if (repaint)
        //repaint_screen_lines(0, VIDEO_Y);
        repaint_screen();
}
Layer_ *addLayerToWindow(Window_ *window) // aka "push to layer stack"
{

    Layer_ *layer;

    if (window->layerIndex >= MAX_NUM_LAYERS)
    {
        //window full
        printErrorText("WINDOW FULL");
    }
    else
    {                          // add layer to window - aka putting layer pointer in layer array
        layer = createLayer(); //malloc layer
        window->layerArray[(short)window->layerIndex] = layer;
        window->layerIndex++;

        layer->visible = 1; // layers visible by default

        return layer; // returning pointer
    }

    return 0; // should never get here
}

short removeLayerFromWindow(Window_ *window) // aka "pop layer stack"
{

    if (window->layerIndex == 0)
    {
        //window full
        printErrorText("NO LAYERS");
        return 1;
    }
    else
    {
        destroyLayer(window->layerArray[(short)window->layerIndex]); // free layer memory

        window->layerIndex--;
        return 0;
    }
}

void movePoint(Point_ *point, Way_ dir, short space)
{

    switch (dir)
    {

    case UP:

        point->y -= space;
        break;
    case DOWN:

        point->y += space;
        break;
    case LEFT:

        point->x -= space;
        break;
    case RIGHT:

        point->x += space;
        break;

    default:
    {
    };
    }

    if (point->x < 0)
        point->x = 0;

    else if (point->x > VIDEO_X)
        point->x = VIDEO_X;

    if (point->y < 0)
        point->y = 0;

    if (point->y > VIDEO_Y)
        point->y = VIDEO_Y;

    return;
}

Button_ *addButtonToLayer(Layer_ *layer)
{

    Button_ *button;

    if (layer->buttonIndex >= MAX_NUM_BUTTONS)
    {
        // layer full
        printErrorText("DATABASE FULL");
    }
    else
    { // add button to layer
        button = pvPortMalloc(sizeof(Button_));
        layer->buttonArray[layer->buttonIndex] = button;
        layer->buttonIndex++;
        return button;
    }

    return (Button_ *)0; // should never get here
}

void setButton(Button_ *button, Point_ topLeft, Point_ bottomRight, char *label, short border,
               short filling, short textColour, void *callbackFunction, Style_t style)
{ // populating the struct

    button->topLeft.x = topLeft.x;
    button->topLeft.y = topLeft.y;
    button->bottomRight.x = bottomRight.x;
    button->bottomRight.y = bottomRight.y;
    _strcpy(button->label, label);
    button->border = border;
    button->filling = filling;
    button->textColour = textColour;
    button->callbackFunction = callbackFunction;
    button->params.style = style;

    button->visible = 1;
}

void drawButton(Button_ *button) // graphics of the button
{
    // fill_screen_bg();
    // load_font();

    Button_ temp = *button;
    Style_t style = temp.params.style;

    set_bg_color(getLongColour(temp.filling));
    set_fg_color(getLongColour(temp.border));

    switch (style)
    {
    case BUTTON_STYLE_DEFAULT_SQUARED:

        draw_filled_rect_bg(temp.topLeft.x,
                            temp.topLeft.y,
                            temp.bottomRight.x,
                            temp.bottomRight.y);

        draw_rect(temp.topLeft.x,
                  temp.topLeft.y,
                  temp.bottomRight.x,
                  temp.bottomRight.y);

        break;

    case BUTTON_STYLE_SQUARED_NOBORDER:

        draw_filled_rect_bg(temp.topLeft.x,
                            temp.topLeft.y,
                            temp.bottomRight.x,
                            temp.bottomRight.y);
        break;

    case BUTTON_STYLE_ROUNDED_NOBORDER:

        draw_filled_rect_bg(temp.topLeft.x + 2,
                            temp.topLeft.y + 2,
                            temp.bottomRight.x - 2,
                            temp.bottomRight.y - 2);

        //set_fg_color(getLongColour(temp.filling));      // lines are drawn in the bg colour, apparently

        draw_horizontal_line(temp.topLeft.y, temp.topLeft.x + 2, temp.bottomRight.x - 2);
        draw_horizontal_line(temp.bottomRight.y, temp.topLeft.x + 2, temp.bottomRight.x - 2);
        // scusa Claudio, ma così è più efficiente
        draw_vertical_line(temp.topLeft.x, temp.topLeft.y + 2, temp.bottomRight.y - 2);
        draw_vertical_line(temp.bottomRight.x, temp.topLeft.y + 2, temp.bottomRight.y - 2);

        break;

    default:
    {
        // should never get here
    };
    }

    if(temp.textColour != temp.filling)
        set_fg_color(getLongColour(temp.textColour)); 
    else
        set_fg_color(~getLongColour(temp.textColour));

    text_out_center(temp.label,                                     // the text
                    (int)(temp.topLeft.x + temp.bottomRight.x) / 2, // median
                    (int)(temp.topLeft.y + temp.bottomRight.y) / 2 - 10);
}

void caffeine(Caffeine_t coffee)
{

    // turn off timeout and exit
    set_display_state_value(8, 1);
    set_display_state_value(2, 1);

    if (coffee) // backlight always on?
        set_display_state_value(4, 1);
}

long getLongColour(short colour)
{

    switch ((unsigned short) colour)
    {

    case COLOR_SH_AQUA:
        return COLOR_AQUA;
    case COLOR_SH_BLACK:
        return COLOR_BLACK;
    case COLOR_SH_BLUE:
        return COLOR_BLUE;
    case COLOR_SH_GREEN:
        return COLOR_GREEN;
    case COLOR_SH_MASK:
        return COLOR_MASK;
    case COLOR_SH_PURPLE:
        return COLOR_PURPLE;
    case COLOR_SH_RED:
        return COLOR_RED;
    case COLOR_SH_WHITE:
        return COLOR_WHITE;
    case COLOR_SH_YELLOW:
        return COLOR_YELLOW;
    default:
    {
        // should never get here..
    };
    };
    return 0;
}

void setLayerBackground(Layer_ *layer, short colour)
{

    layer->backgroundColour = colour;
}

short getLayerBackground(Layer_ *layer)
{

    return layer->backgroundColour;
}

app_data_t *getAppData(void)
{

#ifdef __SIMULATION__
    app_data_t *app_data = get_app_data_ptr();
    app_data_t **app_data_p = &app_data;
#else
    app_data_t **app_data_p = get_ptr_temp_buf_2(); //	pointer to a pointer to screen data
    app_data_t *app_data = *app_data_p;             //	pointer to screen data
#endif

    return app_data;
}

// DEBUG functions

void printErrorText(char *error)
{

    TextBox_ tempText = DEFAULT_TEXTBOX;
    _strcpy(tempText.body, error);

    drawTextBox(&tempText);
    repaint_screen();
}
