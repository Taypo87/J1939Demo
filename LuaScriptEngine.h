#ifndef LUASCRIPTENGINE_H
#define LUASCRIPTENGINE_H

#include <lua.hpp>
#include <string>

class LuaScriptEngine {
public:
    LuaScriptEngine();
    ~LuaScriptEngine();

    bool runFile(const std::string& filename);
    std::string callFunction(const std::string& funcName, int arg);

private:
    lua_State* L;
};

#endif // LUASCRIPTENGINE_H
