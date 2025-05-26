SRCDIR = src
BUILDDIR = build
OUTPUTEXE = game

CC = g++
CFLAGS = -m64 -fno-strict-aliasing -pedantic -Wall -Wextra -Wno-unused-parameter -Wno-format-truncation -std=c++11 -DOS_LINUX=1 -DARCH_X64=1 -D_CRT_SECURE_NO_WARNINGS=1
LFLAGS = -Wl,-t

DEBUG ?= 0
ifneq ($(DEBUG), 0)
	CFLAGS += -g -O0
else
	CFLAGS += -O2
endif

HEADERS = $(SRCDIR)/common.hh $(SRCDIR)/config.hh $(SRCDIR)/connection.hh $(SRCDIR)/containers.hh $(SRCDIR)/creature.hh $(SRCDIR)/enums.hh $(SRCDIR)/map.hh $(SRCDIR)/monster.hh $(SRCDIR)/objects.hh $(SRCDIR)/player.hh $(SRCDIR)/script.hh $(SRCDIR)/skill.hh $(SRCDIR)/thread.hh

$(BUILDDIR)/$(OUTPUTEXE): $(BUILDDIR)/config.obj $(BUILDDIR)/crcombat.obj $(BUILDDIR)/creature.obj $(BUILDDIR)/main.obj $(BUILDDIR)/map.obj $(BUILDDIR)/objects.obj $(BUILDDIR)/player.obj $(BUILDDIR)/script.obj $(BUILDDIR)/shm.obj $(BUILDDIR)/skill.obj $(BUILDDIR)/thread.obj $(BUILDDIR)/time.obj $(BUILDDIR)/util.obj
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^

$(BUILDDIR)/config.obj: $(SRCDIR)/config.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILDDIR)/crcombat.obj: $(SRCDIR)/crcombat.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILDDIR)/creature.obj: $(SRCDIR)/creature.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILDDIR)/main.obj: $(SRCDIR)/main.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILDDIR)/map.obj: $(SRCDIR)/map.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILDDIR)/objects.obj: $(SRCDIR)/objects.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILDDIR)/player.obj: $(SRCDIR)/player.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILDDIR)/script.obj: $(SRCDIR)/script.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILDDIR)/shm.obj: $(SRCDIR)/shm.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILDDIR)/skill.obj: $(SRCDIR)/skill.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILDDIR)/thread.obj: $(SRCDIR)/thread.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILDDIR)/time.obj: $(SRCDIR)/time.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILDDIR)/util.obj: $(SRCDIR)/util.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	@rm -r $(BUILDDIR)
