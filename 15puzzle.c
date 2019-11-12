#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 4
#define NxN (N*N)
#define TRUE 1
#define FALSE 0

struct node {
	int tiles[N][N];
	int f, g, h;
	short zero_row, zero_column;	/* location (row and colum) of blank tile 0 */
	struct node *next;
	struct node *parent;			/* used to trace back the solution */
};

int goal_rows[NxN];
int goal_columns[NxN];
struct node *start,*goal;
struct node *open = NULL, *closed = NULL;
struct node *succ_nodes[4];

void print_a_node(struct node *pnode) {
	int i,j;
	for (i=0;i<N;i++) {
		for (j=0;j<N;j++) 
			printf("%2d ", pnode->tiles[i][j]);
		printf("\n");
	}
	printf("\n");
}

struct node *initialize(char **argv){
	int i,j,k,index, tile;
	struct node *pnode;

	pnode=(struct node *) malloc(sizeof(struct node));
	index = 1;
	for (j=0;j<N;j++)
		for (k=0;k<N;k++) {
			tile=atoi(argv[index++]);
			pnode->tiles[j][k]=tile;
			if(tile==0) {
				pnode->zero_row=j;
				pnode->zero_column=k;
			}
		}
	pnode->f=0;
	pnode->g=0;
	pnode->h=0;
	pnode->next=NULL;
	pnode->parent=NULL;
	start=pnode;
	printf("initial state\n");
	print_a_node(start);

	pnode=(struct node *) malloc(sizeof(struct node));
	goal_rows[0]=3;
	goal_columns[0]=3;

	for(index=1; index<NxN; index++){
		j=(index-1)/N;
		k=(index-1)%N;
		goal_rows[index]=j;
		goal_columns[index]=k;
		pnode->tiles[j][k]=index;
	}
	pnode->tiles[N-1][N-1]=0;	      /* empty tile=0 */
	pnode->f=0;
	pnode->g=0;
	pnode->h=0;
	pnode->next=NULL;
	goal=pnode; 
	printf("goal state\n");
	print_a_node(goal);

	return start;
}

/* merge unrepeated nodes into open list after filtering */
void merge_to_open() { 
	struct node *temp;
    struct node *curr;
    struct node *innode;
    for (int i=0;i<4;i++) {
		curr = open;
        temp = open;

        if (succ_nodes[i] != NULL) {
            innode=(struct node *) malloc(sizeof(struct node));
            for(int j = 0; j < N; j++){
                for(int k = 0; k < N; k++) {
                    innode->tiles[j][k]=succ_nodes[i]->tiles[j][k];
                    }
                }
            innode->zero_row = succ_nodes[i]->zero_row;
            innode->zero_column = succ_nodes[i]->zero_column;
            innode->parent = succ_nodes[i]->parent;
            innode->f = succ_nodes[i]->f;
            innode->g = succ_nodes[i]->g;
            innode->h = succ_nodes[i]->h;
            // insert at beginning if smallest
            if (curr == NULL) {
                open = innode;
            } 
            else if (innode->f <= curr->f) {
                if (innode->h <= curr->h) {
                    innode->next = curr;
                    open = innode;
                } 
                else {
                    while (TRUE) {
                        temp = curr;
                        curr = curr->next;
                        if (curr == NULL) {
                            temp->next = innode;
                            innode->next = NULL;
                            break;
                        }
                        if (innode->h <= curr->h) {
                            innode->next = curr;
                            temp->next = innode;
                            break;
                        }
                    }
                }
            } 
            else { // if not smallest
            while (TRUE) {
                temp = curr;
                curr = curr->next;
                if (curr == NULL) {
                    temp->next = innode;
                    innode->next = NULL;
                    break;
                    }
                    if (innode->f <= curr->f) {
                        innode->next = curr;
                        temp->next = innode;
                        break;
                    }
                }
            }
        }
    }
}

/*swap two tiles in a node*/
void swap(int row1,int column1,int row2,int column2, struct node * pnode){
	int temp;
	temp = pnode->tiles[row1][column1];
    pnode->tiles[row1][column1] = pnode->tiles[row2][column2];
	pnode->tiles[row2][column2] = temp;
}

