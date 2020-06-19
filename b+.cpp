#include<bits/stdc++.h>
using namespace std;
#define MAX 50
struct Block
{
    int NumberOfNodes;
    Block *parentBlock;
    int value[MAX];
    Block *childBlock[MAX];
    Block()
	{
        NumberOfNodes = 0;
        parentBlock = NULL;
        for(int i=0; i<MAX; i++)
		{
            value[i] = INT_MAX;
            childBlock[i] = NULL;
        }
    }
};
int numberOfPointers;
Block *rootBlock = new Block();
void splitLeaf(Block *CurrentBlock)
{
    int x, i, j;

   
    if(numberOfPointers%2)
        x = (numberOfPointers-1)/2;
    else x = numberOfPointers/2;

    Block *rightBlock = new Block();
    CurrentBlock->NumberOfNodes = x;
    rightBlock->NumberOfNodes = numberOfPointers-x;
    rightBlock->parentBlock = CurrentBlock->parentBlock;

    for(i=x, j=0; i<numberOfPointers; i++, j++)
	{
        rightBlock->value[j] = CurrentBlock->value[i];
        CurrentBlock->value[i] = INT_MAX;
    }

    int val = rightBlock->value[0];

    //if the leaf itself is a parent then
    if(CurrentBlock->parentBlock==NULL)
	{
        Block *parentBlock = new Block();
        parentBlock->parentBlock = NULL;
        parentBlock->NumberOfNodes=1;
        parentBlock->value[0] = val;
        parentBlock->childBlock[0] = CurrentBlock;
        parentBlock->childBlock[1] = rightBlock;
        CurrentBlock->parentBlock = rightBlock->parentBlock = parentBlock;
        rootBlock = parentBlock;
        return;
    }
    else
	{   //if the splitted leaf block is not rootBlock then
        CurrentBlock = CurrentBlock->parentBlock;

        Block *newChildBlock = new Block();
        newChildBlock = rightBlock;

        for(i=0; i<=CurrentBlock->NumberOfNodes; i++)
		{
            if(val < CurrentBlock->value[i])
			{
                swap(CurrentBlock->value[i], val);
            }
        }
        CurrentBlock->NumberOfNodes++;

        for(i=0; i<CurrentBlock->NumberOfNodes; i++){
            if(newChildBlock->value[0] < CurrentBlock->childBlock[i]->value[0]){
                swap(CurrentBlock->childBlock[i], newChildBlock);
            }
        }
        CurrentBlock->childBlock[i] = newChildBlock;

        for(i=0;CurrentBlock->childBlock[i]!=NULL;i++){
            CurrentBlock->childBlock[i]->parentBlock = CurrentBlock;
        }
    }

}
void print(vector < Block* > Blocks)
{
    vector < Block* > newBlocks;
    for(int i=0; i<Blocks.size(); i++)
	{
        Block *CurrentBlock = Blocks[i];
        cout <<"[|";
        int j;
        for(j=0; j<CurrentBlock->NumberOfNodes; j++)
		{
            cout << CurrentBlock->value[j] << "|";
            if(CurrentBlock->childBlock[j]!=NULL)
            newBlocks.push_back(CurrentBlock->childBlock[j]);
        }
        if(CurrentBlock->value[j]==INT_MAX && CurrentBlock->childBlock[j]!=NULL)
            newBlocks.push_back(CurrentBlock->childBlock[j]);

        cout << "]  ";
    }

    if(newBlocks.size()==0)
	{
        puts("");
        puts("");
        Blocks.clear();
    }
    else
	{ 
        puts("");
        puts("");
        Blocks.clear();
        print(newBlocks);
    }
}
//function to split the non leaf nodes
void splitNonLeaf(Block *CurrentBlock)
{
    int x, i, j;
	if (x%2 == 1)
    	x = ((numberOfPointers - 1) / 2);
    else
    	x = (numberOfPointers/2);

    Block *rightBlock = new Block();
    CurrentBlock->NumberOfNodes = x;

    rightBlock->NumberOfNodes = numberOfPointers-x-1;
    rightBlock->parentBlock = CurrentBlock->parentBlock;

    for(i=x, j=0; i<=numberOfPointers; i++, j++)
	{
        rightBlock->value[j] = CurrentBlock->value[i];
        rightBlock->childBlock[j] = CurrentBlock->childBlock[i];
        CurrentBlock->value[i] = INT_MAX;
        if(i!=x)CurrentBlock->childBlock[i] = NULL;
    }

    int val = rightBlock->value[0];

    memcpy(&rightBlock->value, &rightBlock->value[1], sizeof(int)*(rightBlock->NumberOfNodes+1));
    memcpy(&rightBlock->childBlock, &rightBlock->childBlock[1], sizeof(rootBlock)*(rightBlock->NumberOfNodes+1));

    for(i=0;CurrentBlock->childBlock[i]!=NULL;i++)
	{
        CurrentBlock->childBlock[i]->parentBlock = CurrentBlock;
    }
    for(i=0;rightBlock->childBlock[i]!=NULL;i++)
	{
        rightBlock->childBlock[i]->parentBlock = rightBlock;
    }

    //if the splitted block itself a parent
    if(CurrentBlock->parentBlock==NULL)
	{
        Block *parentBlock = new Block();
        parentBlock->parentBlock = NULL;
        parentBlock->NumberOfNodes=1;
        parentBlock->value[0] = val;
        parentBlock->childBlock[0] = CurrentBlock;
        parentBlock->childBlock[1] = rightBlock;
        CurrentBlock->parentBlock = rightBlock->parentBlock = parentBlock;
        rootBlock = parentBlock;
        return;
    }
    else
	{   
        CurrentBlock = CurrentBlock->parentBlock;

        Block *newChildBlock = new Block();
        newChildBlock = rightBlock;

        for(i=0; i<=CurrentBlock->NumberOfNodes; i++)
		{
            if(val < CurrentBlock->value[i])
			{
                swap(CurrentBlock->value[i], val);
            }
        }
        CurrentBlock->NumberOfNodes++;

        for(i=0; i<CurrentBlock->NumberOfNodes; i++)
		{
            if(newChildBlock->value[0] < CurrentBlock->childBlock[i]->value[0])
			{
                swap(CurrentBlock->childBlock[i], newChildBlock);
            }
        }
        CurrentBlock->childBlock[i] = newChildBlock;

        for(i=0;CurrentBlock->childBlock[i]!=NULL;i++)
		{
           CurrentBlock->childBlock[i]->parentBlock = CurrentBlock;
        }
    }

}

