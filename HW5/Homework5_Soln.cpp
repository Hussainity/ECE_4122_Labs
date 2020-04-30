/* 
Author: Hussain Miyaziwala 
Class: ECE4122  
Last Date Modified: 11/5/19

Description:

HW 5 Solution: Controlling objects with kinematics using MPI

*/


#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <math.h>
#include <string.h>


#define BUZZY 0
#define ACTIVE 1
#define DOCKED 2
#define DESTROYED 0
#define X 0
#define Y 1
#define Z 2
#define MASS 10000

using namespace std;

bool validVelToDock(double j[3], double b[3]);
double getDistance(double j1[3], double j2[3]);
double thrustCiel(double thrustApply, double MAX);

int main(int argc, char* argv[])
{
	int	taskid,	        /* task ID */
		numtasks,       /* number of tasks */
		rc;             

	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

	//printf("MPI task %d has started...\n", taskid);

	//------------------------------------------------------

	int THRUST_MAX, TIME;
	double init[8][7];

	double initUni[56];

	if (taskid == BUZZY)
	{

		//READ IN FILE, POPULATE INIT MATRIX
		ifstream inFile;
		inFile.open("in.dat");

		inFile >> TIME;
		TIME = 10;
		inFile >> THRUST_MAX;

		for (int r = 0; r < 56; r++)
		{
			inFile >> initUni[r];
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(initUni, 56, MPI_DOUBLE, BUZZY, MPI_COMM_WORLD); // send everyone initial statuses
	MPI_Bcast(&THRUST_MAX, 1, MPI_INT, BUZZY, MPI_COMM_WORLD); // send everyone the max thrust
	MPI_Bcast(&TIME, 1, MPI_INT, BUZZY, MPI_COMM_WORLD);	   // send everyone the time to run
	MPI_Barrier(MPI_COMM_WORLD);

	//ROLL 1D array into a 2D Matrix
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 7; col++)
		{
			init[row][col] = initUni[row * 7 + col];
		}
	}


	//GRAB INITIAL VALUES FOR EACH JACKET
	double F[3], L[3], Vel, stat, V[3];

	stat = ACTIVE;

	L[X] = init[taskid][0];
	L[Y] = init[taskid][1];
	L[Z] = init[taskid][2];

	Vel = init[taskid][3];

	V[X] = init[taskid][4] * Vel;
	V[Y] = init[taskid][5] * Vel;
	V[Z] = init[taskid][6] * Vel;

	double D[3]; // Destination values
	double A[3]; // Acceleration mid values

	F[X] = F[Y] = F[Z] = 0;


	//------------------------------------------------------
	//TIME LOOP STARTS

	for (int t = 0; t < TIME; t++)
	{

		// PACK UP DATA TO SEND TO BUZZY	
		double sendarray[10];
		sendarray[0] = L[X]; sendarray[1] = L[Y]; sendarray[2] = L[Z];
		sendarray[3] = F[X]; sendarray[4] = F[Y]; sendarray[5] = F[Z];
		sendarray[6] = stat;
		sendarray[7] = V[X]; sendarray[8] = V[Y]; sendarray[9] = V[Z];
		double recarray[10 * numtasks];

		//------------------ PARALLEL CODE ----------------------------
		MPI_Allgather(sendarray, 10, MPI_DOUBLE, recarray, 10, MPI_DOUBLE, MPI_COMM_WORLD);
		//------------------ PARALLEL CODE ----------------------------

		if (recarray[6] != ACTIVE)
		{
			//cout << "BUZZY no longer active - terminate!" << endl;
			break;	// if BUZZY is no longer active, break out of loop.
		}

		//BUZZY CHECKS STATUS OF ALL SHIPS
		if (taskid == BUZZY)
		{
			bool abort = true;
			for (int i = 0; i < numtasks; i++)
			{
				abort = abort && (recarray[i * 10 + 6] != ACTIVE);
			}
			if (abort)
			{
				stat = -1;
				continue;
			}
		}

		// BUZZY PRINTS INFORMATION OF EACH JACKET
		if (taskid == BUZZY)
		{

			for (int i = 0; i < numtasks; i++)
			{
				cout << i << ", " << setprecision(0) << fixed << recarray[i * 10 + 6];

				for (int j = 0; j < 6; j++)
				{
					cout << ", " << setprecision(6) << scientific << recarray[i * 10 + j];
				}
				cout << endl;
			}
			//cout << endl;
		}

		if (taskid != BUZZY && stat == ACTIVE)
		{

			// Calculate new force based on direction

			// nobody move! -> no collisions, but likely wont dock.
			F[X] = thrustCiel(-1 * V[X] * MASS, THRUST_MAX); // accelerate in opposite direction of vel
			F[Y] = thrustCiel(-1 * V[Y] * MASS, THRUST_MAX);
			F[Z] = thrustCiel(-1 * V[Z] * MASS, THRUST_MAX);

			//Check for collisions for next iteration
			double colTarget[3];
			for (int i = 0; i < numtasks; i++)
			{
				if (i == taskid || i == BUZZY || recarray[i * 10 + 6] != ACTIVE)
				{
					continue;
				}
				for (int j = 0; j < 3; j++)
				{
					colTarget[j] = recarray[i * 10 + j];
				}
				if (getDistance(L, colTarget) < 250)
				{
					stat = DESTROYED;
					F[0] = F[1] = F[2] = 0;

				}
			}

			//Check for docking status for next iteration
			double buzzLoc[3];
			for (int i = 0; i < 3; i++)

			{
				buzzLoc[i] = recarray[i]; // capture Buzz location
			}

			// if within range
			if (getDistance(L, buzzLoc) < 50)
			{
				double buzzVel[3];
				for (int i = 0; i < 3; i++)
				{
					buzzVel[i] = recarray[i + 7]; // capture Buzz velocity (offset of 7)
				}
				if (validVelToDock(V, buzzVel))
				{
					stat = DOCKED;
					F[0] = F[1] = F[2] = 0;
				}
				else
				{
					stat = DESTROYED;
					F[0] = F[1] = F[2] = 0;
				}
			}
		}

		if (stat == DOCKED)
		{
			memcpy(L, recarray, sizeof(L)); // if docked, set location to that of buzzy's
		}

		//Update kinematics collectively:
		if (stat == ACTIVE)
		{
			for (int i = 0; i < 3; i++)
			{
				A[i] = F[i] / MASS;					// A = F/m : (acceleration)
				L[i] = L[i] + V[i] + 0.5 * A[i];	// L = L0 + V0 + 1/2A (location)
				V[i] = V[i] + A[i];					// V = V0 + A : (velocity)
			}
		}
	}

	MPI_Finalize();
	return 0;
}

