#include "CppUnitTest.h"
#include "main.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
    TEST_CLASS(tests){
    public :
        TEST_METHOD(test_min_path)
        {
            std::vector<std::vector<int>> adj_matrix {{INF, 1, 2}, {3, INF, 1}, {2, 2, INF}};
            auto paths = get_fastest_paths(adj_matrix, 0);
            Assert::IsTrue(paths[0].num == 0);
            Assert::IsTrue(paths[0].ind_from == -1);
            Assert::IsTrue(paths[1].num == 1);
            Assert::IsTrue(paths[1].ind_from == 0);
            Assert::IsTrue(paths[2].num == 2);
            Assert::IsTrue(paths[2].ind_from == 0);
        }
    };
}