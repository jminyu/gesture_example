//------------------------------------------------------------------------------
// <copyright file="GRmodule.h" company="GIST MLV">
//     Copyright (c) GIST MLV Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------
#pragma once

#include <math.h>
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))  

const float prob_table[6][7] = {{0.1, 0.5, 0.1, 0.1, 0.1, 0.1, 0.1},// swipe right hand motion
					{0.1, 0.1, 0.5, 0.1, 0.1, 0.1, 0.1}, //swipe left hand motion
					{0.1, 0.1, 0.1, 0.5, 0.1, 0.1, 0.1}, //up hand motion
					{0.1, 0.1, 0.1, 0.1, 0.5, 0.1, 0.1},// down hand motion
					{0.1, 0.1, 0.1, 0.1, 0.1, 0.5, 0.1},//push hand motion
					{0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.5}, //pull hand motion
					};

int MDirection_feature(float head[], float left[], float right[]);
int Main_Axis(float distace[]);
int Recognition_DBN(int, int motion_chain[]);
float DBN(int,int,int motion_chain[]);

int MDirection_feature(float current_coord[], float past_coord[])
{
	// paramter structure
	// data[0] : x coordinate
	// data[1] : y coordinate
	// data[2] : z coordinate
	//
	
	int hand_direction = 0; //right hand direction

	int temp_direction = 0; //main component of right hand

	int direction = 0; //hand MDF


	//data[0] - distance for left hand motion [0]:x/[1]:y/[2]:z
	//data[1] - distance for right hand motion
	float distance[3] = {0.0,0.0,0.0}; 
	

	distance[0] = current_coord[0]-past_coord[0];//x
	distance[1] = current_coord[1]-past_coord[1];//y
	distance[2] = current_coord[2]-past_coord[2];//z

	direction = Main_Axis(distance);


	return direction;
	
	
}

int Main_Axis(float distance[])
{
	/*
	function for determining a main component of motion
	return 0 : main component is X-axis
	return 1 : main component is Y-axis
	return 2 : main component is Z-axis
	return -1 ; No motion
	*/
	float alpha = 0.02; //motion threshold - setting emperically
	float temp = 0.0;
	temp = MAX(MAX(abs(distance[0]),abs(distance[1])),abs(distance[2]));
	if(temp<=alpha){	
			return 0;// main component is 'No motion'
		}
	if(temp==abs(distance[0])){
		if(distance[0]>0)
		{
			return 1;//main component is +x-axis - hand right
		}else{
			return 2; //main component is -x-axis - hand left
		}
		
	}
	else if(temp==abs(distance[1])){
		if(distance[1]>0)
		{
			return 3;//main component is +y-axis - hand up
		}else{
			return 4; //main component is -y-axis - hand down
		}
	}
	else{
		if(distance[2]>0)
		{
			return 5;//main component is +z-axis - hand pull
		}else{
			return 6; //main component is -z-axis - hand push
		}
	}
}

//Gesture recognition requires a frames of resting state.
//Recognition_DBN is working on with a frame ofresting state.
int Recognition_DBN(int length, int motion_chain[])
{
	float prob_gestures[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	/*
	gesture code and gesturs
	swipe_right = 0;
	swipe_left = 1;
	up = 2;
	down = 3;
	pusl = 4;
	pull = 5
	*/
	float temp = 0.0;
	int count = 0;
	for(int i=0;i<6;i++){
		prob_gestures[i] = DBN(i,length,motion_chain);
		if(temp<prob_gestures[i]){
			temp = prob_gestures[i];
			count = i;
		}
		//probability of {right,left,up,down,push,pull}
	}
	switch(count){
		case 0:
			//Right gesture is recognized.
			return 1;
		case 1:
			//left gesture is recognized.
			return 2;
		case 2:
			//Up gesture is recognized.
			return 3;
		case 3:
			//Down gesture is recognized.
			return 4;
		case 4:
			//Push gesture is recognized.
			return 5;
		case 5:
			//Pull gesture is recognized.
			return 6;
		default:
			return 0; //error is occurs.
	}
			
	
}


float DBN(int gesturecode,int len,int motion_chain[])
{
	float temp = 0;
	int length = len;
	float prob = 1.0;
	float initial = float(1.0/6.0);//initial probability
	prob = prob*initial;
	float **transition = new float*[length];

	//building DBN
	for(int i=0;i<length;i++){//generation transition matrix
		transition[i] = new float[7];
		for(int j =0;j<7;j++){
			temp = prob_table[gesturecode][j];
			transition[i][j] = temp;//assign a probability value using gesture_code
		}
	}

	//computing probability value
	for(int i=0;i<length;i++){
		prob *= transition[i][motion_chain[i]];
	}
	return prob;

}
