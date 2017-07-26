#include <time.h>
#include "game.h"

#define SCORES_FILENAME "scores.txt"
#define SCORES_FORMAT   "%d-%02d-%02d %02d:%02d:%02d\tTime: %02li:%02li\tLevel: %3u\tScore: %9u\tLines: %3u\n"

static void save_score(const char *fname, game *game, time_t duration);


int main(int argc, char **argv)
{
        time_t start, end;

        game *game = init_game(); 
        do {
                start = time(NULL);
                run_game(game);
                end = time(NULL);

                save_score(SCORES_FILENAME, game, end - start);

        } while (game_over(game) == NEW_GAME);

        free_game(game);
}

void save_score(const char *fname, game *game, time_t duration)
{
        FILE *fp;
        time_t t;
        struct tm tm;

        if ((fp = fopen(fname, "a")) == NULL) {
                return;
        }

        t = time(NULL);
        tm = *localtime(&t);

        fprintf(fp, SCORES_FORMAT, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                tm.tm_hour, tm.tm_min, tm.tm_sec,
                duration / 60, duration % 60,
                game->level, game->score, game->lines);

        fclose(fp);
}
