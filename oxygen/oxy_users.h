/**
 * @file oxy_users.h
 * @brief Declares Oxygen's virtual user records and management helpers.
 */
#ifndef OXY_USERS_H
#define OXY_USERS_H

#include <tice.h>
#include <graphx.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Stores one virtual user's profile and authentication data.
 */
struct oxy_user_t {
	// Position of user info in pointer.
	uint8_t user_id;
	
	// Graphical sprite
	gfx_sprite_t *icon;
	
	// User information.
	char name[10];
	char password[16];
	
	// Type of your to limit users.
	uint8_t type;
	
	// For user login in management.
	uint8_t login_time[4];
};
extern struct oxy_user_t *oxy_user;

/**
 * @brief Stores aggregate metadata for the user registry.
 */
struct oxy_user_system_t {
	int user_amount;
};
extern struct oxy_user_system_t oxy_user_system;


/**
 * @brief Creates a new user record.
 * @param name User name.
 * @param password Password used for protected features.
 * @param type Type of user (0 = admin, 1 = user, 2 = guest).
 * @return New user index.
 */
uint8_t oxy_NewUser(char *name, char *password, const uint8_t type);

/**
 * @brief Sets a user's password.
 * @param password New password.
 * @param index User index.
 */
void oxy_SetUserPassword(char *password, const int index);

/**
 * @brief Changes a user's display name.
 * @param name New name.
 * @param index User index.
 */
void oxy_SetUserName(char *name, const int index);

/**
 * @brief Sets the icon sprite used for a user.
 * @param icon New icon sprite.
 * @param index User index.
 */
void oxy_UserSetIcon(gfx_sprite_t *icon, const int index);

/**
 * @brief Deletes a user record.
 * @param index User index.
 */
void oxy_DeleteUser(const int index);

/**
 * @brief Checks whether an input string matches a user's password.
 * @param input Password candidate.
 * @param index User index.
 * @return `true` if the password matches.
 */
bool oxy_UserCheckPassword(const char *input, const int index);

/**
 * This function is used to set login time to current time.
 */
// void oxy_UserSetLoginTime(const int index);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_USERS_H__ */