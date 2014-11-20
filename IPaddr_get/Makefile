CC = gcc
AS = as
LD = ld
CFLAGS += -Wall 


LDFLAGS +=  -lssl 

#### add here include, library
#LIBS	+= 
#LIBS	+= 
#INCLUDE 	+= 
#INCLUDE	+= 
##


#### add here src, headder
SRC	+= get_net_info.c
SRC	+= 
HEAD	+=
#HEAD	+=
##

OBJS	:= $(SRC:%.c=%.o)
EXE =  get_net_info
.PHONY: clean all
.SUFFIXES:.o .c

all: $(EXE) message
$(EXE):	$(OBJS) 

clean:
	@rm -rf $(OBJS)

message:
	@echo "================================"
	@echo ""
	@echo "compiled $(OBJS)"
	@echo ""
	@echo "================================"

