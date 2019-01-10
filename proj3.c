/*
Class: CPSC 346-02
Team Member 1: Connor Cooley
Team Member 2: N/A 
GU Username of project lead: ccooley
Pgm Name: proj3.c 
Pgm Desc: exploraiton of the proc file system 
Usage: 1) standard:  ./a.out -s 
Usage: 2) history:  ./a.out -h 
Usage: 3) load:  ./a.out -l 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void standard();
void history();
void load();

int main(int argc, char* argv[])
{
 if (argc != 2)
  {
   fprintf(stderr, "Error: Options required\n"); 
   fprintf(stderr, "usage: ./a.out -s|-h|-l\n\n"); 
   exit(EXIT_FAILURE);
  }
  
 if (!strcmp(argv[1],"-s"))
  standard();
 if (!strcmp(argv[1],"-h"))
  history();
 if (!strcmp(argv[1],"-l"))
  load();
}
/*
pre: none
post: displays CPU vendor_id, model name, and OS version
*/
void standard()
{
 char ch;
 FILE* ifp;
 char str[80];

 /*
 I've deliberately used two different mechanisms for writing to the console.  
 Use whichever suits you.
 strstr locates a substring
 */

 ifp = fopen("/proc/cpuinfo","r");
 while (fgets(str,80,ifp) != NULL)
  if (strstr(str,"vendor_id") || strstr(str,"model name"))
   puts(str); 
 fclose (ifp);

 ifp = fopen("/proc/version","r");
 while ((ch = getc(ifp)) != EOF)
  putchar(ch);
 fclose (ifp);
}

/*
pre: none
post: displays time since the last reboot (DD:HH:MM:SS), time when the system was last booted 
      (MM/DD/YY - HH:MM), number of processes that have been created since the last reboot.
Hint: strftime could be useful
*/
void history()
{
  char ch;
  FILE* ifp;
  char str[80];
  int i = 0;
  
  ifp = fopen("/proc/uptime", "r");
  while((ch = fgetc(ifp)) != ' ')
  {
    str[i] = ch;
	i++;
  }
  
  int num = atoi(str);
  int days = num/86400;
  int hours = (num%86400/3600);
  int minutes = ((num%3600)/60);
  int seconds = (num%60);
  
  printf("\nTime Since Last Reboot      (%d:%d:%d:%d)\n", days, hours, minutes, seconds);
  fclose(ifp);
  
  struct tm ts;
  time_t time;
  char new[80];
  char final[80];
  char ch2;
  i = 0;
  int k = 0;

  ifp = fopen("/proc/stat", "r");
  while(fgets(str,80,ifp) != NULL)
    if(strstr(str,"btime"))
	  strncpy(new, str, 80);
  
  while(new[i] != ' ')
  {
    i++;
  }
  i++;
  while(new[i] != '\n')
  {
    ch2 = new[i];
	final[k] = ch2;
	i++;
	k++;
  }
	
  num = atoi(final);
  time = num;
  ts = *localtime(&time);
  strftime(final, 80, "(%m/%d/%Y - %H:%M)\n", &ts);
  printf("Last Reboot Was At          %s", final);
  fclose(ifp);
  
  ifp = fopen("/proc/stat", "r");
  while(fgets(str, 80, ifp) != NULL)
    if(strstr(str, "processes"))
      puts(str);
  fclose(ifp);
}

/*
pre: none
post: displays total memory, available memory, load average (avg. number of processes over the last minute) 
*/
void load()
{
  FILE* ifp;
  char str[80];
  int i = 0;
  char final[80];
  char ch;
  
  printf("\n");
  ifp = fopen("/proc/meminfo", "r");
  while(fgets(str, 80, ifp) != NULL)
    if(strstr(str, "MemTotal:") || strstr(str, "MemAvailable:"))
      puts(str);
  fclose(ifp);
  
  ifp = fopen("/proc/loadavg", "r");
  fgets(str, 80, ifp);
  printf("Load Average:    ");
  while(str[i] != ' ')
  {
    ch = str[i];
	final[i] = ch;
	i++;
  }
  printf("%s\n\n", final);
  fclose(ifp);
}
