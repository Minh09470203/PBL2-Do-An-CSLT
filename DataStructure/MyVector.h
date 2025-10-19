#ifndef MYVECTOR_H_INCLUDED
#define MYVECTOR_H_INCLUDED

// Data structure vector
#include <stdexcept>

template<typename T>
class MyVector {
private:
    T* data;
    int capacity;
    int Size;

    void Resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (int i = 0; i < Size; ++i)
            newData[i] = data[i];
        delete[] data;
        data = newData;
    }

public:
    MyVector() : capacity(10), Size(0) {
        data = new T[capacity];
    }

    void Push_back(const T& value) {
        if (Size == capacity)
            Resize();
        data[Size++] = value;
    }

    void Pop_back() {
        if (Size > 0) --Size;
    }

    T& operator[](int index) {
        if (index < 0 || index >= Size)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    int getSize() const {
        return Size;
    }

    bool Empty() const {
        return Size == 0;
    }

    void Clear() {
        Size = 0;
    }

    ~MyVector() {
        delete[] data;
    }
};
#endif // MYVECTOR_H_INCLUDED