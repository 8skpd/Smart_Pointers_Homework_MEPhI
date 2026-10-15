#include <cstddef>

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
    
    TestObject** arr = new TestObject*[N];
    
    for (size_t i = 0; i < N; ++i) {
        arr[i] = new TestObject();
    }
    
    for (size_t i = 0; i < N; ++i) {
        delete arr[i];
    }
    
    delete[] arr;
    
    return 0;
}