#include "matrix.h"

#include <gtest/gtest.h>

TEST(Matrix, Sample) {
    Matrix<int> m(1, 300);

    m[0][0] = 1;
    m[0][1] = 2;
    m[0][2] = 3;
    m[0][200] = 3;

    EXPECT_EQ(m[0][0], 1);
    EXPECT_EQ(m[0][1], 2);
    EXPECT_EQ(m[0][2], 3);
    EXPECT_EQ(m[0][200], 3);

    EXPECT_EQ(m.size(), 1);
    EXPECT_EQ(m[0].size(), 300);
}

TEST(Matrix, Sample2) {
    Matrix<int> m(300, 30);

    m[200][26] = 1;
    m[200][27] = 2;
    m[200][28] = 3;
    m[200][29] = 3;

    EXPECT_EQ(m[200][26], 1);
    EXPECT_EQ(m[200][27], 2);
    EXPECT_EQ(m[200][28], 3);
    EXPECT_EQ(m[200][29], 3);

    EXPECT_EQ(m.size(), 300);
    EXPECT_EQ(m[0].size(), 30);
}

TEST(Matrix, InitList) {
    Matrix<int> m = {
            {1, 2},
            {3, 4},
            {5, 6}
    };

    EXPECT_EQ(m.size(), 3);
    EXPECT_EQ(m[0].size(), 2);

    EXPECT_EQ(m[0][0], 1);
    EXPECT_EQ(m[0][1], 2);

    EXPECT_EQ(m[1][0], 3);
    EXPECT_EQ(m[1][1], 4);

    EXPECT_EQ(m[2][0], 5);
    EXPECT_EQ(m[2][1], 6);
}