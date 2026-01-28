#include "vfs.h"
#include "kmalloc.h"
#include "string_utils.h"

vfs_node_t* vfs_root = NULL;

uint32_t vfs_read(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer) {
    if (node->read != NULL) {
        return node->read(node, offset, size, buffer);
    }
    return 0;
}

vfs_node_t* vfs_finddir(vfs_node_t* node, const char* name) {
    if ((node->flags & VFS_DIRECTORY) && node->finddir != NULL) {
        return node->finddir(node, name);
    }
    return NULL;
}
