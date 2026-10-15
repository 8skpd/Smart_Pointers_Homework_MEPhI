#include <cstddef>
#include "UnqPtr.h"

class TestObject {
    int data[10];
public:
    TestObject() {
        for (int i = 0; i < 10; ++i) data[i] = i;
    }
    ~TestObject() {}
};

int main() {
    const size_t N = 10000000;
    
    UnqPtr<TestObject>* arr = new UnqPtr<TestObject>[N];
    
    for (size_t i = 0; i < N; ++i) {
        arr[i] = UnqPtr<TestObject>(new TestObject());
    }
    
    delete[] arr;
    
    return 0;
}