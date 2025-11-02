
/*
 * 
 * Call Blocking in Circuit Switched Networks
 * 
 * Copyright (C) 2014 Terence D. Todd
 * Hamilton, Ontario, CANADA
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 * 
 */

/*******************************************************************************/

#include <stdio.h>
#include <time.h>
#include <math.h>
#include "simparameters.h"
#include "main.h"
#include "output.h"
#include "call_duration.h"

/*******************************************************************************/

void output_progress_msg_to_screen(Simulation_Run_Ptr this_simulation_run)
{
  // double percentagedone;
  // Simulation_Run_Data_Ptr sim_data;

  // sim_data = (Simulation_Run_Data_Ptr) simulation_run_data(this_simulation_run);

  // sim_data->blip_counter++;

  // if((sim_data->blip_counter >= BLIPRATE)
  //    ||
  //    (sim_data->number_of_calls_processed >= RUNLENGTH))
  //   {
  //     sim_data->blip_counter = 0;

  //     percentagedone =
	// 100 * (double) sim_data->number_of_calls_processed/RUNLENGTH;

  //     printf("%3.0f%% ", percentagedone);

  //     printf("Call Count = %ld \r", sim_data->number_of_calls_processed);

  //     fflush(stdout);
  //   }
}

/*******************************************************************************/

void output_results(Simulation_Run_Ptr this_simulation_run)
{
  double xmtted_fraction;
  Simulation_Run_Data_Ptr sim_data;

  sim_data = (Simulation_Run_Data_Ptr) simulation_run_data(this_simulation_run);

  printf("\n");

  printf("random seed = %d \n", sim_data->random_seed);
  printf("call arrival count = %ld \n", sim_data->call_arrival_count);
  printf("blocked call count = %ld \n", sim_data->blocked_call_count);

  xmtted_fraction = (double) (sim_data->call_arrival_count -
      sim_data->blocked_call_count)/sim_data->call_arrival_count;

  printf("BLOCKING PROBABILITY = %.5f (Service fraction = %.5f)\n",
	 1-xmtted_fraction, xmtted_fraction);
  
  ExcelOpener("L3Part2_Data.csv");
  ExcelNewData(this_simulation_run);

  printf("\n");
}


//EXCEL IMPLEMENTATION FOR PART 2

static FILE* LAB3_EXCEL = NULL;

void ExcelInit(const char* file) {
  time_t now = time(NULL);              //set time to now
  struct tm *t = localtime(&now);       //set tm(ms, s, min, hours, etc) strct to local time 
  LAB3_EXCEL = fopen(file, "a");        //use LAB3EXCEL(FILE data type) to fopen "file.csv" in append mode, so we can keep adding
  fprintf(LAB3_EXCEL, "NEW TRIAL: %02d: %02d: %02d \n Random Seed, Offered Load, Number of Trunks, Blocking Probability, Succesful Probaility \n", t->tm_hour, t->tm_min, t->tm_sec);  //to add time and titles for data

}

void ExcelOpener(const char* file) {
  LAB3_EXCEL = fopen(file, "a");

}

void ExcelNewData(Simulation_Run_Ptr this_simulation_run) {

  Simulation_Run_Data_Ptr sim_data;
  sim_data = (Simulation_Run_Data_Ptr) simulation_run_data(this_simulation_run);


  double xmtted_fraction = (double) (sim_data->call_arrival_count - sim_data->blocked_call_count) / sim_data->call_arrival_count;
  int offered_load = (int)(sim_data->arrival_rate * MEAN_CALL_DURATION); //better to keep (int) outside the loop to keep as much precision as possible until last moment
  double blocking_prob = (double)(1-xmtted_fraction); // delayed probability

  
  double Pw = ((pow(offered_load, sim_data->number_of_channels)) / factorial(sim_data->number_of_channels)) /
              ( ((pow(offered_load, sim_data->number_of_channels)) / factorial(sim_data->number_of_channels)) + 
                  (((1 - (offered_load / sim_data->number_of_channels)) ) * (Pw_series_sum(offered_load, sim_data->number_of_channels))) );

  printf("%.5f\n", sim_data->accumulated_wait_time / sim_data->number_of_calls_processed);
  printf("%.5f\n", (double) sim_data->waited_under_count / sim_data->number_of_calls_processed);

  printf("OFFERED LOAD: %d\n", offered_load);
  printf("NUMBER OF TRUNKS: %d\n", sim_data->number_of_channels);
  printf("Pw: %.5f\n", Pw);
  fprintf(LAB3_EXCEL, "%d,%d,%d,%.5f,%.5f,%.5f \n", sim_data->random_seed, offered_load, sim_data->number_of_channels, blocking_prob, xmtted_fraction, Pw); // no spaces if you want to use pandas later for data management
  fflush(LAB3_EXCEL);
}

void ExcelClose() { 
  fflush(LAB3_EXCEL);
  fclose(LAB3_EXCEL);

}


double factorial(int n) {
  double start = 1.0;
  for(int i = 1; i<= n; i++) {
    start *= i;
  }
  return start;
}


double Pw_series_sum(int N, double A) {
  double sum = 0;
  for(int i = 0; i < N; i++) {
    sum += (pow(A, i)) / (i);
  }
  return sum;
}