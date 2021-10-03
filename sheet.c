#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/****************************/
/*                          */
/*     	 IZP projekt1     	*/
/*  	Praca s textom    	*/
/*	   Patrik Sehnoutek   	*/
/*        (xsehno01)        */
/*		  15.11.2020      	*/
/*							*/
/****************************/

/* The code contains one BONUS solution - split N D. */

#define MAX_CELL 101
#define MAX_LINE 10241 

enum functions {ACOL, AROW, DROW, IROW, DCOL, ICOL, TOLOW, TOUP, INT, ROUND,
				COPY, SWAP, MOVE, DCOLS, DROWS, ROWS, CSET, BEGINS, CONTAINS, 
				SPLIT};

/* 
 * Function: check_arguments_len
 * --------------------------------
 * 	Check if the length of arguments is less than or equal to 100.
 * 
 *  Returns: 0 (if is)
 * 			 -1 (if not)	 
 */
int check_arguments_len(char *argv[], int n)
{
	for (int i = 0; i < n; i++)
		if (strlen(argv[i]) > 100)
			return -1;	
	return 0;
}

/*
 * Function: find_function
 * -----------------------
 * 	Finds the correct function according to given argument.
 * 
 *  Returns: i (i - the position of the function in the array COMMANDS)
 * 			 -1 (if the function doesn't exists)	 
 */
int find_function(char argv[])
{
	const char commands[20][20] = {"acol", "arow", "drow", "irow", "dcol", "icol",
                             "tolower", "toupper", "int", "round", "copy", "swap", 
							 "move", "dcols", "drows", "rows", "cset",
                             "beginswith", "contains", "split"};
	
	for (int i = 0; i < 20; i++)
	{
		if (strcmp(argv, commands[i]) == 0)
			return i;
	}

	return -1;
}

/*
 * Function: check_rows
 * --------------------
 * 	Checks the validity of argument ROWS.
 *  
 *  Returns: true (if argument ROWS is valid)
 *  	 	 false (if not)
*/ 
bool check_rows(char *first, char *second, bool *last)
{
	if (strcmp(first, "-") != 0)
	{
		if (atoi(first) <= 0)
			return false;
		if (strcmp(second, "-") != 0)
			if ((atoi(second) <= 0) || (atoi(first) > atoi(second)))	
				return false;
	}
	else
	{
		if (strcmp(second, "-") != 0)
			return false;
	}

	if (strcmp(first, "-") == 0 && strcmp(second, "-") == 0)
		*last = true;
	
	return true;
}

/*
 * Function: check_arguments_validity
 * ----------------------------------
 * 	Checks the validity of given arguments.
 *  If argument is equal to arow increases the number of arow arguments. 
 * 
 *  Returns:  0 (if arguments are correct)
 * 			 -1 (if not) 	 
 */
int check_arguments_validity(char *argv[], int n, int *arowCount, bool *last)
{
	int pos;
    if (n > 3)
		pos = (strcmp(argv[1], "-d") == 0) ? 3 : 1;
	else 
		pos = n;
	
	for (int i = pos; i < n; i++)
	{
		int function = find_function(argv[i]);
		if (function == -1) return -1;
		
		if (function >= CSET)
		{  
			if (i + 2 >= n) return -1;
			if (atoi(argv[i+1]) <= 0) return -1;
			i += 2;
		}
		else if (function >= ROWS)
		{
			if (i + 2 >= n) return -1;
			if (!check_rows(argv[i+1], argv[i+2], last))
				return -1;
			i += 2;
		}
		else if (function >= COPY)
		{		
			if (i + 2 >= n) return -1;
			if (atoi(argv[i+1]) <= 0 || atoi(argv[i+2]) <= 0)
				return -1;
			if (function >= DCOLS && atoi(argv[i+1]) > atoi(argv[i+2]))
				return -1;
			i += 2;	
		}
		else if (function >= DROW)
		{		
			if (i + 1 >= n) return -1;
			if (atoi(argv[i+1]) <= 0) return -1;
			i++;
		}
		else if (function == AROW) 
			(*arowCount)++;
	}

	return 0;
}

