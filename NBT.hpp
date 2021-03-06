#pragma once
#include "pch.h"
#include "BDS.hpp"
using namespace std;
struct string_span {
	size_t len;
	const char* str;
	string_span(const char* s) : len(strlen(str)), str(s) {}
	string_span(const std::string& s) : len(s.length()), str(s.c_str()) {}
};
enum TagType {
	End, Byte, Short, Int, Int64, Float,
	Double, ByteArray, String, List, Compound,
};
using cm = map<string, struct CompoundTagVariant>;
struct Tag {
	VA vftable;
	VA value[3];

	cm& asCom() {
		return *(cm*)(value);
	}
	Tag() {
		SYMCALL("??0CompoundTag@@QEAA@XZ", this);
	}
	~Tag() {
		SYMCALL("??1CompoundTag@@UEAA@XZ", this);
	}
	void put(const string& key, const Tag* value) {
		return SYMCALL("?put@CompoundTag@@QEAAAEAVTag@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$$QEAV2@@Z",
			this, key, value);
	}
	void putByte(const string& key, const unsigned char value) {
		return SYMCALL("?putByte@CompoundTag@@QEAAAEAEV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@E@Z",
			this, key, value);
	}
	void putShort(const string& key, const short value) {
		return SYMCALL("?putShort@CompoundTag@@QEAAAEAFV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@F@Z",
			this, key, value);
	}
	void putString(const string& key, const string& value) {
		return SYMCALL("?putString@CompoundTag@@QEAAAEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V23@0@Z",
			this, key, value);
	}
	void putInt(const string& key, const int& value) {
		return SYMCALL("?putInt@CompoundTag@@QEAAAEAHV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@Z",
			this, key, value);
	}
	void putInt64(const string& key, const int64_t& value) {
		return SYMCALL("?putInt64@CompoundTag@@QEAAAEA_JV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_J@Z",
			this, key, value);
	}
	void putFloat(const string& key, const float& value) {
		return SYMCALL("?putFloat@CompoundTag@@QEAAAEAMV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@M@Z",
			this, key, value);
	}
	void putBoolean(const string& key, const bool& value) {
		return SYMCALL("?putBoolean@CompoundTag@@QEAAXV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_N@Z",
			this, key, value);
	}
	void putCompound(const string& key, const Tag* value) {
		//return SYMCALL("?putCompound@CompoundTag@@QEAAAEAV1@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V1@@Z",
		return SYMCALL("?putCompound@CompoundTag@@QEAAPEAV1@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$unique_ptr@VCompoundTag@@U?$default_delete@VCompoundTag@@@std@@@3@@Z",
			this, key, &value);
	}

	void add(Tag*& t) {
		SYMCALL("?add@ListTag@@QEAAXV?$unique_ptr@VTag@@U?$default_delete@VTag@@@std@@@std@@@Z",
			this, &t);
	}
	void deList() {
		SYMCALL("??1ListTag@@UEAA@XZ", this);
	}
};
struct CompoundTagVariant {
	int type() {
		return *((char*)this + 40);
	}
	auto& asByte() { return *(unsigned char*)((VA)this + 8); }
	auto& asShort() { return *(short*)((VA)this + 8); }
	auto& asInt() { return *(int*)((VA)this + 8); }
	auto& asInt64() { return *(long long*)((VA)this + 8); }
	auto& asFloat() { return *(float*)((VA)this + 8); }
	auto& asDouble() { return *(double*)((VA)this + 8); }
	auto& asString() { return *(string*)((VA)this + 8); }
	auto& asList() { return *(vector<Tag*>*)((VA)this + 8); }
	auto& asCompound() { return *(map<string, CompoundTagVariant>*)((VA)this + 8); }
};
Tag* newListTag() {
	Tag* t = 0;
	SYMCALL("??$make_unique@VListTag@@$$V$0A@@std@@YA?AV?$unique_ptr@VListTag@@U?$default_delete@VListTag@@@std@@@0@XZ",
		&t);
	return t;
}
Json::Value toJson(Tag* t) {
	Json::Value j;
	for (auto& x : t->asCom()) {
		switch (x.second.type()) {
		case End:
			break;
		case Byte:
			j[x.first + to_string(Byte)] = x.second.asByte();
			break;
		case Short:
			j[x.first + to_string(Short)] = x.second.asShort();
			break;
		case Int:
			j[x.first + to_string(Int)] = x.second.asInt();
			break;
		case Int64:
			j[x.first + to_string(Int64)] = x.second.asInt64();
			break;
		case Float:
			j[x.first + to_string(Float)] = x.second.asFloat();
			break;
		case Double:
			j[x.first + to_string(Double)] = x.second.asDouble();
			break;
		case ByteArray:break;
		case String:
			j[x.first + to_string(String)] = x.second.asString();
			break;
		case List:
			for (auto& i : x.second.asList()) {
				j[x.first + to_string(List)].append(toJson(i));
			}
			break;
		case Compound:
			j[x.first + to_string(Compound)] = toJson((Tag*)&x.second);
			break;
		}
	}
	return move(j);
}
Tag* toTag(const Json::Value& j) {
	if (!j.isObject())
		return 0;
	Tag* c = new Tag;
	for (auto& x : j.getMemberNames()) {
		string key = x;
		auto e = key.end();
		int type = 0;
		if (*(e - 2) == '1' && *(e - 1) == '0') {
			type = 10;
			key.resize(key.length() - 2);
		}
		else if (*(e - 1) >= '0' && *(e - 1) <= '9') {
			type = *(e - 1) - '0';
			key.pop_back();
		}
		else continue;
		//cout << key << endl << type << endl;
		switch (type) {
		case End:break;
		case Byte:
			c->putByte(key, j[x].asInt());
			break;
		case Short:
			c->putShort(key, j[x].asInt());
			break;
		case Int:
			c->putInt(key, j[x].asInt());
			break;
		case Int64:
			c->putInt64(key, j[x].asInt());
			break;
		case Float:
			c->putFloat(key, j[x].asFloat());
			break;
		case Double:
			c->putFloat(key, (float)j[x].asDouble());
			break;
		case ByteArray:break;
		case String:
			c->putString(key, j[x].asString());
			break;
		case List:
		{
			Tag* lt = newListTag();
			for (auto& i : j[x]) {
				Tag* c2 = toTag(i);
				lt->add(c2);
			}
			c->put(key, lt);
			lt->deList();
		}
		break;
		case Compound:
		{
			Tag* ccc = toTag(j[x]);
			c->putCompound(key, ccc);
		}
		break;
		}
	}
	return c;
}