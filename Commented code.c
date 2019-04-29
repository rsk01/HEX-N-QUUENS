//Hex Queens with matrix

//General Header Files
#include <stdio.h>
#include <stdlib.h>
//Header file for mathematical stuff
#include <math.h>
//Header file used specifically only for the delay function
#include <time.h>
//Defining the variable with ANSII value of red color which almost acts like constant
#define ANSI_COLOR_RED     "\x1b[31m"

//Global variable storing the number of solutions
int solutions = 0;

//Delaying the time as per the needed amount. Has concepts of time headerfile.... Learn that for safety.
void delay(int);

/*Function to print Board. Your Hex Board is not hexagonal in real time. It is in a normal matrix format
but you will allocate extra space and provide values in such a way that it is represented as a hexagon
Ex : It is a normal matrix of 1's and 0's where 0's are not considered and only 1's are considered
 	 which form the hexagon

				01010
				10101
				01010

So the formula for your matrix will be : m x n
where,
	m = (number of cells * 2) - 1
	n = (m * 2) - 1
*/
void printBoard(int **, int);

//Creating your hexboard with the same concept as specified above. This function will have memory allocation
int ** createBoard(int);

//Populating the board is basically setting the 1's and 0's to make your matrix look like a hexagon
int ** populateBoard(int **, int);

//Function to check the immediate top left hex cell
int checkTopLeft(int **, int, int, int);

//Function to check the immediate top right hex cell
int checkTopRight(int **, int, int, int);

//Function to check if the current cell is safe for the queen to be placed
int isSafe(int **, int, int, int);


int fillQueen(int **, int, int);


int ** nQueen(int **, int);

//Main function
int main()
{
	//2-D dynamic matrix which will act as your board, intialized to NULL
	int **board = NULL;
	//Side denotes the number of cells each side of the hex board
	int side;

	//Taking side input and 'cls' to clear the screen
	printf("Enter side length\n");
	scanf("%d", &side);
	system("cls");

	//Basic function calls to create board and populate it with hex 1's and 2's and storing the board
	board = createBoard(side);
	board = populateBoard(board, side);


	board = nQueen(board, side);

	//Printing the global variable which contains the number of solutions value
	printf("\nSolutions = %d", solutions);

	return 0;
}

//This fucntion shall delay your execution with the amount of time that you pass for number_of_seconds
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not acheived
    while (clock() < start_time + milli_seconds)
        ;
}

int ** createBoard(int side)
{
	//Initializations
	int **board = NULL;
	int i, j, centre;

	//Calculating your centre with respect to the number of cells on each side
	centre = (side * 2) - 1;
	//Allocating the number of rows first
	board = (int **)malloc(sizeof(int *) * centre);

	//Allocating the number of columns
	for(i = 0; i < centre; i++)
		board[i] = (int *)malloc(sizeof(int) * ((centre * 2) - 1));

	//Initially assigning all the cells of the matrix(not board) to zeros
	for(i = 0; i < centre; i++)
	{
		for(j = 0; j < (centre * 2) - 1; j++)
		{
			board[i][j] = 0;
		}
	}

	return board;
}


int ** populateBoard(int **board, int side)
{
	//Initializations
	int i, j, k, centre;

	//Calculating your centre with respect to the number of cells on each side
	centre = (side * 2) - 1;

	//The below code is used to set the cells with 1 if they are included in the hex and 0 otherwise
	board[centre / 2][0] = 1;

	//This loop is used to set the centre hexagonal line cells to 1's
	/*Ex:
		00000
		10101
		00000
	*/
	for(i = centre / 2, j = 1; j < (centre * 2) - 1; j++)
	{
		if(board[i][j - 1] == 1)
		{
			board[i][j] = 0;
		}
		else
		{
			board[i][j] = 1;
		}
	}

	//This loop is used to set the upper hexagonal cells to 1's
	/*Ex:
		01010
		10101
		00000
	*/
	for(i = (centre / 2) - 1; i >= 0; i--)
	{
		j = 0;

		while(board[i + 1][j] == 0)
		{
			j++;
		}

		for(k = j + 1; j < (centre * 2) - 1 - k; j++)
		{
			if(board[i + 1][j] == 1 && board[i][j] != -1)
			{
				board[i][j] = 0;
			}
			else if (board[i][j] != -1)
			{
				board[i][j] = 1;
			}
		}
	}

	//This loop is used to set the lower hexagonal cells to 1's
	/*Ex:
		01010
		10101
		01010
	*/
	for(i = (centre / 2) + 1; i < centre; i++)
	{
		j = 0;

		while(board[i - 1][j] == 0)
		{
			j++;
		}

		for(k = j + 1; j < (centre * 2) - 1 - k; j++)
		{
			if(board[i - 1][j] == 1 && board[i][j] != -1)
			{
				board[i][j] = 0;
			}
			else if(board[i][j] != -1)
			{
				board[i][j] = 1;
			}
		}
	}

	return board;
}

