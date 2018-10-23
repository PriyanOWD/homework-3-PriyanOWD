#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdarg.h> 
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int inputParser(char*** uInput, int arrSize)
{
	char uString[200];
	fgets(uString, 500, stdin);
	int strLength = 0;

	char* token = strtok(uString, " ");
	
	int i = 0;
	while(token != NULL && i != arrSize)
	{
		if( strcmp(token, ";") == 0)
		{
			((*uInput)[i]) = NULL;
			++i;
		}
		else{}
		
		strcpy((*uInput)[i], token);

		
		strLength = strlen((*uInput)[i]);
		if( ((*uInput)[i])[strLength-1] == '\n')
		{
			//printf("%d %c",strLength, (*uInput)[i][strLength-1]);
			((*uInput)[i])[strLength-1] = '\0';
		}

		token = strtok(NULL, " ");
		++i;
	}

	((*uInput)[i]) = NULL;
	
	return ++i;
}

void clearUserInput(char*** uInput, int arrSize)
{
	int i = 0;
	for(i; i < arrSize; ++i)
	{
		free((*uInput)[i]);
		((*uInput)[i]) = NULL;
	}

	free(*uInput);
	(*uInput) = NULL;
}

void initUserInput(char*** uInput, int arrSize)
{
	(*uInput) = (char **) malloc (sizeof(char*) * arrSize);
	int i = 0;
	for(i; i < arrSize; ++i)
	{
		(*uInput)[i] = (char *) malloc (sizeof(char) * 200);
	}
	return;
}



void arrayResize( char* arr1)
{
	
	
}

int argCommands(char*** uInput, int currI, int arrSize, int tokCount)
{
	int i = currI;

	while(((*uInput)[i] != NULL) && (i <= (tokCount)))
	{
			++i;
	}
	i++;
	return i;
}

void sigint_handler(int sig)
{
  char msg[] = "Signal handled.  Deal with it.\n";
  //write(1, msg, sizeof(msg));
}

int main(int argc, char **argv)
{	
	char** userInput;
	int uiSize = 50;	
	int cPID;
	int tokens;

	initUserInput(&userInput, uiSize);
	
	printf(" 361 > ");
	tokens = inputParser(&userInput, uiSize);
	printf("tok = %d ", tokens);

	int i = 0;
	int cmd = 0;
	while(strcmp(userInput[i],"exit") != 0)
	{
		cmd = i;
		printf(" BEFORE cmd = %d i = %d      ", cmd, i);
		i = argCommands(&userInput, i, uiSize, tokens);
		printf(" AFTER cmd = %d i = %d\n", cmd, i);
		cPID = fork();
		if( cPID == 0)
		{
			if(cmd == 0)
			{
				execv(userInput[cmd], userInput);
			}
			else
			{
				execv(userInput[cmd], userInput + cmd);
			}	
			if(i >= tokens)
			{			
				printf(" CHILD EXIT");
				exit(6);
			}
		}
		else
		{
			printf(" IN ELSE");
			int status;
			wait(&status);
			printf("pid:%d status:%d\n", cPID, status);
			if(userInput[i] == ";")
				++i;
		
			if(i >= tokens)
			{			
				printf(" IN i>=tokens");
				clearUserInput(&userInput, uiSize);
				initUserInput(&userInput, uiSize);
				printf("361 > ");
				tokens = inputParser(&userInput, uiSize);
				printf("tok = %d ", tokens);
				i = 0;
			}
		}
	}
}
	