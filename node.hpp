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
// Implementations of constexpr constructors for various types of nodes.
// It currently requires C++17 because of string_view, but may be tuned
// down to C++11 with some modifications.
//

#ifndef _NODE_HPP_
#define _NODE_HPP_

#include "node.h"

template<typename T> constexpr _NodePayload::_NodePayload(const T* p)
    :
    ptr(static_cast<const void*>(p)) 
{
}

constexpr _NodePayload::_NodePayload(std::uint64_t n)
    : 
    num(n) 
{
}

constexpr _NodeDescriptor::_NodeDescriptor(NodeType t, size_t l, size_t s)
    : 
    len(l),
    sz(s),
    id(t) 
{
}

constexpr Node::Node(const std::string_view& n, uint64_t value)
    :
    name(std::data(n)), 
    payload(value),
    type(Number, 1, sizeof(value))
{
}

template<typename T> constexpr NodeType Node::TypeToId()
{
    return std::is_same<T, char>::value ? 
        String :
        std::is_same<T, Node>::value ? 
            Folder :
            Blob;
}

template<typename T, size_t N> constexpr Node::Node(const std::string_view& n, const T (&s)[N])
    :
    name(std::data(n)),
    payload(std::data(s)),
    type(TypeToId<T>(), N, sizeof(T))
{
}

#define SET_ROOT(folder) \
    extern "C" constexpr Node root("/", (folder)); \
    static_assert(root.type.id == Folder)

#endif

