#pragma once
#include <functional>
#include <typeindex>
#include "Builtin.hpp"

extern std::unordered_map<std::type_index, std::function<std::shared_ptr<Callable>()>> builtinFactory;
extern std::unordered_map<std::string, std::type_index> builtinNames;
