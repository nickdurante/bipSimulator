/*
 * Copyright (C) 2016-2019 Davidson Francis <davidsondfgl@gmail.com>
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

// gesture type
#define GESTURE_CLICK 1		  // simple tap on the screen, click coordinatesв variables touch_pos_x touch_pos_y
#define GESTURE_SWIPE_UP 2	  // swipe up
#define GESTURE_SWIPE_DOWN 3  // swipe down
#define GESTURE_SWIPE_LEFT 4  // swipe right to left
#define GESTURE_SWIPE_RIGHT 5 // swipe left to right

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include <ws.h>

int ID_client;

void onopen(int fd)
{
	char *cli;
	ID_client = fd;

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

	if(type->valueint == 1)	{	//click

		cJSON *x=cJSON_GetObjectItem(gesture, "x");
		cJSON *y=cJSON_GetObjectItem(gesture, "y");

		// dispatch_screen((struct gesture_ *) {type->valueint, (int) x->valueint, (int) y -> y.valueint})
	}
	else	// gesture
	{
	
		// dispatch_screen((struct gesture_ *) {type->valueint, 0, 0)
	}

	char *message = cJSON_Print(msg);

	printf("I receive a message: %s\n", message);
	ws_sendframe(fd, (char *)msg, true);

}

void set_bg_color(long color)
{

	int R = (color & 0xFF);			//extract first byte
	int G = ((color >> 8) & 0xFF);	//extract second byte
	int B = ((color >> 16) & 0xFF); //extract third byte

	cJSON *red_field = NULL;
	cJSON *blue_field = NULL;
	cJSON *green_field = NULL;

	cJSON *message = cJSON_CreateObject();

	cJSON *isfg = cJSON_CreateNumber(0);
	cJSON_AddItemToObject(message, "isfg", isfg);

	cJSON *color_field = cJSON_CreateArray();

	red_field = cJSON_CreateNumber(R);
	cJSON_AddItemToArray(color_field, red_field);

	green_field = cJSON_CreateNumber(G);
	cJSON_AddItemToArray(color_field, green_field);

	blue_field = cJSON_CreateNumber(B);
	cJSON_AddItemToArray(color_field, blue_field);
	ws_sendframe(ID_client, (char *)message, true);
	free(message);
}

void text_out_center(char *text_in, int origin_x, int origin_y)
{

	cJSON *text_out = NULL;
	cJSON *origin = NULL;
	cJSON *x = NULL;
	cJSON *y = NULL;

	cJSON *message = cJSON_CreateObject();

	text_out = cJSON_CreateString(text_in);

	cJSON_AddItemToObject(message, "text", text_out);

	origin = cJSON_CreateObject();
	x = cJSON_CreateNumber(origin_x);
	y = cJSON_CreateNumber(origin_y);

	cJSON_AddItemToObject(origin, "x", x);
	cJSON_AddItemToObject(origin, "y", y);

	cJSON_AddItemToObject(message, "origin", origin);
	ws_sendframe(ID_client, (char *)message, true);
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

	string = cJSON_Print(monitor);
	if (string == NULL)
	{
		fprintf(stderr, "Failed to print monitor.\n");
	}

end:
	cJSON_Delete(monitor);
	return string;
}

int main()
{
	text_out_center("dong", 2, 3);

	struct ws_events evs;
	evs.onopen = &onopen;
	evs.onclose = &onclose;
	evs.onmessage = &onmessage;
	ws_socket(&evs, 8080);

	return 0;
}
