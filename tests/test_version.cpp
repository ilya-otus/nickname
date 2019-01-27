#define BOOST_TEST_MODULE bulk_test_module
#include "../src/lib.h"
#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE(bulk_test_suite)
BOOST_AUTO_TEST_CASE(bulk_test_version)
{
    BOOST_CHECK(versionMajor() >= 0);
    BOOST_CHECK(versionMinor() >= 0);
    BOOST_CHECK(versionPatch() > 0);
    std::string _version = std::to_string(versionMajor()) + "." +
        std::to_string(versionMinor()) + "." +
        std::to_string(versionPatch());
    BOOST_CHECK(version().compare(_version) == 0);
}
BOOST_AUTO_TEST_SUITE_END()
