#include "oxygen.h"

#include <debug.h>
#include <fileioc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define OXY_SAVE_VERSION 1
#define OXY_MAX_NOTIFICATIONS 64
#define OXY_MAX_USERS 16
#define OXY_MAX_FOLDERS 128
#define OXY_MAX_FILES 1024

struct __attribute__((packed)) oxy_save_header_t {
	char magic[4];
	uint8_t version;
	uint8_t header_size;
	uint16_t payload_size;
	uint16_t checksum;
	uint8_t notification_count;
	uint8_t user_count;
	uint16_t folder_count;
	uint16_t file_count;
	uint16_t pin_count;
};

struct __attribute__((packed)) oxy_saved_notification_t {
	char title[9];
	char text[30];
};

struct __attribute__((packed)) oxy_saved_user_t {
	char name[10];
	char password[16];
	uint8_t type;
	uint8_t login_time[4];
};

struct __attribute__((packed)) oxy_saved_mouse_t {
	int16_t x;
	int16_t y;
	int16_t scroll_x;
	int16_t scroll_y;
	uint8_t speed;
};

struct __attribute__((packed)) oxy_saved_folder_t {
	uint8_t user_id;
	char name[9];
	uint8_t locked;
	int16_t location;
	int16_t position;
};

struct __attribute__((packed)) oxy_saved_file_t {
	uint8_t user_id;
	char name[9];
	uint8_t type;
	uint8_t flags;
	int16_t location;
	uint32_t size;
};

static uint16_t oxy_ChecksumUpdate(uint16_t checksum, const void *data, size_t size)
{
	const uint8_t *bytes = data;
	while (size--)
		checksum = (uint16_t)((checksum ^ *bytes++) * 257u);
	return checksum;
}

static struct oxy_saved_notification_t oxy_SaveNotification(uint8_t index)
{
	struct oxy_saved_notification_t saved = {0};
	memcpy(saved.title, oxy_notify[index].title, sizeof(saved.title));
	memcpy(saved.text, oxy_notify[index].text, sizeof(saved.text));
	return saved;
}

static struct oxy_saved_user_t oxy_SaveUser(uint8_t index)
{
	struct oxy_saved_user_t saved = {0};
	memcpy(saved.name, oxy_user[index].name, sizeof(saved.name));
	memcpy(saved.password, oxy_user[index].password, sizeof(saved.password));
	saved.type = oxy_user[index].type;
	memcpy(saved.login_time, oxy_user[index].login_time, sizeof(saved.login_time));
	return saved;
}

static struct oxy_saved_folder_t oxy_SaveFolder(uint16_t index)
{
	struct oxy_saved_folder_t saved = {0};
	saved.user_id = oxy_folder[index].user_id;
	memcpy(saved.name, oxy_folder[index].name, sizeof(saved.name));
	saved.locked = oxy_folder[index].locked;
	saved.location = oxy_folder[index].location;
	saved.position = oxy_folder[index].position;
	return saved;
}

static struct oxy_saved_file_t oxy_SaveFile(uint16_t index)
{
	struct oxy_saved_file_t saved = {0};
	saved.user_id = oxy_file[index].user_id;
	memcpy(saved.name, oxy_file[index].name, sizeof(saved.name));
	saved.type = oxy_file[index].type;
	saved.flags = (oxy_file[index].archived ? 1 : 0) |
		(oxy_file[index].locked ? 2 : 0) |
		(oxy_file[index].hidden ? 4 : 0) |
		(oxy_file[index].pinned ? 8 : 0);
	saved.location = oxy_file[index].location;
	saved.size = oxy_file[index].size;
	return saved;
}

