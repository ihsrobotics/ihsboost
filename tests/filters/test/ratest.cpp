#include "test.hpp"
#include "filters.hpp"
#include <iostream>
#include <list>

using namespace std;

void test_ra(int size)
{
    assert_notequals(0, size, "getting size value");

    list<int> lst;
    RunningAverage<int> filter(size);
    for (int i = 0; i < size * 100; ++i)
    {
        int num = (rand() % 1000) * (rand() % 2 == 0 ? 1 : -1);
        lst.push_back(num);

        if (lst.size() > size)
        {
            lst.pop_front();
        }

        assert_equals(filter.filter(num), accumulate(lst.begin(), lst.end(), 0) / static_cast<double>(lst.size()), "getting running average");
    }
}

int main()
{
    test_ra(20);
    test_ra(50);
    test_ra(100);
    test_ra(200);
    test_ra(500);
    cout << "passed all ra tests" << endl;
    return 0;
}