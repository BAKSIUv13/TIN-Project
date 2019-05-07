// Copyright 2019 Piotrek

#ifndef SERVER_CORE_CONSTRUCT_FN_H_
#define SERVER_CORE_CONSTRUCT_FN_H_

#include "core/instr_struct.h"

namespace tin {
using ConstructFn = void(*)(InstrStruct *);
}  // namespace tin

#endif  // SERVER_CORE_CONSTRUCT_FN_H_