static bool oxy_SaveStateIsValid(void)
{
	uint16_t index;
	if (oxy_notify_system.stack_amount > OXY_MAX_NOTIFICATIONS ||
		oxy_user_system.user_amount < 1 || oxy_user_system.user_amount > OXY_MAX_USERS ||
		oxy_file_system.numfolders < 0 || oxy_file_system.numfolders > OXY_MAX_FOLDERS ||
		oxy_file_system.numfiles < 0 || oxy_file_system.numfiles > OXY_MAX_FILES ||
		oxy_file_system.numpins < 0 || oxy_file_system.numpins > oxy_file_system.numfiles) return false;
	if ((oxy_notify_system.stack_amount && !oxy_notify) ||
		(oxy_user_system.user_amount && !oxy_user) ||
		(oxy_file_system.numfolders && !oxy_folder) ||
		(oxy_file_system.numfiles && !oxy_file)) return false;
	if (oxy_mouse.scroll_X < INT16_MIN || oxy_mouse.scroll_X > INT16_MAX ||
		oxy_mouse.scroll_Y < INT16_MIN || oxy_mouse.scroll_Y > INT16_MAX) return false;
	for (index = 0; index < oxy_file_system.numfolders; index++) {
		if (oxy_folder[index].location < INT16_MIN || oxy_folder[index].location > INT16_MAX ||
			oxy_folder[index].position < INT16_MIN || oxy_folder[index].position > INT16_MAX) return false;
	}
	for (index = 0; index < oxy_file_system.numfiles; index++) {
		if (oxy_file[index].location < INT16_MIN || oxy_file[index].location > INT16_MAX ||
			oxy_file[index].size < 0) return false;
	}
	return true;
}

bool oxy_SaveAllChecked(void)
{
	struct oxy_save_header_t header = {0};
	struct oxy_saved_mouse_t mouse;
	ti_var_t slot;
	uint16_t checksum = 0x4f58;
	uint32_t payload_size;
	uint16_t index;

	if (!oxy_SaveStateIsValid()) return false;
	memcpy(header.magic, "OXYS", sizeof(header.magic));
	header.version = OXY_SAVE_VERSION;
	header.header_size = sizeof(header);
	header.notification_count = oxy_notify_system.stack_amount;
	header.user_count = oxy_user_system.user_amount;
	header.folder_count = oxy_file_system.numfolders;
	header.file_count = oxy_file_system.numfiles;
	header.pin_count = oxy_file_system.numpins;
	payload_size = (uint32_t)header.notification_count * sizeof(struct oxy_saved_notification_t) +
		(uint32_t)header.user_count * sizeof(struct oxy_saved_user_t) + sizeof(mouse) +
		(uint32_t)header.folder_count * sizeof(struct oxy_saved_folder_t) +
		(uint32_t)header.file_count * sizeof(struct oxy_saved_file_t);
	if (payload_size > UINT16_MAX) return false;
	header.payload_size = payload_size;

	for (index = 0; index < header.notification_count; index++) {
		struct oxy_saved_notification_t saved = oxy_SaveNotification(index);
		checksum = oxy_ChecksumUpdate(checksum, &saved, sizeof(saved));
	}
	for (index = 0; index < header.user_count; index++) {
		struct oxy_saved_user_t saved = oxy_SaveUser(index);
		checksum = oxy_ChecksumUpdate(checksum, &saved, sizeof(saved));
	}
	mouse.x = oxy_mouse.x;
	mouse.y = oxy_mouse.y;
	mouse.scroll_x = oxy_mouse.scroll_X;
	mouse.scroll_y = oxy_mouse.scroll_Y;
	mouse.speed = oxy_mouse.speed;
	checksum = oxy_ChecksumUpdate(checksum, &mouse, sizeof(mouse));
	for (index = 0; index < header.folder_count; index++) {
		struct oxy_saved_folder_t saved = oxy_SaveFolder(index);
		checksum = oxy_ChecksumUpdate(checksum, &saved, sizeof(saved));
	}
	for (index = 0; index < header.file_count; index++) {
		struct oxy_saved_file_t saved = oxy_SaveFile(index);
		checksum = oxy_ChecksumUpdate(checksum, &saved, sizeof(saved));
	}
	header.checksum = checksum;

	slot = ti_Open(OXY_APPVAR_NAME, "w");
	if (!slot) return false;
	if (ti_Write(&header, sizeof(header), 1, slot) != 1) goto write_failed;
	for (index = 0; index < header.notification_count; index++) {
		struct oxy_saved_notification_t saved = oxy_SaveNotification(index);
		if (ti_Write(&saved, sizeof(saved), 1, slot) != 1) goto write_failed;
	}
	for (index = 0; index < header.user_count; index++) {
		struct oxy_saved_user_t saved = oxy_SaveUser(index);
		if (ti_Write(&saved, sizeof(saved), 1, slot) != 1) goto write_failed;
	}
	if (ti_Write(&mouse, sizeof(mouse), 1, slot) != 1) goto write_failed;
	for (index = 0; index < header.folder_count; index++) {
		struct oxy_saved_folder_t saved = oxy_SaveFolder(index);
		if (ti_Write(&saved, sizeof(saved), 1, slot) != 1) goto write_failed;
	}
	for (index = 0; index < header.file_count; index++) {
		struct oxy_saved_file_t saved = oxy_SaveFile(index);
		if (ti_Write(&saved, sizeof(saved), 1, slot) != 1) goto write_failed;
	}
	ti_SetArchiveStatus(true, slot);
	ti_Close(slot);
	return true;

write_failed:
	ti_Close(slot);
	ti_Delete(OXY_APPVAR_NAME);
	return false;
}

