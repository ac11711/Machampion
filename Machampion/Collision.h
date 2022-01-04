#pragma once
#include "SDL.h"
#include "Vector2D.h"

//A buffer to shave down hitboxes for better collision - less means a bigger cushion
const static int s_buffer = 6;

//A function that defines a collision
static bool RectRect(SDL_Rect A, SDL_Rect B) {
	//A sliver of the height of A
	int aHBuf = A.h / s_buffer;
	//A sliver of the width of A
	int aWBuf = A.w / s_buffer;

	//A sliver of the height of B
	int bHBuf = B.h / s_buffer;
	//A sliver of the width of B
	int bWBuf = B.w / s_buffer;

	//If the bottom of A (Y + height of A) - (A height buffer) is   <=   the top of B (Y + B height buffer) : no collision
	if ((A.y + A.h) - aHBuf <= B.y + bHBuf) { return false; }

	/*
	0		  ->		50

			  Y
		  ---------
		  |       |
		X |   A   | W
		  |       |
		  ---------
			  H

	|
	V

			  Y
		  ---------
		  |       |
		X |   B   | W
		  |       |
		  ---------
			  H
	100
	*/

	//If the top of A (Y + A height buffer) is   >=   the bottom of B (Y + height of B) - (B height buffer) : no collision
	if (A.y + aHBuf >= (B.y + B.h) - bHBuf) { return false; }

	/*
	0		  ->		50

			  Y
		  ---------
		  |       |
		X |   B   | W
		  |       |
		  ---------
			  H

	|
	V

			  Y
		  ---------
		  |       |
		X |   A   | W
		  |       |
		  ---------
			  H
	100
	*/

	//If the right of A (X + width of A) - (A width buffer) is   <=   left of B (x) + (B width buffer) : no collision
	if ((A.x + A.w) - aWBuf <= B.x + bWBuf) { return false; }

	/*
	0		    		 ->						100

			  Y						Y
		  ---------				---------
		  |       |				|       |
	|	X |   A   | W		  X |   B   | W
	V	  |       |				|       |
		  ---------				---------
			  H						H

	50
	*/

	//If the left of A (x) + (A width buffer) is   >=   right of B (x + width of B) - (B width buffer) : no collision
	if (A.x + aWBuf >= (B.x + B.w) - bWBuf) { return false; }

	/*
	0		    		 ->						100

			  Y						Y
		  ---------				---------
		  |       |				|       |
	|	X |   B   | W		  X |   A   | W
	V	  |       |				|       |
		  ---------				---------
			  H						H

	50
	*/

	//Collision
	return true;
}

//A function that defines a precise collision
static bool RectRectPrecise(SDL_Rect A, SDL_Rect B) {
	// Set buffer
	int buffer = 10;

	//A sliver of the height of A
	int aHBuf = A.h / buffer;
	//A sliver of the width of A
	int aWBuf = A.w / buffer;

	//A sliver of the height of B
	int bHBuf = B.h / buffer;
	//A sliver of the width of B
	int bWBuf = B.w / buffer;

	//If the bottom of A (Y + height of A) - (A height buffer) is   <=   the top of B (Y + B height buffer) : no collision
	if ((A.y + A.h) - aHBuf <= B.y + bHBuf) { return false; }

	/*
	0		  ->		50

			  Y
		  ---------
		  |       |
		X |   A   | W
		  |       |
		  ---------
			  H

	|
	V

			  Y
		  ---------
		  |       |
		X |   B   | W
		  |       |
		  ---------
			  H
	100
	*/

	//If the top of A (Y + A height buffer) is   >=   the bottom of B (Y + height of B) - (B height buffer) : no collision
	if (A.y + aHBuf >= (B.y + B.h) - bHBuf) { return false; }

	/*
	0		  ->		50

			  Y
		  ---------
		  |       |
		X |   B   | W
		  |       |
		  ---------
			  H

	|
	V

			  Y
		  ---------
		  |       |
		X |   A   | W
		  |       |
		  ---------
			  H
	100
	*/

	//If the right of A (X + width of A) - (A width buffer) is   <=   left of B (x) + (B width buffer) : no collision
	if ((A.x + A.w) - aWBuf <= B.x + bWBuf) { return false; }

	/*
	0		    		 ->						100

			  Y						Y
		  ---------				---------
		  |       |				|       |
	|	X |   A   | W		  X |   B   | W
	V	  |       |				|       |
		  ---------				---------
			  H						H

	50
	*/

	//If the left of A (x) + (A width buffer) is   >=   right of B (x + width of B) - (B width buffer) : no collision
	if (A.x + aWBuf >= (B.x + B.w) - bWBuf) { return false; }

	/*
	0		    		 ->						100

			  Y						Y
		  ---------				---------
		  |       |				|       |
	|	X |   B   | W		  X |   A   | W
	V	  |       |				|       |
		  ---------				---------
			  H						H

	50
	*/

	//Collision
	return true;
}