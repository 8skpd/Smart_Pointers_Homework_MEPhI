#include <iostream>
#include <cassert>
#include "UnqPtr.h"
#include "ShrdPtr.h"
#include "TestObj.h"
#include "MemorySpan.h"
#include "MsPtr.h"


// Тесты для UnqPtr<T[]> (массивы)

void testUnqPtrArrayCreation() {
    std::cout << "\n [Test] UnqPtr<T[]>: создание и доступ \n";
    
    TestObj* raw = new TestObj[3]{TestObj(1), TestObj(2), TestObj(3)};
    UnqPtr<TestObj[]> arr(raw, 3);
    
    // Проверка доступа по индексу
    assert(arr[0].getId() == 1);
    assert(arr[1].getId() == 2);
    assert(arr[2].getId() == 3);
    assert(arr.getSize() == 3);
    
    // Изменение элемента
    arr[1] = TestObj(99);
    assert(arr[1].getId() == 99);
    
    std::cout << "PASSED: создание, доступ, изменение\n";
}

void testUnqPtrArrayBoundsCheck() {
    std::cout << "\n [Test] UnqPtr<T[]>: проверка границ \n";
    
    TestObj* raw = new TestObj[2]{TestObj(10), TestObj(20)};
    UnqPtr<TestObj[]> arr(raw, 2);
    
    // Выход за верхнюю границу
    bool caught = false;
    try {
        arr[5].getId();
    } catch (const std::out_of_range& e) {
        caught = true;
        std::cout << "Caught: " << e.what() << "\n";
    }
    assert(caught);
    
    std::cout << "PASSED: исключение при выходе за границы\n";
}



// Тесты для ShrdPtr<T[]> (массивы)

void testShrdPtrArrayCreation() {
    std::cout << "\n [Test] ShrdPtr<T[]>: создание и доступ \n";
    
    TestObj* raw = new TestObj[3]{TestObj(5), TestObj(6), TestObj(7)};
    ShrdPtr<TestObj[]> arr(raw, 3);
    
    assert(arr[0].getId() == 5);
    assert(arr[1].getId() == 6);
    assert(arr[2].getId() == 7);
    assert(arr.useCount() == 1);
    
    std::cout << "PASSED: создание и доступ\n";
}

void testShrdPtrArraySharedOwnership() {
    std::cout << "\n [Test] ShrdPtr<T[]>: разделяемое владение \n";
    
    TestObj* raw = new TestObj[2]{TestObj(11), TestObj(22)};
    ShrdPtr<TestObj[]> arr1(raw, 2);
    
    // Копирование — счетчик должен увеличиться
    ShrdPtr<TestObj[]> arr2 = arr1;
    assert(arr1.useCount() == 2);
    assert(arr2.useCount() == 2);
    
    // Изменение через arr1 должно быть видно в arr2 (общий объект!)
    arr1[0] = TestObj(999);
    assert(arr2[0].getId() == 999);
    
    std::cout << "PASSED: массивы ссылаются на одни данные\n";
}

void testShrdPtrArrayBoundsCheck() {
    std::cout << "\n [Test] ShrdPtr<T[]>: проверка границ \n";
    
    TestObj* raw = new TestObj[2]{TestObj(10), TestObj(20)};
    ShrdPtr<TestObj[]> arr(raw, 2);
    
    bool caught = false;
    try {
        arr[100].getId();
    } catch (const std::out_of_range& e) {
        caught = true;
        std::cout << "Caught: " << e.what() << "\n";
    }
    assert(caught);
    
    std::cout << "PASSED: исключение при выходе за границы\n";
}

void testShrdPtrArrayAssignment() {
    std::cout << "\n [Test] ShrdPtr<T[]>: присваивание \n";
    
    TestObj* raw1 = new TestObj[2]{TestObj(1), TestObj(2)};
    TestObj* raw2 = new TestObj[2]{TestObj(10), TestObj(20)};
    
    ShrdPtr<TestObj[]> arr1(raw1, 2);
    ShrdPtr<TestObj[]> arr2(raw2, 2);
    
    assert(arr1.useCount() == 1);
    assert(arr2.useCount() == 1);
    
    // Присваивание: arr1 должен отпустить raw1 и присоединиться к raw2
    arr1 = arr2;
    
    assert(arr1.useCount() == 2);  // arr1 и arr2 теперь делят raw2
    assert(arr2.useCount() == 2);
    assert(arr1[0].getId() == 10);
    
    // Тест самоприсваивания
    arr1 = arr1;
    assert(arr1.useCount() == 2);
    
    std::cout << "PASSED: присваивание и счетчики ссылок\n";
}

