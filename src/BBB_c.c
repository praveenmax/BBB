#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SYSFS_GPIOPATH "/sys/class/gpio"
#define GPIO_P9_12_DIR "gpio60"
#define GPIO_ATTRIB_DIRECTION "direction"
#define GPIO_ATTRIB_VALUE "value"

char DIRECTION_OUT[]="out";
char DIRECTION_IN[]="in";

//gpio functions prototypes
void exportGPIO(int GPIONum);
void unexportGPIO(int GPIONum);
void setGPIODirection(char * direction);
void setValue(int value);
int readGPIO(char * buffer,char * gpioPath, char * gpioName, char * gpioProp);
int writeGPIO(char * buffer, char * gpioPath, char * gpioName, char * gpioProp);
int toggleGPIO(char * internalGPIONum, char * gpioPath, int state);


int main(void)
{
	char * bufferData=(char *)malloc(10*sizeof(char));


	char optionChar,choice;//for main and sub-menu

	printf("\n##  GPIO manipulation through Sysfs in C [ROOT ACCESS NEEDED] ##\n\n");
	printf("\nWARNING : Refer to your device GPIO headers before using this!! ##\n\n");

	while(optionChar!='x')
	{

			printf("\n\n-- MENU ---------------------------\n");
			printf("1. Export GPIO               : %s \n",GPIO_P9_12_DIR);
			printf("2. UnExport GPIO             : %s \n",GPIO_P9_12_DIR);
			printf("3. Read   DIRECTION  of GPIO : %s \n",GPIO_P9_12_DIR);
			printf("4. Read   VALUE      of GPIO : %s \n",GPIO_P9_12_DIR);
			printf("5. Write  DIRECTION  of GPIO : %s \n",GPIO_P9_12_DIR);
			printf("6. Write  VALUE      of GPIO : %s \n",GPIO_P9_12_DIR);
			printf("x. EXIT                          \n");

			optionChar = getchar();

			switch(optionChar)
			{
			case '1':
				printf("Exporting GPIO %s ",GPIO_P9_12_DIR);

				strcpy(bufferData,"60");
				if(toggleGPIO(bufferData,SYSFS_GPIOPATH,1)==0)
					printf(" %s exported \n", GPIO_P9_12_DIR );

				break;
			case '2':
				printf("UnExporting GPIO %s",GPIO_P9_12_DIR);

				strcpy(bufferData,"60");
				if(toggleGPIO(bufferData,SYSFS_GPIOPATH,0)==0)
					printf(" %s unexported \n", GPIO_P9_12_DIR);

				break;
			case '3':
				printf("Reading GPIO DIRECTION...... \n");

				memset(bufferData,'\0',10);
				if(readGPIO(bufferData, SYSFS_GPIOPATH, GPIO_P9_12_DIR, GPIO_ATTRIB_DIRECTION)!=-1)
					printf("%s : [DIRECTION] = %s \n",GPIO_P9_12_DIR,  bufferData);

				break;
			case '4':
				printf("Reading GPIO VALUE........... \n");

				strcpy(bufferData,"0\0");//reset
				if(readGPIO(bufferData, SYSFS_GPIOPATH, GPIO_P9_12_DIR, GPIO_ATTRIB_VALUE)!=-1)
					printf("%s : [VALUE] = %s \n", GPIO_P9_12_DIR, bufferData);

				break;
			case '5':
				printf("Writing GPIO DIRECTION........\n");

				choice=0;
				printf("Choose a direction -> \n 1.IN \n 2.OUT \n");
				scanf("%d",&choice);

				if(choice==1)
				{
					//snprintf(bufferData,2,"%s",DIRECTION_IN);
					bufferData=DIRECTION_IN;
					if(writeGPIO(bufferData, SYSFS_GPIOPATH, GPIO_P9_12_DIR, GPIO_ATTRIB_DIRECTION)!=-1)
					{
						printf(" DIRECTION changed for %s\n to IN",GPIO_P9_12_DIR);
					}
				}
				else
				if(choice==2)
				{
					//snprintf(bufferData,3,DIRECTION_OUT);
					bufferData=DIRECTION_OUT;
					if(writeGPIO(bufferData, SYSFS_GPIOPATH, GPIO_P9_12_DIR, GPIO_ATTRIB_DIRECTION)!=-1)
					{
						printf(" DIRECTION changed for %s\n to OUT",GPIO_P9_12_DIR);
					}
				}
				else
				{
					printf("Invalid choice given !\n");
				}
				break;
			case '6':
				printf("Writing GPIO VALUE..........\n");

				choice=0;

				printf("Choose the VALUE -> \n 1.Enable \n 2.Disable \n");
				scanf("%d",&choice);

				if(choice==1)
				{
					bufferData="1";
					if(writeGPIO(bufferData, SYSFS_GPIOPATH, GPIO_P9_12_DIR, GPIO_ATTRIB_VALUE)!=-1)
					{
						printf(" VALUE changed for %s\n to 1", GPIO_P9_12_DIR);
					}
				}
				else
				if(choice==2)
				{
					bufferData="0";
					if(writeGPIO(bufferData, SYSFS_GPIOPATH, GPIO_P9_12_DIR, GPIO_ATTRIB_VALUE)!=-1)
					{
						printf(" VALUE changed for %s\n to 0", GPIO_P9_12_DIR);
					}
				}
				else
				{
					printf("Invalid choice given !\n");
				}

				break;
			case 'x':
				//free(bufferData);
				printf("Bye!....\n");
				//exit(0);
				break;
			}//switch

	}//while

/*
	//char gpioValue, gpioDirection;
	printf("*\n Reading %s ... \n", GPIO_P9_12);

	if(readGPIO(buffer, SYSFS_GPIOPATH, GPIO_P9_12, GPIO_DIRECTION)!=-1)
	{
		if(strcmp(buffer,"in")==0)
			printf("%s : [DIRECTION] = %s \n",GPIO_P9_12,  buffer);
		else
		if(strcmp(buffer,"out")==0)
			printf("%s : [DIRECTION] = %s \n",GPIO_P9_12,  buffer);

	}

	printf("*\n Setting DIRECTION to OUT..\n");

	buffer = direction_out;
	if(writeGPIO(buffer, SYSFS_GPIOPATH, GPIO_P9_12, GPIO_DIRECTION)!=-1)
	{
		printf(" DIRECTION changed for %s\n",GPIO_P9_12);
	}

	printf("*\n Setting VALUE to OUT..\n");

	buffer = '1';
	if(writeGPIO(buffer, SYSFS_GPIOPATH, GPIO_P9_12, GPIO_VALUE)!=-1)
	{
		printf(" VALUE changed for %s\n",GPIO_P9_12);
	}



	*/
	return 0;
}