int manhattan(struct node * pnode) {

    int sum = 0;

    int ourValue = 0;

    int fbool = 0;

    // for each point, finding distance to

    // rest of the point

    for (int i = 0; i < N; i++) {

        for (int j = 0; j < N; j++) {

 

            // grab our value

            ourValue = pnode->tiles[i][j];

           fbool = 0;

            for (int m = 0; m < N; m++) {

                for (int n = 0; n < N; n++) {

 

                    // found match

                    if (ourValue == goal->tiles[m][n]) {

                        sum += (abs(i - m) + abs(j - n));

                        fbool = 1;

                        break;

                    }

                }

                if (fbool) {

                    break;

                }

            }

        }

    }

    return sum;

}

/*update the f,g,h function values for a node */
void update_fgh(struct node *pnode){

    // call manhattan distance for h

    pnode->h = manhattan(pnode);

    // check height of list for g

    pnode->g = pnode->parent->g+1;

    // add both for f

    pnode->f = pnode->g + pnode->h;

}

 

/* 0 goes down by a row */
void move_down(struct node * pnode){
	if(pnode->zero_row+1 < N) {
        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row+1, pnode->zero_column, pnode);
        pnode->zero_row++;
    }
	else {
        pnode = NULL;
    }
}

/* 0 goes right by a column */
void move_right(struct node * pnode){
	if(pnode->zero_column+1 < N) {
        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column+1, pnode);
        pnode->zero_column++;
    }
	else {
        pnode = NULL;
    }
}

/* 0 goes up by a row */
void move_up(struct node * pnode){
	if(pnode->zero_row-1 > -1) {
        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row-1, pnode->zero_column, pnode);
        pnode->zero_row--;
    }
	else {
        pnode = NULL;
    }
}

/* 0 goes left by a column */
void move_left(struct node * pnode){
	if(pnode->zero_column-1 > -1) {
        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column - 1, pnode);
        pnode->zero_column--;
    }
	else {
        pnode = NULL;
    }
}

/* expand a node, get its children nodes, and organize the children nodes using
 * array succ_nodes.
 */

void expand(struct node *selected) {

    for(int i = 0; i < N; i++) {
		succ_nodes[i]=NULL;
		}
    int j = selected->zero_row;
    int k = selected->zero_column;

    move_down(succ_nodes[0]);
    move_right(succ_nodes[1]);
    move_up(succ_nodes[2]);
    move_left(succ_nodes[3]);

    for(int i = 0; i < N; i++){
        update_fgh(succ_nodes[i]);
    }
}

int nodes_same(struct node *a,struct node *b) {
	int flg=FALSE;
	if (memcmp(a->tiles, b->tiles, sizeof(int)*NxN) == 0)
		flg=TRUE;
	return flg;
}

/* Filtering. Some nodes in succ_nodes may already be included in either open 
 * or closed list. Remove them. It is important to reduce execution time.
 * This function checks the (i)th node in succ_nodes array. You must call this
 & function in a loop to check all the nodes in succ_nodes.
 */ 
void filter(int i, struct node *pnode_list){ 
	struct node *temp;
	temp = pnode_list;
    if (succ_nodes[i] != NULL) {
		while (temp != NULL) {
			if (nodes_same(succ_nodes[i], temp)) {
				if (succ_nodes[i]->f >= temp->f) {
					succ_nodes[i] = NULL;
					break;
                }
            }
			temp = temp->next;
		}
	}
}


int main(int argc,char **argv) {
	int iter,cnt;
	struct node *copen, *cp, *solution_path;
	int ret, i, pathlen=0, index[N-1];

	solution_path=NULL;
	start=initialize(argv);	/* init initial and goal states */
	open=start; 

	iter=0; 
	while (open!=NULL) {	/* Termination cond with a solution is tested in expand. */
		copen=open;
		open=open->next;  /* get the first node from open to expand */
		if(nodes_same(copen,goal)){ /* goal is found */
			do{ /* trace back and add the nodes on the path to a list */
				copen->next=solution_path;
				solution_path=copen;
				copen=copen->parent;
				pathlen++;
			} while(copen!=NULL);
			printf("Path (lengh=%d):\n", pathlen); 
			copen=solution_path;
			 /* print out the nodes on the list */
			break;
		}
		expand(copen);       /* Find new successors */
		for(i=0;i<4;i++){
			filter(i,open);
			filter(i,closed);
		}
		merge_to_open(); /* New open list */
		copen->next=closed;
		closed=copen;		/* New closed */
		/* print out something so that you know your 
		 * program is still making progress 
		 */
		iter++;
		if(iter %1000 == 0)
			printf("iter %d\n", iter);
	}
	return 0;
} /* end of main */
