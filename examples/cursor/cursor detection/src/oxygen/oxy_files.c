#include "oxy_files.h"
#include "oxy_save.h"

#include <tice.h>
#include <fileioc.h>
#include <string.h>
#include <debug.h>

struct oxy_files_t *oxy_file;
struct oxy_folders_t *oxy_folder;
struct oxy_file_system_t oxy_file_system;

void EditPrgm(); // Created By Espilon5

void oxy_InitFilesStystem(void)
{
	oxy_file_system.numfiles = oxy_file_system.numfolders = oxy_file_system.numpins = 0;

	oxy_AddFolder("HOME", -1);
	oxy_AddFolder("APPVARS", 0);
}

void oxy_CheckFileSystem(void)
{
	void *search_pos = NULL;
	char *file_name;
	uint8_t type;
	int count = 0;

	ti_var_t slot;

	while ((file_name = ti_DetectAny(&search_pos, NULL, &type)) != NULL)
	{
		if (type == TI_PRGM_TYPE || type == TI_PPRGM_TYPE || type == TI_APPVAR_TYPE)
		{
			if ((*file_name != '!') && (*file_name != '#'))
			{
				count++;
			}
		}
	}

	if (count > oxy_file_system.numfiles)
	{ // File was added
		oxy_RescanFileSystem();
		return;
	}

	if (count < oxy_file_system.numfiles)
	{ // File was deleted
		for (int i = 0; i < oxy_file_system.numfiles; i++)
		{
			if (!(slot = ti_OpenVar(oxy_file[i].name, "r", type)))
			{
				oxy_DeleteFile(i);
				ti_Close(slot);
			}
		}
	}

	oxy_GetAsmIcons();
	oxy_GetBasicIcons();
}

static bool oxy_FindFile(char *name)
{
	for (int i = 0; i < oxy_file_system.numfiles; i++)
	{
		if (!strcmp(oxy_file[i].name, name))
			return 1;
	}
	return 0;
}

void oxy_RescanFileSystem(void)
{
	void *search_pos = NULL;
	char *file_name;
	uint8_t type;

	ti_var_t slot;
	struct oxy_files_t *curr_file;

	while ((file_name = ti_DetectAny(&search_pos, NULL, &type)) != NULL)
	{
		if (type == TI_PRGM_TYPE || type == TI_PPRGM_TYPE || type == TI_APPVAR_TYPE)
		{
			if ((*file_name != '!') && (*file_name != '#'))
			{
				if (!oxy_FindFile(file_name))
				{
					if ((slot = ti_OpenVar(file_name, "r", type)))
					{
						if (!oxy_AddFile())
							return;

						curr_file = &oxy_file[oxy_file_system.numfiles - 1];

						strncpy(curr_file->name, file_name, sizeof(curr_file->name));

						switch (type)
						{
						case (TI_PRGM_TYPE):
							curr_file->type = OXY_BASIC_TYPE;
							curr_file->location = 0;
							break;

						case (TI_PPRGM_TYPE):
							curr_file->type = OXY_PBASIC_TYPE;
							curr_file->location = 0;
							break;

						case (TI_APPVAR_TYPE):
							curr_file->type = OXY_APPVAR_TYPE;
							curr_file->location = 1;
							break;

						default:
							curr_file->type = OXY_ERROR_TYPE; // error
							curr_file->location = 0;
							break;
						}

						curr_file->pinned = false;
						curr_file->locked = false;

						curr_file->size = ti_GetSize(slot);
						curr_file->archived = ti_IsArchived(slot);
						curr_file->icon = NULL;

						ti_Close(slot);
					}
				}
			}
		}
	}

	oxy_SortFiles();
	oxy_GetAsmIcons();
	oxy_GetBasicIcons();
}

void oxy_DetectAllFiles(void)
{
	void *search_pos = NULL;
	char *file_name;
	uint8_t type;

	ti_var_t slot;
	struct oxy_files_t *curr_file;

	while ((file_name = ti_DetectAny(&search_pos, NULL, &type)) != NULL)
	{
		if (type == TI_PRGM_TYPE || type == TI_PPRGM_TYPE || type == TI_APPVAR_TYPE)
		{
			if ((*file_name != '!') && (*file_name != '#'))
			{
				if ((slot = ti_OpenVar(file_name, "r", type)))
				{
					if (!oxy_AddFile())
					{
						ti_Close(slot);
						return;
					}

					curr_file = &oxy_file[oxy_file_system.numfiles - 1];

					strncpy(curr_file->name, file_name, sizeof(curr_file->name));

					switch (type)
					{
					case (TI_PRGM_TYPE):
						curr_file->type = OXY_BASIC_TYPE;
						curr_file->location = 0;
						break;

					case (TI_PPRGM_TYPE):
						curr_file->type = OXY_PBASIC_TYPE;
						curr_file->location = 0;
						break;

					case (TI_APPVAR_TYPE):
						curr_file->type = OXY_APPVAR_TYPE;
						curr_file->location = 1;
						break;

					default:
						curr_file->type = OXY_ERROR_TYPE; // error
						curr_file->location = 0;
						break;
					}

					curr_file->pinned = false;
					curr_file->locked = false;

					curr_file->size = ti_GetSize(slot);
					curr_file->archived = ti_IsArchived(slot);
					curr_file->icon = NULL;

					ti_Close(slot);
				}
			}
		}
	}

	oxy_SortFiles();
	oxy_GetAsmIcons();
	oxy_GetBasicIcons();
}