void oxy_SaveAll(void)
{
	(void)oxy_SaveAllChecked();
}

static void oxy_ResetRuntimeState(void)
{
	free(oxy_notify);
	free(oxy_user);
	free(oxy_detect);
	free(oxy_folder);
	free(oxy_file);
	oxy_notify = NULL;
	oxy_user = NULL;
	oxy_detect = NULL;
	oxy_folder = NULL;
	oxy_file = NULL;
	memset(&oxy_notify_system, 0, sizeof(oxy_notify_system));
	memset(&oxy_user_system, 0, sizeof(oxy_user_system));
	memset(&oxy_file_system, 0, sizeof(oxy_file_system));
}

static void oxy_InitAll(void)
{
	oxy_ResetRuntimeState();
	oxy_NewUser("USER", NULL, 0);
	oxy_InitMouse();
	oxy_InitFilesStystem();
	oxy_DetectAllFiles();
}

static bool oxy_HeaderIsValid(const struct oxy_save_header_t *header, size_t file_size)
{
	uint32_t expected_payload;
	if (memcmp(header->magic, "OXYS", 4) || header->version != OXY_SAVE_VERSION ||
		header->header_size != sizeof(*header) ||
		header->notification_count > OXY_MAX_NOTIFICATIONS ||
		header->user_count < 1 || header->user_count > OXY_MAX_USERS || header->folder_count > OXY_MAX_FOLDERS ||
		header->file_count > OXY_MAX_FILES || header->pin_count > header->file_count)
		return false;
	expected_payload = (uint32_t)header->notification_count * sizeof(struct oxy_saved_notification_t) +
		(uint32_t)header->user_count * sizeof(struct oxy_saved_user_t) + sizeof(struct oxy_saved_mouse_t) +
		(uint32_t)header->folder_count * sizeof(struct oxy_saved_folder_t) +
		(uint32_t)header->file_count * sizeof(struct oxy_saved_file_t);
	return expected_payload == header->payload_size && file_size == sizeof(*header) + expected_payload;
}

