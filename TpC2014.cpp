#include<stdio.h>
#include <ctype.h>
#include<stdlib.h>
#include<string.h>
#include <windows.h>
#include<conio.h>
typedef struct page lpage;
//typedef page *listpage ; //tete de liste pointe vers la liste des pages d'un même mot 
struct page
{
	int numpage ;
	int occ ;
	page *svt ; //ou page *svt 
} ;
 
typedef struct mot lmot;
struct mot
{
	page *page1 ;// ou bien listepage page1 
	page *drnpage ;// ou bien listepage drnpage
	char *motindexe ;
	mot *svt ; // ou bien mot *svt
};
typedef struct elmtab
{
	char lettre ;
	lmot *pmot; // ou encore mot *pmot 
}TableIndex;
TableIndex index[26] ; // tableau d'enregistrement
int i, y ;
const int nbligneparpage=20 ; 
char word[30];

void gotoxy(int x, int y)
{
   HANDLE hConsoleOutput;
   COORD dwCursorPosition;
   //cout.flush();
   dwCursorPosition.X = x;
   dwCursorPosition.Y = y;
   hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleCursorPosition(hConsoleOutput,dwCursorPosition);
}
void initialisation (TableIndex index[26] )
{
	index[0].lettre='a';
	for (i=0 ;i<26 ; i++)
	{ 
	index[i].lettre= 'a' + i ;
	index[i].pmot=NULL ;
	}
}


int rechercherMotTabIgnorer(char mot[30]) //retourne 1 si elle trouve le mot sinon 0
{
 int i, j, mid, cmp;
 const char *motaIgnorer[30]={ "ainsi", "aussi", "aussitot", "avant", "bien", "cependant", "ces", "cet",
 "cette","comme", "donc", "elles", "enfin", "ils", "jusqu'a", "lorsque", "mais", "malgre", 
 "neamoins", "parce", "pourtant", "puis", "puisque", "que", "quoi", "sinon", "soit",
 "tandis", "tant", "toutefois"};
 
     i = 0;
     j = 29;
     while (i<= j)
     {
         mid=(i + j)/2;
         cmp=stricmp(mot, motaIgnorer[mid]);
         if (cmp == 0)
          return 1;
         else 
          if (cmp < 0)
              j = mid - 1;
          else
             i = mid + 1;
     }
    return 0;
}

int traiterMot(char mot[30], int pageNbr)
{
  int l;
  /* Ignorer les mots de moins de 3 caracteres */
  l=strlen(mot);
  if (l < 3)
    return 1;
  /* ignorer les mots dont le 1ere caractere n'est pa alphabetique */
   if (isalpha(mot[0])==0) 
    return 1;
  /* Si le mot fait partie des mots a ignorer, ignorer ce mot en retournant 1 */   
   if (rechercherMotTabIgnorer(mot) == 1)
    return 1;
  /* Dans le cas ou le mot est accepte, l'inserer dans l'index */
			return -1;
}
// 3-fonction ajouter un mot à l'index d'une page donnée y
void inseremot(char x [30] , int y) // numero de page
{
bool trouvepg ; int l;
lmot *p,*t,*precedp ;
lpage *q ,*precedq,*s;
i= tolower(x[0]) - 'a' ;
p=index[i].pmot;precedp=p;
while (p!= NULL && strcmp(x ,p->motindexe)>0)
  {precedp=p;
		 p=p->svt ;
		} 
if (p!=NULL && strcmp(x ,p->motindexe)==0)//mot existe ajout page ou incrimente occur
		{
			q=p->page1;
			precedq=q;
			while (q!= NULL && q->numpage<y)
			  {
						precedq=q;
						q=q->svt; 
				 }
   if (q!= NULL && q->numpage==y) //page existe incrimente occur  
    q->occ++ ;
   else //page n'existe pas l'inserer
			 {if(q==p->page1)
       {//insertion au début
       s=(lpage *) malloc(sizeof(lpage));
       s->svt=q ;//ou p->page1
       s->numpage=y ;
       s->occ=1 ;
       p->page1=s;			    	
							}			 	
						{// rajouter au milieu ou à la fin de la liste page un enregi page y
       s=(lpage *) malloc(sizeof(lpage));
       s->svt=q ;
       s->numpage=y ;
       s->occ=1 ;
       precedq->svt=s;			    	
	      if	(p->drnpage==precedq) p->drnpage=s;//si la page est rajouter à la fin alors modifier dernière page du mot
      }
			  }
   	}
	else //mot n'existe pas insertion au début ou à la fin 
  if(p==index[i].pmot) //insertion au debut 
		  {		  	
		  	t=(lmot*) malloc (sizeof(lmot));
					int l = strlen(x);
     t->motindexe = (char *) malloc(l + 1);
     if (t->motindexe == NULL)
     printf("Erreur:pas d'espace mémoire\n");
     strcpy(t->motindexe,x);
     //t->svt->motindexe=x;
		   t->svt=p ;					
					t->drnpage=(lpage*) malloc(sizeof(lpage));
					t->drnpage->svt=NULL ;
     t->drnpage->numpage=y ;
     t->drnpage->occ=1 ;
     t->page1=t->drnpage;//ou encore allower page1 et l'affecter à dernière page
     index[i].pmot=t;
	   }
	  else //insertion au milieu ou à la fin 
  {
  				t=(lmot*) malloc (sizeof(lmot));
					int l = strlen(x);
    t->motindexe = (char *) malloc(l + 1);
    if (t->motindexe == NULL)
     printf("Erreur:pas d'espace mémoire\n");
    strcpy(t->motindexe,x);
		   t->svt=p ;					
					t->drnpage=(lpage*) malloc(sizeof(lpage));
					t->drnpage->svt=NULL ;
     t->drnpage->numpage=y ;
     t->drnpage->occ=1 ;
     t->page1=t->drnpage;//ou encore allower page1 et l'affecter à dernière page
     precedp->svt=t;
  }
}

