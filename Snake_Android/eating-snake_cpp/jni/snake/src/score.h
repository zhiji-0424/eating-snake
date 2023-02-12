#ifndef SCORE_H
#define SCORE_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

typedef struct {
    size_t score, highest;
} score_t;

void set_score_path(const char* dir_path);
void set_score(size_t n);
void set_highest(size_t n);
void score_rmall(void);
void score_add(size_t n);
void score_update(void);
void score_read(void);
void score_write(void);

score_t get_score(void);

#ifdef __cplusplus
}
#endif
#endif