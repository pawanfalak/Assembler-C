#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fp,*ip,*tp,*tp1,*tp2;

char MOT[][11]={"STOP","ADD","SUB","MULT","MOVER","MOVEM","COMP","BC","DIV","READ","PRINT"};
char POT[][7]={"START","END","LTORG","EQU","ORIGIN"};
char reg[][5]={"AREG","BREG","CREG","DREG"};
char decl[][3]={"DC","DS"};

int POOLTAB[10];

int countl=0,counts=0,countp=0;
//counts => count of symbols in symbol table
//countl => count of literals in literal table 


typedef struct symbol
{
	int s_no;
	char s_name[200];
	int s_add;
}symbol;

typedef struct litral
{
	int l_no;
	char l_name[100];
	int l_add;
}litral;


symbol s[10];
litral l[10];

int search_POT(char tok1[10])
{
	int i,e;
	for(i=0;i<5;i++)
	{
		e=strcmp(tok1,POT[i]);
		if(e==0)
			return (i+1);
	}
	return -1;
}	

int search_MOT(char tok1[10])
{
	int i,e;
	for(i=0;i<11;i++)
	{
		e=strcmp(tok1,MOT[i]);
		if(e==0)
			return (i+1);
	}
	return -1;
}

int search_REG(char tok1[10])
{
	int i,e;
	for(i=0;i<4;i++)
	{
		e=strcmp(tok1,reg[i]);
		if(e==0)
			return (i+1);
	}
	return -1;
}

int search_dl(char tok1[10])
{
	int i,e;
	for(i=0;i<2;i++)
	{
		e=strcmp(tok1,decl[i]);
		if(e==0)
			return (i+1);
	}
	return -1;
}

int search_st(char tok3[10],int counts)
{
	int i,e;
	for(i=0;i<=counts;i++)
	{
		e=strcmp(tok3,s[i].s_name);
		if(e==0)
			return i;
	}
	return (-1);
	
}


