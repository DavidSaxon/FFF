#include "fff/fff.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------------UTILITIES-----------------------------------

static const uint8_t g_jump_table[256] =
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

static void print_bits_8(char c)
{
    uint32_t bits = sizeof(char) * 8;
    char* str = malloc(bits + 1);
    str[bits] = '\0';

    for(uint32_t i = 0; i < bits; ++i)
    {
        if(c & (0x80 >> i))
        {
            str[i] = '1';
        }
        else
        {
            str[i] = '0';
        }
    }

    printf("%s\n", str);

    free(str);
}

static void print_bits_32(uint32_t b)
{
    uint32_t bits = sizeof(uint32_t) * 8;
    uint32_t spaces = sizeof(uint32_t) - 1;
    uint32_t full_length = bits + spaces;
    char* str = malloc(full_length + 1);
    for(uint32_t i = 0; i < full_length; ++i)
    {
        str[i] = ' ';
    }

    // add spaces
    for(uint32_t i = 0; i < spaces; ++i)
    {
        str[((i + 1) * 8) + i] = '-';
    }
    // add null terminator
    str[full_length] = '\0';

    for(uint32_t i = 0; i < bits; ++i)
    {
        if(b & (0x80000000 >> i))
        {
            str[i + (i / sizeof(uint32_t))] = '1';
        }
        else
        {
            str[i +  + (i / sizeof(uint32_t))] = '0';
        }
    }

    printf("%s\n", str);

    free(str);
}

static void print_bits_64(uint64_t b)
{
    uint32_t bits = sizeof(uint64_t) * 8;
    uint32_t spaces = sizeof(uint64_t) - 1;
    uint32_t full_length = bits + spaces;
    char* str = malloc(full_length + 1);
    for(uint32_t i = 0; i < full_length; ++i)
    {
        str[i] = ' ';
    }

    // add spaces
    for(uint32_t i = 0; i < spaces; ++i)
    {
        str[((i + 1) * 8) + i] = '-';
    }
    // add null terminator
    str[full_length] = '\0';

    for(uint32_t i = 0; i < bits; ++i)
    {
        if(b & (0x8000000000000000 >> i))
        {
            str[i + (i / sizeof(uint64_t))] = '1';
        }
        else
        {
            str[i +  + (i / sizeof(uint64_t))] = '0';
        }
    }

    printf("%s\n", str);

    free(str);
}

//------------------------------------------------------------------------------
//                                   PROTOTYPES
//------------------------------------------------------------------------------

/*!
 * \brief Performs the Fucking-Fast-Find for ascii strings.
 */
FFFError fff_ascii(
        const char* s,
        uint32_t s_length,
        const char* primary_token,
        uint32_t primary_length,
        const char* secondary_token,
        uint32_t secondary_length,
        FFFFlags flags,
        uint32_t* primary_pos,
        uint32_t* secondary_pos);

// TODO: REMOVE ME
void naive_find(
    const char* s,
    uint32_t s_length,
    const char* primary_token,
    uint32_t primary_length,
    const char* secondary_token,
    uint32_t secondary_length,
    FFFFlags flags,
    uint32_t* primary_pos,
    uint32_t* secondary_pos)
{
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
            *primary_pos = i;
            return;
        }
    }

    *primary_pos = kFFFNpos;
    *secondary_pos = kFFFNpos;
    return;
}

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

// TODO: test with C++ compiler

FFFError fff(
        const char* s,
        uint32_t s_length,
        const char* primary_token,
        uint32_t primary_length,
        const char* secondary_token,
        uint32_t secondary_length,
        FFFEncoding encoding,
        FFFFlags flags,
        uint32_t* primary_pos,
        uint32_t* secondary_pos)
{
    // calculate string lengths if we need to
    if(s_length == kFFFNpos)
    {
        s_length = strlen(s);
    }
    if(primary_length == kFFFNpos)
    {
        primary_length = strlen(primary_token);
    }
    if(secondary_length == kFFFNpos)
    {
        secondary_length = strlen(secondary_token);
    }

    // TODO: check for bad primary length

    switch(encoding)
    {
        case kFFFEncodingASCII:
        {
            return fff_ascii(
                s,
                s_length,
                primary_token,
                primary_length,
                secondary_token,
                secondary_length,
                flags,
                primary_pos,
                secondary_pos
            );
            break;
        }
        case kFFFEncodingUTF8:
        {
            naive_find(
                s,
                s_length,
                primary_token,
                primary_length,
                secondary_token,
                secondary_length,
                flags,
                primary_pos,
                secondary_pos
            );
            return kFFFErrorNone;
            break;
        }
        default:
        {
            return kFFFErrorNotImplemented;
        }
    }

    return kFFFErrorNone;
}

