# Copyright (c) Microsoft Corporation. All rights reserved.
ifeq ($(BUILD_TYPE),Debug)
    DEBUG_CFLAGS = -g
	DEBUG_LDFLAGS = -lgcov
else ifeq ($(BUILD_TYPE),AddressSanitizer)
	DEBUG_CFLAGS = -fsanitize=address
endif


CODELET_FILES = $(wildcard *.c)
CODELET_OBJS = $(CODELET_FILES:.c=.o)

INCLUDES := -I${JANUS_OUT_DIR}/inc
LDFLAGS := -L${JANUS_OUT_DIR}/lib -ljanus -lck -lubpf -lmimalloc -lpthread -ldl -lrt ${DEBUG_LDFLAGS}
CODELET_CC := clang
CODELET_VERIFIER := ${JANUS_OUT_DIR}/bin/janus_verifier_cli

CODELET_CFLAGS := -O2 -target bpf -Wall -DJANUS_DEBUG_ENABLED -D__x86_64__

.PHONY: all clean

all: clean ${CODELET_OBJS}

%.o: %.c
	${CODELET_CC} ${CODELET_CFLAGS} ${INCLUDES} -c $< -o $@
	${CODELET_VERIFIER} $@

clean:
	@for obj in $(CODELET_OBJS); do \
		rm -f $$obj; \
	done

