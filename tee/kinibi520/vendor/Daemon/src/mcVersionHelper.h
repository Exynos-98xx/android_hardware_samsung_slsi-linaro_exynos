/*
 * Copyright (c) 2013-2020 TRUSTONIC LIMITED
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the TRUSTONIC LIMITED nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MCVERSIONHELPER_H
#define MCVERSIONHELPER_H

#include <stdio.h>

/** Create a version number given major and minor numbers. */
#define MC_MAKE_VERSION(major,minor) \
    (   (((major) & 0xffffU) << 16U) |\
        ((minor) & 0xffffU))

/** Get major version number from complete version. */
#define MC_GET_MAJOR_VERSION(version) ((version) >> 16)

/** Get minor version number from complete version. */
#define MC_GET_MINOR_VERSION(version) ((version) & 0xffff)

#define STRINGIFY(x)    #x
#define XSTR(x)         STRINGIFY(x)

// Asserts expression at compile-time (to be used outside a function body).
#define ASSERT_VERSION_IMPLEMENTATION(comp, versionpart, requiredV, actualV, expression) \
        static_assert(expression, XSTR(Actual_##comp##_##versionpart##_VERSION_##actualV##_does_not_match_required_version_##requiredV));

#define ASSERT_VERSION_EVALUATOR(comp, versionpart, requiredV, actualV, expression) \
        ASSERT_VERSION_IMPLEMENTATION(comp, versionpart, requiredV, actualV, expression)

#define ASSERT_VERSION(required, comparator, comp, versionpart) \
    ASSERT_VERSION_EVALUATOR(comp, versionpart, required, comp ##_VERSION_## versionpart, required comparator comp ##_VERSION_## versionpart)

/** Checks at compile-time that an interface version provided by component
 * 'comp' is identical to the required version of a component using this interface.
 * Note! This check is useful for components that IMPLEMENT a particular
 * interface to be alerted of changes to the interface which are likely to
 * require adaptations in the implementation. */
#define MC_CHECK_VERSION_EQUALS(comp, major, minor) \
    ASSERT_VERSION(major, ==, comp, MAJOR) \
    ASSERT_VERSION(minor, ==, comp, MINOR)

/** Checks at compile-time that an interface version provided by component 'comp' meets the
 * required version of a component using this interface. */
#define MC_CHECK_VERSION_STATIC(comp, majorRequired, minorRequired) \
    ASSERT_VERSION(majorRequired, ==, comp, MAJOR) \
    ASSERT_VERSION(minorRequired, <=, comp, MINOR)

/** Version check helper macro for an interface consumer against an interface
 * provider.
 * @param comp          Name of Interface to check.
 * @param majorRequired Required major version of interface provider.
 * @param minorRequired Required minor version of interface provider.
 * Performs a compile-time interface version check that comp_VERSION_MAJOR
 * equals majorRequired and that comp_VERSION_MINOR is at least minorRequired.
 * On success, compilation goes through.
 * On error, compilation breaks, telling the component that did not match in the
 * error message.
 *
 * Additionally, a function is created:
 *
 * checkVersionOk##component(uint32_t version, char** errmsg)
 *
 * Compares version against majorRequired and minorRequired.
 * Additionally, it creates a message string that can be printed out using printf("%s", errmsg).
 * It returns either only the actual version, or on mismatch, actual and required version.
 *
 * @param version[in] component version as returned by layer-specific getVersion.
 * @param errmsg[out] a message string that contains a log.
 *
 */
#if !defined(NDEBUG)
#if !defined(TRUSTLET)

#define MC_CHECK_VERSION(comp, majorRequired, minorRequired) \
	MC_CHECK_VERSION_STATIC(comp, majorRequired, minorRequired) \
    static uint32_t checkVersionOk##comp(uint32_t version, char** errmsg) { \
        static char msgBuf[100]; \
        unsigned int major = MC_GET_MAJOR_VERSION(version); \
        unsigned int minor = MC_GET_MINOR_VERSION(version); \
        uint32_t ret = 0; \
        *errmsg = msgBuf; \
        /* Check equality and superiority separately to avoid warning if minor == 0 */ \
        if ((major == (majorRequired)) && ((minor == (minorRequired)) || (minor > (minorRequired)))) { \
            snprintf(msgBuf, sizeof(msgBuf), \
                #comp " version is %u.%u", major, minor); \
            ret = 1; \
        } else { \
            snprintf(msgBuf, sizeof(msgBuf), \
                #comp " version error. Got: %u.%u, want >= %u.%u", major, minor, (unsigned int)(majorRequired), (unsigned int)(minorRequired)); \
        } \
        msgBuf[sizeof(msgBuf) - 1] = '\0'; \
        return ret; \
    }
#else /* TRUSTLET */
#define MC_CHECK_VERSION(comp, majorRequired, minorRequired) \
    MC_CHECK_VERSION_STATIC(comp, majorRequired, minorRequired) \
    static uint32_t checkVersionOk##comp(uint32_t version, char** errmsg) { \
        static char msgBuf[100]; \
        unsigned int major = MC_GET_MAJOR_VERSION(version); \
        unsigned int minor = MC_GET_MINOR_VERSION(version); \
        *errmsg = msgBuf; \
        if ((major == majorRequired) && (minor >= minorRequired)) { \
            tlDbgPrintf(#comp " version is %u.%u", major, minor); \
            return 1; \
        } else { \
            tlDbgPrintf( \
                #comp " version error. Got: %u.%u, want >= %u.%u", major, minor, (unsigned int)majorRequired, (unsigned int)minorRequired); \
        } \
        return 0; \
    }
#endif /* TRUSTLET */
#else /* NDEBUG */
#define MC_CHECK_VERSION(comp, majorRequired, minorRequired) \
    MC_CHECK_VERSION_STATIC(comp, majorRequired, minorRequired) \
    static uint32_t checkVersionOk##comp(uint32_t version, char** errmsg) { \
        static char msgBuf[100]; \
        uint32_t major = MC_GET_MAJOR_VERSION(version); \
        uint32_t minor = MC_GET_MINOR_VERSION(version); \
        *errmsg = msgBuf; \
        /* Check equality and superiority separately to avoid warning if minor == 0 */ \
        if ((major == majorRequired) && ((minor == minorRequired) || (minor > minorRequired))) { \
            return 1; \
        }; \
        return 0; \
    }
#endif /* NDEBUG */

#endif // MCVERSIONHELPER_H
