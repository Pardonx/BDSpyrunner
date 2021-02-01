// Json.h made by twoone3
// All development is aimed at learning
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
using std::string;
using std::to_string;
using std::cerr;
using std::endl;

class Json {
public:
	using Array = std::vector<Json>;
	using Object = std::map<string, Json>;
	enum Type {
		_null, _bool, _int, _longlong,
		_double, _string, _array, _object
	};

	inline auto& asBool()const { return data.b; }
	inline auto& asInt()const { return data.i; }
	inline auto& asLongLong()const { return data.l; }
	inline auto& asDouble()const { return data.d; }
	inline auto& asString()const { return *data.s; }
	inline auto& asArray()const { return *data.a; }
	inline auto& asObject()const { return *data.o; }

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

	string toFastString()const {
		switch (t) {
		case _null:return "null";
		case _bool:return data.b ? "true" : "false";
		case _int:return to_string(data.i);
		case _longlong:return to_string(data.l);
		case _double:return tostring(data.d);
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
	string toStyledString()const {
		static unsigned tab = 0;
		switch (t) {
		case _null:return "null";
		case _bool:return data.b ? "true" : "false";
		case _int:return to_string(data.i);
		case _longlong:return to_string(data.l);
		case _double:return tostring(data.d);
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
				s += i.toStyledString();
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
				s += i.second.toStyledString();
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

	Type getType()const { return t; }
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
		else if (t == _null) {
			*this = _array;
			data.a->push_back(j);
		}
	}
	bool empty()const {
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
	bool Has(const string& key)const {
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
		if (t != _object) {
			clear();
			*this = _object;
		}
		return (*data.o)[i];
	}
	Json& operator[](const unsigned i) {
		if (t != _array) {
			clear();
			*this = _array;
		}
		if (i > data.a->size())
			data.a->resize(i);
		return (*data.a)[i];
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

#define CHECK_END if(!*it) throw Error("Unexpected character");
#define skip while (*it == ' ' or *it == '\n' or *it == '\t' or *it == '\r')it++
	bool fromString(const char* s) {
		clear();
		try {
			parse_value(s);
		}
		catch (const Error& x) {
			cerr << "[Json] " << x.data << endl;
			return 0;
		}
		catch (...) {
			cerr << "[Json] Unknown exception\n";
			return 0;
		}
		return 1;
	}
private:
	struct Error {
		string data;
		Error(const char* s) :data(s) {}
		Error(const string& s) :data(s) {}
		Error(const string& s, const char* it) {
			string str(it - 7, 15);
			for (auto& c : str) {
				if (c == '\n')
					c = 0;
			}
			data = s + '\n' + str;
		}
	};
	void parse_value(const char*& it) {
		skip;
		switch (*it) {
		case 'n':
			if (!strncmp(it, "null", 4))it += 4;
			else throw Error("Missing \"null\"", it);
			break;
		case 't':
			if (!strncmp(it, "true", 4)) { *this = true; it += 4; }
			else throw Error("Missing \"true\"", it);
			break;
		case 'f':
			if (!strncmp(it, "false", 5)) { *this = false; it += 5; }
			else throw Error("Missing \"false\"", it);
			break;
		case '[': parse_array(it); break;
		case '{': parse_object(it); break;
		case '"':*this = parse_string(it); break;
		default:
			if (*it == '-' || (*it >= '0' && *it <= '9')) {
				parse_number(it);
			}
			else throw Error("Unexpected character", it);
		}
	}
	void parse_number(const char*& it) {
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
	}
	void parse_object(const char*& it) {
		it++;
		skip;
		*this = _object;
		if (*it == '}') {
			it++;
			return;
		}
		while (1) {
			skip;
			CHECK_END;
			if (*it != '"')
				throw Error("Missing '\"'", it);
			string key = parse_string(it);
			skip;
			CHECK_END;
			if (*it != ':')
				throw Error("Missing ':'", it);
			it++;
			skip;
			CHECK_END;
			Json j;
			j.parse_value(it);
			(*data.o)[key] = static_cast<Json&&>(j);
			skip;
			CHECK_END;
			if (*it == '}') {
				it++;
				return;
			}
			else if (*it == ',')
				it++;
			else {
				throw Error("Missing ',' or '}'", it);
			}
		}
	}
	static string parse_string(const char*& it) {
		string s;
		char ch = 0;
		it++;
		while (1) {
			ch = *it++;
			switch (ch) {
			case '\"':return s;
			case '\\':
				CHECK_END;
				switch (*it++) {
				case '"': s += '"'; break;
				case 'n': s += '\n'; break;
				case 'r': s += '\r'; break;
				case 't': s += '\t'; break;
				case 'f': s += '\f'; break;
				case 'b': s += '\b'; break;
				case '/': s += '/'; break;
				case '\\': s += '\\'; break;
				case 'u': {
					unsigned u = 0;
					parse_hex4(it, u);
					if (u >= 0xD800 && u <= 0xDBFF) {
						CHECK_END;
						if (*it++ != '\\')
							throw Error("Invalid UNICODE surrogate", it);
						if (*it++ != 'u')
							throw Error("Invalid UNICODE surrogate", it);
						unsigned tmp_u;
						parse_hex4(it, tmp_u);
						if (tmp_u < 0xDC00 || tmp_u > 0xDFFF)
							throw Error("Invalid UNICODE surrogate", it);
						u = 0x10000 + (u - 0xD800) * 0x400 + (tmp_u - 0xDC00);
					}
					if (u > 0x10FFFF)
						throw Error("Invalid UNICODE hex", it);
					encode_utf8(u, s);
					break;
				}
				default:throw Error("Unknown escape", it);
				}
				break;
			default:
				//if (ch < ' ')return 0;
				s += ch;
				break;
			}
		}
		return s;
	}
	void parse_array(const char*& it) {
		it++;
		*this = _array;
		skip;
		if (*it == ']') {
			it++;
			return;
		}
		while (*it) {
			Json j;
			j.parse_value(it);
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
				throw Error("Missing ',' or ']'", it);
		}
	}
	static string tostring(const double d) {
		char buf[16];
		sprintf_s(buf, "%lf", d);
		string s = buf;
		while (s.back() == '0') {
			s.pop_back();
			if (s.back() == '.')
				break;
		}
		return s;
	}
	static void parse_hex4(const char*& it, unsigned& u) {
		u = 0;
		char ch = 0;
		for (int i = 0; i < 4; i++) {
			u <<= 4;
			CHECK_END;
			ch = *it++;
			if (ch >= '0' && ch <= '9')
				u |= ch - '0';
			else if (ch >= 'a' && ch <= 'f')
				u |= ch - 'a' + 10;
			else if (ch >= 'A' && ch <= 'F')
				u |= ch - 'A' + 10;
			else throw Error("Unknown hex", it);
		}
	}
	static void encode_utf8(unsigned& u, string& s) {
		if (u <= 0x7F)
			s += (u & 0xFF);
		else if (u <= 0x7FF) {
			s += (0xC0 | (0xFF & (u >> 6)));
			s += (0x80 | (0x3F & u));
		}
		else if (u <= 0xffff) {
			s += (0xE0 | (0xFF & (u >> 12)));
			s += (0x80 | (0x3F & (u >> 6)));
			s += (0x80 | (0x3F & u));
		}
		else {
			s += (0xF0 | (0xFF & (u >> 18)));
			s += (0x80 | (0x3F & (u >> 12)));
			s += (0x80 | (0x3F & (u >> 6)));
			s += (0x80 | (0x3F & u));
		}
	}

	union Var {
		bool b; int i; long long l; double d;
		string* s; Array* a; Object* o;
	} data;
	Type t;
};
static_assert(sizeof(Json) == 16);