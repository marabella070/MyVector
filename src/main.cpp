#include "vector.h"

#include <iostream>
#include <numeric>
#include <vector>
#include <list>
#include <type_traits>

template <typename Container>
void PrintRange(const Container& container)
{
    for (const auto& element : container)
    {
        std::cout << element << ' ';
    }
    std::cout << " size - " << container.Size() << ", capacity - " << container.Capacity() << std::endl;
}

struct point
{
    double x;
    double y;
};

std::ostream& operator<< (std::ostream& out, const point& p)
{
    out << "{" << p.x << ", " << p.y << "}" << std::endl;
    return out;
}

int main()
{
    using namespace std::string_literals;

    Vector<int> first{ 1, 2, 4, 6 };
    Vector<int> second{ 1, 2, 3, 5 };
    std::cout << std::boolalpha << (first < second) << std::endl;


    // Vector<int> test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 };
    // test.Erase(test.begin() + 13, test.begin() + 20); PrintRange(test);
    // test.ShrinkToFit(); PrintRange(test);


    // std::vector<int> test{ 1, 2, 3, 4, 5, 6, 7 };
    // auto it = test.erase(test.begin() + 2);
    // std::cout << *it << std::endl;


    // Vector<int> twist{ 1, 2, 3};
    // twist.PushBack(4);
    // const int*  data = twist.Data();

    // for (int i = 0; i < twist.Size(); ++i) {
    //     std::cout << *(data + i) << ' ';
    // }
    
    // std::cout << std::endl;

    // for (int i = 0; i < twist.Capacity(); ++i) {
    //     std::cout << *(data + i) << ' ';
    // } 

    return 0;
}