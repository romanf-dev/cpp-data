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
// Interface to firmware-side API.
//

#ifndef _CPPDATA_H_
#define _CPPDATA_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct cppd_dirent_t
{
    const char* name;
    const void* data;
    size_t size;
    int type;
};

struct cppd_t
{
    const unsigned char* base;
    const void* root_node;
};

struct Node;

int cppd_init(struct cppd_t* handle, const void* buffer);
const struct Node* cppd_open(const struct cppd_t* handle, const char* path);

size_t cppd_read(
    const struct cppd_t* handle, 
    const struct Node* node, 
    void* buffer, 
    size_t len
);

int cppd_readdir(
    const struct cppd_t* handle, 
    const struct Node* node, 
    size_t index, 
    struct cppd_dirent_t* dirent
);

#endif

