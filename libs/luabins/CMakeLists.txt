
project(luabins LANGUAGES C)

add_definitions(-DluaL_reg=luaL_Reg)
add_definitions(-DLUA_COMPAT_ALL)

set(BINS_HEADERS
	submodule/src/fwrite.h
	submodule/src/luabins.h
	submodule/src/luaheaders.h
	submodule/src/luainternals.h
	submodule/src/lualess.h
	submodule/src/savebuffer.h
	submodule/src/saveload.h
	submodule/src/write.h
)

set(BINS_SOURCES
	submodule/src/fwrite.c
	submodule/src/luabins.c
	submodule/src/luainternals.c
	submodule/src/lualess.c
	submodule/src/save.c
	submodule/src/savebuffer.c
	submodule/src/write.c
	submodule/src/load.c
)

add_library(luabins STATIC ${BINS_HEADERS} ${BINS_SOURCES})

target_include_directories(luabins PUBLIC ../lua-5.2.2/src)

target_link_libraries(luabins PUBLIC liblua)
