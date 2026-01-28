#ifndef VFS_H
#define VFS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct vfs_node;

// Function pointers that every FS driver must provide
typedef uint32_t (*vfs_read_func)(struct vfs_node*, uint32_t, uint32_t, uint8_t*);
typedef struct vfs_node* (*vfs_finddir_func)(struct vfs_node*, const char* name);

typedef struct vfs_node {
    char name[128];
    uint32_t mask;        // Permissions
    uint32_t uid;         // User ID
    uint32_t gid;         // Group ID
    uint32_t flags;       // Node type (File, Directory, etc.)
    uint32_t inode;       // FS-specific identifier
    uint32_t length;      // Size in bytes
    
    // Function table
    vfs_read_func    read;
    vfs_finddir_func finddir;
    
    struct vfs_node* ptr; // Used for mountpoints or symlinks
} vfs_node_t;

// Standard node types
#define VFS_FILE        0x01
#define VFS_DIRECTORY   0x02
#define VFS_MOUNTPOINT  0x04

// The root of the filesystem (/)
extern vfs_node_t* vfs_root;

// Public API
uint32_t vfs_read(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer);
vfs_node_t* vfs_finddir(vfs_node_t* node, const char* name);

#endif