// Тесты на корректность delete[] (деструкторы для всех элементов)

void testDestructorCallForAllElements() {
    std::cout << "\n [Test] Корректность delete[] (деструкторы для всех элементов) \n";
    std::cout << "Создаем массив из 3 объектов...\n";
    
    {
        TestObj* raw = new TestObj[3]{TestObj(1001), TestObj(1002), TestObj(1003)};
        UnqPtr<TestObj[]> arr(raw, 3);
        std::cout << "Выходим из области видимости (должны вызваться 3 деструктора)...\n";
    }
    // Здесь должны быть вызваны деструкторы для ВСЕХ 3 объектов
    
    std::cout << "PASSED: если выше вы видите 3 строки [Destroyed], delete[] работает корректно\n";
}


void testMemorySpanCreation() {
    std::cout << "\n [Test] MemorySpan: создание \n";
    
    MemorySpan<int> span1(5);
    assert(span1.getSize() == 5);
    
    MemorySpan<int> span2(3, 42);
    assert(span2[0] == 42);
    assert(span2[1] == 42);
    assert(span2[2] == 42);
    
    std::cout << "PASSED: создание MemorySpan\n";
}

void testMemorySpanCopy() {
    std::cout << "\n [Test] MemorySpan: копирование \n";
    
    MemorySpan<int> span1(3, 10);
    MemorySpan<int> span2 = span1;  // Deep copy
    
    span1[0] = 999;
    assert(span2[0] == 10);  // Независимая копия
    
    std::cout << "PASSED: глубокое копирование\n";
}

void testMsPtrCreation() {
    std::cout << "\n [Test] MsPtr: создание \n";
    
    MemorySpan<int> span(5);
    for (size_t i = 0; i < 5; ++i) span[i] = i * 10;
    
    MsPtr<int> ptr(&span, 0);
    assert(*ptr == 0);
    
    MsPtr<int> ptr2(&span, 2);
    assert(*ptr2 == 20);
    
    std::cout << "PASSED: создание MsPtr\n";
}

void testMsPtrArithmetic() {
    std::cout << "\n [Test] MsPtr: арифметика \n";
    
    MemorySpan<int> span(5);
    for (size_t i = 0; i < 5; ++i) span[i] = i * 10;
    
    MsPtr<int> ptr(&span, 0);
    
    // Инкремент
    ++ptr;
    assert(*ptr == 10);
    
    ptr++;
    assert(*ptr == 20);
    
    // Декремент
    --ptr;
    assert(*ptr == 10);
    
    ptr--;
    assert(*ptr == 0);
    
    // Сложение
    MsPtr<int> ptr2 = ptr + 3;
    assert(*ptr2 == 30);
    
    // Вычитание
    MsPtr<int> ptr3 = ptr2 - 2;
    assert(*ptr3 == 10);
    
    // Разность указателей
    ptrdiff_t diff = ptr2 - ptr;
    assert(diff == 3);
    
    // Составное присваивание
    ptr += 4;
    assert(*ptr == 40);
    
    ptr -= 2;
    assert(*ptr == 20);
    
    std::cout << "PASSED: арифметика указателей\n";
}

void testMsPtrBoundsCheck() {
    std::cout << "\n [Test] MsPtr: проверка границ \n";
    
    MemorySpan<int> span(3);
    MsPtr<int> ptr(&span, 0);
    
    // Выход за верхнюю границу при инкременте
    bool caught = false;
    try {
        ptr += 10;
    } catch (const std::out_of_range& e) {
        caught = true;
        std::cout << "Caught: " << e.what() << "\n";
    }
    assert(caught);
    
    // Выход за нижнюю границу при декременте
    caught = false;
    try {
        ptr--;
    } catch (const std::out_of_range& e) {
        caught = true;
        std::cout << "Caught: " << e.what() << "\n";
    }
    assert(caught);
    
    // Выход за границы при сложении
    caught = false;
    try {
        MsPtr<int> bad = ptr + 100;
    } catch (const std::out_of_range& e) {
        caught = true;
        std::cout << "Caught: " << e.what() << "\n";
    }
    assert(caught);
    
    std::cout << "PASSED: проверка границ работает\n";
}

