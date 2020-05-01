/*
	
	Layers Demo for BipOS custom firmware
	
	by Enrico Rovere - https://github.com/enricorov/bipos-layersdemo
	
*/

#define CENTER_WINDOW_INDEX 0
#define LEFT_WINDOW_INDEX 1
#define RIGHT_WINDOW_INDEX 2
#define UP_WINDOW_INDEX 3
#define DOWN_WINDOW_INDEX 4
#define SETTINGS_WINDOW_INDEX 5
#define HELP_WINDOW_INDEX 6

#include "bipos-layersdemo.h"

#ifdef __SIMULATION__
#include <stdio.h>
#include <string.h>
#endif
//	screen menu structure - each screen has its own
struct regmenu_ screen_data = {
    55,                 //	curr_screen - main screen number, value 0-255, for custom windows it is better to take from 50 and above
    1,                  //	swipe_scr - auxiliary screen number (usually 1)
    0,                  //	overlay - 0
    interactionHandler, //          - pointer to the handler of interaction (touch, swipe, long press)
    key_press_screen,   //	        - handler of short button press
    refreshScreen,      //	        - timer callback function and..
    0,                  //	            ..the variable passed to it
    show_screen,        //	        - function writing to video buffer and..
    0,                  //              ..the variable passed to it
    0                   //	        - handler of long button press
};