FFFError fff_ascii(
        const char* s,
        uint32_t s_length,
        const char* primary_token,
        uint32_t primary_length,
        const char* secondary_token,
        uint32_t secondary_length,
        FFFFlags flags,
        uint32_t* primary_pos,
        uint32_t* secondary_pos)
{
    uint64_t primary_token_b_0 = primary_token[0];
    uint64_t primary_token_b_1 = primary_token_b_0 << 8;
    uint64_t primary_token_b_2 = primary_token_b_0 << 16;
    uint64_t primary_token_b_3 = primary_token_b_0 << 24;
    uint64_t primary_token_b_4 = primary_token_b_0 << 32;
    uint64_t primary_token_b_5 = primary_token_b_0 << 40;
    uint64_t primary_token_b_6 = primary_token_b_0 << 48;
    uint64_t primary_token_b_7 = primary_token_b_0 << 56;

    uint64_t start_token =
        primary_token_b_0 |
        primary_token_b_1 |
        primary_token_b_2 |
        primary_token_b_3 |
        primary_token_b_4 |
        primary_token_b_5 |
        primary_token_b_6 |
        primary_token_b_7;

    uint64_t bitwise_match_0 = 0;
    uint64_t clear_length =  primary_length - 1;
    // I think this might be a compiler error, for some reason if the clear
    // length is 0 which means 0xFFFFFFFFFFFFFFFF should be shifted by 64-bits
    // it resolves to 0xFFFFFFFFFFFFFFFF rather than 0x0000000000000000
    if(clear_length != 0)
    {
        bitwise_match_0 = *(const uint64_t*) (primary_token + 1);
        bitwise_match_0 &=
            0xFFFFFFFFFFFFFFFFUL >> ((sizeof(uint64_t) - clear_length) * 8);
    }

    // search for the first character
    const uint64_t* bitwise_s = (const uint64_t*) s;
    uint32_t iterations = s_length / sizeof(uint64_t);
    uint32_t whole_iterations = iterations;
    uint32_t remainder_bytes = s_length % sizeof(uint64_t);
    if(remainder_bytes != 0)
    {
        ++iterations;
    }
    for(uint32_t i = 0; i < iterations; ++i)
    {
        // get the character index
        uint32_t v = i * sizeof(uint64_t);
        // get the string section to check
        uint64_t check_s = bitwise_s[i];
        // if this is the last iteration and it's non-whole, zero out the
        // irrelevant bytes
        if(i >= whole_iterations)
        {
            check_s &= 0xFFFFFFFFFFFFFFFF >>
                ((sizeof(uint64_t) - remainder_bytes) * 8);
        }

        uint8_t location_mask =
            (((check_s & 0x00000000000000FF) == primary_token_b_0) << 0) |
            (((check_s & 0x000000000000FF00) == primary_token_b_1) << 1) |
            (((check_s & 0x0000000000FF0000) == primary_token_b_2) << 2) |
            (((check_s & 0x00000000FF000000) == primary_token_b_3) << 3) |
            (((check_s & 0x000000FF00000000) == primary_token_b_4) << 4) |
            (((check_s & 0x0000FF0000000000) == primary_token_b_5) << 5) |
            (((check_s & 0x00FF000000000000) == primary_token_b_6) << 6) |
            (((check_s & 0xFF00000000000000) == primary_token_b_7) << 7);

        uint8_t check_index = g_jump_table[location_mask];
        while(check_index != 255)
        {
            uint8_t matched = 1;
            uint64_t bitwise_match = bitwise_match_0;
            uint32_t j = 1;

            // perform the first match outside of the while to avoid branching
            uint64_t search_bits =
                *(const uint64_t*) (s + v + check_index + j);
            matched = ((bitwise_match & search_bits) ^ bitwise_match) == 0;
            if(matched && (primary_length - 1) <= sizeof(uint64_t))
            {
                *primary_pos = v + check_index;
                return kFFFErrorNone;
            }
            else if(matched)
            {
                while(matched != 0)
                {
                    uint64_t search_bits =
                        *(const uint64_t*) (s + v + check_index + j);
                    matched =
                        ((bitwise_match & search_bits) ^ bitwise_match) == 0;

                    j += sizeof(uint64_t);
                    if(j < primary_length)
                    {
                        // build the next matched - this is slower from here but
                        // still faster than naive or std finds
                        bitwise_match = *(const uint64_t*) (primary_token + j);
                        bitwise_match &= 0xFFFFFFFFFFFFFFFF >>
                            ((sizeof(uint64_t) - (primary_length - j)) * 8);
                        continue;
                    }
                    break;
                }

                if(matched)
                {
                    *primary_pos = v + check_index;
                    return kFFFErrorNone;
                }
            }

            // update the location mask
            location_mask &= ~(0x01 << check_index);
            check_index = g_jump_table[location_mask];
        }
    }

    *primary_pos = kFFFNpos;
    *secondary_pos = kFFFNpos;
    return kFFFErrorNone;
}
