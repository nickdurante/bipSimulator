
#ifdef __SIMULATION__
#include <simulator.h>
#endif

#include <bipui.h>

void blank_screen(void)
{

    set_bg_color(getLongColour(COLOR_SH_BLACK));
    fill_screen_bg();
}

Viewport_ *getCurrentViewport(app_data_t *app_data)
{

    return &app_data->vp;
}

/* void setViewportLayer(Viewport_ *vp, Layer_ *layer, Way_ dir)
{

    switch (dir)
    {

    case UP:

        vp->up = layer;
        break;

    case DOWN:

        vp->down = layer;
        break;

    case LEFT:

        vp->left = layer;
        break;

    case RIGHT:

        vp->right = layer;
        break;

    default:
    {
        // touch wood
    };
    }
} */

/* void destroyViewport(Viewport_ *vp)
{

    //vPortFree(vp);
}

Viewport_ *createViewport(void)
{

    Viewport_ *temp = (Viewport_ *)pvPortMalloc(sizeof(Viewport_));

    if (temp == NULL)
        vibrate(2, 50, 50);
    else
    {
        _memclr(temp, sizeof(Viewport_));
        initializeLayer(temp->active);
    }

    return temp;
}

void initializeViewport(Viewport_ *vp)
{

    initializeLayer(vp->active);
} */

void initializeWindow(Window_ *window)
{

    window->layerIndex = -1;
}

void drawTextBox(TextBox_ *box)
{

    set_bg_color(getLongColour(box->background));
    set_fg_color(getLongColour(box->colour));

    text_out_center(box->body,                                      // the text
                    (int)(box->topLeft.x + box->bottomRight.x) / 2, // median
                    (int)box->topLeft.y - 2);                       // slightly up
}

void refreshWindow(Window_ *window)
{
    short i;
    for (i = 0; i < window->layerIndex; i++) {

        if (i == 0)
        { //hack to have transparency
            set_bg_color(getLongColour(window->layerArray[i]->backgroundColour));
            fill_screen_bg();
        }
    refreshLayer(window->layerArray[i], 0); // do not repaint since we're drawing more stuff on top of this layer
    }
    set_graph_callback_to_ram_1();
    //repaint_screen_lines(0, VIDEO_Y);
    repaint_screen();
}

/* void spawnLayer(Layer_ *layer, Window_ *window)
{

    if (!addLayerToWindow(layer, window))
    {

        initializeLayer(layer);
    }
} */

short removeLayerFromWindow(Window_ *window) // aka "pop layer stack"
{

    if (window->layerIndex == -1)
    {
        //window full
        printErrorText("NO LAYERS");
        return 1;
    }
    else
    {
        destroyLayer(window->layerArray[window->layerIndex]); // free layer memory

        window->layerIndex--;
        return 0;
    }
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
        window->layerArray[window->layerIndex] = layer;
        window->layerIndex++;

        return layer; // returning pointer
    }
}

void setLayerTextBox(Layer_ *layer, TextBox_ tbox)
{

    layer->textBox = tbox;
};

void initializeLayer(Layer_ *layer)
{

    layer->backgroundColour = COLOR_SH_BLACK;
    layer->buttonIndex = 0;
    layer->visible = 1;
}

short getCurrentLayerIndex(Window_ *window)
{

    return window->layerIndex;
}

/* Window_ *getCurrentWindow(app_data_t *app_data) {

    return(&app_data->mainWindow);
} */

short findHighestOpaqueLayer(Window_ *window)
{

    short highest = 0;
    short i = 0;
    for (i = 0; i < window->layerIndex; i++)
    {
        if (window->layerArray[i]->backgroundColour != COLOR_SH_MASK)
            highest = i;
    }

    return highest;
}

