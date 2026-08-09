# ----------------------------
# Program Options
# ----------------------------

NAME         ?= OXYWIDGETS
ICON         ?= icon.png
DESCRIPTION  ?= "Oxygen Widget Demo"
COMPRESSED   ?= YES
ARCHIVED     ?= NO

# ----------------------------

CFLAGS ?= -Wall -Wextra -Oz
CXXFLAGS ?= -Wall -Wextra -Oz

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

EXTRA_C_SOURCES += \
	$(CURDIR)/../../../oxygen/oxy_cursor.c \
	$(CURDIR)/../../../oxygen/oxy_files.c \
	$(CURDIR)/../../../oxygen/oxy_gfx.c \
	$(CURDIR)/../../../oxygen/oxy_gui.c \
	$(CURDIR)/../../../oxygen/oxy_mouse.c \
	$(CURDIR)/../../../oxygen/oxy_notify.c \
	$(CURDIR)/../../../oxygen/oxy_save.c \
	$(CURDIR)/../../../oxygen/oxy_users.c \
	$(CURDIR)/../../../oxygen/widget/color.c \
	$(CURDIR)/../../../oxygen/widget/loop.c \
	$(CURDIR)/../../../oxygen/widget/util.c \
	$(CURDIR)/../../../oxygen/widget/elements/button.c \
	$(CURDIR)/../../../oxygen/widget/elements/checkbox.c \
	$(CURDIR)/../../../oxygen/widget/elements/colorbox.c \
	$(CURDIR)/../../../oxygen/widget/elements/dropdown.c \
	$(CURDIR)/../../../oxygen/widget/elements/hmargin.c \
	$(CURDIR)/../../../oxygen/widget/elements/vmargin.c \
	$(CURDIR)/../../../oxygen/widget/elements/menu.c \
	$(CURDIR)/../../../oxygen/widget/elements/rectangle.c \
	$(CURDIR)/../../../oxygen/widget/elements/scrollbar.c \
	$(CURDIR)/../../../oxygen/widget/elements/slider.c \
	$(CURDIR)/../../../oxygen/widget/elements/sprite.c \
	$(CURDIR)/../../../oxygen/widget/elements/switch.c \
	$(CURDIR)/../../../oxygen/widget/elements/label.c \
	$(CURDIR)/../../../oxygen/widget/elements/entry.c \
	$(CURDIR)/../../../oxygen/widget/elements/window.c \
	$(CURDIR)/../../../oxygen/gfx/charge.c \
	$(CURDIR)/../../../oxygen/gfx/cursorA.c \
	$(CURDIR)/../../../oxygen/gfx/cursorB.c \
	$(CURDIR)/../../../oxygen/gfx/cursorC.c \
	$(CURDIR)/../../../oxygen/gfx/cursorD.c \
	$(CURDIR)/../../../oxygen/gfx/cursorE.c \
	$(CURDIR)/../../../oxygen/gfx/xlibc.c \
	$(CURDIR)/../../../oxygen/gfx/xlibc_condensed.c \
	$(CURDIR)/../../../oxygen/gfx/xlibc_condensed_palette.c

include $(CEDEV)/meta/makefile.mk