void testMsPtrIndexing() {
    std::cout << "\n [Test] MsPtr: индексация \n";
    
    MemorySpan<int> span(5);
    for (size_t i = 0; i < 5; ++i) span[i] = i * 10;
    
    MsPtr<int> ptr(&span, 1);
    
    assert(ptr[0] == 10);
    assert(ptr[1] == 20);
    assert(ptr[2] == 30);
    
    // Изменение через индексацию
    ptr[0] = 999;
    assert(span[1] == 999);
    
    std::cout << "PASSED: индексация работает\n";
}

void testMsPtrComparison() {
    std::cout << "\n [Test] MsPtr: сравнение \n";
    
    MemorySpan<int> span(5);
    MsPtr<int> ptr1(&span, 1);
    MsPtr<int> ptr2(&span, 3);
    MsPtr<int> ptr3(&span, 1);
    
    assert(ptr1 == ptr3);
    assert(ptr1 != ptr2);
    assert(ptr1 < ptr2);
    
    std::cout << "PASSED: сравнение работает\n";
}

void testMsPtrWithObjects() {
    std::cout << "\n [Test] MsPtr: работа с объектами \n";
    
    struct Point {
        int x, y;
        void print() const { std::cout << "(" << x << ", " << y << ")\n"; }
    };
    
    MemorySpan<Point> span(3);
    span[0] = {1, 2};
    span[1] = {3, 4};
    span[2] = {5, 6};
    
    MsPtr<Point> ptr(&span, 0);
    
    // Доступ через ->
    ptr->print();
    assert(ptr->x == 1);
    assert(ptr->y == 2);
    
    // Изменение через ->
    ptr->x = 100;
    assert(span[0].x == 100);
    
    std::cout << "PASSED: работа с объектами\n";
}

// Тесты подтипизации для UnqPtr<T> (одиночные объекты)

void testUnqPtrSubtypingMove() {
    std::cout << "\n [Test] UnqPtr<T>: move при подтипизации \n";
    
    UnqPtr<DerivedObj> derivedPtr(new DerivedObj(30, 300));
    TestObj* originalAddr = derivedPtr.get();
    
    // Move-конвертация: владение передаётся без копирования
    UnqPtr<TestObj> basePtr = std::move(derivedPtr);
    std::cout << "Move-converted Derived to Base\n";
    
    // Адрес должен быть тот же (не было копирования!)
    assert(basePtr.get() == originalAddr);
    std::cout << "Same address confirmed (no copy occurred)\n";
    
    // Источник должен быть пустым
    assert(derivedPtr.get() == nullptr);
    std::cout << "Source is now nullptr\n";
    
    // Данные на месте
    assert(basePtr->getId() == 30);
    std::cout << "Data intact (id = 30)\n";
    
    std::cout << "PASSED: move при подтипизации работает\n";
}

void testUnqPtrSubtypingArraysMove() {
    std::cout << "\n [Test] UnqPtr<T[]>: move массивов при подтипизации \n";
    
    DerivedObj* rawArr = new DerivedObj[2]{
        DerivedObj(70, 700),
        DerivedObj(80, 800)
    };
    UnqPtr<DerivedObj[]> derivedArr(rawArr, 2);
    DerivedObj* originalAddr = derivedArr.get();
    
    // Move-конвертация массива
    UnqPtr<TestObj[]> baseArr = std::move(derivedArr);
    std::cout << "Move-converted array Derived[] to Base[]\n";
    
    // Адрес должен быть тот же
    assert(baseArr.get() == originalAddr);
    std::cout << "Same address confirmed (no copy)\n";
    
    // Источник пуст
    assert(derivedArr.get() == nullptr);
    assert(derivedArr.getSize() == 0);
    std::cout << "Source array is now empty\n";
    
    // Данные на месте
    assert(baseArr[0].getId() == 70);
    assert(baseArr[1].getId() == 80);
    
    std::cout << "PASSED: move массивов работает\n";
}

