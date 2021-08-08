
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include <simulator.h>
#include <cJSON.h>
#include <ws.h>
#include <sha1.h>
#include <base64.h>

#include <pthread.h>

#define WS_PORT 6789

// App includes go here!
#include <metronome.h>

int connected = 0;

void *(tap_cbk);
void *(sPress_cbk);
void *(lPress_cbk);

// App-side stuff
extern void show_screen(void * param0);
extern _interactionCallbacks app_int_callbacks;

extern app_data_t app_data_global;

void onopen(int fd)
{
	char *cli;
	setClientID(fd);
	connected = 1;

	cli = ws_getaddress(fd);
	printf("Connection opened, client: %d | addr: %s\n", fd, cli);
	free(cli);
}

void onclose(int fd)
{
	char *cli;
	cli = ws_getaddress(fd);
	printf("Connection closed, client: %d | addr: %s\n", fd, cli);
	free(cli);
}

// Here we parse data coming from the sim window interactions
void onmessage(int fd, const unsigned char *msg)
{

	// action
	cJSON *gesture = cJSON_Parse(msg);
	cJSON *type = cJSON_GetObjectItem(gesture, "action"); // type is an int
	cJSON *x_out = cJSON_GetObjectItem(gesture, "x");
	cJSON *y_out = cJSON_GetObjectItem(gesture, "y");

	// tap_cbk = app_data_global.screen_data_p->dispatch_func;
	// sPress_cbk = app_data_global.screen_data_p->key_press;
	// lPress_cbk = app_data_global.screen_data_p->long_key_press;

	struct gesture_ tempGest;

	// function = app_data_global.

	if (type->valueint == 1) {		// tap

		tempGest.gesture = type->valueint;
		tempGest.touch_pos_x= x_out->valueint;
		tempGest.touch_pos_y = y_out->valueint;
		// void *(tap_func) () = screen_data->dispatch_func;
		app_data_global.screen_data_p->dispatch_func(&tempGest);

		printf("Received a tap");
	}
	else if (type->valueint > 1 && type->valueint < 6) {	// swipe

		tempGest.gesture = type->valueint;
		tempGest.touch_pos_x= 0;
		tempGest.touch_pos_y = 0;
		app_data_global.screen_data_p->dispatch_func(&tempGest);
		printf("Received a swipe");

	} else if (type->valueint == 6) {			// button short

		app_data_global.screen_data_p->key_press(&tempGest);
		printf("Received a SHORT keypress");

	} else if (type->valueint == 7) {			// button long

		app_data_global.screen_data_p->long_key_press(&tempGest);
		printf("Received a LONG keypress");

	}else {

		exit(9);	//invalid message
	}


	

}

void *threadFunc( void *arg) {

	struct ws_events evs;
	evs.onopen = &onopen;
	evs.onclose = &onclose;
	evs.onmessage = &onmessage;
	ws_socket(&evs, WS_PORT);

}

int main()
{
	
	pthread_t  threadID;

	if(pthread_create(&threadID, NULL, &threadFunc, NULL))
		printf("Failed to create thread");

	printf("Open or refresh webapp/index.html.\n");

	do {
		sleep(1);
	} while(!connected);

	// invoking the app's show_screen a.k.a. "the main" and only func called from main
	// show_screen(0);
	main(0);

	pthread_join(threadID, NULL);

	return 10;
}
