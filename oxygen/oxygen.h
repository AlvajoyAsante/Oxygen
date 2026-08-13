/**
 * @file oxygen.h
 * @brief Aggregates the public Oxygen subsystem headers.
 */
#ifndef OXYGEN_H
#define OXYGEN_H

/**
 * Virtual file system API.
 */
#include "oxy_files.h"

/**
 * Low-level program editor entrypoint.
 */
#include "asm/editprgm.h"

/**
 * Drawing and sprite utilities.
 */
#include "oxy_gfx.h"

/**
 * High-level 16bpp VRAM drawing API.
 */
#include "oxy_vram16.h"

/**
 * Graphical input helpers and HUD widgets.
 */
#include "oxy_gui.h"

/**
 * Pointer state and hover tracking.
 */
#include "oxy_mouse.h"

/**
 * Notification queue and rendering helpers.
 */
#include "oxy_notify.h"

/**
 * AppVar-backed persistence helpers.
 */
#include "oxy_save.h"

/**
 * Virtual user management.
 */
#include "oxy_users.h"

/**
 * Windowing and form helpers.
 */
#include "oxy_gtk.h"

/**
 * Bundled Oxygen sprite assets.
 */
#include "gfx/oxy_sprites.h"

#endif