// Тесты подтипизации для ShrdPtr<T> (одиночные объекты)

void testShrdPtrSubtypingConversion() {
    std::cout << "\n [Test] ShrdPtr<T>: конвертация Derived → Base \n";
    
    ShrdPtr<DerivedObj> derivedPtr(new DerivedObj(90, 900));
    std::cout << "Created ShrdPtr<DerivedObj>, refCount: " << derivedPtr.useCount() << "\n";
    
    // Конвертация в базовый класс
    ShrdPtr<TestObj> basePtr = derivedPtr;
    std::cout << "Converted to ShrdPtr<TestObj>, refCount: " << basePtr.useCount() << "\n";
    
    // Проверяем, что это shallow copy (один объект, счетчик = 2)
    assert(derivedPtr.get() == basePtr.get());
    assert(derivedPtr.useCount() == 2);
    assert(basePtr.useCount() == 2);
    std::cout << "Shallow copy confirmed (same address, refCount = 2)\n";
    
    // Проверяем полиморфизм
    std::cout << "Calling virtual method:\n";
    basePtr->printType();
    
    std::cout << "PASSED: конвертация работает\n";
}

void testShrdPtrSubtypingAssignment() {
    std::cout << "\n [Test] ShrdPtr<T>: присваивание Derived → Base \n";
    
    ShrdPtr<DerivedObj> derivedPtr(new DerivedObj(100, 1000));
    ShrdPtr<TestObj> basePtr;
    
    // Конвертирующее присваивание
    basePtr = derivedPtr;
    std::cout << "Assigned Derived to Base, refCount: " << basePtr.useCount() << "\n";
    
    assert(derivedPtr.get() == basePtr.get());
    assert(derivedPtr.useCount() == 2);
    
    std::cout << "PASSED: присваивание работает\n";
}

void testShrdPtrSubtypingMove() {
    std::cout << "\n [Test] ShrdPtr<T>: move при подтипизации \n";
    
    ShrdPtr<DerivedObj> derivedPtr(new DerivedObj(110, 1100));
    TestObj* originalAddr = derivedPtr.get();
    std::cout << "Created ShrdPtr<DerivedObj>, refCount: " << derivedPtr.useCount() << "\n";
    
    // Move-конвертация: владение передаётся без инкремента счётчика
    ShrdPtr<TestObj> basePtr = std::move(derivedPtr);
    std::cout << "Move-converted Derived to Base\n";
    
    // Адрес тот же
    assert(basePtr.get() == originalAddr);
    std::cout << "Same address confirmed\n";
    
    // Счётчик НЕ увеличился (move, а не copy!)
    assert(basePtr.useCount() == 1);
    std::cout << "RefCount = 1 (no increment during move)\n";
    
    // Источник пуст
    assert(derivedPtr.get() == nullptr);
    assert(derivedPtr.useCount() == 0);
    std::cout << "Source is now empty\n";
    
    std::cout << "PASSED: move при подтипизации работает\n";
}

void testShrdPtrSubtypingArrays() {
    std::cout << "\n [Test] ShrdPtr<T[]>: конвертация массивов \n";
    
    DerivedObj* rawArr = new DerivedObj[2]{
        DerivedObj(120, 1200),
        DerivedObj(130, 1300)
    };
    ShrdPtr<DerivedObj[]> derivedArr(rawArr, 2);
    
    // Конвертация массива
    ShrdPtr<TestObj[]> baseArr = derivedArr;
    std::cout << "Converted ShrdPtr<DerivedObj[]> to ShrdPtr<TestObj[]>\n";
    
    // Проверяем shallow copy (один массив, счетчик = 2)
    assert(derivedArr.get() == baseArr.get());
    assert(derivedArr.useCount() == 2);
    assert(baseArr.useCount() == 2);
    std::cout << "Shallow copy for arrays confirmed (refCount = 2)\n";
    
    // Изменение через baseArr должно быть видно в derivedArr
    baseArr[0] = DerivedObj(999, 999);
    assert(derivedArr[0].getId() == 999);
    std::cout << "Shared ownership confirmed\n";
    
    std::cout << "PASSED: конвертация массивов работает\n";
}

