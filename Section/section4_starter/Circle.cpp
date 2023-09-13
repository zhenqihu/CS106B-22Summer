#include "Circle.h"
using namespace std;
#include "testing/SimpleTest.h"
#include "gmath.h"
#include "strlib.h"

/**
 * constructs a new circle with the given radius (Construct)
 */
Circle::Circle(double r) {
    this->radius = r;
}


/**
 * returns the area occupied by the circle
 */
double Circle::area() const {
    return PI * (radius * radius);
}

/**
 * returns the distance around the circle
 */
double Circle::circumference() const{
    return 2 * PI * radius;
}

/**
 * returns the radius as a real number
 */
double Circle::getRadius() const{
    return radius;
}

/**
 * returns a string representation such as "Circle{radius=2.5}"
 */
string Circle::toString() const{
    return "Circle{radius=" + doubleToString(radius) + "}";
}

/* Provided Tests Below This Line */

PROVIDED_TEST("Simple Example"){
    Circle c(2.5);
    EXPECT_EQUAL(c.getRadius(), 2.5);
    EXPECT_EQUAL(c.circumference(), 2 * PI * 2.5);
    EXPECT_EQUAL(c.area(), PI * 2.5 * 2.5);
    EXPECT_EQUAL(c.toString(), "Circle{radius=2.5}");
}
