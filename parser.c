//
// Copyright 2022 romanf
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright 
// notice, this list of conditions and the following disclaimer in the 
// documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
// THE POSSIBILITY OF SUCH DAMAGE.
//

//
// Implementations of firmware-side parsing API.
//

#include "node.h"
#include "cppdata.h"
#include <string.h>

_Static_assert(sizeof(struct Node) == SizeOfNode, "Wrong node sizeof!");

struct substring_t {
    const char* ptr;
    size_t size;
};

static inline enum cppd_entry_type_t translate_type(enum NodeType inner_type) {
    static const enum cppd_entry_type_t lookup_table[] = {
        [Folder] = CPPD_FOLDER,
        [Number] = CPPD_NUMBER,
        [String] = CPPD_STRING,
        [Blob] = CPPD_BLOB,
    };

    return lookup_table[inner_type];
}

//
// Since binary data contains only offsets it is required to add base
// to them to get valid pointers.
//
static inline const void* to_ptr(const struct cppd_t* handle, size_t offset) {
    return (handle->base + offset);
}

//
// Compares whether the first n symbols of s1 and s2 are equal.
//
static inline bool str_equal(const char* s1, const char* s2, size_t n) {
    bool equal = true;

    for (size_t i = 0; i < n; ++i) {
        if (s1[i] != s2[i]) {
            equal = false;
            break;
        }
    }

    return equal;
}

//
// Extracts next path item from string like /first/second/foo/bar.
// Returns pointer to first symbol and length. Source string is not modified.
//
static inline bool next_path_item(const char* s, struct substring_t* out) {
    size_t i = 0;
    
    if (*s == '/') { 
        ++s;
    }

    if (*s != '\0') {
        
        while ((s[i] != 0) && (s[i] != '/')) { 
            ++i; 
        }

        out->ptr = s;
        out->size = i;
    }

    return (i != 0);
}

//
// Looks up for "item" in folder represented by "folder".
// Returns either pointer to the found node or NULL.
//
static const struct Node* lookup(
    const struct cppd_t* handle, 
    const struct Node* folder, 
    const struct substring_t* item) {
        
    const struct Node* const folder_content = to_ptr(
        handle, 
        folder->payload.val
    );

    for (size_t i = 0; i < folder->type.len; ++i) {
        const char* const name = to_ptr(
            handle, 
            (uintptr_t) folder_content[i].name
        );

        if (str_equal(name, item->ptr, item->size)) {
            return &folder_content[i];
        }
    }

    return (const void*)0;
}

//
// Validates user buffer and creates a handle to access binary storage.
// Return 0 on success.
//
int cppd_init(struct cppd_t* handle, const void* buffer) {
    int error = 1;
    const uint64_t* const header = buffer;

    if (*header == 0xcafebabe) {
        handle->base = buffer;
        handle->root_node = (const void*)(handle->base + header[1]);
        error = 0;
    }

    return error;
}

//
// Returns pointer to node correspoinding to given path or NULL.
//
const struct Node* cppd_open(const struct cppd_t* handle, const char* path) {
    size_t offset = 0;
    struct substring_t part;
    const struct Node* folder = handle->root_node;

    while (next_path_item(path + offset, &part)) {
        offset += part.size + 1;
        folder = lookup(handle, folder, &part);

        if (folder == (void*)0) {
            break;
        }
    }

    return folder;
}

//
// Returns pointer to data.
//
const void* cppd_dataptr(const struct cppd_t* handle, const struct Node* node, size_t* len) {
    size_t datasize = 0;

    if (node->type.id != Folder) {
        const void* const data = (node->type.id == Number) ?
            &(node->payload.num) :
            to_ptr(handle, node->payload.val);

        if (len) {
            *len = node->type.len * node->type.sz;
        }

        return data;
    }

    return 0;
}

//
// Reads data from binary node into the buffer.
// Returns size of read data or 0 if no data.
//
size_t cppd_read(
    const struct cppd_t* handle, 
    const struct Node* node, 
    void* buffer, 
    size_t len) {  
        
    size_t datasize = 0;
    const void* const ptr = cppd_dataptr(handle, node, &datasize);

    if (ptr && len >= datasize) {
        memcpy(buffer, ptr, datasize);
        return datasize;
    }

    return 0;
}

//
// Reads folder items.
//
int cppd_readdir(
    const struct cppd_t* handle, 
    const struct Node* node, 
    size_t index, 
    struct cppd_dirent_t* dirent) {
        
    int error = 1;

    if ((node->type.id == Folder) && (index < node->type.len)) {
        const struct Node* const n = to_ptr(handle, node->payload.val);

        dirent->name = to_ptr(handle, (uintptr_t) n[index].name);
        dirent->data = n + index;
        dirent->size = n[index].type.sz * n[index].type.len;
        dirent->type = translate_type(n[index].type.id);
        error = 0;
    }

    return error;
}