void processTap(Layer_ *layer, int x, int y)
{

    short i;
    Button_ temp;
    //Layer_ *layer = &window->layerArray[window->layerIndex];

    for (i = 0; i < layer->buttonIndex; i++)
    {
        temp = layer->buttonArray[i];
        // was the tap inside the button?
        if (temp.topLeft.x < x && temp.bottomRight.x > x && temp.topLeft.y < y && temp.bottomRight.y > y)
        {
            //vibrate(1, 50, 0); // vibrate if successful
            // set_close_timer(5); // paramose il culo
            temp.callbackFunction(layer, i);
        }
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

#ifdef __SIMULATION__
    free(layer);
#else
    vPortFree(layer);
#endif
}

void refreshLayer(Layer_ *layer, short repaint)
{

    //set_bg_color(layer->backgroundColour);
    //fill_screen_bg();
    //    set_graph_callback_to_ram_1();

    short i;
    for (i = 0; i < layer->buttonIndex; i++)
    {

        drawButton(&layer->buttonArray[i]);
    }

    drawTextBox(&layer->textBox);

    if (repaint)
        //repaint_screen_lines(0, VIDEO_Y);
        repaint_screen();
}

/* Layer_ *getTopLayer(app_data_t *app_data){

    short top = app_data->mainWindow.index;

    top++;

    return &app_data->mainWindow.layerArray[top];
} */

Window_ *getActiveWindow(Viewport_ *vp)
{

    switch (vp->active)
    {

    case UP:

        return &vp->up;
        break;

    case DOWN:

        return &vp->down;
        break;

    case LEFT:

        return &vp->left;
        break;

    case RIGHT:

        return &vp->right;
        break;

    case CENTER:

        return &vp->center;
        break;

    default:
    {
        // touch wood
    };
    };
}

Button_ mirrorInDirectionButton(Button_ *button, Way_ dir)
{

    Button_ temp = *button;

    switch (dir)
    {

    case UP:

        temp.topLeft.x = abssub(VIDEO_Y, temp.bottomRight.x);
        temp.bottomRight.x = abssub(VIDEO_Y, temp.topLeft.x);
        break;

    case DOWN:

        temp.topLeft.x = VIDEO_Y - temp.bottomRight.x;
        temp.bottomRight.x = VIDEO_Y - temp.topLeft.x;
        break;

    case LEFT:

        temp.topLeft.y = abssub(VIDEO_X, temp.bottomRight.y);
        temp.bottomRight.y = abssub(VIDEO_X, temp.topLeft.y);
        break;

    case RIGHT:

        temp.topLeft.y = VIDEO_X - temp.bottomRight.y;
        temp.bottomRight.y = VIDEO_X - temp.topLeft.y;
        break;

    default:
    {
        // touch wood
    };
    };

    return temp;
}

void moveInDirectionButton(Button_ *button, Way_ dir, short separation)
{

    //Button_ temp = *button;

    short width = (short)abssub(button->topLeft.x, button->bottomRight.x);  // considering the case when (a,b) is the bottom right point and
    short height = (short)abssub(button->topLeft.y, button->bottomRight.y); //      not the top left

    // a = x1, b = y1, c = x2, d = y2

    switch (dir)
    {

    case UP:

        button->topLeft.y -= (height + separation);
        button->bottomRight.y -= (height + separation);
        break;

    case DOWN:

        button->topLeft.y += (height + separation);
        button->bottomRight.y += (height + separation);
        break;

    case LEFT:

        button->topLeft.x -= (width + separation);
        button->bottomRight.x -= (width + separation);
        break;

    case RIGHT:

        button->topLeft.x += (width + separation);
        button->bottomRight.x += (width + separation);
        break;

    default:
    {
        // touch wood
    };
    };

    if (button->topLeft.x < 0) // display manager will freak out and hang when called with
        button->topLeft.x = 0; //       negative values, so this limits the max range

    if (button->topLeft.y < 0)
        button->topLeft.y = 0;

    if (button->bottomRight.x < 0)
        button->bottomRight.x = 0;

    if (button->bottomRight.y < 0)
        button->bottomRight.y = 0;

    //return temp;
}

/* void spawnButton(Button_ *button, Layer_ *layer)
{

    if (!addButtonToLayer(button, layer))
    {

        drawButton(button);
    }
} */

short addButtonToLayer(Button_ *button, Layer_ *layer)
{

    if (layer->buttonIndex >= MAX_NUM_BUTTONS)
    {
        // layer full
        printErrorText("DATABASE FULL");
        return 1;
    }
    else
    { // add button to layer
        layer->buttonArray[layer->buttonIndex] = *button;
        layer->buttonIndex++;
        return 0;
    }
}

void initButton(Button_ *button, Point_ topLeft, Point_ bottomRight, char *label, short border,
                short filling, short text, void *callbackFunction)
{ // populating the struct

    button->topLeft.x = topLeft.x;
    button->topLeft.y = topLeft.y;
    button->bottomRight.x = bottomRight.x;
    button->bottomRight.y = bottomRight.y;
    _strcpy(button->label, label);
    button->border = border;
    button->filling = filling;
    button->textColour = text;
    button->callbackFunction = callbackFunction;
    button->params.style = BUTTON_STYLE_ROUNDED_NOBORDER;
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

    set_fg_color(getLongColour(temp.textColour)); // Text is universal for now

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

    switch (colour)
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

void setActiveWindowViewport(Viewport_ *vp, Way_ way)
{ // sets the active window

    vp->active = way;
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

/* void setActiveOverlayValue(Layer_ *layer)
{

    layer->params.overlay = 1;
}

void resetActiveOverlayValue(Layer_ *layer)
{

    layer->params.overlay = 0;
}

char getActiveOverlayValue(Layer_ *layer)
{

    return layer->params.overlay; // that's a long return
} */

// DEBUG functions

void printErrorText(char *error)
{

    TextBox_ tempText = DEFAULT_TEXTBOX;
    _strcpy(tempText.body, error);

    drawTextBox(&tempText);
}
