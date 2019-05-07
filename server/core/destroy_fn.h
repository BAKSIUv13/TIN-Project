// Copyright 2019 Piotrek

#ifndef SERVER_CORE_DESTROY_FN_H_
#define SERVER_CORE_DESTROY_FN_H_

#include "core/instr_struct.h"

namespace tin {
using DestroyFn = void(*)(InstrStruct *);
}  // namespace tin

#endif  // SERVER_CORE_DESTROY_FN_H_
