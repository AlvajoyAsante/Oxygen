#ifndef OXY_NOTIFY_H
#define OXY_NOTIFY_H

#include <graphx.h>

// K.I.S.S. PRINCIPLE

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Notifications structure file.  
 */ 
struct oxy_notify_t{
	gfx_sprite_t *icon;
	char title[9];
	char text[30];
};
extern struct oxy_notify_t *oxy_notify;

/**
 * @brief Notification stack system (manages amount of notifications).  
 */ 
struct oxy_notify_system_t{
	uint8_t stack_amount;
};
extern struct oxy_notify_system_t oxy_notify_system;

/**
 * Creates a new notification.
 * @param title Title of the notification (Name of Program).
 * @param text Text or dialog of the notifications.
 * @param time Use oxy_GetNotifyTime();
 * @param date Use oxy_GetNotifyDate();
 */
void oxy_NewNotify(gfx_sprite_t *icon, char title[9], char text[30]);

/**
 * Converts Given time into a pointer for oxy_NewNotify();
 * @param hour 24 hour clock.
 * @param min 60 mins on clock.
 * @return Pointer with given time 
 */
uint8_t* oxy_GetNotifyTime(uint8_t hour, uint8_t min);

/**
 * Converts Given date into a pointer for oxy_NewNotify();
 * @param month month clock (ranges from 1 - 12).
 * @param day Day of the months (ranges from 1 - 31).
 * @return Pointer with given date 
 */
uint8_t* oxy_GetNotifyDate(uint8_t month, uint8_t day);

/**
 * Deletes Notification out of a stack,
 * @param index Pos or index of Notification.
 */
void oxy_DeleteNotify(uint8_t index);

/**
 * @brief Deletes all notification.
 */ 
void oxy_DeleteAllNotify(void);

/**
 * @brief Detects if there are any active notifications and when to alert the user.
 */ 
int oxy_CheckNotify(void);

/**
 * @brief Displays a notification.
 */ 
void oxy_AlertNotify(int index);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_NOTIFY_H__ */