// Copyright 2019 TIN

#ifndef SERVER_CORE_FACTORY_H_
#define SERVER_CORE_FACTORY_H_

#include <memory>
#include <functional>

namespace tin {
class InstrStruct;

using Factory = std::function<std::unique_ptr<InstrStruct>()>;
}  // namespace tin

#endif  // SERVER_CORE_FACTORY_H_