void afficheindexemot (char m [30] ) // 2-afficher l'index d'un mot donné par l'utilisateur
{
	bool trouve ;
	lmot *p ;
	lpage *q ;
 i=m[0]-'a' ;
 p=index[i].pmot ;
 trouve=false ;
while (p!= NULL && trouve==false)
  { 
   if (stricmp(m ,p->motindexe)==0)
		 {
			q=p->page1;
			trouve= true ;
			while (q!= NULL)
			{
				printf("le numéro de page est %d \n le nombre d'occurence %d\n" ,q->numpage , q->occ);
		  q=q->svt;
			}
   }
   else p=p->svt ;
			}
			if (trouve== false ) // le mot n'est pas dans l'indexe , il trouve pas le mot
			{
				printf ("le mot n'est pas dans l'indexe");
			}
}

void suppmot(char m[30] ,int y) // 4-enlever un mot de l'index d'une page donnée
{ //1
	lpage *q , *precedq ;
	bool trouve ,trouvepg ;
	lmot *p, *precedp ;
 i= tolower(m[0])-'a' ;
 p=index[i].pmot ;
 trouve=false ;
 while (p!=NULL && trouve==false)
 {
 	if(strcmp(p->motindexe ,m)==0)
 	{
 		trouve= true ;
		}
		else
		{
			precedp=p ; 
			p=p->svt;
		}
	}
if ( trouve==false)
{
 	printf("le mot que vous voulez supprimé n'existe pas dans l'indexe\n");
}
else
{
	 trouvepg=false ;
	 q=p->page1 ;
	 while (q!=NULL && trouvepg==false )
		{
			if (q->numpage=y )
			{
				trouvepg=true ;
			}
			else
			{
				precedq=q;
				q=q->svt;
			}
		}
		if (trouvepg==false )
		{
			printf("le mot n'existe pas dans la page donnée\n");
		}
		else
		{
		 if (q->occ>1) 
		 {
		 (q->occ)-- ;
	  }
		 else // ça veut dire que le nombre d'occurence est égal à 1 donc on doit supprimer l'enegistrement page , et s'il n'y a que cette page on doit supprimer l'enrgistrement mot aussi 
		 {
			 	if (p->page1==q)
			 	{// suppression au début de l'enregistrement page , quel soit l'unique page ou pas 
			 	 p->page1=q->svt ; //on a ici 2cas, soit y a plusieurs page et donc q->svt pointe sur le prochain enregistrement page,ou bien c'est l'unique page et donc q->svt pointe sur NULL et donc on doit supprimer l'enregistrement mot aussi, alors on va tester le q->svt pr connaitre s'il y a plusieurs page ou une seule pr le mot qu'on veut enlever de l'indexe
			 	 free(q);
			 	 if (p->page1==NULL) //c'est un if sans else , veut dire que c'est l'unique page, ça veut dire qu'on doit supprimer l'enregistrement mot , on peut utiliser une 2eme méthode pr savoir si c'est l'unique page, on teste si p->drnpage et p->page1 pointe sur le même enregistrement page, donc ont une même valeur
					 {// cas se présente ici aussi , ça peut-ètre une suppression au début ou à la fin , donc on teste aussi 
					  if(index[i].pmot==p) 
					  { //  suppresion au début
					  index[i].pmot=p->svt ;
					  free(p) ; // si c l'unique enregistrement mot alors index[i].pmot->svt est NULL
							}
							else 
							{  // supression au milieu ou à la fin 
							precedp->svt=p->svt ;
							free(p) ;
							}
						}
				 // veut dire que c'est pas l'unique page, donc on va pas supprimer l'enregistrement mot mais c'est une suppresion page au début (déja fait dans le 1er if )
					}
				else 
				{ // suppresion de page au milieu ou à la fin, on vérifie pas si c'est l'unique page car puisqu'elle est pas au début ça veut dire qu'elle est pas unique
					precedq=q->svt ;
					free(q);
				}
   }
	 }
}
}
void affichindex () //1-afficher l'index sur écran
{ 
lmot *p ;
bool cont ;
int j,x ;
lpage *q ;
j=0; cont=true;
while (j<7 && cont==true)
{system("cls");int y=0;//int y=0;
 for(i=j*4;i<j*4+4;i++)
 {
 if(i<26)
	  {
	  gotoxy(0,y);y++;
			printf ("%c" , toupper(index[i].lettre));
			p=index[i].pmot ;
			while (p!=NULL) // pour afficher tous les mots de la lettre
			 {
 	  gotoxy(2,y);
    printf("%s",p->motindexe );
    q=p->page1 ;
     while (q!=NULL) //Pour afficher les pages d'un même mot
     {
	    gotoxy(32,y);
	    printf("pg: %d" , q->numpage);
	    gotoxy(42,y);
	    printf("nbrOcc: %d" , q->occ);
	    q=q->svt ;y++;
	    }
	    p=p->svt ;y++;
	   }
	  }//fin de if
 } // fin de for aussi aprés 4fois
   gotoxy(10,y);
			printf("voulez-vous continuer ?tapez 1 pour oui et 0 pour non : ");
	  scanf("%d" , &x);
	  if (x==0) cont=false;
	          else j++;  
}
}
void saveindex () //5-sauvegarder l'index dans un fichier texte
{
	FILE *fichtxt ;
 lmot *p ;
 lpage *q ;
	fichtxt=fopen("FichierIndex" ,"w");
 for (i=0 ; i<26 ; i++)
	{
			fprintf (fichtxt,"%c" ,index[i].lettre);
			p=index[i].pmot ;
			 while (p!=NULL) // pour afficher tous les mots de la lettre
			 {
    fprintf(fichtxt ,"%5s",p->motindexe);
	   q=p->page1 ;
    while (q!=NULL) //Pour afficher les pages d'un même mot
     {
	    fprintf(fichtxt ,"pg: %5d" , q->numpage);
	    fprintf(fichtxt ,"nbrOcc: %d" , q->occ);
	    fprintf(fichtxt ,"\n");
	    q=q->svt ;
	    }
	   p=p->svt ;
	   }
	  
 }
	fclose(fichtxt); 
}
char recupermot(char chaine[100], int &pos ,char mottxt[30]) //lit un seul mot à la fois
{
	int j=0 ;
	while(chaine[pos]!=' ' && chaine[pos]!='\n'&& pos<100)
 {
 mottxt[j]=chaine[pos] ;
 j++ ;
 pos++ ;
 }
 if (mottxt[j]!='\0') mottxt[j]='\0';
	while(chaine[pos]==' ' && chaine[pos]!='\n'&& pos<100 )
	{
	pos++ ;
	} //pour sauter les blancs qu'il y a entre les mots dans le fichier
		
 }
	
