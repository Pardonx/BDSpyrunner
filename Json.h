// Json.h made by twoone3
// All development is aimed at learning
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
using std::string;
using std::to_string;

enum Type {
	_null, _bool, _int, _longlong,
	_double, _string, _array, _object
};
class Json {
	using Array = std::vector<Json>;
	using Object = std::map<string, Json>;
	union Var {
		bool b; int i; long long l; double d;
		string* s; Array* a; Object* o;
	} data;
	Type t;

public:
	inline auto& asBool() { return data.b; }
	inline auto& asInt() { return data.i; }
	inline auto& asLongLong() { return data.l; }
	inline auto& asDouble() { return data.d; }
	inline auto& asString() { return *data.s; }
	inline auto& asArray() { return *data.a; }
	inline auto& asObject() { return *data.o; }

	Json() : t(_null) { data.b = 0; }
	Json(const bool b) : t(_bool) { data.b = b; }
	Json(const int i) : t(_int) { data.i = i; }
	Json(const long long l) : t(_longlong) { data.l = l; }
	Json(const double d) : t(_double) { data.d = d; }
	Json(const char* s) : t(_string) { data.s = new string(s); }
	Json(const string& s) : t(_string) { data.s = new string(s); }
	Json(const Array& a) : t(_array) { data.a = new Array(a); }
	Json(const Object& o) : t(_object) { data.o = new Object(o); }
	Json(const Type& type) : t(type) {
		switch (type) {
		case _null:break;
		case _bool:break;
		case _int:break;
		case _longlong:break;
		case _double:break;
		case _string:
			data.s = new string;
			break;
		case _array:
			data.a = new Array;
			break;
		case _object:
			data.o = new Object;
			break;
		}
	}
	Json(const Json& j) {
		clear();
		switch (j.t) {
		case _null:break;
		case _bool:data.b = j.data.b; break;
		case _int:data.i = j.data.i; break;
		case _longlong:data.l = j.data.l; break;
		case _double:data.d = j.data.d; break;
		case _string:
			data.s = new string;
			*data.s = *j.data.s;
			break;
		case _array:
			data.a = new Array;
			*data.a = *j.data.a;
			break;
		case _object:
			data.o = new Object;
			*data.o = *j.data.o;
			break;
		}
		t = j.t;
	};
	Json(Json&& j) {
		t = j.t;
		data.l = j.data.l;
		j.t = _null;
		j.data.l = 0;
	};
	~Json() { clear(); }

	string toFastString() const {
		switch (t) {
		case _null:return "null";
		case _bool:return data.b ? "true" : "false";
		case _int:return to_string(data.i);
		case _longlong:return to_string(data.l);
		case _double:return to_string(data.d);
		case _string:return '"' + *data.s + '"';
		case _array: {
			string s;
			s += '[';
			if (data.a->empty())
				return s += ']';
			for (auto& i : *data.a) {
				s += i.toFastString();
				s += ',';
			}
			s.pop_back();
			s += ']';
			return s;
		}
		case _object: {
			string s;
			s += '{';
			if (data.o->empty())
				return s += '}';
			for (auto& i : *data.o) {
				s += '"';
				s += i.first;
				s += '"';
				s += ':';
				s += i.second.toFastString();
				s += ',';
			}
			s.pop_back();
			s += '}';
			return s;
		}
		}
		return "";
	}
#define addTab for (unsigned num = tab; num; num--)s += '\t';
	string toStyleString() const {
		static unsigned tab = 0;
		switch (t) {
		case _null:return "null";
		case _bool:return data.b ? "true" : "false";
		case _int:return to_string(data.i);
		case _longlong:return to_string(data.l);
		case _double:return to_string(data.d);
		case _string:return '"' + *data.s + '"';
		case _array: {
			string s;
			s += '[';
			if (data.a->empty()) {
				return s += ']';
			}
			s += '\n';
			tab++;
			for (auto& i : *data.a) {
				addTab;
				s += i.toStyleString();
				s += ',';
				s += '\n';
			}
			tab--;
			s.resize(s.length() - 2);
			s += '\n';
			addTab;
			s += ']';
			return s;
		}
		case _object: {
			string s;
			s += '{';
			if (data.o->empty()) {
				return s += '}';
			}
			s += '\n';
			tab++;
			for (auto& i : *data.o) {
				addTab;
				s += '"';
				s += i.first;
				s += '"';
				s += ':';
				s += ' ';
				s += i.second.toStyleString();
				s += ',';
				s += '\n';
			}
			tab--;
			s.resize(s.length() - 2);
			s += '\n';
			addTab;
			s += '}';
			return s;
		}
		}
		return "";
	}
	Type getType() { return t; }
	void remove(const unsigned i) {
		if (t == _array)
			data.a->erase(data.a->begin() + i);
	}
	void remove(const string& i) {
		if (t == _object)
			data.o->erase(i);
	}
	void append(const Json& j) {
		if (t == _array)
			data.a->push_back(j);
	}
	bool empty() {
		switch (t) {
		case _null:return 1;
		case _bool:break;
		case _int:break;
		case _longlong:break;
		case _double:break;
		case _string:break;
		case _array:return data.a->empty();
		case _object:return data.o->empty();
		}
		return 0;
	}
	bool Has(const string& key) {
		if (t == _object) {
			return data.o->find(key) != data.o->end();
		}
		return 0;
	}
	void clear() {
		switch (t) {
		case _null:break;
		case _bool:break;
		case _int:break;
		case _longlong:break;
		case _double:break;
		case _string:
			delete data.s;
			break;
		case _array:
			delete data.a;
			break;
		case _object:
			delete data.o;
			break;
		}
		t = _null;
		data.o = 0;
	}