//This function is used to print the board
void printBoard(int **board, int side)
{
	//Initializations
	int i, j, centre;

	//Calculating your centre with respect to the number of cells on each side
	centre = (side * 2) - 1;

	//Loop to print the board, as we know it is in matrix format we have to take care of the hex shape
	for(i = 0; i < centre; i++)
	{
		for(j = 0; j < (centre * 2) - 1; j++)
		{
			//Printing space for the value 0 which are the cells of matrix that are not part of the hexagon
			if(board[i][j] == 0)
			{
				printf("  ");
			}
			//Printing 0 for the value 1 which are the cells of matrix that are part of the hexagon
			else if(board[i][j] == 1)
			{
				printf("0 ");
			}
			//Printing 1 for any other value(2 in this case) which are the cells containing the queen
			else
			{
				printf("1 ");
			}
		}

		printf("\n");
	}
}

int checkTopLeft(int **board, int side, int i, int j)
{
	//Initializations
	int m, n;

	//Loop used to navigate towards the left upper side diagonally till end of board
	for(m = i - 1, n = j - 1; m >= 0 && n >= 0; m--, n--)
	{
		//Immediately returning the value 0 to specify that it is not safe to place the queen
		if(board[m][n] == 2)
		{
			return 0;
		}
	}

	//Returning 1 if no queen is encountered
	return 1;
}

int checkTopRight(int **board, int side, int i, int j)
{
	//Initializations
	int m, n, centre;

	//Calculating your centre with respect to the number of cells on each side
	centre = (side * 2) - 1;

	//Loop used to navigate towards the right upper side diagonally till end of the board
	for(m = i - 1, n = j + 1; m >= 0 && n < (centre * 2) - 1; m--, n++)
	{
		//Immediately returning the value 0 to specify that it is not safe to place the queen
		if(board[m][n] == 2)
		{
			return 0;
		}
	}

	return 1;
}

int isSafe(int **board, int side, int i, int j)
{
	//Because we are placing the queen from top to bottom order we will only have to check the upper half
	//Hence we traverse towards both the top sides diagonally
	if(checkTopLeft(board, side, i, j) == 1 && checkTopRight(board, side, i, j) == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int fillQueen(int **board, int side, int index)
{
	//Initializations
	int j, centre, result;

	//Calculating your centre with respect to the number of cells on each side
	centre = (side * 2) - 1;

	//Increasing the solutions when current row(index value) reaches to the last row(centre value)
	if(index == centre)
	{
		solutions++;
		return 1;
	}

	j = 0;

	//Navigating through the columns for the current row inorder to place the queen
	while(j < (centre * 2) - 1)
	{
		//Checking if the particular matrix cell is not part of the hex board
		if(board[index][j] == -1 || board[index][j] == 0)
		{

		}
		//Executing the else part if the cell is part of the hex
		else
		{
			//Checking if the current cell is safe to place the queen ignoring the if bloack is it is not
			if(isSafe(board, side, index, j) == 1)
			{
				//Placing the queen by setting cell value to 2
				board[index][j] = 2;
				//Filling queen for the next row
				result = fillQueen(board, side, (index + 1));

				//If the hex board was filled then result value is 1 which means the solution is complete
				if(result == 1)
				{
					//After backtracking to the starting function call,repeating procedure for other solutions
					if(index == 0 && j < (centre * 2) - 1)
					{
						printf("Solution : %d  \n\n", solutions);
						printBoard(board, side);
						/*while(!kbhit())
						{

						}*/
						delay(3);
						//system("cls");
						printf("\n\n");
						board = createBoard(side);
						board = populateBoard(board, side);
					}
					//If the solution is mid way and has not reached the starting of the stack trace(backtracking)
					else
					{
						return 1;
					}
				}
				//Assigning the cell with 1 if the queen is not placable
				else
				{
					board[index][j] = 1;
				}
			}
		}
		//Moving to next column
		j++;
	}

	return 0;
}

//Driver funtion to run the fill queen
int ** nQueen(int **board, int side)
{
	int result;

	result = fillQueen(board, side, 0);

	return board;
}