int pass1()
{	

	int i,lc=0,ch,sym1,m,m1,m2,m3,litral=0,ltorg=0,sadd,a=0,m4,m5;
	char buffer[80],tok1[10],tok2[10],tok3[10],tok4[10];
	
	
	while(fgets(buffer,80,fp))
	{

		ch=sscanf(buffer,"%s%s%s%s",tok1,tok2,tok3,tok4);

		switch(ch)
		{

			case 1:
				//ltorg & END
				m=search_POT(tok1);
				POOLTAB[countp]=l[a].l_no;
				a=a+litral;
				countp++;
				litral=0;
				if(m==2)//end
				{
					for(i=POOLTAB[countp-1];i<countl;i++)
					{
						l[i].l_add=lc;
						fprintf(ip,"%d (AD,0%d) (L,%d)\n",lc,m,i);
						lc++;
					}
					
				}
				
				else //ltorg
				{
					if(ltorg==0)
					{
						for(i=0;i<countl;i++)
						{
							l[i].l_add=lc;
							
							fprintf(ip,"%d (AD,0%d) (L,%d)\n",lc,m,i);
							lc++;
							
						}
					}
					else
					{
						for(i=POOLTAB[countp-1];i<countl;i++)
						{
							l[i].l_add=lc;
							fprintf(ip,"%d (AD,0%d) (L,%d)\n",lc,m,i);
							lc++;
						}
					}
					ltorg++;
				
				}
				
				
				break;

			case 2: //START
				
				m=search_POT(tok1);
				
				if(m==1)// found start
				{
					
					lc=atoi(tok2);// initialize location counter
					fprintf(ip,"    (AD,01) (C,%d)\n",lc);
				}
				else if(m==5)//ORIGIN
				{
					
					int z=0;
					for(i=0;i<strlen(tok2);i++)
					{
						if(tok2[i]!='+') // found symbol
						{
							tok4[i]=tok2[i];
							z++;
						}
						else 
							break;
						
					}
						tok4[i]='\0';
						z++;
						//tok3[0]=tok2[i+1];
						strncpy(tok3,tok2+i+1,strlen(tok2)-z);
						m3=atoi(tok3);//generating constant to add
						m1=search_st(tok4,counts);
						m2=s[m1].s_add;
						int x=lc;
						lc=m2+m3;// setting the location couner value with new value (symbol address+ constant)
						fprintf(ip,"    (AD,0%d) (C,%d)\n",m,lc);
						
				}
				else
				{
					// label and stop
					int z=search_st(tok1,counts);
					if(z==-1)
					{
						s[counts].s_no=counts;
						strcpy(s[counts].s_name,tok1);
						s[counts].s_add=lc;
						sym1=counts;
						counts++;
					}
					else
					{
						s[z].s_add=lc;
						sym1=z;
					}
					fprintf(ip,"%d (S,0%d) (IS,01) \n",lc,sym1);
						
				}
				
				
				break;
			
			case 3:
		
				// imperative statement
				m=search_MOT(tok1);//store index of mot table
				m1=search_REG(tok2);//REGISTER
				m2=search_POT(tok2);//EQU
				if(m2==4)//EQU
				{
					m3=search_st(tok1,counts);
					m4=search_st(tok3,counts);
					if(m3==-1)
					{
						//symbol not found
						//adding symbol to table
						s[counts].s_no=counts;
						strcpy(s[counts].s_name,tok1);	
						s[counts].s_add=s[m4].s_add;
						m3=counts;
						counts++;//increment count of symbols
					}
		
					else
					{
						s[m3].s_add=s[m4].s_add;//setting new symbol address for LHS operand	
					}
					fprintf(ip,"    (S,%d) (AD,0%d) (S,%d)\n",m3,m2,m4);
				
				}
				
				else if(strcmp(tok1,"BC")==0)
				{
		
					int sym=search_st(tok3,counts);
					if(sym==-1)
					{
						//symbol not found
						s[counts].s_no=counts;
						strcpy(s[counts].s_name,tok3);
						sym=counts;
						counts++;
					}
					if(strcmp(tok2,"LT")==0)
					{
		
						fprintf(ip,"%d (IS,08) 01 (S,%d) \n",lc,s[sym].s_no);
						lc++;
					}
					else if(strcmp(tok2,"LE")==0)
					{
		
						fprintf(ip,"%d (IS,08) 02 (S,%d) \n",lc,s[sym].s_no);
						lc++;
					}
					else if(strcmp(tok2,"EQ")==0)
					{
		
						fprintf(ip,"%d (IS,08) 03 (S,%d) \n",lc,s[sym].s_no);
						lc++;
					}
					else if(strcmp(tok2,"GT")==0)
					{
						fprintf(ip,"%d (IS,08) 04 (S,%d) \n",lc,s[sym].s_no);
						lc++;
					}
					else if(strcmp(tok2,"GE")==0)
					{
		
						fprintf(ip,"%d (IS,08) 05 (S,%d) \n",lc,s[sym].s_no);
						lc++;
					}
					else if(strcmp(tok2,"ANY")==0)
					{
		
						fprintf(ip,"%d (IS,08) 06 (S,%d) \n",lc,s[sym].s_no);
						lc++;
					}
				}
		
				else if(m>=1)
				{
					int sym;
					if(tok3[0]=='=')//litrals
					{
						l[countl].l_no=countl;
						strcpy(l[countl].l_name,tok3);
						fprintf(ip,"%d (IS,0%d) %d (L,%d) \n",lc,m,m1,l[countl].l_no);
						lc++;
						litral++;
						countl++;
					}
					else //symbol
					{
						if(counts==0)
						{
							//first symbol inserted in table
							s[counts].s_no=counts;
							strcpy(s[counts].s_name,tok3);
							sym=counts;
							counts++;
						}
						else
						{
							sym=search_st(tok3,counts);
							if(sym==-1)
							{
								//symbol not found
								s[counts].s_no=counts;
								strcpy(s[counts].s_name,tok3);
								sym=counts;
								counts++;
							}
						}
						
						fprintf(ip,"%d (IS,0%d) %d (S,%d) \n",lc,m,m1,s[sym].s_no);
						lc++;
					}
				}
				else if(strcmp(tok2,"DC")==0)
				{
		
					m3=search_st(tok1,counts);
					s[m3].s_add=lc;
					sadd=atoi(tok3);
					fprintf(ip,"%d (S,%d) (DL,01) (C,%d) \n",lc,m3,sadd);
					lc++;
					
				}
				else if(strcmp(tok2,"DS")==0)
				{
		
					m3=search_st(tok1,counts);
					s[m3].s_add=lc;
					sadd=atoi(tok3);
					fprintf(ip,"%d (S,%d) (DL,02) (C,%d) \n",lc,m3,sadd);
					lc=lc+sadd;
	
				}
				
				break;
		
			case 4:
				sym1=search_st(tok1,counts);
				if(sym1==-1)
				{
					s[counts].s_no=counts;
					strcpy(s[counts].s_name,tok1);
					s[counts].s_add=lc;
					sym1=counts;
					counts++;
				}
				else
					s[sym1].s_add=lc;
				m=search_MOT(tok2);
				m1=search_REG(tok3);// search registers
		
				if(m>=1)
				{
					if(tok4[0]=='=')//litrals
					{
						l[countl].l_no=countl;
						strcpy(l[countl].l_name,tok4);
						
					fprintf(ip,"%d (IS,0%d) %d (L,%d)\n",lc,m,m1,l[countl].l_no);
						
						lc++;
						litral++;
						countl++;
					}
					else	//symbol
					{
						
						int sym=search_st(tok4,counts);
						if(sym==-1)
						{
							s[counts].s_no=counts;
							strcpy(s[counts].s_name,tok4);
							sym=counts;
							counts++;
						}
						
						fprintf(ip,"%d (IS,0%d) %d (S,%d) \n",lc,m,m1,s[sym].s_no);
				
						lc++;
					}	
				}
				break;				
		}
	}
	
	fclose(fp);
	
	return 0;	
}

void disp_symbol_table()
{

	for(int p=0;p<counts;p++)
	{
		fprintf(tp,"%d\t%s\t%d\t\n",s[p].s_no,s[p].s_name,s[p].s_add);
	}
}

void disp_litral_table()
{

	for(int p=0;p<countl;p++)
	{
		fprintf(tp1,"%d\t%s\t%d\t\n",l[p].l_no,l[p].l_name,l[p].l_add);
	}
}

void disp_pool_table()
{

	for(int p=0;p<countp;p++)
	{
		fprintf(tp2,"\t%d\t%d\t\n",(p+1),POOLTAB[p]);
	}
}


int main()
{
	fp=fopen("prog2.asm","r");
	ip=fopen("ic.txt","w");// file descriptor for intermedite code 
	if(pass1()==-1)
		printf("\nincorrect syntax for .asm file ..try again\n");
	else
	{
		fclose(ip);
		tp=fopen("symbol_table.txt","w");
		tp1=fopen("litral_table.txt","w");
		tp2=fopen("pool_table.txt","w");
		disp_symbol_table();
		disp_litral_table();
		disp_pool_table();
		printf("\nIntermediate code for .asm file is generated\n");
		fclose(tp);
		fclose(tp1);
		fclose(tp2);
	}
	return 0;
}
