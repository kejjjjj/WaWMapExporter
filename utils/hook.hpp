#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <functional>
#include <string>

#include "mh/MinHook.h"

//#include <iostream>

//template <typename T>
//using is_any_pointer = std::disjunction<
//	std::is_unsigned<std::remove_pointer_t<T>>,
//	std::is_pointer<T>
//>;

#ifndef MINHOOK_LIBRARY
#error "Minhook library is not in use"
#endif

struct hookbase
{
	hookbase(const std::string& identifier, void* t, void* d) : target(t), detour(d), backup(t), s_id(identifier) {}
	virtual ~hookbase() = default;

	virtual bool create() = 0;
	virtual bool enable() = 0;
	virtual bool disable() = 0;
	virtual bool release() = 0;

	size_t get() const noexcept { return reinterpret_cast<size_t>(target); }
	const std::string& id() const noexcept { return s_id; }
	void* get_jmp() const noexcept { return backup; }
	void* detour = {};

protected:

	void* target = {};
	void* backup = {};
	std::string s_id;
};

template<typename Type, typename ... Args>
struct hook : public hookbase
{
	hook(const std::string& identifier, void* t, void* d, bool enabled = true) : hookbase(identifier, t, d) {
		create();

		if (enabled)
			enable();
	}
	~hook() = default;

	[[maybe_unused]] bool create() override { return MH_CreateHook(target, detour, &backup) == MH_OK; }
	[[maybe_unused]] bool enable() override { return MH_EnableHook(target) == MH_OK; }
	[[maybe_unused]] bool disable() override { return MH_DisableHook(target) == MH_OK; }
	[[maybe_unused]] bool release() override { return MH_RemoveHook(target) == MH_OK; }

	inline bool hook_owner() const noexcept(true) { return functions.empty(); }
	inline void exit() noexcept(true) { exit_hook = true; }
	inline void set_owner(hook<Type, Args...>* o) noexcept { owner = o; }
	inline auto get_owner() const { return owner; }
	inline auto& get_functions() { return functions; }
	decltype(auto) call(const Args... args) const
	{
		//std::cout << "calling: " << id() << " -> " << std::hex << backup << '\n';
		//call_queue(args...);
		return (reinterpret_cast<Type(*)(Args...)>(backup))(args...);
	}
	template<void(Args...)>
	void call(const Args... args) const
	{
		//call_queue(args...);
		//std::cout << "calling: " << id() << " -> " << std::hex << backup << '\n';
		(reinterpret_cast<void(*)(Args...)>(backup))(args...);
	}

	decltype(auto) call_queueless(const Args... args) const { return (reinterpret_cast<Type(*)(Args...)>(backup))(args...); }
	template<void(Args...)>
	void call_queueless(const Args... args) const { (reinterpret_cast<Type(*)(Args...)>(backup))(args...); }

	decltype(auto) call_queue(const Args... args) const
	{
		std::for_each(functions.begin(), functions.end(), [&args...](const std::function<Type(Args...)>& func)
			{ func(args...); });
	}
	template<void(Args...)>
	void call_queue(const Args... args) const
	{
		std::for_each(functions.begin(), functions.end(), [&args...](const std::function<Type(Args...)>& func)
			{ func(args...); });

	}

	void add_callable(std::function<Type(Args...)>&& _detour_) {
		functions.emplace_back(std::forward<std::function<Type(Args...)>&&>(_detour_));
	}

protected:
	hook<Type, Args...>* owner = {};
	std::vector<std::function<Type(Args...)>> functions;

	bool exit_hook = false;

};
using phookbase = std::unique_ptr<hookbase>;
struct hooktable
{
	static bool initialize() { return MH_Initialize() == MH_OK; }
	static bool release() { return MH_Uninitialize() == MH_OK; }

	//overwrites any previous hooks if they exist
	template<typename Type, typename ... Args>
	[[maybe_unused]] static hook<Type, Args...>* overwriter(const std::string& id, auto target, void* detour) {
		return dynamic_cast<hook<Type, Args...>*>(
			table.emplace_back(std::make_unique<hook<Type, Args...>>(id, reinterpret_cast<void*>(target), detour)).get());
	}

	//will not destroy any existing hook (any instance of this hook library)
	//will push this detour to the call queue at the end of the original detour
	template<typename Type, typename ... Args>
	[[maybe_unused]] static hook<Type, Args...>* preserver(const std::string& id, auto target, void* detour) {
		return preserver<Type, Args...>(id, target, detour, reinterpret_cast<hook<Type, Args...>*>(nullptr));
	}

	template<typename Type, typename ... Args>
	[[maybe_unused]] static hook<Type, Args...>* preserver(const std::string& id, auto target, void* detour,
		hook<Type, Args...>* owner) {

		std::vector<phookbase>::iterator result;
		if ((result = std::find_if(table.begin(), table.end(), [&target](const phookbase& _hook_) {
			return _hook_->get() == size_t(target);
			})) == table.end()) {
			return overwriter<Type, Args...>(id, target, detour);
		}

		hook<Type, Args...>* current = dynamic_cast<hook<Type, Args...>*>((result->get()));

		if (!owner || !current) {
			owner = current;
			if (!owner)
				return nullptr;
		}

		//auto last_target = tgt->get_functions().back().target<Type(*)(Args...)>();
		owner->add_callable(std::function<Type(Args...)>(reinterpret_cast<Type(*)(Args...)>(detour)));

		//create a new hook inside of the detour
		hook<Type, Args...>* item = preserver<Type, Args...>(
			id,
			current->detour,
			reinterpret_cast<Type(*)(Args...)>(detour),
			owner);

		item->set_owner(owner);
		return item;
	}

	template<typename Type, typename ... Args>
	static hook<Type, Args...>* find(const auto memory) {

		decltype(auto) _hook = std::find_if(table.begin(), table.end(), [&memory](const phookbase& _hook_)
			{ return _hook_->get() == size_t(memory); });

		if (_hook == table.end())
			return nullptr;

		return dynamic_cast<hook<Type, Args...>*>(_hook->get());
	}
	template<typename Type, typename ... Args>
	static hook<Type, Args...>* find(const std::string& identifier) {

		decltype(auto) _hook = std::find_if(table.begin(), table.end(), [&identifier](const phookbase& _hook_)
			{ return _hook_->id() == identifier; });

		if (_hook == table.end())
			return nullptr;

		return dynamic_cast<hook<Type, Args...>*>(_hook->get());
	}

protected:
	static std::vector<phookbase> table;

};
inline std::vector<phookbase> hooktable::table = {};