// Description: Ensures that thrust applied is within limits
// Input: Thrust to apply and max thrust limit
// Output: Thrust to be actually applied
double thrustCiel(double thrustApply, double MAX)
{
	if (thrustApply >= 0)
	{
		return (thrustApply > MAX ? MAX : thrustApply);
	}
	else
	{
		return (thrustApply < -1 * MAX ? -1 * MAX : thrustApply);
	}
}

// Description: Gets distance between two points
// Input: Two points represented in length 3 arrays
// Output: Double distance
double getDistance(double j1[3], double j2[3])
{
	double sum = 0;
	for (int i = 0; i < 3; i++)
	{
		sum += pow(j2[i] - j1[i], 2);
	}
	return pow(sum, 0.5);
}

// Description: Ensures that a jacket satisfies docking conditions
// Inputs: Velocities of a jacket and buzzy
// Output: Boolean of whether conditions are satsifeid
bool validVelToDock(double j[3], double b[3])
{
	// dot product 
	double dp = 0;
	for (int i = 0; i < 3; i++)
	{
		dp += j[i] * b[i];
	}

	//magnitudes
	double jMag, bMag;
	jMag = bMag = 0;
	for (int i = 0; i < 3; i++)
	{
		jMag += pow(j[i], 2);
		bMag += pow(b[i], 2);
	}

	jMag = pow(jMag, 0.5);
	bMag = pow(bMag, 0.5);

	//conditions
	bool ang, mag;
	ang = (dp / (jMag * bMag)) > 0.8; // is cosTheta > 0.8
	mag = jMag < 1.1 * bMag;          // is jacket's speed within 1.1 * of buzzy's

	return ang && mag;
}

