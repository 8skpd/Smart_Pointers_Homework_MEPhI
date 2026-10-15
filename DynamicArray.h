#pragma once
#include <stdexcept>
#include <cstddef>
#include "ShrdPtr.h"

template <typename T>
class DynamicArray {
private:
    ShrdPtr<T>* data;  // Массив умных указателей
    size_t size;
    size_t capacity;

    void resize(size_t newCapacity) {
        ShrdPtr<T>* newData = new ShrdPtr<T>[newCapacity];
        
        // Копируем указатели (увеличивается счетчик ссылок)
        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    // Конструктор по умолчанию
    DynamicArray() : data(new ShrdPtr<T>[4]), size(0), capacity(4) {}

    // Конструктор с начальным размером
    explicit DynamicArray(size_t initialCapacity) 
        : data(new ShrdPtr<T>[initialCapacity]), size(0), capacity(initialCapacity) {}

    // Деструктор
    ~DynamicArray() {
        delete[] data;
    }

    // Конструктор копирования
    DynamicArray(const DynamicArray& other) 
        : data(new ShrdPtr<T>[other.capacity]), size(other.size), capacity(other.capacity) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];  // Копируем ShrdPtr (счетчик ссылок++)
        }
    }

    // Оператор присваивания
    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this;  // Проверка на самоприсваивание
        
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        data = new ShrdPtr<T>[capacity];
        
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        
        return *this;
    }

    // Получение элемента по индексу
    ShrdPtr<T> Get(size_t index) const {
        if (index >= size) {
            throw std::out_of_range("DynamicArray: index out of bounds");
        }
        return data[index];
    }

    // Установка элемента по индексу
    void Set(size_t index, const T& value) {
        if (index >= size) {
            throw std::out_of_range("DynamicArray: index out of bounds");
        }
        data[index] = ShrdPtr<T>(new T(value));
    }

    // Добавление элемента в конец
    void Append(const T& value) {
        if (size == capacity) {
            resize(capacity * 2);
        }
        data[size] = ShrdPtr<T>(new T(value));
        ++size;
    }

    // Вставка элемента по индексу
    void Insert(size_t index, const T& value) {
        if (index > size) {
            throw std::out_of_range("DynamicArray: insert index out of bounds");
        }
        
        if (size == capacity) {
            resize(capacity * 2);
        }
        
        // Сдвигаем элементы вправо
        for (size_t i = size; i > index; --i) {
            data[i] = data[i - 1];
        }
        
        data[index] = ShrdPtr<T>(new T(value));
        ++size;
    }

    // Удаление элемента по индексу
    void Remove(size_t index) {
        if (index >= size) {
            throw std::out_of_range("DynamicArray: remove index out of bounds");
        }
        
        // Сдвигаем элементы влево
        for (size_t i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        
        --size;
        
        // Уменьшение capacity, если массив слишком пустой
        // if (size > 0 && size == capacity / 4) {
        //     resize(capacity / 2);
        // }
    }

    // Получение размера
    size_t GetSize() const {
        return size;
    }

    // Проверка на пустоту
    bool IsEmpty() const {
        return size == 0;
    }

    // Очистка массива
    void Clear() {
        size = 0;
    }
};