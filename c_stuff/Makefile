#Copyright (C) 2016  Davidson Francis <davidsondfgl@gmail.com>
#
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>

LIB_NAME = libws
CC=gcc
AR=ar

LIB_BIP_FOLDER=C:\dev\bip\libbip
INCLUDES  = -I$(CURDIR)/include/sim
SRC      = $(CURDIR)/src
BUILD_DIR	= ./build
LIBWS_DIR	= ./bin 
CFLAGS   =  -Wall -std=c99
# CFLAGS  +=  -std=c99
ARFLAGS  =  cr
LIB_A      =  $(LIB_NAME).a

# C_SRC = $(wildcard $(SRC)/sim/base64/*.c)    \
# 		$(wildcard $(SRC)/sim/handshake/*.c) \
# 		$(wildcard $(SRC)/sim/sha1/*.c)      \
# 		$(wildcard $(SRC)/sim/neo/*.c)      \
# 		$(wildcard $(SRC)/sim/cJSON/*.c)    \
# 		$(wildcard $(SRC)/*.c)

SOURCES_C := $(wildcard src/*.c)
SOURCES_NEO := $(shell find include/neo -name *.c)
SOURCES_ADDITIONAL := $(shell find src/base64 -name *.c) \
						$(shell find src/cJSON -name *.c) \
						$(shell find src/handshake -name *.c) \
						$(shell find src/sha1 -name *.c)

SOURCES_SIM := $(wildcard src/simu*.c)
OBJS_SIM := $(SOURCES_SIM:%.c=%.o)


SRCS	=  $(SOURCES_C) \
			$(SOURCES_NEO) \
			$(SOURCES_ADDITIONAL)


_OBJS 	:= $(notdir $(SRCS:%.c=%.o))
OBJS 	= $(patsubst %,$(BUILD_DIR)/%,$(_OBJS))
# OBJS 	= $(BUILD_DIR)/$(_OBJS)

all: $(LIB_NAME)

mock:
	$(info objs: $(OBJS))
	$(info =)
	$(info _objs: $(_OBJS))
	$(info =)
	$(info - neo: $(SOURCES_NEO))
	$(info - deps: $(SOURCES_ADDITIONAL))
	$(info - sources: $(SOURCES_C))
	$(info =)

sim : 
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SOURCES_SIM) -o $(OBJS_SIM)


app: $(LIB_NAME) libbip

libbip:
	$(MAKE) -C app

# libws: $(BUILD_DIR)/%.o
$(LIB_NAME): $(_OBJS)
	@$(AR) $(ARFLAGS) $(LIB_A) $^
# $(info sources: $(SRCS))
# $(info objs: $(OBJ))
	$(info Build complete)

$(_OBJS): $(SRCS)
	$(info objs: $(OBJ))
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f *.o
	$(info Clean complete)