#pragma once
#include <iostream>

class TestObj {
protected:
    int id;
public:
    TestObj() : id(0) {}
    
    TestObj(int i) : id(i) { 
        std::cout << "  [Created TestObj " << id << "]\n"; 
    }
    
    // Виртуальный деструктор обязателен для полиморфного удаления!
    virtual ~TestObj() { 
        std::cout << "  [Destroyed TestObj " << id << "]\n"; 
    }
    
    int getId() const { return id; }
    
    // Виртуальный метод для демонстрации полиморфизма
    virtual void printType() const {
        std::cout << "I am TestObj " << id << "\n";
    }
};

// Производный класс для тестирования подтипизации
class DerivedObj : public TestObj {
    int extra;
public:
    DerivedObj() : TestObj(), extra(0) {}
    
    DerivedObj(int i, int e) : TestObj(i), extra(e) {
        std::cout << "  [Created DerivedObj " << id << " with extra=" << extra << "]\n";
    }
    
    ~DerivedObj() override {
        std::cout << "  [Destroyed DerivedObj " << id << "]\n";
    }
    
    void printType() const override {
        std::cout << "I am DerivedObj " << id << " with extra=" << extra << "\n";
    }
    
    int getExtra() const { return extra; }
};