/* Deleting folders and files */
void oxy_DeleteFolder(int index)
{
	if (oxy_file_system.numfolders > 0)
	{
		oxy_folder[index] = oxy_folder[oxy_file_system.numfolders - 1];
		oxy_folder = realloc(oxy_folder, --oxy_file_system.numfolders * sizeof(struct oxy_folders_t));
	}
	// fix this, delete all files in the folder (maybe change later)
	for (int i = 0; i < oxy_file_system.numfiles; i++)
	{
		struct oxy_files_t *curr_file = &oxy_file[i];

		if (curr_file->location == index)
		{
			oxy_Delete(false, i);
			continue; // basically skip to next interrelation.
		}

		// the last folder is now this folder
		if (curr_file->location == oxy_file_system.numfolders)
			curr_file->location = index;
	}

	for (int i = 0; i < oxy_file_system.numfolders; i++)
	{
		struct oxy_folders_t *currfolder = &oxy_folder[i];

		if (currfolder->location == index)
		{
			oxy_Delete(true, i);
			continue;
		}

		// the last folder is now this folder
		if (currfolder->location == oxy_file_system.numfolders)
			currfolder->location = index;
	}

	return;
}

void oxy_DeleteFile(int index)
{
	if (oxy_file_system.numfiles > 0)
	{
		oxy_file[index] = oxy_file[oxy_file_system.numfiles - 1];
		oxy_file = realloc(oxy_file, oxy_file_system.numfiles-- * sizeof(struct oxy_files_t));
	}
	return;
}

bool oxy_Delete(bool isfolder, int index)
{
	uint8_t progtype;
	uint8_t type = oxy_file[index].type;

	if (isfolder)
	{
		/* true */
		oxy_DeleteFolder(index);
	}
	else
	{

		/* false */
		if ((progtype = oxy_GetFileType(type)) == 255)
		{
			return 0;
		}

		/* delete the file */
		if (ti_DeleteVar(oxy_file[index].name, progtype))
		{
			oxy_DeleteFile(index);
		}
		else
		{
			return 0;
		}

		return 1;
	}

	return 0;
}

/* Creating folders and files */
bool oxy_AddFolder(char *name, int position)
{
	if ((oxy_folder = realloc(oxy_folder, ++oxy_file_system.numfolders * sizeof(struct oxy_folders_t))) != NULL)
	{

		strncpy(oxy_folder[oxy_file_system.numfolders - 1].name, name, 9);
		oxy_folder[oxy_file_system.numfolders - 1].location = oxy_file_system.numfolders;
		oxy_folder[oxy_file_system.numfolders - 1].position = position;
		return true;
	}
	else
		return false;
}

bool oxy_AddFile(void)
{
	if ((oxy_file = realloc(oxy_file, ++oxy_file_system.numfiles * sizeof(struct oxy_files_t))) != NULL)
	{
		oxy_file[oxy_file_system.numfiles - 1].icon = NULL;
		oxy_file[oxy_file_system.numfiles - 1].location = 0;
		return true;
	}
	else
		return false;
}

