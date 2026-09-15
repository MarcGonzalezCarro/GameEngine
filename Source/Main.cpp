#include <cstddef>
#include <stdio.h>
template<typename T>
class vector {
    T* beginPtr = nullptr;
    T* endPtr = nullptr;
    void Realloc(std::size_t newCapacity) {
        T* tmp = new T(newCapacity)
            for (std::size_t i = 0; i < Size(); i++) {
            
            }
    }
public:
    std::size_t Size() const{
        return endPtr - beginPtr;
    }
    T& push_back(const T& data) {
        beginPtr = new T();
    }
};
int main()
{
    printf("Hola");
        return 0;
}