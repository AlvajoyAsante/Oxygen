/**
 * @file oxy_files.h
 * @brief Declares Oxygen's virtual file system data structures and helpers.
 */
#ifndef OXY_FILES_H
#define OXY_FILES_H

#include <tice.h>
#include <graphx.h>

#define OXY_BASIC_TYPE 0
#define OXY_PBASIC_TYPE 1
#define OXY_ICES_TYPE 2
#define OXY_ICE_TYPE 3
#define OXY_C_TYPE 4
#define OXY_ASM_TYPE 5
#define OXY_APPVAR_TYPE 6
#define OXY_ERROR_TYPE 7

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Describes a program or AppVar tracked by the Oxygen file index.
 */
struct oxy_files_t {
    uint8_t user_id;
	
	char name[9];
	uint8_t type;
	
	gfx_sprite_t *icon;
    char *description;
    
	bool archived;
    bool locked;
	bool hidden;
	
    int location;
    int size;
    
    bool pinned;
};
extern struct oxy_files_t *oxy_file;

/**
 * @brief Describes a virtual folder tracked by the Oxygen file index.
 */
struct oxy_folders_t {
	uint8_t user_id;
    
	char name[9];
    gfx_sprite_t *icon;
	
	bool locked;
	
    int location;
	int position;
};
extern struct oxy_folders_t *oxy_folder;

/**
 * @brief Stores aggregate counts for the indexed file system.
 */
struct oxy_file_system_t {
    int numfiles;
    int numfolders;
	int numpins;
};
extern struct oxy_file_system_t oxy_file_system;


/**
 * @brief Initializes the Oxygen file system state.
 */
void oxy_InitFilesStystem(void);

/**
 * @brief Verifies that the indexed file-system state is still valid.
 */
void oxy_CheckFileSystem(void);

/**
 * @brief Rebuilds the indexed file-system state from calculator storage.
 */
void oxy_RescanFileSystem(void);

/** 
 * @brief Detects all files on calculator (programs and app-vars).   
 */
void oxy_DetectAllFiles(void);

/* Deleting folders and files */
/**
 * @brief Removes a folder entry from the folder index.
 * @param index Folder index to remove.
 */
void oxy_DeleteFolder(int index);

/**
 * @brief Removes a file entry from the file index.
 * @param index File index to remove.
 */
void oxy_DeleteFile(int index);

/* Creating folders and files */ 
/**
 * @brief Allocates space for a new file entry.
 * @return `true` if a new entry was reserved successfully.
 */
bool oxy_AddFile(void);

/**
 * @brief Adds a folder entry at the requested position.
 * @param name Folder name to store.
 * @param position Position for the new folder entry.
 * @return `true` if the folder was added successfully.
 */
bool oxy_AddFolder(char *name, int position);

/* Detecting files icons */
/**
 * @brief Assigns assembly program icons to indexed file entries.
 */
void oxy_GetAsmIcons(void);

/**
 * @brief Assigns BASIC program icons to indexed file entries.
 */
void oxy_GetBasicIcons(void);

/* Sorting all files */
/**
 * @brief Sorts all indexed folders.
 */
void oxy_SortFolders(void);

/**
 * @brief Sorts all indexed files.
 */
void oxy_SortFiles(void);

/**
 * @brief Maps an Oxygen file type to the corresponding TI-OS file type.
 * @param type Oxygen file type constant.
 * @return TI-OS file type identifier.
 */
uint8_t oxy_GetFileType(uint8_t type);

/* Other */
/**
 * @brief Delete a file or folder.
 * @param isfolder Whether the indexed item is a folder.
 * @param index File or folder index.
 * @return `true` if the item was removed successfully.
 */
bool oxy_Delete(bool isfolder, int index);

/**
 * @brief Launches an editor for the specified program.
 * @param prog_name Program name to edit.
 * @param editor_name Name of the editor program to run.
 * @param callback Callback invoked by the OS while the editor is running.
 * @return `true` if the editor launched successfully.
 */
bool oxy_EditPrgm(char *prog_name, const char *editor_name, os_runprgm_callback_t callback);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OXY_FILES_H__ */