/* Detecting files icons */
void oxy_GetAsmIcons(void)
{
	uint8_t data_pos;
	ti_var_t slot;

	for (int i = 0; i < oxy_file_system.numfiles; i++)
	{
		struct oxy_files_t *curr_file = &oxy_file[i];

		if ((slot = ti_OpenVar(curr_file->name, "r", oxy_GetFileType(curr_file->type))))
		{
			if (oxy_GetFileType(curr_file->type) == TI_PPRGM_TYPE)
			{
				for (int r = 0; r < 20; r++)
				{
					ti_Read(&data_pos, 1, 1, slot);

					if (r == 0)
					{
						if ((data_pos != 0xEF) && (data_pos + 1) != 0x7B)
						{
							curr_file->type = OXY_PBASIC_TYPE;
							curr_file->locked = true;
							break;
						}
					}

					if (r == 2)
					{
						switch (data_pos)
						{
						case 0x00:
							curr_file->type = OXY_C_TYPE;
							break;

						case 0x7F:
							curr_file->type = OXY_ICE_TYPE;
							break;

						default:
							curr_file->type = OXY_ASM_TYPE;
							break;
						}

						curr_file->locked = true;
					}

					if (data_pos == 16)
					{
						curr_file->icon = ti_GetDataPtr(slot) - 2;
						curr_file->description = (char *)(ti_GetDataPtr(slot) + 256);
					}

					data_pos++;
				}
			}
			ti_Close(slot);
		}
	}
}

void oxy_GetBasicIcons(void)
{
	ti_var_t slot;
	uint8_t palette[] = {223, 24, 224, 0, 248, 6, 228, 96, 16, 29, 231, 255, 222, 189, 148, 74};
	uint8_t data_pos = '\0';
	char temp[2] = " ";
	uint8_t search[] = {tColon, tD, tC, tS, tEnter, tString};
	uint8_t search_ice[] = {tColon, tii, tEnter};

	for (int i = 0; i < oxy_file_system.numfiles; i++)
	{

		struct oxy_files_t *curr_file = &oxy_file[i];

		if (curr_file->type == OXY_PBASIC_TYPE || curr_file->type == OXY_BASIC_TYPE)
		{

			if ((slot = ti_OpenVar(curr_file->name, "r", oxy_GetFileType(curr_file->type))))
			{
				if (!memcmp(search, ti_GetDataPtr(slot), 6))
				{

					curr_file->icon = gfx_MallocSprite(16, 16);
					ti_Seek(6, 0, slot);

					for (int r = 0; r < 256; r++)
					{
						ti_Read(temp, 1, 1, slot);
						ti_Read((void *)(data_pos + r), 1, 1, slot);

						if (data_pos != tString)
						{

							curr_file->icon->data[r] = palette[strtol(temp, NULL, 16)];
						}
						else
						{
							free(curr_file->icon);
							curr_file->icon = NULL;
							break;
						}

						data_pos++;
					}
				}
				else if (memcmp(search_ice, ti_GetDataPtr(slot), 3) > 0)
				{
					curr_file->type = OXY_ICES_TYPE;
					continue;
				}
				ti_Close(slot);
			}
		}

		if (curr_file->type == OXY_BASIC_TYPE)
			curr_file->locked = false;
	}
}

/* Sorting all files */
static int oxy_CompareFolderNames(const void *a, const void *b)
{
	return strcmp(((struct oxy_folders_t *)a)->name, ((struct oxy_folders_t *)b)->name);
}

static int oxy_CompareFileNames(const void *a, const void *b)
{
	return strcmp(((struct oxy_files_t *)a)->name, ((struct oxy_files_t *)b)->name);
}

void oxy_SortFolders(void)
{
	qsort(&(oxy_folder[2]), oxy_file_system.numfolders - 2, sizeof(struct oxy_folders_t), oxy_CompareFolderNames);
}

void oxy_SortFiles(void)
{
	qsort(oxy_file, oxy_file_system.numfiles, sizeof(struct oxy_files_t), oxy_CompareFileNames);
}

/* Other */
uint8_t oxy_GetFileType(uint8_t type)
{
	switch (type)
	{
	case OXY_BASIC_TYPE:
		return TI_PRGM_TYPE;

	case OXY_PBASIC_TYPE:
	case OXY_ASM_TYPE:
	case OXY_C_TYPE:
	case OXY_ICE_TYPE:
	case OXY_ICES_TYPE:
		return TI_PPRGM_TYPE;

	case OXY_APPVAR_TYPE:
		return TI_APPVAR_TYPE;

	default:
		return 255;
	}
}

void oxy_EditPrgm(const char *name)
{
	ti_var_t slot;

	oxy_SaveAll();

	slot = ti_OpenVar(name, "r", TI_PRGM_TYPE);

	if (slot)
	{

		gfx_End();

		os_ClrHomeFull();
		os_ClrHome();

		switch (ti_IsArchived(slot))
		{
		case 0:
			ti_Close(slot);
			EditPrgm(name, 0);
			break;

		case 1:
			ti_SetArchiveStatus(0, slot);
			ti_Close(slot);
			EditPrgm(name, 0);
			slot = ti_OpenVar(name, "r", TI_PRGM_TYPE);
			ti_SetArchiveStatus(1, slot);
			break;
		}

		ti_Close(slot);
	}

	return;
}