/*
 * Function: check_delimiters
 * --------------------------
 * 	Removes duplicate delimiters.
 */
void check_delimiters(char *argv[], int argc, char *delimiters, int *n)
{	
	delimiters[0] = ' ';
	
	if (argc < 3)
		return; 
	
	if (strcmp(argv[1], "-d") == 0)
	{
		*n = strlen(argv[2]);
		for (int i = 0; i < *n; i++)
		{
			delimiters[i] = argv[2][i];
		}	
	}
	else 
		return;
	// remove duplicates
	for (int i = 0; i < *n; i++)
	{
		if (delimiters[i] != '\0')
			for (int j = i+1; j < *n; j++)
			{
				if (delimiters[i] == delimiters[j])
					delimiters[j] = '\0';
			}
	}
}

/* 
 * Function: check_col_count  
 * -------------------------
 * 	Checks if rows have the same number of columns.
 *
 * 	Returns: 0 
 * 			-1 (if not) 
*/
int check_col_count(int *colCount, int newColCount)
{
	if (*colCount == -1)
		*colCount = newColCount + 1;
	else if (*colCount != newColCount + 1)
		return -1;
	
	return 0;
}

/*
 * Function: split_line
 * --------------------
 * 	Splits a row using delimiters.
 */
int split_line(int *colCount, char *delimiters, int nDel, char *stdinLine, char newLine[][MAX_CELL])
{	
	int place = 0;
	int columns = 0;
	bool isDelimiter = false;
	int nStdin = strlen(stdinLine);

	for (int i = 0; i < nStdin; i++)
	{
		memset(newLine[i], '\0', MAX_CELL); // clear the cell
		for (int j = 0; j < nDel; j++)
		{
			if(stdinLine[i] == delimiters[j])
			{
				isDelimiter = true;
				columns++;
				place = 0;
			}	
		}
		// check the max length of the cell
		if (place == 100)
		{
			fprintf(stderr, "Exceeded character limit in the cell\n"); 
			return -1;
		}
		if (!isDelimiter && stdinLine[i]!= '\n')
		{	
			newLine[columns][place] = stdinLine[i];
			place++;
		}
		isDelimiter = false;
	}

	if (check_col_count(colCount, columns) == -1)
		return -1;
	return 0;
}

/*
 * Function: beginstwith_value
 * ---------------------------
 *  Checks if string in the column pos begins with the given value
 *
 *	returns: true  (if the column pos begins with the given value)
 *			 false (if doesn't or the position of the column is greater then
 *			 number of columns in a row)
 */
bool beginswith_value(char line[][MAX_CELL], int n, int pos, char value[])
{
	if (pos > n || strlen(value) > strlen(line[pos-1]))
		return false;

	int len = strlen(value);	
	for (int i = 0; i < len; i++)
	{
		if (value[i] != line[pos-1][i])
			return false;
	}
	
	return true;
}

/*
 * Function: contains_value
 * ------------------------
 *  Checks if string in the column POS contains the given substring
 *
 *	returns: true (if the column POS contains the given substring)
 *			 false (if doesn't or the position of the column is greater than
 *			 the number of columns in the row)
 */
bool contains_value(char line[][MAX_CELL], int n, int pos, char value[])
{
	if (pos > n || strlen(value) > strlen(line[pos-1]))
		return false;
	
	if (strstr(line[pos-1], value) == NULL)
		return false;

	return true;
}

/*
 * Function: insert_column
 * -----------------------
 * 	Inserts a column before the column POS.
 */
void insert_column(char line[][MAX_CELL], int *n, int oldN, int pos, bool last)
{	
	if (!last)
	{	
		// original table
		pos += *n - oldN;

		if (pos > *n || pos < 1)
			return;

		for (int i = *n-1 ; i >= pos-1; i--)
		{
			strcpy(line[i+1], line[i]);			
		}
		memset(line[pos-1], '\0', MAX_CELL);  // set empty column
	}
	*n = (*n) + 1;
}

/*
 * Function: delete_column
 * -------------------
 *  Deletes columns with indexes <START,END>, if START==END delete only column
 *  START. Decreases the number of columns.
 */
