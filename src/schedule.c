#include <pebble.h>
#include "schedule.h"

void schedule_set(struct schedule *sched, char *gd, char *gt, char *op, bool h, int u, int t) {
  strncpy(sched->game_date, gd, strlen(gd));
  strncpy(sched->game_time, gt, strlen(gt));
  strncpy(sched->opponent, op, strlen(op));
  sched->home = h;
  sched->us = u;
  sched->them = t;
}

void set_schedule(struct schedule season[]) {
 int game = 0;
 schedule_set(&season[game++], "20150913","1300","Browns",   true,  31, 10);
 schedule_set(&season[game++], "20150921","1300","Colts",    false, 20, 7);
 schedule_set(&season[game++], "20150927","1300","Eagles",   true,  17, 24);
 schedule_set(&season[game++], "20151018","1300","Redskins", true,  34, 20);
 schedule_set(&season[game++], "20151025","1300","Patriots", false, 23, 30);
 schedule_set(&season[game++], "20151108","1300","Jaguars",  true,  28, 23);
 schedule_set(&season[game++], "20151112","1300","Bills",    true,  17, 22);
 schedule_set(&season[game++], "20151122","1300","Texans",   false, 17, 24);
 schedule_set(&season[game++], "20151129","1300","Dolphins", true,  38, 20);
 schedule_set(&season[game++], "20151206","1300","Giants",   false, 23, 20);
 schedule_set(&season[game++], "20151213","1300","Titans",   true,  30, 8);
 schedule_set(&season[game++], "20151219","1300","Cowboys",  false, 19, 16);
 schedule_set(&season[game++], "20151227","1300","Patriots", true,  26, 20);
 schedule_set(&season[game++], "20160103","1300","Bills",    true,  0, 0);
}

bool game_time(struct schedule season[], int season_size, char *date_check, char *time_check) {
  for(int x = 0; x < season_size; x++) {
    if((strcmp(date_check, season[x].game_date) == 0) && (strcmp(time_check, season[x].game_time) == 0)) {
      return true;
    }
  }
  return false;
}