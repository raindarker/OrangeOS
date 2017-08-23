/*
 * File Name: assert.h
 * Author: Sphantix
 * Mail: hangxu@antiy.cn
 * Created Time: Tue 22 Aug 2017 11:21:15 AM CST
 */

#ifndef __ORANGE_ASSERT_H__
#define __ORANGE_ASSERT_H__

#define ASSERT
#ifdef  ASSERT
void assertion_failure(char *exp, char *file, char *base_file, int line);
#define assert(exp)                                                 \
    if (exp);                                                       \
    else                                                            \
        assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)
#else
#define assert(exp)
#endif

#endif /* __ORANGE_ASSERT_H__ */
