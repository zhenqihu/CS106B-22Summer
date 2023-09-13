#pragma once
#include <string>

class BigNum {
public:
    BigNum(int value = 0); // Default to zero unless specified otherwise.
    ~BigNum();

    std::string toString() const; // Get a string representation
    void add(const BigNum& value);
private:
    int* digits;
    int allocatedSize;
    int numDigits;
    void reserve(int space);
    int numDigitsOf(int value) const;
};
