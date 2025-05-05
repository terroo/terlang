#include "BuiltinFactory.hpp"
#include "Builtin.hpp"

// Map to store built-in function factories
std::unordered_map<std::type_index, std::function<std::shared_ptr<Callable>()>> builtinFactory = {
    {typeid(std::shared_ptr<Clock>), [](){ return std::make_shared<Clock>(); }},
    {typeid(std::shared_ptr<Rand>), [](){ return std::make_shared<Rand>(); }},
    {typeid(std::shared_ptr<GetEnv>), [](){ return std::make_shared<GetEnv>(); }},
    {typeid(std::shared_ptr<ToString>), [](){ return std::make_shared<ToString>(); }},
    {typeid(std::shared_ptr<Args>), [](){ return std::make_shared<Args>(); }},
    {typeid(std::shared_ptr<Exec>), [](){ return std::make_shared<Exec>(); }}
};

// Map of built-in function names
std::unordered_map<std::string, std::type_index> builtinNames = {
    {"clock", typeid(std::shared_ptr<Clock>)},
    {"rand", typeid(std::shared_ptr<Rand>)},
    {"getenv", typeid(std::shared_ptr<GetEnv>)},
    {"to_string", typeid(std::shared_ptr<ToString>)},
    {"args", typeid(std::shared_ptr<Args>)},
    {"exec", typeid(std::shared_ptr<Exec>)}
};
