#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * Generic status codes for data structure operations
 */
typedef enum {
    STATUS_OK = 0,
    STATUS_ERR_ALLOC = 1,
    STATUS_ERR_INVALID_ARGUMENT = 2,
    STATUS_ERR_DUPLICATE_KEY = 3,
    STATUS_ERR_KEY_NOT_FOUND = 4,
    STATUS_ERR_UNDERFLOW = 5,
    STATUS_ERR_OVERFLOW = 6,
    STATUS_ERR_EMPTY = 7,
    STATUS_ERR_FULL = 8,
    STATUS_ERR_UNKNOWN = 100
} STATUS;

/**
 * Status struct for insert operations
 */
typedef struct {
    bool isInserted;
    bool isDuplicate;
    bool isAllocFailed;
} InsertStatus;

/**
 * Status struct for delete operations
 */
typedef struct {
    bool isDeleted;
    bool isKeyNotFound;
} DeleteStatus;

#endif // COMMON_H
