//
// Created by Amadeus Gebauer on 30.04.18.
//

#include <gtest/gtest.h>
#include "../src/MPI/MpiInfo.h"

TEST(MpiTests, MoreThanOneProcessor) {

    MPI::MpiInfo info = MPI::MpiInfo::Create();

    ASSERT_GT(info.getSize(), 1);
}