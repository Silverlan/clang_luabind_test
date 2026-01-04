#include <iostream>

// Include Lua and Luabind headers
extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

import luabind;

struct SomeTest {
	void Print4() { std::cout << "HELLO4" << std::endl; }
};

static luabind::detail::class_rep *get_crep(luabind::object o)
{
	auto *L = o.interpreter();
	luabind::detail::class_rep *crep = nullptr;
	o.push(L);
	if(luabind::detail::is_class_rep(L, -1)) {
		crep = static_cast<luabind::detail::class_rep *>(lua_touserdata(L, -1));
		lua_pop(L, 1);
	}
	else {
		auto *obj = luabind::detail::get_instance(L, -1);
		if(!obj)
			lua_pop(L, 1);
		else {
			lua_pop(L, 1);
			// OK, we were given an object - gotta get the crep.
			crep = obj->crep();
		}
	}
	return crep;
}

static void run_lua(lua_State* L, const std::string& script) {
    // luaL_dostring returns 0 on success and 1 (non-zero) on error
    if (luaL_dostring(L, script.c_str()) != 0) {
        // If there's an error, the message is pushed onto the Lua stack
        std::cerr << "LUA ERROR: " << lua_tostring(L, -1) << std::endl;
        
        // Remove the error message from the stack to keep it clean
        lua_pop(L, 1);
    }
}

int main() {
    // 1. Create a new Lua state
    lua_State* L = luaL_newstate();

    if (!L) {
        std::cerr << "Failed to initialize Lua state." << std::endl;
        return 1;
    }

    // 2. Load standard Lua libraries (math, table, string, etc.)
    luaL_openlibs(L);

    // 3. Initialize luabind
    // This connects luabind's internal machinery to the Lua state
    luabind::open(L);

    std::cout << "Lua and Luabind initialized successfully!" << std::endl;

	{
		auto testQ = luabind::module(L, "testQ");

		auto defTest = luabind::class_<SomeTest>("SomeTest");
		defTest.def("Print", +[](luabind::object o) { std::cout << "HELLO" << std::endl; });
		defTest.def("Print2", +[](SomeTest *t) { std::cout << "HELLO2" << std::endl; });
		defTest.def("Print3", +[](SomeTest &t) { std::cout << "HELLO3" << std::endl; });
		defTest.def("Print4", &SomeTest::Print4);
		testQ[defTest];
		run_lua(L, "print(\"Test1\")");

		SomeTest test {};

		auto _G = luabind::globals(L);
		_G["testst"] = test;

		run_lua(L, "testst:Print()");
		run_lua(L, "testst:Print2()");
		run_lua(L, "testst:Print3()");
		run_lua(L, "testst:Print4()");

		luabind::object val {L, _G["testst"]};
		auto *crep = get_crep(val);
		if(crep) {
			std::cout<<"Name: "<<crep->name()<<std::endl;
		}
	}

    // 4. Close the Lua state when finished
    lua_close(L);

    return 0;
}