void delete_column(char line[][MAX_CELL], int *n, int oldN, char *s1, char *s2)
{
	int start = atoi(s1);
	int end = atoi(s2);
	
	// original table
	start += *n - oldN;
	end += *n - oldN;	

	if (start > *n)
		return;	

	if (end > *n)
		end = *n;	

	for (int i = start-1; i < *n; i++)
	{
		strcpy(line[i], line[i+1+(end-start)]);			
	}
	*n = (*n) - (end-start) - 1;
}

/*
 * Function: set_new_value
 * -----------------------
 *	Moves column with index pos1(pos2) before column with index pos2(pos1)
 */
void set_new_value(char line[][MAX_CELL], int n, char *s, char new_value[])
{
	int pos = atoi(s);	
	if (pos <= n)
	{
		strcpy(line[pos-1], new_value);			
	}			
}

/*
 * Function: to_lower
 * ------------------
 *  Changes all uppercase letters in the column POS to lowercase.	
 */
void to_lower(char line[][MAX_CELL], int n, int pos)
{
	if (pos <= n)
	{
		int i = 0;
		while(line[pos-1][i])
		{
			if (line[pos-1][i] >= 'A' && line[pos-1][i] <= 'Z') 	
				line[pos-1][i] = line[pos-1][i] + ('a' - 'A');
			i++;
		}
	}		
}

/*
 * Function: to_upper
 * ------------------
 *  Changes all lowercase letters in the column POS to uppercase.	
 */
void to_upper(char line[][MAX_CELL], int n, int pos)
{
	if (pos <= n)
	{
		int i = 0;
		while(line[pos-1][i])
		{
			if (line[pos-1][i] >= 'a' && line[pos-1][i] <= 'z') 	
				line[pos-1][i] = line[pos-1][i] + ('A' - 'a');
			i++;
		}
	}		
}

/*
 * Function: check_number
 * ----------------------
 *  Checks if the string can be converted to number.	
 */
int check_number(char *string)
{
	int commas_count = 0;
	int len = strlen(string);
	
	for (int i = 0; i < len; i++)
	{
		if (string[0] == '-' || string[0] == '+')
			continue;
		if (string[i] == '.')
			commas_count++;
		else if (string[i] < '0' || string[i] > '9')
			return -1;
	}	

	if (commas_count > 1)
		return -1;

	return 0;
}

/*
 * Function: round_number
 * ----------------------
 *  Rounds the number in the column POS.	
 */
void round_number(char line[][MAX_CELL], int n, int pos)
{
	if (pos > n)
		return;

	if (check_number(line[pos-1]) != -1)
	{
		float number = atof(line[pos-1]);
		memset(line[pos-1], '\0', MAX_CELL);
		sprintf(line[pos-1], "%0.0f", number);	
	}
}

/*
 * Function: trunc_number
 * ----------------------
 *  Removes the decimal part of the number in the column POS.	
 */
void trunc_number(char line[][MAX_CELL], int n, int pos)
{
	if (pos > n)
		return;

	if (check_number(line[pos-1]) != -1)
	{
		int number = atoi(line[pos-1]);
		memset(line[pos-1], '\0', MAX_CELL);
		sprintf(line[pos-1], "%d", number);
	}	
}

/*
 * Function: copy_value
 * --------------------
 *  Replace the content from the column pos2 with the content from
 *  the column pos1
 */
void copy_value(char line[][MAX_CELL], int n, char *s1, char *s2)
{
	int pos1 = atoi(s1);
	int pos2 = atoi(s2);

	if (pos1 > n || pos2 > n)
		return;

	strcpy(line[pos2-1], line[pos1-1]); 	
}

/*
 * Function: swap_values
 * ---------------------
 *	Swap columns with given indexes (pos1 and pos2)
 */
void swap_values(char line[][MAX_CELL], int n, char *s1, char *s2)
{
	int pos1 = atoi(s1);
	int pos2 = atoi(s2);

	if (pos1 > n || pos2 > n)
		return;
	
	char help[MAX_CELL];
	strcpy(help, line[pos1-1]);
	strcpy(line[pos1-1], line[pos2-1]);
	strcpy(line[pos2-1], help);
}

