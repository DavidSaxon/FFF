#include "arcanecore/test/ArcTest.hpp"

ARC_TEST_MODULE(fff.unit)

#include <arcanecore/base/clock/ClockOperations.hpp>

#include <fff/fff.h>

namespace
{

//------------------------------------------------------------------------------
//                                    FIXTURE
//------------------------------------------------------------------------------

class FFF_Fixture : public arc::test::Fixture
{
public:

    //-----------------------------PUBLIC FUNCTIONS-----------------------------

    uint32_t naive_find(
        const char* s,
        uint32_t s_length,
        const char* primary_token,
        uint32_t primary_length,
        uint32_t pos = 0)
    {
        // if the null position has been supplied for lengths, evaluate the actual
        // lengths
        if(s_length == kFFFNpos)
        {
            s_length = strlen(s);
        }
        if(primary_length == kFFFNpos)
        {
            primary_length = strlen(primary_token);
        }

        // if either string has a length of 0 or the search position is beyond the
        // length of the string
        if(s_length == 0 || pos >= s_length)
        {
            return kFFFNpos;
        }
        if(primary_length == 0)
        {
            return 0;
        }

        s += pos;
        s_length -= pos;

        for(uint32_t i = 0; i < (s_length - (primary_length - 1)); ++i)
        {
            uint32_t found = 1;
            for(uint32_t j = 0; j < primary_length; ++j)
            {
                if(s[i + j] != primary_token[j])
                {
                    found = 0;
                    break;
                }
            }
            if(found != 0)
            {
                return i;
            }
        }

        return kFFFNpos;
    }
};

//------------------------------------------------------------------------------
//                                  PROTOTYPING
//------------------------------------------------------------------------------

// ARC_TEST_UNIT_FIXTURE(proto, FFF_Fixture)
// {
//     std::string s("this is a eclectic sentence with some content");
//     std::string primary("ent");

//     const char* fff_s = s.c_str();
//     uint32_t fff_s_length = s.length();
//     const char* fff_primary_token = primary.c_str();
//     uint32_t fff_primary_length = primary.length();

//     uint32_t primary_pos = kFFFNpos;
//     uint32_t secondary_pos = kFFFNpos;

//     FFFError error_code = fff(
//         fff_s,
//         fff_s_length,
//         fff_primary_token,
//         fff_primary_length,
//         0x0,
//         0,
//         kFFFEncodingASCII,
//         kFFFFlagNone,
//         &primary_pos,
//         &secondary_pos
//     );

//     std::cout << "error: " << error_code << std::endl;

//     ARC_CHECK_EQUAL(error_code, kFFFErrorNone);
// }

// TODO: test decection in remainder
// TODO: test long search string in almost repeating string

//------------------------------------------------------------------------------
//                               ASCII_PERFORMANCE
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(ascii_performance, FFF_Fixture)
{
    std::string s("this is an eclectic sentence with some content");
    std::string primary("ent");

    // std::string s("this is an eclectic sent this is an eclectic sentence");
    // std::string primary("this is an eclectic sentence");

    const char* fff_s = s.c_str();
    uint32_t fff_s_length = s.length();
    const char* fff_primary_token = primary.c_str();
    uint32_t fff_primary_length = primary.length();

    //--------------------------------------------------------------------------
    // test dummy - since first run is normally slower
    //--------------------------------------------------------------------------
    // ARC_TEST_MESSAGE("Running dummy find");
    // uint32_t dummy_result = kFFFNpos;
    // for(std::size_t i = 0; i < 10; ++i)
    // {
    //     fixture->naive_find(
    //         fff_s,
    //         fff_s_length,
    //         fff_primary_token,
    //         fff_primary_length,
    //         &dummy_result
    //     );
    // }
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // naive
    //--------------------------------------------------------------------------
    ARC_TEST_MESSAGE("Running naive find");
    uint32_t naive_result = kFFFNpos;
    arc::uint64 naive_start =
        arc::clock::get_current_time(arc::clock::METRIC_NANOSECONDS);
    for(std::size_t i = 0; i < 1; ++i)
    {
        naive_result = fixture->naive_find(
            fff_s,
            fff_s_length,
            fff_primary_token,
            fff_primary_length
        );
        // naive_result = fff_find(
        //     fff_s,
        //     fff_s_length,
        //     fff_primary_token,
        //     fff_primary_length,
        //     0x0,
        //     0,
        //     kFFFEncodingUTF8,
        //     kFFFFlagNone,
        //     &naive_result,
        //     &secondary_pos
        // );
    }
    arc::uint64 naive_end =
        arc::clock::get_current_time(arc::clock::METRIC_NANOSECONDS);
    arc::uint64 naive_time = naive_end - naive_start;
    std::cout << "naive time: " << naive_time << std::endl;
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // std
    //--------------------------------------------------------------------------
    ARC_TEST_MESSAGE("Running std::string find");
    std::string::size_type std_result = std::string::npos;
    arc::uint64 std_start =
        arc::clock::get_current_time(arc::clock::METRIC_NANOSECONDS);
    for(std::size_t i = 0; i < 1; ++i)
    {
        std_result = s.find(primary);
    }
    arc::uint64 std_end =
        arc::clock::get_current_time(arc::clock::METRIC_NANOSECONDS);
    arc::uint64 std_time = std_end - std_start;
    std::cout << "std time: " << std_time << std::endl;
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // fff
    //--------------------------------------------------------------------------
    ARC_TEST_MESSAGE("Running FFF");
    uint32_t fff_result = kFFFNpos;
    arc::uint64 fff_start =
        arc::clock::get_current_time(arc::clock::METRIC_NANOSECONDS);
    // TODO:
    for(std::size_t i = 0; i < 1; ++i)
    {
        fff_result = fff_find(
            fff_s,
            fff_s_length,
            fff_primary_token,
            fff_primary_length
        );
    }
    arc::uint64 fff_end =
        arc::clock::get_current_time(arc::clock::METRIC_NANOSECONDS);
    arc::uint64 fff_time = fff_end - fff_start;
    std::cout << "fff time: " << fff_time << std::endl;
    //--------------------------------------------------------------------------

    ARC_CHECK_EQUAL(fff_result, naive_result);
    ARC_CHECK_TRUE(fff_time < naive_time);

    ARC_CHECK_EQUAL(fff_result, std_result);
    ARC_CHECK_TRUE(fff_time < std_time);

    //--------------------------------------------------------------------------
    // TODO: front hint
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // TODO: back end
    //--------------------------------------------------------------------------
}

} // namespace anonymous