void insertNode(Block *CurrentBlock, int val)
{
    for(int i=0; i<=CurrentBlock->NumberOfNodes; i++)
	{
        if(val < CurrentBlock->value[i] && CurrentBlock->childBlock[i]!=NULL)
		{
            insertNode(CurrentBlock->childBlock[i], val);
            if(CurrentBlock->NumberOfNodes==numberOfPointers)
                splitNonLeaf(CurrentBlock);
            return;
        }
        else if(val < CurrentBlock->value[i] && CurrentBlock->childBlock[i]==NULL)
		{
            swap(CurrentBlock->value[i], val);
            if(i==CurrentBlock->NumberOfNodes)
			{
                CurrentBlock->NumberOfNodes++;
                break;
            }
        }
    }

    if(CurrentBlock->NumberOfNodes==numberOfPointers)
	{
            splitLeaf(CurrentBlock);
    }
}


void redistributeBlock(Block *leftBlock, Block *rightBlock, bool isLeaf, int posOfLeftBlock, int whichOneisCurrentBlock)
{
    int PrevRightFirstVal = rightBlock->value[0];

    if(whichOneisCurrentBlock==0)
	{ 
        if(!isLeaf)
		{
            leftBlock->value[leftBlock->NumberOfNodes] = leftBlock->parentBlock->value[posOfLeftBlock];
            leftBlock->childBlock[leftBlock->NumberOfNodes+1] = rightBlock->childBlock[0];
            leftBlock->NumberOfNodes++;
            leftBlock->parentBlock->value[posOfLeftBlock] = rightBlock->value[0];
            
            memcpy(&rightBlock->value[0], &rightBlock->value[1], sizeof(int)*(rightBlock->NumberOfNodes+1));
            memcpy(&rightBlock->childBlock[0], &rightBlock->childBlock[1], sizeof(rootBlock)*(rightBlock->NumberOfNodes+1));
            rightBlock->NumberOfNodes--;

        }
		else
		{
            leftBlock->value[leftBlock->NumberOfNodes] = rightBlock->value[0];
            leftBlock->NumberOfNodes++;
            
            memcpy(&rightBlock->value[0], &rightBlock->value[1], sizeof(int)*(rightBlock->NumberOfNodes+1));

            rightBlock->NumberOfNodes--;
            leftBlock->parentBlock->value[posOfLeftBlock] = rightBlock->value[0];
        }
    }
	else
	{ //rightBlock is CurrentBlock

        if(!isLeaf)
		{
            memcpy(&rightBlock->value[1], &rightBlock->value[0], sizeof(int)*(rightBlock->NumberOfNodes+1));
            memcpy(&rightBlock->childBlock[1], &rightBlock->childBlock[0], sizeof(rootBlock)*(rightBlock->NumberOfNodes+1));
            
            rightBlock->value[0] = leftBlock->parentBlock->value[posOfLeftBlock];
            rightBlock->childBlock[0] = leftBlock->childBlock[leftBlock->NumberOfNodes];
            rightBlock->NumberOfNodes++;

            leftBlock->parentBlock->value[posOfLeftBlock] = leftBlock->value[leftBlock->NumberOfNodes-1];
            leftBlock->value[leftBlock->NumberOfNodes-1] = INT_MAX;
            leftBlock->childBlock[leftBlock->NumberOfNodes] = NULL;
            leftBlock->NumberOfNodes--;
        }
		else
		{
            memcpy(&rightBlock->value[1], &rightBlock->value[0], sizeof(int)*(rightBlock->NumberOfNodes+1));

            rightBlock->value[0] = leftBlock->value[leftBlock->NumberOfNodes-1];
            rightBlock->NumberOfNodes++;

            leftBlock->value[leftBlock->NumberOfNodes-1] = INT_MAX;
            leftBlock->NumberOfNodes--;
            leftBlock->parentBlock->value[posOfLeftBlock] = rightBlock->value[0];
        }
    }
}

