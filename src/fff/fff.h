/*!
 * \file
 * \author David Saxon
 * \brief Fucking-Fast-Find function definitions.
 */
#ifndef FFF_FFF_H_
#define FFF_FFF_H_

#include <limits.h>
#include <stdint.h>

//------------------------------------------------------------------------------
//                                  ENUMERATORS
//------------------------------------------------------------------------------

/*!
 * \brief TODO
 */
#define kFFFNpos UINT_MAX

/*!
 * \brief TODO
 */
typedef uint32_t FFFEncoding;
/// TODO:
#define kFFFEncodingASCII 0
/// TODO:
#define kFFFEncodingUTF8 1

/*!
 * \brief TODO
 */
typedef uint32_t FFFFlags;
/// TODO:
#define kFFFFlagNone 0
/// TODO:
#define kFFFFlagFrontHint 1
/// TODO:
#define kFFFFlagBackHint 2
/// TODO:
#define kFFFFlagSecondaryOrEnd 4

/*!
 * \brief TODO
 */
typedef int32_t FFFError;
/// TODO:
#define kFFFErrorNone 0
/// TODO:
#define kFFFErrorNotImplemented 1

#ifdef __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief TODO
 */
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
        uint32_t* secondary_pos);

#endif

#ifdef __cplusplus
} // extern "C"
#endif
