/**
 * @file oxy_notify.h
 * @brief Declares notification storage and display helpers.
 */
#ifndef OXY_NOTIFY_H
#define OXY_NOTIFY_H

#include <tice.h>
#include <graphx.h>

#define oxy_BlackText(a) \
gfx_SetTextFGColor(0); \
gfx_SetTextBGColor(a);

#define oxy_WhiteText(a) \
gfx_SetTextFGColor(255); \
gfx_SetTextBGColor(a);

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Represents a single queued notification.
 */
struct oxy_notify_t{
	gfx_sprite_t *icon;
	char title[9];
	char text[30];
};
extern struct oxy_notify_t *oxy_notify;

/**
 * @brief Stores notification queue metadata.
 */
struct oxy_notify_system_t{
	uint8_t stack_amount;
};
extern struct oxy_notify_system_t oxy_notify_system;

/**
 * @brief Queues a new notification.
 * @param icon Sprite displayed with the notification.
 * @param title Title of the notification (Name of Program).
 * @param text Notification message text.
 */
void oxy_NewNotify(gfx_sprite_t *icon, char title[9], char text[30]);

/**
 * @brief Deletes one notification from the queue.
 * @param index Notification index.
 */
void oxy_DeleteNotify(uint8_t index);

/**
 * @brief Deletes every notification in the queue.
 */
void oxy_DeleteAllNotify(void);

/**
 * @brief Returns the index of the next notification that should be displayed.
 * @return Notification index, or a negative value when nothing is pending.
 */
int oxy_CheckNotify(void);

/**
 * @brief Renders the default notification UI for a queued notification.
 * @param index Notification index to display.
 */
void oxy_AlertNotify(int index);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_NOTIFY_H__ */