CFLAGS+= -std=c11 -Wall -Werror -Wextra -pedantic -Wfloat-equal -Waggregate-return -Wstack-usage=1024 
CFLAGS+=-D _XOPEN_SOURCE=500
CFLAGS+=-D _POSIX_C_SOURCE=200809L

TARGET1=dispatcher
TARGET2=listener
OBJS1=dispatcher.o
OBJS2=listener.o
.PHONY: clean debug profile

all: $(TARGET1) $(TARGET2)
$(TARGET1): $(OBJS1) 
$(TARGET2): $(OBJS2) 

debug: CFLAGS+=-g
debug: all


clean:
	-$(RM) $(TARGET1) $(OBJS1) $(TARGET2) $(OBJS2) 

profile: CFLAGS+=-pg
profile: LDFLAGS+=-pg
profile: $(TARGET)
