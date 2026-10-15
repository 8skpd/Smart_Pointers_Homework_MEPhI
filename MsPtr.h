#pragma once
#include <stdexcept>
#include <cstddef>
#include "MemorySpan.h"

template <typename T>
class MsPtr {
private:
    MemorySpan<T>* owner;  // Указатель на владельца памяти
    size_t index;          // Текущая позиция в MemorySpan

    // Вспомогательный метод для проверки границ
    void checkBounds(size_t idx) const {
        if (idx >= owner->getSize()) {
            throw std::out_of_range("MsPtr: index out of bounds");
        }
    }

public:
    MsPtr(MemorySpan<T>* span, size_t idx = 0) : owner(span), index(idx) {
        checkBounds(idx);
    }

    // Конструктор по умолчанию (nullptr-подобный)
    MsPtr() : owner(nullptr), index(0) {}

    // Деструктор (не освобождаем память, т.к. не владеем ею)
    ~MsPtr() = default;

    // Конструктор копирования
    MsPtr(const MsPtr& other) = default;

    // Оператор присваивания
    MsPtr& operator=(const MsPtr& other) = default;

    // Move-конструктор
    MsPtr(MsPtr&& other) = default;

    // Move-оператор присваивания
    MsPtr& operator=(MsPtr&& other) = default;

    // Префиксный инкремент: ++ptr
    MsPtr& operator++() {
        if (index + 1 >= owner->getSize()) {
            throw std::out_of_range("MsPtr: increment out of bounds");
        }
        ++index;
        return *this;
    }

    // Постфиксный инкремент: ptr++
    MsPtr operator++(int) {
        MsPtr temp = *this;
        ++(*this);
        return temp;
    }

    // Префиксный декремент: --ptr
    MsPtr& operator--() {
        if (index == 0) {
            throw std::out_of_range("MsPtr: decrement out of bounds");
        }
        --index;
        return *this;
    }

    // Постфиксный декремент: ptr--
    MsPtr operator--(int) {
        MsPtr temp = *this;
        --(*this);
        return temp;
    }

    // Сложение: ptr + n
    MsPtr operator+(size_t offset) const {
        size_t newIndex = index + offset;
        if (newIndex >= owner->getSize()) {
            throw std::out_of_range("MsPtr: addition out of bounds");
        }
        return MsPtr(owner, newIndex);
    }

    // Вычитание: ptr - n
    MsPtr operator-(size_t offset) const {
        if (offset > index) {
            throw std::out_of_range("MsPtr: subtraction out of bounds");
        }
        return MsPtr(owner, index - offset);
    }

    // Разность двух указателей: ptr1 - ptr2
    ptrdiff_t operator-(const MsPtr& other) const {
        if (owner != other.owner) {
            throw std::logic_error("MsPtr: cannot subtract pointers from different MemorySpans");
        }
        return static_cast<ptrdiff_t>(index) - static_cast<ptrdiff_t>(other.index);
    }

    // Составное присваивание: ptr += n
    MsPtr& operator+=(size_t offset) {
        size_t newIndex = index + offset;
        if (newIndex >= owner->getSize()) {
            throw std::out_of_range("MsPtr: += out of bounds");
        }
        index = newIndex;
        return *this;
    }

    // Составное присваивание: ptr -= n
    MsPtr& operator-=(size_t offset) {
        if (offset > index) {
            throw std::out_of_range("MsPtr: -= out of bounds");
        }
        index -= offset;
        return *this;
    }


    // Разыменование: *ptr
    T& operator*() {
        checkBounds(index);
        return owner->getData()[index];
    }

    const T& operator*() const {
        checkBounds(index);
        return owner->getData()[index];
    }

    // Доступ к членам: ptr->member
    T* operator->() {
        checkBounds(index);
        return &owner->getData()[index];
    }

    const T* operator->() const {
        checkBounds(index);
        return &owner->getData()[index];
    }

    // Индексация: ptr[n]
    T& operator[](size_t offset) {
        size_t newIndex = index + offset;
        checkBounds(newIndex);
        return owner->getData()[newIndex];
    }

    const T& operator[](size_t offset) const {
        size_t newIndex = index + offset;
        checkBounds(newIndex);
        return owner->getData()[newIndex];
    }

    bool operator==(const MsPtr& other) const {
        return owner == other.owner && index == other.index;
    }

    bool operator!=(const MsPtr& other) const {
        return !(*this == other);
    }

    bool operator<(const MsPtr& other) const {
        if (owner != other.owner) {
            throw std::logic_error("MsPtr: cannot compare pointers from different MemorySpans");
        }
        return index < other.index;
    }


    // Получение текущего индекса
    size_t getIndex() const {
        return index;
    }

    // Получение владельца
    MemorySpan<T>* getOwner() const {
        return owner;
    }
};