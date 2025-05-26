/*
    The Fluent Programming Language
    -----------------------------------------------------
    This code is released under the GNU GPL v3 license.
    For more information, please visit:
    https://www.gnu.org/licenses/gpl-3.0.html
    -----------------------------------------------------
    Copyright (c) 2025 Rodrigo R. & All Fluent Contributors
    This program comes with ABSOLUTELY NO WARRANTY.
    For details type `fluent l`. This is free software,
    and you are welcome to redistribute it under certain
    conditions; type `fluent l -f` for details.
*/

//
// Created by rodrigo on 5/15/25.
//

#ifndef FLUENT_LIBC_STRING_BUILDER_H
#define FLUENT_LIBC_STRING_BUILDER_H

#if defined(__cplusplus)
extern "C"
{
#endif

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * \struct StringBuilder
 * \brief A simple dynamic string builder for efficient string concatenation.
 *
 * This structure manages a dynamically allocated character buffer,
 * its current length (idx), and its total capacity.
 */
typedef struct
{
    char *buf;        /**< Pointer to the character buffer. */
    size_t idx;       /**< Current index (length) of the string. */
    size_t capacity;  /**< Total capacity of the buffer. */
} StringBuilder;

/**
 * \brief Initializes a StringBuilder with a given capacity.
 *
 * Allocates memory for the buffer and sets the initial index to 0.
 * Exits the program if memory allocation fails.
 *
 * \param builder Pointer to the StringBuilder to initialize.
 * \param capacity Initial capacity of the buffer (excluding null terminator).
 */
inline void init_string_builder(StringBuilder *builder, const size_t capacity)
{
    builder->capacity = capacity;

    // Allocate a new buffer (+1 for null terminator)
    char *buf = (char *) malloc(sizeof(char) * (capacity + 1));

    // Check if we got NULL
    if (buf == NULL)
    {
#       ifndef _WIN32
            perror("malloc");
#       else
            puts("Runtime error: Out of memory");
#       endif
        exit(1);
    }

    builder->buf = buf;
    builder->idx = 0;
}

/**
 * \brief Finalizes the string and returns the internal buffer without copying.
 *
 * Appends a null terminator at the current index and returns the buffer.
 * The caller must not free the returned pointer directly.
 *
 * \param builder Pointer to the StringBuilder.
 * \return Pointer to the internal buffer (null-terminated string).
 */
inline char *collect_string_builder_no_copy(const StringBuilder *builder)
{
    // Add a null terminator
    builder->buf[builder->idx] = '\0';
    return builder->buf;
}

/**
 * \brief Finalizes the string and returns a newly allocated copy.
 *
 * Appends a null terminator and returns a heap-allocated copy of the string.
 * The caller is responsible for freeing the returned pointer.
 *
 * \param builder Pointer to the StringBuilder.
 * \return Newly allocated null-terminated string.
 */
inline char *collect_string_builder(const StringBuilder *builder)
{
    // Copy the string
    return strdup(collect_string_builder_no_copy(builder));
}

/**
 * \brief Appends a single character to the StringBuilder.
 *
 * Automatically reallocates the buffer if needed.
 * Exits the program if memory allocation fails.
 *
 * \param builder Pointer to the StringBuilder.
 * \param c Character to append.
 */
inline void write_char_string_builder(StringBuilder *builder, const char c)
{
    // Check if we have to reallocate the buffer
    if (builder->idx == builder->capacity)
    {
        // Double the capacity
        builder->capacity *= 2;

        // Reallocate immediately (+1 for null terminator)
        char *new_buffer = (char *) realloc(builder->buf, sizeof(char) * (builder->capacity + 1));

        // Check if we got NULL
        if (new_buffer == NULL)
        {
#       ifndef _WIN32
            perror("realloc");
#       else
            puts("Runtime error: Out of memory");
#       endif
            exit(1);
        }

        builder->buf = new_buffer;
    }

    // Write the character to the buffer
    builder->buf[builder->idx] = c;
    builder->idx++;
}

/**
 * \brief Appends a null-terminated string to the StringBuilder.
 *
 * Iterates over the input string and appends each character.
 *
 * \param builder Pointer to the StringBuilder.
 * \param str Null-terminated string to append.
 */
inline void write_string_builder(StringBuilder *builder, const char *str)
{
    // Iterate over the string
    while (*str != '\0')
    {
        // Write the character
        write_char_string_builder(builder, *str);
        // Move to the next character
        str++;
    }
}

/**
 * \brief Frees the memory used by the StringBuilder's buffer.
 *
 * After calling this function, the buffer pointer is set to NULL.
 *
 * \param builder Pointer to the StringBuilder to destroy.
 */
inline void destroy_string_builder(StringBuilder *builder)
{
    // Make sure the buffer is not NULL
    if (builder->buf == NULL) return;

    // Free the buffer and set it to NULL
    free(builder->buf);
    builder->buf = NULL;
}

/**
 * \brief Resets the StringBuilder to its initial state.
 *
 * Frees the current buffer and reinitializes it with the same capacity.
 *
 * \param builder Pointer to the StringBuilder to reset.
 */
inline void reset_string_builder(StringBuilder *builder)
{
    destroy_string_builder(builder);
    init_string_builder(builder, builder->capacity);
}

#if defined(__cplusplus)
}
#endif

#endif //FLUENT_LIBC_STRING_BUILDER_H
