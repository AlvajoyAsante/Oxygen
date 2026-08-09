/**
 * @file oxy_save.h
 * @brief Declares AppVar-backed persistence helpers for Oxygen state.
 */
#ifndef OXY_SAVE_H
#define OXY_SAVE_H

#define OXY_APPVAR_NAME "OXYGEN"
#define OXY_VERSION "1.0"
#define OXY_SAVE_FORMAT_VERSION 1

#include <tice.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Loads all systems to oxygen main app-var.
 * @return `true` if the Oxygen AppVar was found and loaded.
 */
bool oxy_LoadAll(void);
#define oxy_Begin() \
oxy_LoadAll()

/**
 * @brief Saves all systems to the Oxygen AppVar.
 */
void oxy_SaveAll(void);

/**
 * @brief Saves all systems and reports whether the write succeeded.
 * @return `true` if the state was saved successfully.
 */
bool oxy_SaveAllChecked(void);
#define oxy_End() \
oxy_SaveAll()

/**
 * @brief Init all systems mainly used in "oxy_LoadAll"
 */
// void oxy_InitAll(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_SAVE_H__ */