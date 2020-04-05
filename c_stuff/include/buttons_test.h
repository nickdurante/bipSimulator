/*
	Application template for Amazfit Bip BipOS
	(C) Maxim Volkov  2019 <Maxim.N.Volkov@ya.ru>
	
	Application template, header file

*/

#ifndef __APP_BUTTONS_H__
#define __APP_BUTTONS_H__
#define COLORS_COUNT 4

#include "bipui.h"

/* #define BUILTINSPRINTF 	_sprintf
#define GNUSPRINTF		sprintf

#define MAKE_FN_NAME(x) int ## x (char *buf, const char *format, ...)
#define FUNCTION_NAME(signal) MAKE_FN_NAME(signal) */


// template.c
int main_app(int param0);
void show_screen(void *return_screen);
void key_press_screen();
int dispatch_screen(void *param);
void refreshScreen();
void testCallbackFunction(Layer_ *layer, short button_id);
Layer_ *layerSplashConstructor(app_data_t *app_data);
Layer_ *layerButtonsConstructor(app_data_t *app_data);
void splashCallbackFunction(Viewport_ *vp);
void begin(app_data_t *app_data);

#endif