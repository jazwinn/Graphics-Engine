#pragma once
#include <unordered_map>
#include <functional>

template<typename... Args>
class Delegate {
public:
    using Func = std::function<void(Args...)>;
    using ID = std::uint64_t;

    // Subscribe and return an ID (to unsubscribe later)
    ID Add(Func f) {
        ID id = nextId++;
        callbacks[id] = std::move(f);
        return id;
    }

    // Remove by ID
    void Remove(ID id) {
        callbacks.erase(id);
    }

    // Trigger all callbacks
    void Invoke(Args... args) {
        for (auto& [id, cb] : callbacks) {
            cb(args...);
        }
    }

    // Clear all
    void Clear() {
        callbacks.clear();
    }

private:
    std::unordered_map<ID, Func> callbacks;
    ID nextId = 1;
};
