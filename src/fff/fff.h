/*!
 * \file
 * \author David Saxon
 * \brief Single header the contains the entirety of the Fucking-Fast-Find
 *        functionality.
 */
#ifndef FFF_FFF_H_
#define FFF_FFF_H_

#include <limits.h>
#include <stdint.h>
#include <string.h>

//------------------------------------------------------------------------------
//                                NO MANGLE BEGIN
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                    DEFINES
//------------------------------------------------------------------------------

#ifndef FFF_CHUNK_SIZE
    #define FFF_CHUNK_SIZE 8
#endif

#if FFF_CHUNK_SIZE == 1
    #define FFF_CHUNK_TYPE uint8_t
    #define FFF_CHUNK_MAX 0xFFU
#elif FFF_CHUNK_SIZE == 2
    #define FFF_CHUNK_TYPE uint16_t
    #define FFF_CHUNK_MAX 0xFFFFU
#elif FFF_CHUNK_SIZE == 4
    #define FFF_CHUNK_TYPE uint64_t
    #define FFF_CHUNK_MAX 0xFFFFFFFFU
#elif FFF_CHUNK_SIZE == 8
    #define FFF_CHUNK_TYPE uint64_t
    #define FFF_CHUNK_MAX 0xFFFFFFFFFFFFFFFFUL
#else
    #error "FFF_CHUNK_SIZE must be 1, 2, 4, or 8"
#endif

/*!
* \brief The length value that represents a null position in a string or
*        byte stream.
*/
#define kFFFNpos UINT_MAX

/*!
* \brief The possible error codes of FFF.
*/
typedef int32_t FFFError;
/*!
 * \brief No error occurred - execution successful.
 */
#define kFFFErrorNone 0
/*!
 * \brief The requested functionality has not been implemented.
 */
#define kFFFErrorNotImplemented 1

//------------------------------------------------------------------------------
//                                   CONSTANTS
//------------------------------------------------------------------------------

static const uint8_t FFF_JUMP_TABLE[256] =
{
 255, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3,
 0, 1, 0, 2, 0, 1, 0, 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0,
 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1,
 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5, 0, 1, 0,
 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2,
 0, 1, 0, 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0,
 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1,
 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6, 0, 1, 0, 2, 0, 1, 0,
 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0,
 1, 0, 2, 0, 1, 0
};

//------------------------------------------------------------------------------
//                                   PROTOTYPES
//------------------------------------------------------------------------------

/*!
 * \brief TODO
 */
inline uint32_t fff_find(
        const char* str,
        uint32_t str_length,
        const char* token,
        uint32_t token_length);

/*!
 * \brief TODO
 */
inline uint32_t fff_find_pos(
        const char* str,
        uint32_t str_length,
        const char* token,
        uint32_t token_length,
        uint32_t pos);

/*!
 * \brief TODO
 */
// inline uint32_t fff_rfind(
//         const char* str,
//         uint32_t str_length,
//         const char* token,
//         uint32_t token_length);

/*!
 * \brief TODO
 */
// inline uint32_t fff_rfind_pos(
//         const char* str,
//         uint32_t str_length,
//         const char* token,
//         uint32_t token_length,
//         uint32_t pos);

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

inline uint32_t fff_find(
        const char* str,
        uint32_t str_length,
        const char* token,
        uint32_t token_length)
{
    return fff_find_pos(str, str_length, token, token_length, 0);
}

