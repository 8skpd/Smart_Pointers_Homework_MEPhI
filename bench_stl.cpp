#include <cstddef>
#include <memory>

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
    
    std::unique_ptr<TestObject>* arr = new std::unique_ptr<TestObject>[N];
    
    for (size_t i = 0; i < N; ++i) {
        arr[i] = std::make_unique<TestObject>();
    }
    
    delete[] arr;
    
    return 0;
}