void testShrdPtrSubtypingArraysMove() {
    std::cout << "\n [Test] ShrdPtr<T[]>: move массивов при подтипизации \n";
    
    DerivedObj* rawArr = new DerivedObj[2]{
        DerivedObj(140, 1400),
        DerivedObj(150, 1500)
    };
    ShrdPtr<DerivedObj[]> derivedArr(rawArr, 2);
    DerivedObj* originalAddr = derivedArr.get();
    std::cout << "Created ShrdPtr<DerivedObj[]>, refCount: " << derivedArr.useCount() << "\n";
    
    // Move-конвертация массива
    ShrdPtr<TestObj[]> baseArr = std::move(derivedArr);
    std::cout << "Move-converted array Derived[] to Base[]\n";
    
    // Адрес тот же
    assert(baseArr.get() == originalAddr);
    std::cout << "Same address confirmed\n";
    
    // Счётчик НЕ увеличился
    assert(baseArr.useCount() == 1);
    std::cout << "RefCount = 1 (no increment during move)\n";
    
    // Источник пуст
    assert(derivedArr.get() == nullptr);
    assert(derivedArr.getSize() == 0);
    std::cout << "Source array is now empty\n";
    
    std::cout << "PASSED: move массивов работает\n";
}

// Тест полиморфизма в контейнере


void testPolymorphismInContainer() {
    std::cout << "\n [Test] Полиморфизм в контейнере \n";
    
    // Создаем массив базовых указателей
    ShrdPtr<TestObj> arr[3];
    
    // Заполняем разными типами (конвертация Derived → Base)
    arr[0] = ShrdPtr<DerivedObj>(new DerivedObj(1, 10));
    arr[1] = ShrdPtr<TestObj>(new TestObj(2));
    arr[2] = ShrdPtr<DerivedObj>(new DerivedObj(3, 30));
    
    std::cout << "Array filled with mixed types:\n";
    
    // Вызываем виртуальные методы (должны работать корректно)
    for (int i = 0; i < 3; ++i) {
        std::cout << "  Element " << i << ": ";
        arr[i]->printType();
    }
    
    // Проверяем, что все элементы разделяют владение корректно
    assert(arr[0].useCount() == 1);
    assert(arr[1].useCount() == 1);
    assert(arr[2].useCount() == 1);
    std::cout << "All elements have refCount = 1\n";
    
    std::cout << "PASSED: полиморфизм работает в контейнере\n";
}

int main() {
    //std::ios_base::sync_with_stdio(false);
    
    std::cout << "   ТЕСТЫ ДЛЯ УКАЗАТЕЛЕЙ С [] (МАССИВЫ)\n";
    
    // UnqPtr<T[]>
    testUnqPtrArrayCreation();
    testUnqPtrArrayBoundsCheck();
    
    // ShrdPtr<T[]>
    testShrdPtrArrayCreation();
    testShrdPtrArraySharedOwnership();
    testShrdPtrArrayBoundsCheck();
    testShrdPtrArrayAssignment();
    
    // Критический тест на delete[]
    testDestructorCallForAllElements();

    // memoryspan
    testMemorySpanCreation();
    testMemorySpanCopy();
    testMsPtrCreation();
    testMsPtrArithmetic();
    testMsPtrBoundsCheck();
    testMsPtrIndexing();
    testMsPtrComparison();
    testMsPtrWithObjects();

    // UnqPtr — одиночные объекты
    testUnqPtrSubtypingMove();
    
    // UnqPtr — массивы
    testUnqPtrSubtypingArraysMove();
    
    // ShrdPtr — одиночные объекты
    testShrdPtrSubtypingConversion();
    testShrdPtrSubtypingAssignment();
    testShrdPtrSubtypingMove();
    
    // ShrdPtr — массивы
    testShrdPtrSubtypingArrays();
    testShrdPtrSubtypingArraysMove();
    
    // Полиморфизм
    testPolymorphismInContainer();
    
    std::cout << "   ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!\n";
    
    return 0;
}