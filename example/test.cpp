#include <cstdint>
#include <cstdlib>
#include "od_data.hpp"
#include "od.hpp"

using namespace std;

struct A
{
    int a;
    int b;
};

struct B
{
    int a;
    int b[];
};

int main()
{

    OD_ObjectDictionnary dict;

    return EXIT_SUCCESS;
}
