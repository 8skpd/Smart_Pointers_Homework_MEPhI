#pragma once
#include <stdexcept>
#include <cstddef>
#include <iostream>

template <typename T>
class MemorySpan {
private:
    T* data;
    size_t size;

public:
    explicit MemorySpan(size_t s) : data(new T[s]()), size(s) {
    }

    MemorySpan(size_t s, const T& value) : data(new T[s]), size(s) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = value;
        }
    }

    ~MemorySpan() {
        delete[] data;
    }

    MemorySpan(const MemorySpan& other) : data(new T[other.size]), size(other.size) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }
    
    // Move-конструктор
    MemorySpan& operator=(const MemorySpan& other) {
        if (this == &other) return *this; // Проверка на самоприсваивание
        
        delete[] data;
        size = other.size;
        data = new T[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        return *this;
    }

        MemorySpan(MemorySpan&& other) noexcept 
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        std::cout << "  [MemorySpan: moved " << size << " elements]\n";
    }

    // Move-оператор присваивания
    MemorySpan& operator=(MemorySpan&& other) noexcept {
        if (this == &other) return *this;
        
        delete[] data;
        
        data = other.data;
        size = other.size;
        
        other.data = nullptr;
        other.size = 0;
        
        return *this;
    }

    // Доступ к элементу по индексу
    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    // Получение размера
    size_t getSize() const {
        return size;
    }

    // Получение сырого указателя
    T* getData() {
        return data;
    }

    const T* getData() const {
        return data;
    }
};