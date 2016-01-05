#pragma once

#define SEASON_NUMBER_OF_GAMES 16

struct schedule {
  char game_date[9]; // YYYYMMDD
  char game_time[5]; //24MM
  bool home;
  char opponent[20];
  int us;
  int them;
};

void set_schedule(struct schedule season[]);

void schedule_set(struct schedule *sched, char *gd, char *gt, char *op, bool h, int u, int t);

bool game_time(struct schedule season[], int season_size, char *date_check, char *time_check);