bool oxy_LoadAll(void)
{
	struct oxy_save_header_t header;
	struct oxy_saved_mouse_t mouse;
	struct oxy_notify_t *notifications = NULL;
	struct oxy_user_t *users = NULL;
	struct oxy_folders_t *folders = NULL;
	struct oxy_files_t *files = NULL;
	ti_var_t slot = ti_Open(OXY_APPVAR_NAME, "r");
	uint16_t checksum = 0x4f58;
	uint16_t index;

	if (!slot || ti_Read(&header, sizeof(header), 1, slot) != 1 ||
		!oxy_HeaderIsValid(&header, ti_GetSize(slot))) goto load_failed;
	if (header.notification_count && !(notifications = calloc(header.notification_count, sizeof(*notifications)))) goto load_failed;
	if (header.user_count && !(users = calloc(header.user_count, sizeof(*users)))) goto load_failed;
	if (header.folder_count && !(folders = calloc(header.folder_count, sizeof(*folders)))) goto load_failed;
	if (header.file_count && !(files = calloc(header.file_count, sizeof(*files)))) goto load_failed;

	for (index = 0; index < header.notification_count; index++) {
		struct oxy_saved_notification_t saved;
		if (ti_Read(&saved, sizeof(saved), 1, slot) != 1) goto load_failed;
		checksum = oxy_ChecksumUpdate(checksum, &saved, sizeof(saved));
		memcpy(notifications[index].title, saved.title, sizeof(saved.title));
		memcpy(notifications[index].text, saved.text, sizeof(saved.text));
		notifications[index].title[8] = '\0';
		notifications[index].text[29] = '\0';
	}
	for (index = 0; index < header.user_count; index++) {
		struct oxy_saved_user_t saved;
		if (ti_Read(&saved, sizeof(saved), 1, slot) != 1) goto load_failed;
		checksum = oxy_ChecksumUpdate(checksum, &saved, sizeof(saved));
		users[index].user_id = index;
		memcpy(users[index].name, saved.name, sizeof(saved.name));
		memcpy(users[index].password, saved.password, sizeof(saved.password));
		users[index].name[9] = '\0';
		users[index].password[15] = '\0';
		users[index].type = saved.type;
		memcpy(users[index].login_time, saved.login_time, sizeof(saved.login_time));
	}
	if (ti_Read(&mouse, sizeof(mouse), 1, slot) != 1 || mouse.speed < 1 || mouse.speed > 10 ||
		mouse.x < -1 || mouse.x > LCD_WIDTH || mouse.y < -1 || mouse.y > LCD_HEIGHT) goto load_failed;
	checksum = oxy_ChecksumUpdate(checksum, &mouse, sizeof(mouse));
	for (index = 0; index < header.folder_count; index++) {
		struct oxy_saved_folder_t saved;
		if (ti_Read(&saved, sizeof(saved), 1, slot) != 1) goto load_failed;
		checksum = oxy_ChecksumUpdate(checksum, &saved, sizeof(saved));
		folders[index].user_id = saved.user_id;
		memcpy(folders[index].name, saved.name, sizeof(saved.name));
		folders[index].name[8] = '\0';
		folders[index].locked = saved.locked != 0;
		folders[index].location = saved.location;
		folders[index].position = saved.position;
	}
	for (index = 0; index < header.file_count; index++) {
		struct oxy_saved_file_t saved;
		if (ti_Read(&saved, sizeof(saved), 1, slot) != 1 || saved.type > OXY_ERROR_TYPE) goto load_failed;
		checksum = oxy_ChecksumUpdate(checksum, &saved, sizeof(saved));
		files[index].user_id = saved.user_id;
		memcpy(files[index].name, saved.name, sizeof(saved.name));
		files[index].name[8] = '\0';
		files[index].type = saved.type;
		files[index].archived = saved.flags & 1;
		files[index].locked = saved.flags & 2;
		files[index].hidden = saved.flags & 4;
		files[index].pinned = saved.flags & 8;
		files[index].location = saved.location;
		files[index].size = saved.size;
	}
	if (checksum != header.checksum) goto load_failed;
	ti_Close(slot);
	oxy_ResetRuntimeState();
	oxy_notify = notifications;
	oxy_notify_system.stack_amount = header.notification_count;
	oxy_user = users;
	oxy_user_system.user_amount = header.user_count;
	oxy_folder = folders;
	oxy_file = files;
	oxy_file_system.numfolders = header.folder_count;
	oxy_file_system.numfiles = header.file_count;
	oxy_file_system.numpins = header.pin_count;
	oxy_InitMouse();
	oxy_mouse.x = mouse.x;
	oxy_mouse.y = mouse.y;
	oxy_mouse.scroll_X = mouse.scroll_x;
	oxy_mouse.scroll_Y = mouse.scroll_y;
	oxy_mouse.speed = mouse.speed;
	oxy_CheckFileSystem();
	return true;

load_failed:
	if (slot) ti_Close(slot);
	free(notifications);
	free(users);
	free(folders);
	free(files);
	dbg_sprintf(dbgout, "Oxygen: save missing, incompatible, or invalid; initializing defaults.\n");
	oxy_InitAll();
	return false;
}