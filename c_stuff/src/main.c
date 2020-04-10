/*
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "cJSON.h"
#include <ws.h>
 

#include "buttons_test.h"
#include <pthread.h>

#define WS_PORT 6789

int connected = 0;

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

void onmessage(int fd, const unsigned char *msg)
{

	cJSON *gesture = cJSON_Parse(msg);
	cJSON *type = cJSON_GetObjectItem(gesture, "action"); // type is an int
	cJSON *x_out = cJSON_GetObjectItem(gesture, "x");
	cJSON *y_out = cJSON_GetObjectItem(gesture, "y");
	
	struct gesture_ tempGest;

	if (type->valueint == 1) {		// tap

		tempGest.gesture = type->valueint;
		tempGest.touch_pos_x= x_out->valueint;
		tempGest.touch_pos_y = y_out->valueint;
	}
	else {

		tempGest.gesture = type->valueint;
		tempGest.touch_pos_x= 0;
		tempGest.touch_pos_y = 0;
	}


	interactionHandler(&tempGest);

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

	main_app(0);

	pthread_join(threadID, NULL);

	return 10;
}
