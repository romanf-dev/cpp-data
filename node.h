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
// This file may be included in both C and C++ sources and contains shared
// data structures such as binary nodes descriptions.
//

#ifndef _NODE_H_
#define _NODE_H_

#ifdef __cplusplus
#   include <string_view>
#   include <cstdint>
#else
#   include <stdint.h>
#endif

enum NodeType {
    Folder,
    Number,
    String,
    Blob
};

typedef struct _NodeDescriptor {
    const uint32_t len:24;
    const uint32_t sz:6;
    const uint32_t id:2;
    
#ifdef __cplusplus
    consteval _NodeDescriptor(NodeType, size_t, size_t);
#endif

}
NodeDescriptor;

typedef union _NodePayload {
    const uint64_t num;
    const void* const ptr;
    const uintptr_t val;
    
#ifdef __cplusplus
    template<typename T> consteval _NodePayload(const T*);
    consteval _NodePayload(uint64_t);
#endif

}
NodePayload;

//
// This is the main building block of binary file.
//
struct Node {
    const char* const name;
    const NodePayload payload;
    const NodeDescriptor type;
    
#ifdef __cplusplus
    consteval Node(const std::string_view&, uint64_t);  
    template<typename T> static consteval NodeType TypeToId();
    template<typename T, size_t N> 
        consteval Node(const std::string_view&, const T (&)[N]);
    consteval Node() = delete;
#endif

};

enum {
    SizeOfNode = 24,
};

#endif