/**
 * Used to export and unexport GPIO pins
 * For export , set state =1
 * For unexport, set state =0
 */
int toggleGPIO(char * internalGPIONum, char * gpioPath, int state)
{
	FILE *fp;
	char fullPath[80];


	if(state==1)
		sprintf(fullPath,"%s/export",gpioPath);
	else
		sprintf(fullPath,"%s/unexport",gpioPath);

	printf("Reading the export path %s \n",fullPath);
	if((fp=fopen(fullPath,"w"))==NULL)
	{
		perror("Unable to export GPIO!");
		return -1;
	}
	else
	{
		printf("File accessed to write : %s \n",fullPath);
		fputs(internalGPIONum,fp);
	}

	int returnValue=fclose(fp);
	if(returnValue!=0)
		perror("Error while closing file stream !");

	return returnValue;

}

int writeGPIO(char * buffer, char * gpioPath, char * gpioName, char * gpioProp)
{
	//Form the path..
	//Open the file stream in write mode
	//write the chars
	//check whether write is successful
	//close the file stream
	//return appr. exit code

	char fullPath[80];
	FILE *fp;
	sprintf(fullPath,"%s/%s/%s",gpioPath,gpioName,gpioProp);

	if((fp=fopen(fullPath,"rb+"))==NULL)
	{
		perror("Unable to write to the GPIO file!");
		return -1;
	}
	else
	{
		printf("File accessed to write : %s \n",fullPath);
		fputs(buffer,fp);
	}

	int returnValue=fclose(fp);
	if(returnValue!=0)
		perror("Error while closing file stream !");

	return returnValue;


}


int readGPIO(char * buffer, char * gpioPath, char * gpioName, char * gpioProp)
{

	char fullPath[80];
	int count=0;
	char tempChar;

	FILE *fp;

	sprintf(fullPath,"%s/%s/%s",gpioPath,gpioName,gpioProp);
	if((fp=fopen(fullPath,"r+"))==NULL)
	{
		perror("Unable to read the GPIO file!");
		return -1;
	}
	else
	{
		printf("File accessed to read : %s \n", fullPath);
	}

	while((tempChar = fgetc(fp))!='\n')
	{
	  *(buffer++)=tempChar;
	}

	int returnValue=fclose(fp);
	if(returnValue!=0)
		perror("Error while closing file stream !");

	return returnValue;


}

void exportGPIO(int GPIONum)
{
	//write to file
}

void unexportGPIO(int intGPIONum)
{
	///write to file
}

void setGPIODirection(char* direction)
{
	///write to file
}

void setValue(int value)
{
	//write to file
}

