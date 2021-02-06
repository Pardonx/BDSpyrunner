#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <functional>
#include <unordered_map>
#include "json/json.h"
#include "include/Python.h"
#define f(type, ptr) (*(type*)(ptr))
#define SYMAS(ret, fn, ...) ((ret(*)(__VA_ARGS__))SYM(fn))
#define SYM dlsym_real
using VA = unsigned long long;
extern "C" {
	_declspec(dllimport) int HookFunction(void*, void*, void*);
	_declspec(dllimport) void* dlsym_real(const char*);
}
template<typename ret = void, typename... Args>
inline ret SYMCALL(const char* sym, Args... args) {
	return ((ret(*)(Args...))SYM(sym))(args...);
}
void* HookRegister(const char* sym, void* hook, void* org) {
	void* found = SYM(sym);
	if (!found)printf("Failed to hook %s\n", sym);
	HookFunction(found, org, hook);
	return org;
}
#define Hook(name, ret, sym, ...)		\
struct name {							\
	static ret(*original)(__VA_ARGS__);	\
	static ret _hook(__VA_ARGS__);		\
};										\
ret(*name::original)(__VA_ARGS__)=*(ret(**)(__VA_ARGS__))HookRegister(sym, &name::_hook,&name::original);\
ret name::_hook(__VA_ARGS__)