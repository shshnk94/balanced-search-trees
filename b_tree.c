#include<stdio.h>
#include<stdlib.h>

/* an extra cell is reserved only
to manage the overflow conditions
in which the new element is stored 
and later sent for further processing*/

int p, disg;

struct node{

	int *key, count, disl;
	struct node **link; 
};

typedef struct node* NODE;

NODE getnode(){

	int i;
	NODE x;

	x = (NODE) malloc(sizeof(struct node));

	x->key = (int*) malloc(p * sizeof(int));
	x->link = (NODE*) malloc(p*sizeof(NODE));

	for(i=0; i < p; i++){

		x->link[i] = NULL;

		if(i != (p - 1))
			x->key[i] = 0;
	}

	x->disl = disg;
	disg++;
	x->count = 0;

	return x;
}

void nodesort(int a[], int n){

	int i, j, temp, t;

	for(i=0; i < n; i++){

		t=i;

		for(j=i+1; j < n; j++)
			if(a[j] < a[t])
				t=j;

		temp = a[i];
		a[i] = a[t];
		a[t] = temp;
	}

	return;
}

int insert(NODE root, int ele, NODE *t1, NODE *t2){

	int i, j, k, q, l, r;
	NODE temp1 = NULL, temp2 = NULL;

	if(root != NULL){

		for(i=0; (i < root->count) && (ele > root->key[i]); i++);

		k = (p & 1) ? (p / 2) : (p / 2) - 1;

		if((i < k) && (i < root->count)){

			ele = insert(root->link[i], ele, &temp1, &temp2);

			if(ele)
				if(root->count < (p - 1)){

					root->key[root->count] = ele;
					root->count++;

					nodesort(root->key, root->count);

					for(j=root->count; j >= (i + 2); j--)
						root->link[j] = root->link[j-1];

					root->link[i] = temp1;
					root->link[i+1] = temp2;

					return 0;
				}
				else{
					root->key[p-1] = ele;

					nodesort(root->key, p);
		
					*t1 = getnode();
					*t2 = getnode();

					for(l=0, r=0; l < k; l++, r++){

						(*t1)->key[r] = root->key[l];
						(*t1)->count++;
						(*t1)->link[r] = root->link[l];
					}

					(*t2)->link[0] = root->link[k];

					for(l=k+1, r=1; l < p; l++, r++){

						(*t2)->key[r-1] = root->key[l];
						(*t2)->count++;
						(*t2)->link[r] = root->link[l];
					}	

					for(r=(*t1)->count+1; r >= (i + 2); r--)
						(*t1)->link[r] = (*t1)->link[r-1];

					(*t1)->link[r-1] = temp1;
					(*t1)->link[r] = temp2;

					return root->key[k];
				}
		}		
		else if((i > k) || (i == root->count)){

			ele = insert(root->link[i], ele, &temp1, &temp2);

			if(ele)
				if(root->count < (p - 1)){

					root->key[root->count] = ele;
					root->count++;

					nodesort(root->key, root->count);

					for(j=root->count; j >= (i + 2); j--)
						root->link[j] = root->link[j-1];

					root->link[i] = temp1;
					root->link[i+1] = temp2;

					return 0;
				}
				else{
					root->key[p-1] = ele;

					nodesort(root->key, p);
	
					*t1 = getnode();
					*t2 = getnode();

					for(l=0, r=0; l <= k;l++, r++){
						if(l != k){

							(*t1)->key[r] = root->key[l];
							(*t1)->count++;
						}

						(*t1)->link[r] = root->link[l];
					}

					for(l=k+1, r=0; l < p; l++, r++){

						(*t2)->key[r] = root->key[l];
						(*t2)->count++;
						(*t2)->link[r] = root->link[l];
					}

					if(i < root->count){

						for(l=root->count+1, r=(*t2)->count; l >= (i + 2); l--, r--)
							(*t2)->link[r] = (*t2)->link[r-1];

						(*t2)->link[r-1] = temp1;
						(*t2)->link[r] = temp2;
					}
					else{

						(*t2)->link[(*t2)->count-1] = temp1;
						(*t2)->link[(*t2)->count] = temp2;
					}

					return root->key[k];
				}
		}
		else if(i == k){

			ele = insert(root->link[i], ele, &temp1, &temp2);

			if(ele)
				if(root->count < (p - 1)){

					root->key[root->count] = ele;
					root->count++;

					nodesort(root->key, root->count);

					for(j=root->count; j >= (i + 2); j--)
						root->link[j] = root->link[j-1];

					root->link[i] = temp1;
					root->link[i+1] = temp2;

					return 0;
				}
				else{
 
					root->key[p-1] = ele;

					nodesort(root->key, p);	

					*t1 = getnode();
					*t2 = getnode();

					for(l=0, r=0; l <= k; l++, r++){
						if(l != k){

							(*t1)->key[r] = root->key[l];
							(*t1)->count++;	
						}

						(*t1)->link[r] = root->link[l];
					}

					(*t1)->link[(*t1)->count] = temp1;

					for(l=k+1, r=1; l < p; l++, r++){

						(*t2)->key[r-1] = root->key[l];
						(*t2)->count++;	
						(*t2)->link[r] = root->link[l];	
					}		

	  				(*t2)->link[0] = temp2;

					return root->key[k];
				}
		}		
	}

	return ele;
}


