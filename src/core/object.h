#pragma once

#include "comm/comm_def.h"

class Object
{

public:
    Object() = default;
    virtual ~Object() = default;
    
    virtual s32 init() { return 0; }
    virtual s32 handle_events() { return 0; }
    virtual s32 update(s64 now_ms) { return 0; }
    virtual s32 render() { return 0; }
    virtual s32 clean() { return 0; }

protected:


};