int creerindex(FILE *fichtxt ,char nomfich[60] ,TableIndex index[26])
{
	//int nmpage;
	char mottxt[30];
	char chaine[100];
	int pos=0 ;
	int numpage=1 ;
	int numligne=0 ;
 char ligne[100];
	fichtxt=fopen(nomfich ,"r");
	if(fichtxt==NULL)
	{
 printf("le fichier n'a pas été ouvert\n");
  return 1 ;
	}
 while (!feof(fichtxt))// ou fgets(ligne[100],100,fichtxt)!=NULL sans utiliser fgets une 2eme fois;
 {pos=0;
	 fgets(ligne,100,fichtxt);
	 while(ligne[pos]!='\n'&& pos<100) // veut dire tant qu'on a pas atteint la fin de ligne
	  {
	  recupermot(ligne,pos,mottxt);
	  if(traiterMot(mottxt,numpage)==-1)
	      inseremot(mottxt ,numpage);
	  printf("%s\n",mottxt);    
	  }
	 numligne++ ; 	 printf("%d\n",numpage);
	 if(numligne==nbligneparpage)
	  {
		 numpage++ ;
		 //printf("%d",numpage);
		 numligne=0;
	  }	  
	}
}


main ()
{char rpns ;
	int nmpg ;
	int choix ;
	FILE *fichtxt ;
	char ligne[100];
	char nomfich[60];
	initialisation (index );
	printf("Donner le nom du fichier à indexer\n");
	scanf("%s",nomfich);
	if (creerindex(fichtxt,nomfich ,index)==1)
	{
	printf("impossible créer l'indexe \n");
	return 0;
 }
	choix=0;
 do // répeter l'affichage du menu dans le prgrm principal
 {system("cls");
 gotoxy(10,5);
	printf("1-Afficher l'index sur écran.");
 gotoxy(10,7);
	printf("2-Afficher l'index d'un mot.");
 gotoxy(10,9);
	printf("3-Ajouter un mot à l'index à une page donnée.");
 gotoxy(10,11);
	printf("4-Enlever un mot de l'index d'une page donnée.");
 gotoxy(10,13);
	printf("5-Sauvegarder l'index dans un fichier texte.");
 gotoxy(10,15);
	printf("6-Quitter l'index."); //chercher fonction qui efface l'écran d'exécution et positionne le curseur avant l'affichage
 gotoxy(20,18);
	printf("Choix: ");
	scanf("%d",&choix);
 switch(choix)
	{
	case 1:
	affichindex ();
	break;
	case 2:
	system("cls");	
	printf("donner le mot à afficher l'index :  ");
	scanf("%s",word);
 getchar();
	afficheindexemot (word );
	printf("\n Taper une touche pour terminer ");
	getchar();
	break ;
	case 3:
	system("cls");	
	gotoxy(10,12);
	printf("donner le mot à ajouter :  ");
	gotoxy(36,12);
	scanf("%s",word);
	gotoxy(10,14);
	printf("donner la page correspondante :  ");
	gotoxy(42,14);
	scanf("%d",&nmpg );
 //getchar();
	if(traiterMot(word,nmpg)==-1)
	  inseremot(word , nmpg);
	 else {printf("Erreur. Le mot ne peut pas être indexée");
	      getchar();} 
	break ;
	case 4:
	system("cls");	
	gotoxy(10,12);
	printf("donner le mot à enlever de l'index :  ");
	gotoxy(47,12);
	scanf("%s",word);
	gotoxy(10,14);
	printf("donner la page correspondante :  ");
	gotoxy(43,14);
	scanf("%d",&nmpg );
	gotoxy(10,16);
	printf("voulez-vous vraiment supprimer le mot de l'indexe o/n? ");
	gotoxy(64,16);
	scanf("%c", &rpns);
 rpns=getchar();
	 //getchar();
	if (rpns=='o')	suppmot(word ,nmpg);
	break ;
	case 5:
	system("cls");	
	printf("Sauvegarde en cours. Taper une touche pour terminer ");
	saveindex ();
 getchar();
	break;
	case 6:
	system("cls");	
	printf("voulez-vous vraiment quitter le programme o/n? ");
 scanf("%c",&rpns);
 //getchar();
 rpns=getchar();
	if(rpns=='n')
	    	choix=0;
	break ;
	default:
	printf("Erreur du choix. Tapez un nombre entre 1 et 6\n");
	break;
	};
}while (choix!=6);
}