void printofile(NODE root, FILE *op){

	int i, k, un;
	
	if(root != NULL){

		//since the tree is balanced only one check is sufficient
		if(root->link[0]==NULL)
			return;

		for(k=0; k <= root->count; k++){

			fprintf(op,"%d[label=\"(",root->disl);

			for(i=0; i < root->count; i++){

				if(i == root->count-1)
					fprintf(op, "%d", root->key[i]);
				else
					fprintf(op, "%d,", root->key[i]);
			}

			fprintf(op, ")\"];");
			fprintf(op, "%d->%d\n", root->disl, (root->link[k])->disl);
			fprintf(op, "%d[label=\"(", (root->link[k])->disl);

			for(i=0; i< (root->link[k])->count; i++){

				if(i == ((root->link[k])->count) - 1)
					fprintf(op, "%d", (root->link[k])->key[i]);
				else
					fprintf(op, "%d,", (root->link[k])->key[i]);
			}

			fprintf(op, ")\"];");
			printofile(root->link[k], op);
		}	
	}
	
	return;
}

void dotDump(NODE root, FILE *op){

	op=fopen("kd.dot", "w");

	fprintf(op,"digraph kd{\n");
	printofile(root, op);
	fprintf(op,"}\n");

	fclose(op);
	system("dot -Tps -O kd.dot");
	system("xdg-open kd.dot.ps &");
}

int search(NODE root, int ele){

	int i, f=0;

	if(root != NULL){
		for(i=0; i <= root->count; i++){

			f = search(root->link[i], ele);

			if(i != root->count){
				if((ele == root->key[i]) && f)
					return 1;
			}
		}
	}

	return 0;
}

/*NODE mergenode(NODE a,NODE b,int med){
	int i,k;
	NODE temp;

	temp=getnode();
	for(i=0,k=0;i<=a->count;i++,k++){
		if(i!=a->count){
			temp->key[k]=a->key[i];
			temp->count++;
		}
		temp->link[k]=a->link[i];	
	}
	temp->key[k++]=ele;
	for(i=0;i<=b->count;i++,k++){
		if(i!=b->count){
			temp->key[k]=b->key[i];
			temp->count++;
		}
		temp->link[k]=b->link[k];
	}

	return temp;
}

int delete(NODE par,NODE root,int ele,int pos,NODE *temp){
	int i,j,f;
	NODE point,new;

	if(root!=NULL){
		for(i=0;i<root->count&&ele!=root->key[i];i++){
			f=delete(root,root->link[i],ele,i,&new);
			if(i){
				
			}
		}
		if(i==root->count)
			f=delete(root,root->link[i],ele,i,&new);
		if(ele==root->key[i]&&root->link[0]==NULL){     //deleting the key in leaf node
			if(!pos||(par->link[pos+1])->count>p/2){
				point=par->link[pos+1];
				root->key[i]=par->key[pos];
				nodesort(root->key,root->count);
				par->key[pos]=point->key[0];
				(point->count)--;
				return 0;
			}
			else if(pos==par->count||(par->link[pos-1])->count>p/2){
				point=par->link[pos-1];
				root->key[i]=par->key[pos-1];
				nodesort(root->key,root->count);
				par->key[pos-1]=point->key[count-1];
				(point->count)--;
				return 0;			
			}
		}
		if(!pos){
			point=par->link[pos+1];
			(*temp)=mergenode(root,point,par->key[pos]);
		}
		else{
			point=par->link[pos-1];
			(*temp)=mergenode(point,root,par->key[pos-1]);
		}	
		return 1;		
		}
	}
}*/
	
int main(){

	NODE root=NULL, temp1=NULL, temp2=NULL;
	int n, ele, i;
	FILE *fp;
	
	printf("Enter the order of the B-tree\n");
	scanf("%d",&p);

	printf("Enter the number of random elements to be entered\n");
	scanf("%d",&n);

	while(n){
		ele = rand() % 1234;

		if(ele && !search(root, ele)){
			
			ele = insert(root, ele, &temp1, &temp2);
			n--;
		}

		if(ele){

			root = getnode();
			root->key[0] = ele;
			root->count++;
			root->link[0] = temp1;
			root->link[1] = temp2;
		}
	}

	dotDump(root,fp);

	return 0;
}

