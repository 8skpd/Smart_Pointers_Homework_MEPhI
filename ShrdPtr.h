#pragma once
#include <stdexcept>
#include <cstddef>
#include <concepts>

// ShrdPtr<T> - Для одиночных объектов
template <typename T>
class ShrdPtr {
private:
    T* ptr;
    size_t* refCount;

    // Все инстанциации ShrdPtr — друзья друг друга
    template <typename U>
    friend class ShrdPtr;

    void release() {
        if (refCount) {
            --(*refCount);
            if (*refCount == 0) {
                delete ptr;
                delete refCount;
            }
        }
    }

public:
    explicit ShrdPtr(T* p = nullptr) : ptr(p), refCount(p ? new size_t(1) : nullptr) {}

    ~ShrdPtr() { release(); }

    ShrdPtr(const ShrdPtr& other) : ptr(other.ptr), refCount(other.refCount) {
        if (refCount) ++(*refCount);
    }

    ShrdPtr& operator=(const ShrdPtr& other) {
        if (this == &other) return *this;
        release();
        ptr = other.ptr;
        refCount = other.refCount;
        if (refCount) ++(*refCount);
        return *this;
    }

    ShrdPtr(ShrdPtr&& other) noexcept : ptr(other.ptr), refCount(other.refCount) {
        other.ptr = nullptr;
        other.refCount = nullptr;
    }

    ShrdPtr& operator=(ShrdPtr&& other) noexcept {
        if (this == &other) return *this;
        release();
        ptr = other.ptr;
        refCount = other.refCount;
        other.ptr = nullptr;
        other.refCount = nullptr;
        return *this;
    }

    // Подтипизация
    template <typename U>
    requires std::convertible_to<U*, T*>
    ShrdPtr(const ShrdPtr<U>& other) : ptr(other.ptr), refCount(other.refCount) {
        if (refCount) ++(*refCount);
    }

    template <typename U>
    requires std::convertible_to<U*, T*>
    ShrdPtr& operator=(const ShrdPtr<U>& other) {
        release();
        ptr = other.ptr;
        refCount = other.refCount;
        if (refCount) ++(*refCount);
        return *this;
    }

    template <typename U>
    requires std::convertible_to<U*, T*>
    ShrdPtr(ShrdPtr<U>&& other) noexcept : ptr(other.ptr), refCount(other.refCount) {
        other.ptr = nullptr;
        other.refCount = nullptr;
    }

    template <typename U>
    requires std::convertible_to<U*, T*>
    ShrdPtr& operator=(ShrdPtr<U>&& other) noexcept {
        release();
        ptr = other.ptr;
        refCount = other.refCount;
        other.ptr = nullptr;
        other.refCount = nullptr;
        return *this;
    }


    T& operator*() { return *ptr; }
    const T& operator*() const { return *ptr; }
    T* operator->() { return ptr; }
    const T* operator->() const { return ptr; }
    
    size_t useCount() const { return refCount ? *refCount : 0; }
    T* get() const { return ptr; }  // для некоторых тестов
};


template <typename T>
class ShrdPtr<T[]> {
private:
    T* ptr;
    size_t* refCount;
    size_t size;

    template <typename U>
    friend class ShrdPtr;

    void release() {
        if (refCount) {
            --(*refCount);
            if (*refCount == 0) {
                delete[] ptr;
                delete refCount;
            }
        }
    }

public:
    ShrdPtr(T* p, size_t s) : ptr(p), refCount(p ? new size_t(1) : nullptr), size(s) {}
    ShrdPtr() : ptr(nullptr), refCount(nullptr), size(0) {}

    ~ShrdPtr() { release(); }

    ShrdPtr(const ShrdPtr& other) : ptr(other.ptr), refCount(other.refCount), size(other.size) {
        if (refCount) ++(*refCount);
    }

    ShrdPtr& operator=(const ShrdPtr& other) {
        if (this == &other) return *this;
        release();
        ptr = other.ptr;
        refCount = other.refCount;
        size = other.size;
        if (refCount) ++(*refCount);
        return *this;
    }

    ShrdPtr(ShrdPtr&& other) noexcept 
        : ptr(other.ptr), refCount(other.refCount), size(other.size) {
        other.ptr = nullptr;
        other.refCount = nullptr;
        other.size = 0;
    }

    ShrdPtr& operator=(ShrdPtr&& other) noexcept {
        if (this == &other) return *this;
        release();
        ptr = other.ptr;
        refCount = other.refCount;
        size = other.size;
        other.ptr = nullptr;
        other.refCount = nullptr;
        other.size = 0;
        return *this;
    }

    // подтипизация для массивов
    template <typename U>
    requires std::convertible_to<U*, T*>
    ShrdPtr(const ShrdPtr<U[]>& other) 
        : ptr(other.ptr), refCount(other.refCount), size(other.size) {
        if (refCount) ++(*refCount);
    }

    template <typename U>
    requires std::convertible_to<U*, T*>
    ShrdPtr& operator=(const ShrdPtr<U[]>& other) {
        release();
        ptr = other.ptr;
        refCount = other.refCount;
        size = other.size;
        if (refCount) ++(*refCount);
        return *this;
    }

    template <typename U>
    requires std::convertible_to<U*, T*>
    ShrdPtr(ShrdPtr<U[]>&& other) noexcept 
        : ptr(other.ptr), refCount(other.refCount), size(other.size) {
        other.ptr = nullptr;
        other.refCount = nullptr;
        other.size = 0;
    }

    template <typename U>
    requires std::convertible_to<U*, T*>
    ShrdPtr& operator=(ShrdPtr<U[]>&& other) noexcept {
        release();
        ptr = other.ptr;
        refCount = other.refCount;
        size = other.size;
        other.ptr = nullptr;
        other.refCount = nullptr;
        other.size = 0;
        return *this;
    }

    T& operator[](size_t index) {
        if (index >= size) throw std::out_of_range("ShrdPtr<T[]>: index out of bounds");
        return ptr[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size) throw std::out_of_range("ShrdPtr<T[]>: index out of bounds");
        return ptr[index];
    }

    size_t useCount() const { return refCount ? *refCount : 0; }
    size_t getSize() const { return size; }
    T* get() const { return ptr; }
};