//
// Created by Ivan Kishchenko on 2019-06-16.
//

#define BOOST_TEST_MODULE System

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(SystemTest)

    BOOST_AUTO_TEST_CASE(test) {
        BOOST_ASSERT_MSG("message", "message");
    }

BOOST_AUTO_TEST_SUITE_END()