	Json& operator[](const string& i) {
		if (t == _object) {
				return (*data.o)[i];
		}
		static Json j;
		return j;
	}
	Json& operator[](const unsigned i) {
		if (t == _array)
			return (*data.a)[i];
		else {
			static Json j;
			return j;
		}
	}
	Json& operator=(const Json& j) {
		clear();
		switch (j.t) {
		case _null:break;
		case _bool:data.b = j.data.b; break;
		case _int:data.i = j.data.i; break;
		case _longlong:data.l = j.data.l; break;
		case _double:data.d = j.data.d; break;
		case _string:
			data.s = new string;
			*data.s = *j.data.s;
			break;
		case _array:
			data.a = new Array;
			*data.a = *j.data.a;
			break;
		case _object:
			data.o = new Object;
			*data.o = *j.data.o;
			break;
		}
		t = j.t;
		return *this;
	};
	Json& operator=(Json&& j) {
		t = j.t;
		data.l = j.data.l;
		j.t = _null;
		j.data.l = 0;
		return *this;
	};

#define CHECK_END if(!*it)return 0
#define skip while (*it  &&( *it == ' ' or *it == '\n' or *it == '\t' or *it == '\r'))it++
	using It = const char*;
	bool fromString(const char* s) {
		clear();
		return parse_value(s);
	}
private:
	bool parse_value(It& it) {
		skip;
		switch (*it) {
		case 'n':if (!strncmp(&(*it), "null", 4)) { it += 4; }break;
		case 't':if (!strncmp(&(*it), "true", 4)) { *this = true; it += 4; }break;
		case 'f':if (!strncmp(&(*it), "false", 5)) { *this = false; it += 5; }break;
		case '[': {return parse_array(it); }break;
		case '{': {return parse_object(it); }break;
		case '"': {
			string tmps;
			bool ret = parse_string(tmps, it);
			*this = tmps;
			return ret;
		} break;
		default:
			if (*it == '-' || (*it >= '0' && *it <= '9')) {
				return parse_number(it);
			}
			return 0;
		}
		return 1;
	}
	bool parse_number(It& it) {
		string tmp;
		bool dou = 0;
		while ((*it == '.' or *it == '-' or (*it >= '0' && *it <= '9')) && *it) {
			tmp += *it;
			it++;
			if (*it == '.')dou = 1;
		}
		if (dou)
			*this = strtod(tmp.c_str(), 0);
		else
			*this = strtoll(tmp.c_str(), 0, 0);
		return 1;
	}
	bool parse_object(It& it) {
		it++;
		skip;
		t = _object;
		data.o = new Object;
		if (*it == '}') {
			it++;
			return 1;
		}
		while (1) {
			skip;
			CHECK_END;
			if (*it != '\"')
				return 0;
			string key;
			if (!(parse_string(key, it)))
				return 0;
			skip;
			CHECK_END;
			if (*it != ':')
				return 0;
			it++;
			skip;
			CHECK_END;
			Json j;
			if (!j.parse_value(it))
				return 0;
#if __cplusplus < 201703L
			(*data.o)[key] = std::move(j);
#else
			data.o->insert(make_pair(key, j));
#endif
			skip;
			CHECK_END;
			if (*it == '}') {
				it++;
				return 1;
			}
			else if (*it == ',')
				it++;
			else {
				return 0;
			}
		}
	}
	static bool parse_string(string& tmp_str, It& it) {
		char ch = 0;
		it++;
		while (*it) {
			ch = *it++;
			switch (ch) {
			case '\"':
				return 1;
			case '\\':
				if (*it) {
					switch (*it++) {
					case '\"': tmp_str += '\"'; break;
					case 'n': tmp_str += '\n'; break;
					case 'r': tmp_str += '\r'; break;
					case 't': tmp_str += '\t'; break;
					case 'f': tmp_str += '\f'; break;
					case 'b': tmp_str += '\b'; break;
					case '/': tmp_str += '/'; break;
					case '\\': tmp_str += '\\'; break;
					default: return 0;
					}
				}
				break;
			default:
				//if (ch < ' ')return 0;
				tmp_str += ch;
				break;
			}
		}
		return 1;
	}
	bool parse_array(It& it) {
		it++;
		t = _array;
		data.a = new Array;
		skip;
		if (*it == ']') {
			it++;
			return 1;
		}
		while (*it) {
			Json j;
			skip;
			if (!j.parse_value(it))
				return 0;
			skip;
			CHECK_END;
			data.a->push_back(j);
			if (*it == ',') {
				it++;
			}
			else if (*it == ']') {
				it++;
				break;
			}
			else
				return 0;
		}
		return 1;
	}
};
static_assert(sizeof(Json) == 16);