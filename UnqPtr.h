#pragma once
#include <stdexcept>
#include <cstddef>
#include <concepts>
#include <utility>

// UnqPtr<T> - Умный указатель для одиночных объектов

template <typename T>
class UnqPtr {
private:
    T* ptr;

    template <typename U>
    friend class UnqPtr;

public:
    explicit UnqPtr(T* p = nullptr) : ptr(p) {}

    ~UnqPtr() { delete ptr; }

    UnqPtr(const UnqPtr&) = delete;

    UnqPtr& operator=(const UnqPtr&) = delete;

    // MOVE-конструктор (передача владения)
    UnqPtr(UnqPtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    // MOVE-оператор присваивания (передача владения)
    UnqPtr& operator=(UnqPtr&& other) noexcept {
        if (this == &other) return *this;
        delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
        return *this;
    }

    // подтипизация (только move, copy удалён)

    template <typename U>
    requires std::convertible_to<U*, T*>
    UnqPtr(const UnqPtr<U>&) = delete;

    template <typename U>
    requires std::convertible_to<U*, T*>
    UnqPtr& operator=(const UnqPtr<U>&) = delete;

    // Конвертирующий move-конструктор
    template <typename U>
    requires std::convertible_to<U*, T*>
    UnqPtr(UnqPtr<U>&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    // Конвертирующий move-оператор присваивания
    template <typename U>
    requires std::convertible_to<U*, T*>
    UnqPtr& operator=(UnqPtr<U>&& other) noexcept {
        delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
        return *this;
    }


    T& operator*() { return *ptr; }
    const T& operator*() const { return *ptr; }
    T* operator->() { return ptr; }
    const T* operator->() const { return ptr; }
    T* get() const { return ptr; }
};


// UnqPtr<T[]>
template <typename T>
class UnqPtr<T[]> {
private:
    T* ptr;
    size_t size;

    template <typename U>
    friend class UnqPtr;

public:
    UnqPtr(T* p, size_t s) : ptr(p), size(s) {}
    UnqPtr() : ptr(nullptr), size(0) {}

    ~UnqPtr() { delete[] ptr; }

    UnqPtr(const UnqPtr&) = delete;
    UnqPtr& operator=(const UnqPtr&) = delete;

    UnqPtr(UnqPtr&& other) noexcept : ptr(other.ptr), size(other.size) {
        other.ptr = nullptr;
        other.size = 0;
    }

    UnqPtr& operator=(UnqPtr&& other) noexcept {
        if (this == &other) return *this;
        delete[] ptr;
        ptr = other.ptr;
        size = other.size;
        other.ptr = nullptr;
        other.size = 0;
        return *this;
    }

    // подтипизация для массивов (только move)

    template <typename U>
    requires std::convertible_to<U*, T*>
    UnqPtr(const UnqPtr<U[]>&) = delete;

    template <typename U>
    requires std::convertible_to<U*, T*>
    UnqPtr& operator=(const UnqPtr<U[]>&) = delete;

    template <typename U>
    requires std::convertible_to<U*, T*>
    UnqPtr(UnqPtr<U[]>&& other) noexcept : ptr(other.ptr), size(other.size) {
        other.ptr = nullptr;
        other.size = 0;
    }

    template <typename U>
    requires std::convertible_to<U*, T*>
    UnqPtr& operator=(UnqPtr<U[]>&& other) noexcept {
        delete[] ptr;
        ptr = other.ptr;
        size = other.size;
        other.ptr = nullptr;
        other.size = 0;
        return *this;
    }


    T& operator[](size_t index) {
        if (index >= size) throw std::out_of_range("UnqPtr<T[]>: index out of bounds");
        return ptr[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size) throw std::out_of_range("UnqPtr<T[]>: index out of bounds");
        return ptr[index];
    }

    size_t getSize() const { return size; }
    T* get() const { return ptr; }
};