/*
 * Function: move_value
 * --------------------
 *	Move the column pos1(pos2) before the column pos2(pos1)
 */  
void move_value(char line[][MAX_CELL], int n, char *s1, char *s2)
{
	int pos1 = atoi(s1);
	int pos2 = atoi(s2);	

	if (pos1 > n || pos2 > n)
		return;
	
	char help[MAX_CELL];	
	strcpy(help, line[pos1-1]);

	if (pos1 < pos2)
	{	
		for (int i = pos1-1; i < pos2-1; i++)
		{	
			strcpy(line[i], line[i+1]);			
		}
		strcpy(line[pos2-2], help);
	}
	else if (pos1 > pos2)
	{
		for(int i = pos1-1; i > pos2-1; i--)
		{
			strcpy(line[i], line[i-1]);		
		}
		strcpy(line[pos2-1], help);
	}
}

/*
 * Function: insert_value
 * --------------------
 *	Inserts the given value to column POS and moves other columns to the right.
 */ 
void insert_value(char line[][MAX_CELL], int *n, int *pos, char value[MAX_CELL])
{
	for (int i = *n-1 ; i >= *pos-1; i--)
	{
		strcpy(line[i+1], line[i]);			
	}
	memset(line[*pos-1], '\0', MAX_CELL);  // set empty column

	int len = strlen(value);	
	for (int i = 0; i < len; i++)
	{
		line[*pos-1][i] = value[i];
	}
	
	*pos = (*pos) + 1;
	*n = (*n) + 1;
}

/*
 * Function: split_cell
 * --------------------
 *	Splits the column N according to the given delimiter DEL. Columns behind 
 *	column N moves to the right. 
 */  
void split_cell(char line[][MAX_CELL], int *n, char *s1, char *del)
{
	int pos = atoi(s1);
	if (pos > *n)
		return;

	int len = strlen(line[pos-1]);
	char help[MAX_CELL];
	int newPos = 0;
	int lastDel = 0;

	for (int i = 0; i < len; i++)
	{
		if (line[pos-1][i] != *del) 
		{
			help[newPos] = line[pos-1][i]; 
			newPos++;
		}
		else
		{
			insert_value(line, n, &pos, help);
			newPos = 0;
			lastDel = i + 1;
			memset(help, '\0', MAX_CELL);
		}
	}
	// Changes the content of the cell according to the last delimiter
	strcpy(help, line[pos-1]);
	memset(line[pos-1], '\0', MAX_CELL);
	strcpy(line[pos-1], help + lastDel);
}

/*
 * Function: print_table
 * ---------------------
 *	Prints a row, columns are separated by the first given delimiter.
 *	If EMPTY, prints only empty columns with delimiters.
 */  
void print_table(char line[][MAX_CELL], int n, char delimiter, bool empty)
{
	for (int i = 0; i < n; i++)
	{	if (!empty)
			fprintf(stdout, "%s", line[i]);
		if (i != n-1)		
			fprintf(stdout, "%c", delimiter);
	}
	
	fprintf(stdout, "%s", "\n");
}

/*
 * Function: change_table
 * ----------------------
 *  Changes the table row according to the given arguments. 
 */ 
