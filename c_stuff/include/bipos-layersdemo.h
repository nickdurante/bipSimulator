/*
	Application template for Amazfit Bip BipOS
	(C) Maxim Volkov  2019 <Maxim.N.Volkov@ya.ru>
	
	Application template, header file

*/

#ifndef __SIM_TEMPLATE_H__
#define __SIM_TEMPLATE_H__
#define COLORS_COUNT 4

#include "bipui/bipui.h"


// sim_template.c
int main_app(int param0);

void show_screen(void *return_screen);
void shortKeyPressHandler();
void longKeyPressHandler(void);

int interactionHandler(void *param);
void refreshScreen();

void begin(app_data_t *app_data);
void simpleWindowCallbackFunction(Window_ *window, Way_ way);
void settingsWindowCallbackFunction(Window_ *window, Way_ way);
void changeWindowColourCallbackFunction(Layer_ *layer, short button_id);

void layerMainConstructor(Layer_ *layer);
void layerHelpConstructor(Layer_ *layer);
void layerSettingsConstructor(Layer_ *layer);

#endif