
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

#ifndef _SIMPARAMETERS_H_
#define _SIMPARAMETERS_H_

/*******************************************************************************/

#define Call_ARRIVALRATES 20   /* calls/minute */
#define MEAN_CALL_DURATION 1 /* minutes */
#define RUNLENGTH 5e6 /* number of successful calls */
#define BLIPRATE 1e3
#define NUMBERS_OF_CHANNELS 25
#define wt_time 5

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 22, 333, 400381481, 400343389
/*******************************************************************************/

#define EXCEL_FILE "L3Part4_Data.csv"


#endif /* simparameters.h */