#ifdef __SIMULATION__
int main_app(int param0)
{
#else
int main(int param0, char **argv)
{                                                   //	here the variable argv is not defined
#endif
    show_screen((void *)param0);
}

// CALLBACK FUNCTIONS - functions associated to objects i.e. buttons or windows

void overlayWindowCallbackFunction(Window_ *window, Way_ way)
{
    Window_ *centerWindow = getAppData()->vp.windowArray[CENTER_WINDOW_INDEX];

    if (way == DOWN)
        getAppData()->vp.active = centerWindow;

    refreshWindow(getAppData()->vp.active, 1);
}

void simpleWindowCallbackFunction(Window_ *window, Way_ way)
{
    if (window->neighbors[way] != 0)
        getAppData()->vp.active = (Window_ *)window->neighbors[(int)way];
}

void changeWindowColourCallbackFunction(Layer_ *layer, short button_id){

    short nextColour = (unsigned short) rand() % COLORS_COUNT; // colour number

    layer->buttonArray[button_id]->filling = nextColour;
    getAppData()->vp.windowArray[button_id]->layerArray[0]->backgroundColour = nextColour;    

    refreshWindow(getAppData()->vp.active, 1);

};

void goToSettingsCallbackFunction(Layer_ *layer, short button_id)
{
    Window_ *settingsWindow = getAppData()->vp.windowArray[SETTINGS_WINDOW_INDEX];

    getAppData()->vp.active = settingsWindow;

    refreshWindow(getAppData()->vp.active, 1);
}

void goToHelpCallbackFunction(Layer_ *layer, short button_id)
{
    Window_ *tempWindow = getAppData()->vp.windowArray[HELP_WINDOW_INDEX];

    getAppData()->vp.active = tempWindow;

    refreshWindow(getAppData()->vp.active, 1);
}

// CONSTRUCTORS - Defining elements, put the messy intializations here

void layerSettingsConstructor(Layer_ *layerSettings)
{

    setLayerBackground(layerSettings, COLOR_SH_BLACK);

    Button_ *tempButton;


    tempButton = addButtonToLayer(layerSettings);           // button 0

    setButton(tempButton,
        BIPUI_UNDER_WINDOW_LABEL_LEFT_POINT,
        BIPUI_UNDER_WINDOW_LABEL_RIGHT_POINT,
        "Center",
        COLOR_SH_WHITE,
        getWindowByIndex(CENTER_WINDOW_INDEX)->layerArray[0]->backgroundColour,
        COLOR_SH_BLACK,
        changeWindowColourCallbackFunction,
        BUTTON_STYLE_DEFAULT_SQUARED);

    movePoint(&tempButton->bottomRight, DOWN, DEFAULT_BUTTON_HEIGHT); 

    tempButton = addButtonToLayer(layerSettings);  // button 1

    setButton(tempButton,
        BIPUI_BOTTOM_LEFT_POINT,
        BIPUI_BOTTOM_LEFT_POINT,
        getWindowByIndex(LEFT_WINDOW_INDEX)->name,
        COLOR_SH_WHITE,
        getWindowByIndex(LEFT_WINDOW_INDEX)->layerArray[0]->backgroundColour,
        COLOR_SH_BLACK,
        changeWindowColourCallbackFunction,
        BUTTON_STYLE_DEFAULT_SQUARED);

    movePoint(&tempButton->topLeft, UP, DEFAULT_BUTTON_HEIGHT);
    movePoint(&tempButton->bottomRight, RIGHT, DEFAULT_BUTTON_WIDTH);

    tempButton = addButtonToLayer(layerSettings);           // button 2

    setButton(tempButton,
        BIPUI_BOTTOM_RIGHT_POINT,
        BIPUI_BOTTOM_RIGHT_POINT,
        getWindowByIndex(RIGHT_WINDOW_INDEX)->name,
        COLOR_SH_WHITE,
        getWindowByIndex(RIGHT_WINDOW_INDEX)->layerArray[0]->backgroundColour,
        COLOR_SH_BLACK,
        changeWindowColourCallbackFunction,
        BUTTON_STYLE_DEFAULT_SQUARED);

    movePoint(&tempButton->topLeft, LEFT, DEFAULT_BUTTON_WIDTH);
    movePoint(&tempButton->topLeft, UP, DEFAULT_BUTTON_HEIGHT); 

    tempButton = addButtonToLayer(layerSettings);           // button 3

    setButton(tempButton,
        BIPUI_BOTTOM_LEFT_POINT,
        BIPUI_BOTTOM_LEFT_POINT,
        getWindowByIndex(UP_WINDOW_INDEX)->name,
        COLOR_SH_WHITE,
        getWindowByIndex(UP_WINDOW_INDEX)->layerArray[0]->backgroundColour,
        COLOR_SH_BLACK,
        changeWindowColourCallbackFunction,
        BUTTON_STYLE_DEFAULT_SQUARED);

    movePoint(&tempButton->topLeft, UP, DEFAULT_BUTTON_HEIGHT);
    movePoint(&tempButton->bottomRight, RIGHT, DEFAULT_BUTTON_WIDTH);

    movePoint(&tempButton->topLeft, UP, DEFAULT_BUTTON_HEIGHT + 6);
    movePoint(&tempButton->bottomRight, UP, DEFAULT_BUTTON_HEIGHT + 6);

    tempButton = addButtonToLayer(layerSettings);           // button 4

    setButton(tempButton,
        BIPUI_BOTTOM_RIGHT_POINT,
        BIPUI_BOTTOM_RIGHT_POINT,
        getWindowByIndex(DOWN_WINDOW_INDEX)->name,
        COLOR_SH_WHITE,
        getWindowByIndex(DOWN_WINDOW_INDEX)->layerArray[0]->backgroundColour,
        COLOR_SH_BLACK,
        changeWindowColourCallbackFunction,
        BUTTON_STYLE_DEFAULT_SQUARED);

    movePoint(&tempButton->topLeft, LEFT, DEFAULT_BUTTON_WIDTH);
    movePoint(&tempButton->topLeft, UP, DEFAULT_BUTTON_HEIGHT); 

    movePoint(&tempButton->topLeft, UP, DEFAULT_BUTTON_HEIGHT + 6);
    movePoint(&tempButton->bottomRight, UP, DEFAULT_BUTTON_HEIGHT + 6);


}

void layerHelpConstructor(Layer_ *layerHelp)
{

    setLayerBackground(layerHelp, COLOR_SH_BLACK);

    TextBox_ *tbox = createTextbox();

    tbox->topLeft = BIPUI_UNDER_WINDOW_LABEL_LEFT_POINT;
    tbox->bottomRight = BIPUI_BOTTOM_RIGHT_POINT;

    tbox->background = COLOR_SH_BLACK;
    tbox->colour = COLOR_SH_WHITE;
    tbox->visible = 1;
    tbox->centerText = 0;

    _strncpy(tbox->body, "All the windows were\ndefined in begin(). \n\nUsing the \"Settings\"\nbutton you can change \nthe background of \nthe other windows.", MAX_SIZE_TEXT_BOX);
    
    layerHelp->textBox = tbox;

}

void layerCenterConstructor(Layer_ *layerMain)
{

    TextBox_ *temp = createTextbox(); // allocating textbox

    temp->topLeft = BIPUI_CENTER_LEFT_POINT; // setting parameters
    temp->bottomRight = BIPUI_CENTER_RIGHT_POINT;
    temp->background = COLOR_SH_MASK;
    temp->colour = COLOR_SH_BLACK;
    _strcpy(temp->body, "Swipe in\nany direction");
    temp->visible = 1; // do not forget to set visibility
    temp->centerText = 1;

    layerMain->textBox = temp; // assigning pointer to layer

    movePoint(&temp->topLeft, UP, DEFAULT_TEXT_HEIGHT);
    movePoint(&temp->bottomRight, DOWN, DEFAULT_TEXT_HEIGHT);

    short width = 80;
    short height = 30;

    Button_ *placeholderButton = addButtonToLayer(layerMain);

    setButton(placeholderButton,
              BIPUI_BOTTOM_LEFT_POINT,
              BIPUI_BOTTOM_LEFT_POINT,
              "SETTINGS",
              COLOR_SH_WHITE,
              COLOR_SH_RED,
              COLOR_SH_WHITE,
              goToSettingsCallbackFunction,
              BUTTON_STYLE_SQUARED_NOBORDER);

    movePoint(&placeholderButton->topLeft, UP, height);
    movePoint(&placeholderButton->bottomRight, RIGHT, width);

    placeholderButton = addButtonToLayer(layerMain);

    setButton(placeholderButton,
              BIPUI_BOTTOM_RIGHT_POINT,
              BIPUI_BOTTOM_RIGHT_POINT,
              "HELP",
              COLOR_SH_WHITE,
              COLOR_SH_BLUE,
              COLOR_SH_WHITE,
              goToHelpCallbackFunction,
              BUTTON_STYLE_SQUARED_NOBORDER);

    movePoint(&placeholderButton->topLeft, UP, height);
    movePoint(&placeholderButton->topLeft, LEFT, width);
}

void begin(app_data_t *app_data)
{

    _srand(get_tick_count());

    // creating windows here, adding them to the viewport allocates them and increments the index

    Viewport_ *vp = &app_data->vp;

    Window_ *centerWindow = addWindowToViewport(vp); // index 0
    setWindowName("Layers Demo", centerWindow);
    centerWindow->callbackFunction = simpleWindowCallbackFunction;

    Window_ *leftWindow = addWindowToViewport(vp); // index 1
    setWindowName("Left", leftWindow);
    leftWindow->callbackFunction = simpleWindowCallbackFunction;

    Window_ *rightWindow = addWindowToViewport(vp); // index 2
    setWindowName("Right", rightWindow);
    rightWindow->callbackFunction = simpleWindowCallbackFunction;

    Window_ *upWindow = addWindowToViewport(vp); // index 3
    setWindowName("Up", upWindow);
    upWindow->callbackFunction = simpleWindowCallbackFunction;

    Window_ *downWindow = addWindowToViewport(vp); // index 4
    setWindowName("Down", downWindow);
    downWindow->callbackFunction = simpleWindowCallbackFunction;

    Window_ *settingsWindow = addWindowToViewport(vp); // index 5
    setWindowName("Tap to change colour", settingsWindow);
    settingsWindow->callbackFunction = overlayWindowCallbackFunction;

    Window_ *helpWindow = addWindowToViewport(vp); //index 6
    setWindowName("Help", helpWindow);
    helpWindow->callbackFunction = overlayWindowCallbackFunction;

    // linking windows for swipe navigation

    linkWindows(centerWindow, LEFT, leftWindow);
    linkWindows(centerWindow, RIGHT, rightWindow);
    linkWindows(centerWindow, UP, upWindow);
    linkWindows(centerWindow, DOWN, downWindow);

    // Creating (i.e. allocating) layers and using constructors to define their elements

    Layer_ *layerTemp = addLayerToWindow(centerWindow);
    layerTemp->backgroundColour = COLOR_SH_BLACK;
    layerCenterConstructor(layerTemp);

    layerTemp = addLayerToWindow(leftWindow);
    layerTemp->backgroundColour = COLOR_SH_RED;

    layerTemp = addLayerToWindow(rightWindow);
    layerTemp->backgroundColour = COLOR_SH_AQUA;

    layerTemp = addLayerToWindow(upWindow);
    layerTemp->backgroundColour = COLOR_SH_BLUE;

    layerTemp = addLayerToWindow(downWindow);
    layerTemp->backgroundColour = COLOR_SH_YELLOW;

    layerTemp = addLayerToWindow(settingsWindow);
    layerSettingsConstructor(layerTemp);

    layerTemp = addLayerToWindow(helpWindow);
    layerHelpConstructor(layerTemp);

    vp->active = centerWindow;
}

void end(app_data_t *app_data)
{

    destroyViewport(&app_data->vp);
}

// Utility functions

void show_screen(void *param0)
{
#ifdef __SIMULATION__
    app_data_t *app_data = get_app_data_ptr();
    app_data_t **app_data_p = &app_data;
#else
    app_data_t **app_data_p = get_ptr_temp_buf_2(); //	pointer to a pointer to screen data
    app_data_t *app_data;                           //	pointer to screen data
#endif

    Elf_proc_ *proc;

    // check the source at the procedure launch
    if ((param0 == *app_data_p) && get_var_menu_overlay())
    { // return from the overlay screen (incoming call, notification, alarm, target, etc.)

        app_data = *app_data_p; //	the data pointer must be saved for the deletion
                                //	release memory function reg_menu
        *app_data_p = NULL;     //	reset the pointer to pass it to the function reg_menu

        // 	create a new screen when the pointer temp_buf_2 is equal to 0 and the memory is not released
        reg_menu(&screen_data, 0); // 	menu_overlay=0

        *app_data_p = app_data;
    }
    else
    { // if the function is started for the first time i.e. from the menu

        // create a screen (register in the system)
        reg_menu(&screen_data, 0);

        // allocate the necessary memory and place the data in it (the memory by the pointer stored at temp_buf_2 is released automatically by the function reg_menu of another screen)
        *app_data_p = (app_data_t *)pvPortMalloc(sizeof(app_data_t));
        app_data = *app_data_p; //	data pointer

        // clear the memory for data
        _memclr(app_data, sizeof(app_data_t));

        //	param0 value contains a pointer to the data of the running process structure Elf_proc_
        proc = param0;

        // remember the address of the pointer to the function you need to return to after finishing this screen
        if (param0 && proc->ret_f) //	if the return pointer is passed, then return to it
            app_data->ret_f = proc->elf_finish;
        else //	if not, to the watchface
            app_data->ret_f = show_watchface;

#ifdef __SIMULATION__
        set_app_data_ptr(app_data);
#endif

        begin(app_data);
    }
    caffeine(WEAK);

    refreshWindow(app_data->vp.active, 1);
}

void key_press_screen()
{
    
    app_data_t *app_data = getAppData();

    // destroy all elements, memory leaks are bad
    destroyViewport(&app_data->vp);

    // call the return function (usually this is the start menu), specify the address of the function of our application as a parameter
    show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
};

void refreshScreen()
{ // triggered by set_update_period

    app_data_t *app_data = getAppData();

    refreshWindow(app_data->vp.active, 1);
    vibrate(2, 50, 150);
}

int interactionHandler(void *param)
{
    
    app_data_t *app_data = getAppData();

    struct gesture_ *gest = param;
    int result = 0;

    Window_ *activeWindow = app_data->vp.active;
    Layer_ *activeLayer = activeWindow->layerArray[0]; // for now, interaction can be had with the first layer

    switch (gest->gesture)
    {
    case GESTURE_CLICK:
    {
        processTap(activeLayer, gest->touch_pos_x, gest->touch_pos_y);
        break;
    };
    case GESTURE_SWIPE_RIGHT:
    { //	swipe to the right
        processSwipe(activeWindow, gest->gesture);
        refreshWindow(app_data->vp.active, 1);
        // show_menu_animate(show_menu, (unsigned int) refreshWindow(app_data->vp.active, 0), gest->gesture%4);
        break;
    };
    case GESTURE_SWIPE_LEFT:
    { // swipe to the left
        processSwipe(activeWindow, gest->gesture);
        refreshWindow(app_data->vp.active, 1);
        // show_menu_animate(show_menu, (unsigned int) refreshWindow(app_data->vp.active, 0), gest->gesture%4);
        break;
    };
    case GESTURE_SWIPE_UP:
    { // swipe up
        processSwipe(activeWindow, gest->gesture);
        refreshWindow(app_data->vp.active, 1);
        // show_menu_animate(show_menu, (unsigned int) refreshWindow(app_data->vp.active, 0), gest->gesture%4);
        break;
    };
    case GESTURE_SWIPE_DOWN:
    { // swipe down

        processSwipe(activeWindow, gest->gesture);
        refreshWindow(app_data->vp.active, 1);
        // show_menu_animate(show_menu, (unsigned int) refreshWindow(app_data->vp.active, 0), gest->gesture%4);
        break;
    };
    default:
    { // something went wrong ...

        break;
    };
    }
    //	}

    return result;
};