void mergeBlock(Block *leftBlock, Block *rightBlock, bool isLeaf, int posOfRightBlock)
{
    if(!isLeaf)
	{
        leftBlock->value[leftBlock->NumberOfNodes] = leftBlock->parentBlock->value[posOfRightBlock-1];
        leftBlock->NumberOfNodes++;
    }
    memcpy(&leftBlock->value[leftBlock->NumberOfNodes], &rightBlock->value[0], sizeof(int)*(rightBlock->NumberOfNodes+1));
    memcpy(&leftBlock->childBlock[leftBlock->NumberOfNodes], &rightBlock->childBlock[0], sizeof(rootBlock)*(rightBlock->NumberOfNodes+1));

    leftBlock->NumberOfNodes += rightBlock->NumberOfNodes;
    
    memcpy(&leftBlock->parentBlock->value[posOfRightBlock-1], &leftBlock->parentBlock->value[posOfRightBlock], sizeof(int)*(leftBlock->parentBlock->NumberOfNodes+1));
    memcpy(&leftBlock->parentBlock->childBlock[posOfRightBlock], &leftBlock->parentBlock->childBlock[posOfRightBlock+1], sizeof(rootBlock)*(leftBlock->parentBlock->NumberOfNodes+1));
    leftBlock->parentBlock->NumberOfNodes--;

    for(int i=0;leftBlock->childBlock[i]!=NULL;i++)
	{
        leftBlock->childBlock[i]->parentBlock = leftBlock;
    }
}

bool dataFound;

