#pragma once
#ifndef __P2E_H__
#define __P2E_H__

#include <string>
#include <vector>
#include <unordered_map>

namespace p2e {

	// Standard C++ type renaming for this namespace
	using string = std::string;
	template<typename val_t> using vector = std::vector<val_t>;
	template<typename key_t, typename val_t> using umap = std::unordered_map<key_t, val_t>;

	typedef unsigned int id_t;
	typedef int size_t;

	class Object;
	class Module;
	class Core;

	class Object {
	private:
		id_t id_;
		string name_;
		Core* core_;

	public:
		Object(Core* core);
		Object(Core* core, const id_t& id);
		Object(Core* core, const string& name);
		Object(Core* core, const id_t& id, const string& name);
		Object(const Object& source);

		id_t id() const;
		string name() const;
		virtual string name(const string& newname);
	};

	typedef Object* (*ObjectFactory)();

	class Core {
	private:
		umap<id_t, Object*> objects_;

	public:
		Core();
		~Core();

		bool hasID(const id_t& id) const;
		bool generateID() const;
		size_t size() const;
	};
}

#endif