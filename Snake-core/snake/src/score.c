#include "score.h"

static score_t score = { 0, 0 };
#define len 100
static char score_file_path[len];


void set_score_path(const char* dir_path) {
    memset(score_file_path, '\0', len);
    strcpy(score_file_path, dir_path);
    strcat(score_file_path, "snake-score.dat");
}

void set_score(size_t n) {
    score.score = n;
}

void set_highest(size_t n) {
    score.highest = n;
}

void score_rmall() {
    score.score = 0;
    score.highest = 0;
    score_write();
}

void score_add(size_t n) {
    score.score += n;
}

void score_update() {
    if (score.score > score.highest) {
        score.highest = score.score;
    }
}

void score_read() {
    FILE* fp = fopen(score_file_path, "rb");
    if (fp) {
        if (0 == fread(&score.highest, sizeof(size_t), 1, fp))
            score.highest = 0;
        fclose(fp);
        return;
    }
    score.highest = 0;
}

void score_write() {
    FILE* fp = fopen(score_file_path, "wb");
    if (fp) {
        fwrite(&score.highest, sizeof(size_t), 1, fp);
        fclose(fp);
        return;
    }
}

score_t get_score() {
    return score;
}