void deleteNode(Block *CurrentBlock, int val, int CurrentBlockPosition)
{
    bool isLeaf;
    if(CurrentBlock->childBlock[0]==NULL)
       isLeaf = true;
    else isLeaf = false;
     
    int prevLeftMostVal = CurrentBlock->value[0];
    
    for(int i=0;dataFound==false &&  i<=CurrentBlock->NumberOfNodes; i++)
	{
        if(val < CurrentBlock->value[i] && CurrentBlock->childBlock[i] != NULL)
		{
            deleteNode(CurrentBlock->childBlock[i], val, i);

        }
        else if(val == CurrentBlock->value[i] && CurrentBlock->childBlock[i] == NULL)
		{
            memcpy(&CurrentBlock->value[i], &CurrentBlock->value[i+1], sizeof(int)*(CurrentBlock->NumberOfNodes+1));
            CurrentBlock->NumberOfNodes--;
            dataFound = true;
            break;
        }
     }

    //if the root is the only leaf
    if(CurrentBlock->parentBlock == NULL && CurrentBlock->childBlock[0] == NULL)
	{
       return;
    }


    if(CurrentBlock->parentBlock==NULL && CurrentBlock->childBlock[0] != NULL && CurrentBlock->NumberOfNodes == 0)
	{
        rootBlock = CurrentBlock->childBlock[0];
        rootBlock->parentBlock = NULL;
        return;
    }
    
    if(isLeaf && CurrentBlock->parentBlock!=NULL)
	{
        if(CurrentBlockPosition==0)
		{
            Block *rightBlock = new Block();
            rightBlock = CurrentBlock->parentBlock->childBlock[1];
            
            if(rightBlock!=NULL && rightBlock->NumberOfNodes > (numberOfPointers+1)/2)
			{
                    redistributeBlock(CurrentBlock, rightBlock, isLeaf, 0, 0);
            }
            else if (rightBlock!=NULL && CurrentBlock->NumberOfNodes+rightBlock->NumberOfNodes < numberOfPointers)
			{
                    mergeBlock(CurrentBlock, rightBlock, isLeaf, 1);
            }
        }
        else
		{
            Block *leftBlock = new Block();
            Block *rightBlock = new Block();

            leftBlock = CurrentBlock->parentBlock->childBlock[CurrentBlockPosition-1];
            rightBlock = CurrentBlock->parentBlock->childBlock[CurrentBlockPosition+1];
            
            if(leftBlock!=NULL && leftBlock->NumberOfNodes > (numberOfPointers+1)/2)
			{
                 redistributeBlock(leftBlock, CurrentBlock, isLeaf, CurrentBlockPosition-1, 1);
            }
            else if(rightBlock!=NULL && rightBlock->NumberOfNodes > (numberOfPointers+1)/2)
			{
                redistributeBlock(CurrentBlock, rightBlock, isLeaf, CurrentBlockPosition, 0);
            }
            else if (leftBlock!=NULL && CurrentBlock->NumberOfNodes+leftBlock->NumberOfNodes < numberOfPointers)
			{
                mergeBlock(leftBlock, CurrentBlock, isLeaf, CurrentBlockPosition);
            }
            else if (rightBlock!=NULL && CurrentBlock->NumberOfNodes+rightBlock->NumberOfNodes < numberOfPointers)
			{
               mergeBlock(CurrentBlock, rightBlock, isLeaf, CurrentBlockPosition+1);
            }
        }
    }
    else if(!isLeaf && CurrentBlock->parentBlock!=NULL)
	{
        if(CurrentBlockPosition==0)
		{
            Block *rightBlock = new Block();
            rightBlock = CurrentBlock->parentBlock->childBlock[1];

            if( rightBlock!=NULL && rightBlock->NumberOfNodes-1 >= ceil((numberOfPointers-1)/2))
			{
                redistributeBlock(CurrentBlock, rightBlock, isLeaf, 0, 0);
            }
            else if (rightBlock!=NULL && CurrentBlock->NumberOfNodes+rightBlock->NumberOfNodes < numberOfPointers - 1)
			{
                mergeBlock(CurrentBlock, rightBlock, isLeaf, 1);
            }
        }
        else
		{
            Block *leftBlock = new Block();
            Block *rightBlock = new Block();

            leftBlock = CurrentBlock->parentBlock->childBlock[CurrentBlockPosition-1];
            rightBlock = CurrentBlock->parentBlock->childBlock[CurrentBlockPosition+1];

            if( leftBlock!=NULL && leftBlock->NumberOfNodes-1 >= ceil((numberOfPointers-1)/2))
			{
                redistributeBlock(leftBlock, CurrentBlock, isLeaf, CurrentBlockPosition-1, 1);
            }
            else if(rightBlock!=NULL && rightBlock->NumberOfNodes-1 >=  ceil((numberOfPointers-1)/2))
			{
                redistributeBlock(CurrentBlock, rightBlock, isLeaf, CurrentBlockPosition, 0);
            }
            else if ( leftBlock!=NULL && CurrentBlock->NumberOfNodes+leftBlock->NumberOfNodes < numberOfPointers-1)
			{
                mergeBlock(leftBlock, CurrentBlock, isLeaf, CurrentBlockPosition);
            }
            else if ( rightBlock!=NULL && CurrentBlock->NumberOfNodes+rightBlock->NumberOfNodes < numberOfPointers-1)
			{
               mergeBlock(CurrentBlock, rightBlock, isLeaf, CurrentBlockPosition+1);
            }
        }
    }

    //delete the duplicate.
    Block *tempBlock = new Block();
    tempBlock = CurrentBlock->parentBlock;
    while(tempBlock!=NULL)
	{
            for(int i=0; i<tempBlock->NumberOfNodes;i++)
			{
                if(tempBlock->value[i]==prevLeftMostVal)
				{
                    tempBlock->value[i] = CurrentBlock->value[0];
                    break;
                }
        }
        tempBlock = tempBlock->parentBlock;
    }
}


int main(){
    int num[50];
    
    printf("Enter order of the tree: ");
    scanf("%d", &numberOfPointers);

    vector < Block* > Blocks;

    int ch;
    int i = 0;
    int totalValues = 0;

    printf("1.Insert a value\n2.Print the tree\n3.Delete a value\n");

     while(scanf("%d", &ch)!=4){


        if(ch==1){
            scanf("%d", &num[i]);
            printf("------Insert %d-----\n\n", num[i]);
            insertNode(rootBlock, num[i]);
            i++;
            totalValues++;

        }else if(ch==2){
            Blocks.clear();
            Blocks.push_back(rootBlock);
            print(Blocks);
            puts("");

        }else if(ch==3) {
            int val;
            scanf("%d", &val);
            if(totalValues==0) {
                printf("Cannot delete further since there is no more data to be deleted!");
                continue;
            }
            printf("\\\\\\\\\\Delete %d \\\\\\\\\n\n", val);
            dataFound = false;
            deleteNode(rootBlock, val, 0);
            totalValues--;
        }
    }

    return 0;
}

