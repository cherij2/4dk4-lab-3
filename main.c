
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "output.h"
#include "trace.h"
#include "simparameters.h"
#include "cleanup.h"
#include "call_arrival.h"
#include "main.h"

/*******************************************************************************/

int main(void)
{
  int i;
  int j = 0;
  int k = 0;
  int l = 0;

  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data; /* Simulation_Run_Data is defined in main.h. */

  /*
   * Get the list of random number generator seeds defined in simparameters.h.
   */

  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  unsigned NUM_TRUNKS[] = {NUM_TRUNKS_LIST, 0};
  unsigned ARRIVAL_RATES[] = {Call_ARRIVALRATE_LIST, 0};
  unsigned random_seed;
  unsigned num_trunks;
  unsigned arr;

  /*
   * Loop for each random number generator seed, doing a separate
   * simulation_run run for each.
   */

  /*
  10 random seeds
    15 trunks
      15 offered loads/ trunk



  */

  

  while ((random_seed = RANDOM_SEEDS[j++]) != 0)
  {
    

    while ((num_trunks = NUM_TRUNKS[k++]) != 0)
    {
      while ((arr = ARRIVAL_RATES[l++]) != 0)
      {
        /* Create a new simulation_run. This gives a clock and eventlist. */
        simulation_run = simulation_run_new();

        /* Add our data definitions to the simulation_run. */
        simulation_run_set_data(simulation_run, (void *)&data);

        /* Initialize our simulation_run data variables. */
        data.mean_call_duration = MEAN_CALL_DURATION;
        data.random_seed = random_seed;
        data.blip_counter = 0;
        data.call_arrival_count = 0;
        data.blocked_call_count = 0;
        data.number_of_calls_processed = 0;
        data.accumulated_call_time = 0.0;
        data.arrival_rate = arr;
        

        /* Create the channels. */
        data.channels = (Channel_Ptr *)xcalloc((int)num_trunks,             //changed from NUMBER_OF_CHANNELS to num_trunks for pt 2
                                               sizeof(Channel_Ptr));

        /* Initialize the channels. */
        for (i = 0; i < num_trunks; i++)
        {
          *(data.channels + i) = server_new();
        }

        /* Set the random number generator seed. */
        random_generator_initialize((unsigned)random_seed);

        /* Schedule the initial call arrival. */
        schedule_call_arrival_event(simulation_run,
                                    simulation_run_get_time(simulation_run) +
                                        exponential_generator((double)1 / data.arrival_rate));

        /* Execute events until we are finished. */
        while (data.number_of_calls_processed < RUNLENGTH)
        {
          simulation_run_execute_event(simulation_run);
        }

        /* Print out some results. */
        output_results(simulation_run);

        /* Clean up memory. */
        cleanup(simulation_run);
      }
    }
  }

  /* Pause before finishing. */
  getchar();
  return 0;
}
