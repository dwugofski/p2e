#pragma once
#ifndef __P2E_CORE__
#define __P2E_CORE__

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
	typedef int rollval_t;
	typedef bool sign_t;

	class Object;
	class Core;
	class RolLVal;

	namespace ui {
		class AbstractCore;
		class AbstractObject;
	}

	class Object {
	protected:
		id_t id_;
		string name_;
		Core* core_;
		vector<ui::AbstractObject*> uiobjs_;

		void create_(Core& core);
		void create_(Core& core, const id_t& id);
		void create_(Core& core, const string& name);
		void create_(Core& core, const id_t& id, const string& name);
		void create_(Core& core, ui::AbstractObject* uiObj);
		void create_(Core& core, const id_t& id, ui::AbstractObject* uiObj);
		void create_(Core& core, const string& name, ui::AbstractObject* uiObj);
		void create_(Core& core, const id_t& id, const string& name, ui::AbstractObject* uiObj);
		void create_(const Object& source);

	public:
		id_t id() const;
		string name() const;
		virtual string name(const string& newname);

		bool hasBondedUI() const;
		bool hasBondedUI(const ui::AbstractObject& testUIObj) const;
		virtual void bindUI(ui::AbstractObject& newUIObj);
		virtual void unbindUI(ui::AbstractObject& oldUIObj);
	};

	class Core {
	private:
		umap<id_t, Object*> objects_;
		ui::AbstractCore* uiCore_;

	public:
		Core();
		Core(ui::AbstractCore* uiCore);
		~Core();

		bool hasID(const id_t& id) const;
		bool generateID() const;
		size_t size() const;

		bool hasBondedUI() const;
		bool hasBondedUI(const ui::AbstractCore& testUICore) const;
		virtual void bindUI(ui::AbstractCore& newUICore);
		virtual void unbindUI(ui::AbstractCore& oldUICore);
	};

	class Roll {
	public:
		enum class Die : rollval_t {
			TWO,
			THREE,
			FOUR,
			SIX,
			EIGHT,
			TEN,
			TWELVE,
			TWENTY,
			HUNDRED,
		};

		static Die rv2d(const rollval_t& source);
		static rollval_t d2rv(const Die& source);

		static rollval_t roll(const string& specification);

	private:
		vector<Die> dice_;
		vector<size_t> counts_;
		vector<sign_t> signs_;
		rollval_t mod_;

	public:
		Roll();
		Roll(const string& specification);
		Roll(const Die& die);
		Roll(const size_t& count, const Die& die);
		Roll(const Die& die, const rollval_t& mod);
		Roll(const size_t& count, const Die& die, const rollval_t& mod);
		Roll(const rollval_t& mod);

		rollval_t roll() const;
		string spec() const;

		void add(const Roll& roll);
		void add(const string& specification);
		void add(const Die& die);
		void add(const size_t& count, const Die& die);
		void add(const Die& die, const rollval_t& mod);
		void add(const size_t& count, const Die& die, const rollval_t& mod);
		void add(const rollval_t& mod);

		void subtract(const Roll& roll);
		void subtract(const string& specification);
		void subtract(const Die& die);
		void subtract(const size_t& count, const Die& die);
		void subtract(const Die& die, const rollval_t& mod);
		void subtract(const size_t& count, const Die& die, const rollval_t& mod);
		void subtract(const rollval_t& mod);

		void set(const Roll& roll);
		void set(const string& specification);
		void set(const Die& die);
		void set(const size_t& count, const Die& die);
		void set(const Die& die, const rollval_t& mod);
		void set(const size_t& count, const Die& die, const rollval_t& mod);
		void set(const rollval_t& mod);

		void clear();

		operator string() const;

		bool operator==(const Roll& rhs) const;
		bool operator==(const string& rhs) const;

		Roll& operator=(const Roll& rhs);
		Roll& operator=(const string& rhs);

		Roll& operator+(const Roll& rhs) const;
		Roll& operator+(const rollval_t& rhs) const;
		Roll& operator+(const Die& rhs) const;
		Roll& operator+(const string& rhs) const;

		Roll& operator+=(const Roll& rhs);
		Roll& operator+=(const rollval_t& rhs);
		Roll& operator+=(const Die& rhs);
		Roll& operator+=(const string& rhs);
	};

}


#endif