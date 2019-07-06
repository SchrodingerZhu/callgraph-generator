#include <iostream>
#include <vector>
static void D() { }
static void Y() { D(); }
static void X() { Y(); }
static void C() { D(); X(); }
static void B() { C(); }
static void S() { D(); }
static void P() { S(); }
static void O() { P(); }
static void N() { O(); }
static void M() { N(); }
static void G() { M(); }
static void A() { B(); G(); }

int main() {
    A();
    auto m = std::vector<int>();
    m.push_back(123);
    std::cout << "test" << std::endl;
}