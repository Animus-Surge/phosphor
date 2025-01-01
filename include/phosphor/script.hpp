#pragma once

/**
* Phosphor include/phosphor/script.hpp
* Script class
*/



#include <luajit-2.1/lua.hpp>

#include "phosphor/object.hpp"

class Script : public Component {
    lua_State* L;
};