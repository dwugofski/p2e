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

	const sign_t POS = false;
	const sign_t NEG = true;

	class Object;
	class Core;
	class Updateable;
	class Roll;
	class Value;
	class ValueMod;

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

		vector<ui::AbstractObject*>::const_iterator findBondedUI_(const ui::AbstractObject& testUiObj) const;
		vector<ui::AbstractObject*>::iterator findBondedUI_(const ui::AbstractObject& testUiObj);

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

	protected:
		vector<ui::AbstractCore*>::const_iterator findBondedUI_(const ui::AbstractCore& testUiObj) const;
		vector<ui::AbstractCore*>::iterator findBondedUI_(const ui::AbstractCore& testUiObj);

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

	typedef void (*UpdateCallback)(Updateable*);

	struct UpdateCallbackSpec {
		UpdateCallback callback;
		Updateable* source;
	};

	class Updateable {
	protected:
		vector<Updateable*> dependents_;
		vector<Updateable*> parents_;
		vector<UpdateCallbackSpec> callbacks_;

		Updateable(); // To prevent public access to the constructor

	public:
		~Updateable();

		bool hasDependent(const Updateable& testDep, bool recursive = false) const;
		void addDependent(Updateable& newDep);
		void removeDependent(Updateable& oldDep);

		bool hasParent(const Updateable& testParent, bool recursive = false) const;
		void addParent(Updateable& newParent);
		void removeParent(Updateable& oldParent);

		bool hasCallback(const UpdateCallbackSpec& spec) const;
		bool hasCallback(const UpdateCallback callback) const;
		bool hasCallback(const Updateable& source, const UpdateCallback callback) const;
		void addCallback(UpdateCallbackSpec& spec);
		void addCallback(UpdateCallback callback);
		void addCallback(Updateable& source, UpdateCallback callback);
		void removeCallback(UpdateCallbackSpec& spec);
		void removeCallback(UpdateCallback callback);
		void removeCallback(Updateable& source, UpdateCallback callback);

		void doUpdate() const;
	};

	class Roll : public Updateable {
	public:
		enum class Die {
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

	protected:
		vector<Die> dice_;
		vector<size_t> counts_;
		vector<sign_t> signs_;
		rollval_t mod_;

	public:
		Roll();
		Roll(const string& specification);
		Roll(const Die& die);
		Roll(const size_t& count, const Die& die);
		Roll(const sign_t& sign, const size_t& count, const Die& die);
		Roll(const Die& die, const rollval_t& mod);
		Roll(const size_t& count, const Die& die, const rollval_t& mod);
		Roll(const sign_t& sign, const size_t& count, const Die& die, const rollval_t& mod);
		Roll(const rollval_t& mod);
		Roll(const Roll& source);

		rollval_t roll() const;
		rollval_t average(bool roundup = false) const;
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

		operator string() const; // Returns specification
		operator rollval_t() const; // Returns roll()

		bool operator==(const Roll& rhs) const;
		bool operator==(const string& rhs) const;

		Roll& operator=(const Roll& rhs);
		Roll& operator=(const rollval_t& rhs);
		Roll& operator=(const Die& rhs);
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

	class ValueMod : public Updateable {
	public:
		enum class Type {
			ADD,
			BASE,
			OVERRIDE
		};

	private:
		Type type_;
		string name_;
		string description_;
		rollval_t value_;
		bool set_;
		Value& parent_;

	public:
		ValueMod(Value& parent);
		ValueMod(Value& parent, const ValueMod& source);
		~ValueMod();

		Type type() const;
		void type(const Type& newtype);

		rollval_t value() const;
		void value(const rollval_t& newval);

		string name() const;
		void name(const string& newname);

		string description() const;
		void description(const string& newdesc);

		bool enabled() const;
		void setEnabled(const bool& enval);
		void toggle();
		void enable();
		void disable();
	};

	class Value : public Updateable {
	protected:
		rollval_t base_;
		umap<string, ValueMod::Type> mod_locator_;
		umap<string, ValueMod> additive_mods_;
		umap<string, ValueMod> base_mods_;
		umap<string, ValueMod> overriding_mods_;

	public:
		Value();
		Value(const Value& source);
		~Value();

		rollval_t value() const;
		rollval_t base() const;
		void base(const rollval_t& newval) const;

		bool hasMod() const;
		bool hasMod(const string& testMod) const;
		void addMod(const ValueMod& source);
		void removeMod(const ValueMod& source);
		void renameMod(const string& oldname, const string& newname);
		void enableMod(const string& modname);
		void disableMod(const string& modname);
		void retypeMod(const string& modname, const ValueMod::Type& newtype);
	};
}


#endif