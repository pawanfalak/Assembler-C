#include "header.h"
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
void enter_ST()
{
	int i=0;
	char buffer[1000],tok1[100],tok2[100],tok3[100];
	FILE *fp;
	fp=fopen("symbol_table.txt","r");
	while(fgets(buffer,80,fp))
	{
		sscanf(buffer,"%s%s%s",tok1,tok2,tok3);
		strcpy(s[i].s_name,tok2);
		s[i].s_add=atoi(tok3);
		s[i].s_no=atoi(tok1);
		i++;
	}
	fclose(fp);
}

void enter_LT()
{
	int i=0;
	char buffer[1000],tok1[100],tok2[100],tok3[100];
	FILE *fp;
	fp=fopen("litral_table.txt","r");
	while(fgets(buffer,80,fp))
	{
		sscanf(buffer,"%s%s%s",tok1,tok2,tok3);
		strcpy(l[i].l_name,tok2);
		l[i].l_add=atoi(tok3);
		l[i].l_no=atoi(tok1);
		i++;
	}
	fclose(fp);
}
void enter_PT()
{
	int i=0;
	char buffer[1000],tok1[100],tok2[100];
	FILE *fp;
	fp=fopen("pool_table.txt","r");
	while(fgets(buffer,80,fp))
	{
		sscanf(buffer,"%s%s",tok1,tok2);
		POOLTAB[i]=atoi(tok2);
		i++;
	}
	fclose(fp);
}



int main()
{
	FILE *f1,*f2;
	char buff[80],tok1[100],tok2[100],tok3[100],tok4[100],tok5[100],s1;
	int ch,lc=0,r,add,i=0,j=0;
	f1=fopen("ic.txt","r");
	f2=fopen("mcode.txt","w");
	enter_ST();
	enter_LT();
	enter_PT();
	if(f1==NULL)
	{
		printf("FILE does not exist");
	}
	else
	{		
		while(fgets(buff,80,f1))
		{
			ch=sscanf(buff,"%s%s%s%s",tok1,tok2,tok3,tok4);
			if(tok1[0]=='(')
				continue;
			else
			{
				if(ch==4&&tok2[1]=='I')
				{
				// IS REG L/S
					lc=atoi(tok1);
					r=atoi(tok3);
					s1=tok2[5];
					if(tok4[1]=='S')
					{
						char t=tok4[3];
						i=t-'0';
						printf("%d 0%c %d %d\n",lc,s1,r,s[i].s_add);
						fprintf(f2,"%d 0%c %d %d\n",lc,s1,r,s[i].s_add);
					}
					else if(tok4[1]=='L')
					{
						char t=tok4[3];
						i=t-'0';
						printf("%d 0%c %d %d\n",lc,s1,r,l[i].l_add);
						fprintf(f2,"%d 0%c %d %d\n",lc,s1,r,l[i].l_add);
					}
					
					
				}
				else if(ch==4&&tok3[1]=='D')
				{
				//DL
					lc=atoi(tok1);
					//r=atoi(tok3);
					char x= tok3[5];
					s1=tok4[3];
					if(tok2[1]=='S')
					{
						char t=tok2[3];
						i=t-'0';
						printf("%d %d %c %c\n",lc,s[i].s_add,x,s1);
						fprintf(f2,"%d %d %c %c\n",lc,s[i].s_add,x,s1);
					}

					
				}
				
				else if(ch==3&&tok2[1]=='A')
				{
				//LTORG processing => (AD)
					lc=atoi(tok1);
					//r=atoi(tok3);
					s1=tok2[5];
					if(tok3[1]=='L')
					{
						char t=tok3[3];
						i=t-'0';
						printf("%d 0%c %d\n",lc,s1,l[i].l_add);
						fprintf(f2,"%d 0%c %d\n",lc,s1,l[i].l_add);
					}

					
				}
				else if(ch==3&&tok3[1]=='I')
				{
					// (S) (IS)
					lc=atoi(tok1);
					//r=atoi(tok3);
					char x= tok3[5];
					if(tok2[1]=='S')
					{
						char t=tok2[3];
						i=t-'0';
						printf("%d %d %c\n",lc,s[i].s_add,x);
						fprintf(f2,"%d %d %c\n",lc,s[i].s_add,x);
					}
				}
	
			}
		}
	}
	
	fclose(f1);
	fclose(f2);
	
	return 0;
	
		
}
