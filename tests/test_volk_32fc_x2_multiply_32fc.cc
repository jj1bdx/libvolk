/* -*- c++ -*- */
/*
 * Copyright 2022 Johannes Demel
 *
 * This file is part of VOLK
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "volk_test.h"
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <volk/volk.h>
#include <volk/volk_alloc.hh>
#include <chrono>

class volk_32fc_x2_multiply_32fc_test : public VolkTest
{
protected:
    void SetUp() override
    {
        initialize_test(GetParam());
        initialize_data(vector_length);
    }

    void initialize_data(const size_t length)
    {
        vector_length = length;
        vec0 = volk::vector<lv_32fc_t>(length);
        vec1 = volk::vector<lv_32fc_t>(length);
        result = volk::vector<lv_32fc_t>(length);
        for (size_t i = 0; i < length; ++i) {
            vec0[i] = std::complex<float>(i * 3.14, i * 0.45);
            vec1[i] = std::complex<float>(i * -2.78, i * 5.44);
        }

        expected = volk::vector<lv_32fc_t>(length);
        for (size_t i = 0; i < length; ++i) {
            expected[i] = vec0[i] * vec1[i];
        }

        // This is a hacky solution to have unaligned tests.
        ua_result = result;
        ua_result.at(0) = expected.at(0);
    }

    void execute_aligned(const std::string impl_name)
    {
        volk_32fc_x2_multiply_32fc_manual(
            result.data(), vec0.data(), vec1.data(), vector_length, impl_name.c_str());
        EXPECT_TRUE(AreComplexFloatingPointArraysAlmostEqual(expected, result));
    }

    void execute_unaligned(const std::string impl_name)
    {
        volk_32fc_x2_multiply_32fc_manual(ua_result.data() + 1,
                                          vec0.data() + 1,
                                          vec1.data() + 1,
                                          vector_length - 1,
                                          impl_name.c_str());
        EXPECT_TRUE(AreComplexFloatingPointArraysAlmostEqual(expected, ua_result));
    }

    volk::vector<lv_32fc_t> vec0;
    volk::vector<lv_32fc_t> vec1;
    volk::vector<lv_32fc_t> result;
    volk::vector<lv_32fc_t> expected;

    volk::vector<lv_32fc_t> ua_result;
};

TEST_P(volk_32fc_x2_multiply_32fc_test, run)
{
    fmt::print("test {} implementation: {:>12}, size={} ...",
               is_aligned_implementation ? "aligned" : "unaligned",
               implementation_name,
               vector_length);
    auto start = std::chrono::steady_clock::now();

    if (is_aligned_implementation) {
        execute_aligned(implementation_name);
    } else {
        execute_unaligned(implementation_name);
    }


    std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;
    fmt::print("\tduration={}\n", elapsed);
}


INSTANTIATE_TEST_SUITE_P(
    volk_32fc_x2_multiply_32fc,
    volk_32fc_x2_multiply_32fc_test,
    testing::Combine(testing::ValuesIn(get_kernel_implementation_name_list(
                         volk_32fc_x2_multiply_32fc_get_func_desc())),
                     testing::ValuesIn(default_vector_sizes)),
    generate_volk_test_name());
