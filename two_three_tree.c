#include<stdio.h>
#include<stdlib.h>

int count=0;

struct node{
	int k1,k2,cnt;
	struct node *left,*right,*mid;
};

typedef struct node* NODE;

void swap(int *x,int *y){
	*x = *x + *y;
	*y = *x - *y;
	*x = *x - *y;
}

NODE getnode(){
	NODE x;

	x=(NODE)malloc(sizeof(struct node));
	count++;
	x->cnt=count;
	x->k1=x->k2=0;
	x->left=x->right=x->mid=NULL;

	return x;
}

int insert(int ele,NODE root,NODE *t1,NODE *t2){
	NODE temp1=NULL,temp2=NULL;

	if(root!=NULL){
		if(ele<root->k1){
			ele=insert(ele,root->left,&temp1,&temp2);
			if(ele){
				if(!root->k2){
					root->left=temp1;
					root->mid=temp2;
				}
				else{	
					*t1=getnode();
					*t2=getnode();
					(*t1)->k1=root->k1;
					(*t2)->k1=root->k2;
					(*t1)->left=temp1;
					(*t2)->left=root->mid;
					(*t1)->right=temp2;
					(*t2)->right=root->right;
				}
			}
		}
		else if(ele>root->k2){
			ele=insert(ele,root->right,&temp1,&temp2);
			if(ele){
				if(!root->k2){
					root->mid=temp1;
					root->right=temp2;	
				}
				else{
					*t1=getnode();
					*t2=getnode();
					(*t1)->k1=root->k1;
					(*t2)->k1=root->k2;
					(*t1)->left=root->left;
					(*t1)->right=root->mid;
					(*t2)->left=temp1;
					(*t2)->right=temp2;
				}
			}
		}
		else{
			ele=insert(ele,root->mid,&temp1,&temp2);
			if(ele){
				if(!root->k2){
					root->left=temp1;
					root->right=temp2;
				}
				else{	
					*t1=getnode();
					*t2=getnode();
					(*t1)->k1=root->k1;
					(*t2)->k1=root->k2;
					(*t1)->left=root->left;
					(*t1)->right=temp1;
					(*t2)->left=temp2;
					(*t2)->right=root->right;
				}
			}
		}
		if(!ele)
			return 0;
		if(!root->k2){
			if(ele<root->k1){
				root->k2=root->k1;
				root->k1=ele;
			}
			else
				root->k2=ele;	
			return 0;
		}
		else{	
			if(ele<root->k1)
				swap(&ele,&((*t1)->k1));
			if(ele>root->k2)
				swap(&ele,&((*t2)->k1));
			return ele;
		}
	}

	return ele;
}

void display(NODE root){
	if(root!=NULL){
		display(root->left);
		printf("%d ",root->k1);
		display(root->mid);
		if(root->k2)
			printf("%d \n",root->k2);
		display(root->right);
	}
}

void printofile(NODE root,FILE *op){
	if(root!=NULL){
		if(root->left!=NULL||root->mid!=NULL||root->right!=NULL){
			fprintf(op,"%d[label=\"%d",root->cnt,root->k1);
			if(root->k2)
				fprintf(op,",%d\"];\n",root->k2);
                        else
                                fprintf(op,"\"];\n");
			if(root->left!=NULL){
				fprintf(op,"%d->%d;\n",root->cnt,(root->left)->cnt);
				fprintf(op,"%d[label=\"%d",(root->left)->cnt,(root->left)->k1);
				if((root->left)->k2)
                                fprintf(op,",%d\"];\n",(root->left)->k2);
                        else
                                fprintf(op,"\"];\n");
			}
			if(root->mid!=NULL){
                                fprintf(op,"%d->%d;\n",root->cnt,(root->mid)->cnt);
                                fprintf(op,"%d[label=\"%d",(root->mid)->cnt,(root->mid)->k1);
                                if((root->mid)->k2)
                                fprintf(op,",%d\"];\n",(root->mid)->k2);
                        else
                                fprintf(op,"\"];\n");
                        }
			if(root->right!=NULL){
                                fprintf(op,"%d->%d;\n",root->cnt,(root->right)->cnt);
                                fprintf(op,"%d[label=\"%d",(root->right)->cnt,(root->right)->k1);
                                if((root->right)->k2)
                                fprintf(op,",%d\"];\n",(root->right)->k2);
                        else
                                fprintf(op,"\"];\n");
                        }
		}
		printofile(root->left,op);
		printofile(root->mid,op);
		printofile(root->right,op);
	}
}

void dotDump(NODE root,FILE *op){
	op=fopen("two.dot","w");
	fprintf(op,"digraph two{\n");
	printofile(root,op);
	fprintf(op,"}\n");
	fclose(op);
	system("dot -Tps -O two.dot");
	system("xdg-open two.dot.ps &");

}

int main(){
	int ele,ch,n;
	NODE root=NULL,temp1=NULL,temp2=NULL;
	FILE *fp;
	//INTERFACE
	while(1){
		printf("\nEnter the choice\n\n1.Insert the required element and display\n\n2.Insert a required number of random elements");
		printf(" and display\n\n3.Inorder display\n\n4.Exit\n\n");
		scanf("%d",&ch);
		switch(ch){
			case 1: printf("Enter the element != zero\n");
				scanf("%d",&ele);
				ele=insert(ele,root,&temp1,&temp2);
				if(ele){
					root=getnode();
					root->k1=ele;
					root->left=temp1;
					root->right=temp2;	
				}
				dotDump(root,fp);
				break;
			case 2:	printf("Enter the required number of random elements\n");
				scanf("%d",&n);
				while(n--){
					ele=rand()%1234;
					if(ele)
						ele=insert(ele,root,&temp1,&temp2);
					if(ele){
						root=getnode();
						root->k1=ele;
						root->left=temp1;
						root->right=temp2;
					}
				}
				dotDump(root,fp);
				break;
			case 3:	display(root);
				break;
			default:printf("exit\n");
				exit(0);
		}
	}

	return 0;
}