inline uint32_t fff_find_pos(
        const char* str,
        uint32_t str_length,
        const char* token,
        uint32_t token_length,
        uint32_t pos)
{
    // if the null position has been supplied for lengths, evaluate the actual
    // lengths
    if(str_length == kFFFNpos)
    {
        str_length = strlen(str);
    }
    if(token_length == kFFFNpos)
    {
        token_length = strlen(token);
    }

    // if either string has a length of 0 or the search position is beyond the
    // length of the string
    if(str_length == 0 || pos >= str_length)
    {
        return kFFFNpos;
    }
    if(token_length == 0)
    {
        return 0;
    }

    str += pos;
    str_length -= pos;

    FFF_CHUNK_TYPE token_b_0 = token[0];
    #if FFF_CHUNK_SIZE > 1
        FFF_CHUNK_TYPE token_b_1 = token_b_0 << 8;
    #endif
    #if FFF_CHUNK_SIZE > 2
        FFF_CHUNK_TYPE token_b_2 = token_b_0 << 16;
        FFF_CHUNK_TYPE token_b_3 = token_b_0 << 24;
    #endif
    #if FFF_CHUNK_SIZE > 4
        FFF_CHUNK_TYPE token_b_4 = token_b_0 << 32;
        FFF_CHUNK_TYPE token_b_5 = token_b_0 << 40;
        FFF_CHUNK_TYPE token_b_6 = token_b_0 << 48;
        FFF_CHUNK_TYPE token_b_7 = token_b_0 << 56;
    #endif

    FFF_CHUNK_TYPE bitwise_match_0 = 0;
    uint32_t clear_length =  token_length - 1;
    // I think this might be a compiler bug, for some reason if the clear
    // length is 0 which means 0xFFFFFFFFFFFFFFFF should be shifted by 64-bits
    // it resolves to 0xFFFFFFFFFFFFFFFF but if you shift by the literal 64 you
    // get 0x0000000000000000
    if(clear_length != 0)
    {
        bitwise_match_0 = *(const FFF_CHUNK_TYPE*) (token + 1);
        bitwise_match_0 &=
            FFF_CHUNK_MAX >> ((FFF_CHUNK_SIZE - clear_length) * 8);
    }

    // search for the first character
    const FFF_CHUNK_TYPE* bitwise_s = (const FFF_CHUNK_TYPE*) str;
    uint32_t iterations = str_length / FFF_CHUNK_SIZE;
    uint32_t whole_iterations = iterations;
    uint32_t remainder_bytes = str_length % FFF_CHUNK_SIZE;
    if(remainder_bytes != 0)
    {
        ++iterations;
    }
    for(uint32_t i = 0; i < iterations; ++i)
    {
        // get the character index
        uint32_t v = i * FFF_CHUNK_SIZE;
        // get the string section to check
        FFF_CHUNK_TYPE check_s = bitwise_s[i];
        // if this is the last iteration and it's non-whole, zero out the
        // irrelevant bytes
        if(i >= whole_iterations)
        {
            check_s &=
                FFF_CHUNK_MAX >> ((FFF_CHUNK_SIZE - remainder_bytes) * 8);
        }

        uint8_t location_mask =
            (((check_s & 0x00000000000000FF) == token_b_0) << 0)
        #if FFF_CHUNK_SIZE > 1
            | (((check_s & 0x000000000000FF00) == token_b_1) << 1)
        #endif
        #if FFF_CHUNK_SIZE > 2
            | (((check_s & 0x0000000000FF0000) == token_b_2) << 2)
            | (((check_s & 0x00000000FF000000) == token_b_3) << 3)
        #endif
        #if FFF_CHUNK_SIZE > 4
            | (((check_s & 0x000000FF00000000) == token_b_4) << 4)
            | (((check_s & 0x0000FF0000000000) == token_b_5) << 5)
            | (((check_s & 0x00FF000000000000) == token_b_6) << 6)
            | (((check_s & 0xFF00000000000000) == token_b_7) << 7)
        #endif
        ;

        uint8_t check_index = FFF_JUMP_TABLE[location_mask];
        while(check_index != 255)
        {
            uint8_t matched = 1;
            FFF_CHUNK_TYPE bitwise_match = bitwise_match_0;
            uint32_t j = 1;

            // perform the first match outside of the while to avoid branching
            FFF_CHUNK_TYPE search_bits =
                *(const FFF_CHUNK_TYPE*) (str + v + check_index + j);
            matched = ((bitwise_match & search_bits) ^ bitwise_match) == 0;
            if(matched && (token_length - 1) <= FFF_CHUNK_SIZE)
            {
                return v + check_index;
            }
            else if(matched)
            {
                while(matched != 0)
                {
                    FFF_CHUNK_TYPE search_bits =
                        *(const FFF_CHUNK_TYPE*) (str + v + check_index + j);
                    matched =
                        ((bitwise_match & search_bits) ^ bitwise_match) == 0;

                    j += FFF_CHUNK_SIZE;
                    if(j < token_length)
                    {
                        // build the next matched - this is slower from here but
                        // still faster than naive or std finds
                        bitwise_match = *(const FFF_CHUNK_TYPE*) (token + j);
                        bitwise_match &= FFF_CHUNK_MAX >>
                            ((FFF_CHUNK_SIZE - (token_length - j)) * 8);
                        continue;
                    }
                    break;
                }

                if(matched)
                {
                    return v + check_index;
                }
            }

            // update the location mask
            location_mask &= ~(0x01 << check_index);
            check_index = FFF_JUMP_TABLE[location_mask];
        }
    }

    return kFFFNpos;
}

//------------------------------------------------------------------------------
//                                 NO MANGLE END
//------------------------------------------------------------------------------
#ifdef __cplusplus
// extern "C"
}
#endif
//------------------------------------------------------------------------------

#endif
