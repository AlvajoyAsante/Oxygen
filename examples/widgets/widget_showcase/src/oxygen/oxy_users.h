#ifndef OXY_USERS_H
#define OXY_USERS_H

#include <graphx.h>

// K.I.S.S. PRINCIPLE

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Users Dynamic Pointer (Stores simple user information).  
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
 * @brief User System Structure (stores amount of users)
 */ 
struct oxy_user_system_t {
	int user_amount;
};
extern struct oxy_user_system_t oxy_user_system;


/**
 * Creates a new user.
 * @param name Users name.
 * @param password password for locking features.
 * @param type Type of user (0 = admin, 1 = user, 2 = guest).
 * @returns Users index.
 */ 
uint8_t oxy_NewUser(char *name, char *password, const uint8_t type);


void oxy_UserSetIcon(gfx_sprite_t *icon, const int index);

/**
 * Deletes current user at set index.
 * @param index Set this to the users index.
 */ 
void oxy_DeleteUser(const int index);

/**
 * Sets Password of a user.
 * @param password New password.
 * @param index Set this to the users index.
 */ 
void oxy_SetUserPassword(char *password, const int index);

/**
 * Changes name of a user.
 * @param name New name.
 * @param index Set this to the users index.
 */ 
void oxy_SetUserName(char *name, const int index);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_USERS_H__ */