int change_table(char line[][MAX_CELL], int n, int *rowPos, char *argv[], int argc, char del)
{
	int newColCount = n;
	bool addRow = false;

	for (int i = 0; i < argc; i++)
	{
		int function = find_function(argv[i]);
		switch(function)
		{
			case IROW:
				if (*rowPos == atoi(argv[i+1]))
					addRow = true;
				break;
			case DROW:
				if (*rowPos == atoi(argv[i+1]))
					return -1;
				break;
			case DROWS:
				if (*rowPos >= atoi(argv[i+1]) && *rowPos <= atoi(argv[i+2]))
					return -1;
				break;
			case ICOL: insert_column(line, &newColCount, n, atoi(argv[i+1]), false);
				break;
			case ACOL: insert_column(line, &newColCount, n, newColCount, true);
				break;
			case DCOL: delete_column(line, &newColCount, n, argv[i+1], argv[i+1]);
				break;
			case DCOLS: delete_column(line, &newColCount, n, argv[i+1], argv[i+2]);
				break;
			case CSET: set_new_value(line, newColCount, argv[i+1], argv[i+2]);
				break;
			case TOLOW: to_lower(line, newColCount, atoi(argv[i+1]));
				break;
			case TOUP: to_upper(line, newColCount, atoi(argv[i+1]));
				break;
			case ROUND: round_number(line, newColCount, atoi(argv[i+1]));
				break;
			case INT: trunc_number(line, newColCount, atoi(argv[i+1]));
				break;
			case COPY: copy_value(line, newColCount, argv[i+1], argv[i+2]); 
				break;
			case MOVE: move_value(line, newColCount, argv[i+1], argv[i+2]);
				break;
			case SWAP: swap_values(line, newColCount, argv[i+1], argv[i+2]);	
				break;
			case ROWS:
				if (strcmp(argv[i+2], "-") == 0 && *rowPos < atoi(argv[i+1]))
					i = argc;		
				else if ((strcmp(argv[i+2], "-") != 0) && ( *rowPos < atoi(argv[i+1]) 
						|| *rowPos > atoi(argv[i+2])))
					i = argc;
				break;
			case BEGINS:
				if(!beginswith_value(line, newColCount, atoi(argv[i+1]), argv[i+2]))
					i = argc;
				break;
			case CONTAINS:
				if(!contains_value(line, newColCount, atoi(argv[i+1]), argv[i+2]))
					i = argc;
				break;
			// BONUS
			case SPLIT: split_cell(line, &newColCount, argv[i+1], argv[i+2]);
				break;
		}
	}
	if (addRow)	
		print_table(NULL, newColCount, del, true);

	return newColCount;
}

/************************/
/*						*/
/*		  Main			*/
/*						*/
/************************/

int main(int argc, char *argv[])
{		
	int lenDelimiters = 2;
	char delimiters[MAX_CELL];	
	
	if (argc == 1)
	{
		fprintf(stderr, "No given argument.\n");
	}
	
	// checks delimiters
	check_delimiters(argv, argc, delimiters, &lenDelimiters);
	
	// checks arguments length and validity
	int arowCount = 0;
	bool lastRow = false;	

	if(check_arguments_len(argv, argc) == -1)
	{	
		fprintf(stderr, "Exceeded character limit in the argument.\n");
		return 2;
	}
	if(check_arguments_validity(argv, argc, &arowCount, &lastRow) == -1)
	{	
		fprintf(stderr, "Invalid given argument/-s\n");	
		return 2;	
	}
	
	char stdinLine[MAX_LINE+1];
	char line[MAX_LINE][MAX_CELL]; 
	int colCount = -1;
	int newCol = -1;
	int rowPos = 0;
	
	fgets(stdinLine, MAX_LINE+1, stdin);	

	while (stdinLine[0] != '\0')
	{	
		rowPos++;
		if (strlen(stdinLine) >= MAX_LINE) 
		{
			fprintf(stderr, "Exceeded character limit in the row.\n");
			return 1;
		} 
		if(split_line(&colCount, delimiters, lenDelimiters, stdinLine, line) == -1)
		{
			fprintf(stderr, "Wrong number of columns in a row\n");		
			return 1;
		}
		
		if (!lastRow)
			newCol = change_table(line, colCount, &rowPos, argv, argc, delimiters[0]);
		else
			newCol = colCount;		
		
		memset(stdinLine, '\0', MAX_LINE+1);
		if (fgets(stdinLine, MAX_LINE+1, stdin) == NULL  && lastRow)
			break;

		if (!(newCol == -1))
			print_table(line, newCol, delimiters[0], false);
	}
	// the input table is empty
	if (!rowPos)
	{	
		fprintf(stderr, "Empty input table.\n");		
		return 3;
	}		
	// rows - -: changes only affect the last row	
	if (lastRow)
	{	
		newCol = change_table(line, colCount, &rowPos, argv, argc, delimiters[0]);
        print_table(line, newCol, delimiters[0], false);
	}
	// arow: prints empty rows at the end of the table
	for (int i = 0; i < arowCount; i++)
	{
		print_table(NULL, newCol, delimiters[0], true);
	}
	
	return 0;
}