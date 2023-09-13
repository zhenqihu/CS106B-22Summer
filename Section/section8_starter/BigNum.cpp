#include "BigNum.h"
#include "testing/SimpleTest.h"
#include <iostream>

using namespace std;

const int kDefaultSize = 10;

BigNum::BigNum(int value) {
    /* initialize the member variables */
    numDigits = numDigitsOf(value);
    if (numDigits > kDefaultSize) {
        allocatedSize = 2 * numDigits;
    } else {
        allocatedSize = kDefaultSize;
    }
    digits = new int[allocatedSize];

    /* store value in digits */
    for (int i = 0; i < numDigits; i++) {
        digits[i] = value % 10;
        value /= 10;
    }
}

int BigNum::numDigitsOf(int value) const {
    /* count the digit number of value */
    int count = 1;
    while (value >= 10) {
        count++;
        value = value / 10;
    }
    return count;
}

BigNum::~BigNum() {
    delete[] digits;
}

string BigNum::toString() const {
    string str;
    for (int i = numDigits - 1; i >= 0; i--) {
        str += to_string(digits[i]);
    }
    return str;
}

void BigNum::add(const BigNum& value) {
    int digitsToVisit = max(numDigits, value.numDigits);
    reserve(1 + digitsToVisit);

    int carry = 0;
    for (int i = 0; i < digitsToVisit; i++) {
        int sum;
        if (i < numDigits && i < value.numDigits) {
            sum = digits[i] + value.digits[i];
        } else if (i < numDigits) {
            sum = digits[i];
        } else {
            sum = value.digits[i];
        }
        digits[i] = sum % 10 + carry;
        carry = sum / 10;
    }
    if (carry > 0) {
        digits[digitsToVisit] = carry;
        numDigits = 1 + digitsToVisit;
    } else {
        numDigits = digitsToVisit;
    }
}

void BigNum::reserve(int space) {
    if (space <= allocatedSize) {
        return;
    } else {
        allocatedSize = 2 * space;
        int* newDigits = new int[allocatedSize];
        for (int i = 0; i < numDigits; i++) {
            newDigits[i] = digits[i];
        }
        delete[] digits;
        digits = newDigits;
    }
}

PROVIDED_TEST("Simple BigNum Tests") {
    BigNum zero(0);
    BigNum small(106);
    BigNum large(78979871);

    EXPECT_EQUAL(zero.toString(), "0");
    EXPECT_EQUAL(small.toString(), "106");
    EXPECT_EQUAL(large.toString(), "78979871");

    small.add(zero);
    EXPECT_EQUAL(zero.toString(), "0");
    EXPECT_EQUAL(small.toString(), "106");

    zero.add(small);
    EXPECT_EQUAL(zero.toString(), "106");
    EXPECT_EQUAL(small.toString(), "106");

    large.add(small);
    EXPECT_EQUAL(small.toString(), "106");
    EXPECT_EQUAL(large.toString(), "78979977");
}
