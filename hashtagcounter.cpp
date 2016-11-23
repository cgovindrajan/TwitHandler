//
//  hastagcountr.cpp
//  TwitCount
//
//  Created by govindrajan chandra on 14/11/16.
//  Copyright © 2016 Free. All rights reserved.
//

//Header Session
#include <iostream>
#include<map>
#include<string>
#include<math.h>
#include<queue>
#include<fstream>
#include <iostream>
using namespace std;

/* Structure Declration of Fibnonaci Heap
 TwitHandler  -> stores the number of occurance of twit hashtag
 Value        -> stores the number of occurance of twit hashtag
 Degree       -> define the number of children of the node
 Parent       -> pointer to the parent of the node
 Child        -> points to one the children of the node
 Left Child   -> left child pointer of circular link list
 Right Child  -> right child pointer of circular link list
 hasLostChild -> indicates whether the node has lost any children in past or not <valid value true:false>
 used for cascading cut operation in case of increase operation is performed
 */

struct fibNode {
    string key;
    int value;
    int degree;
    fibNode * parent;
    fibNode * child;
    fibNode * leftChild;
    fibNode * rightChild;
    bool hasLostChild;
};


/*CLass declaration for F-Heap
 Variable :
 numberNode -> defines the total number of nodes in F-heap
 maxElement -> Pointer to the max element in F-Heap
 Function :
 initFibHeap          -> routine to Initialize F-Heap
 insertFibHeap        -> routine to insert new values into F-Heap
 restoreFibHeap       -> routine used to re insert the nodes back in F-heap after
 n remove max Operation to regain all orginal heap values
 joinCircularLinkList -> Used to merge two Nodes into single Double Circular List
 removeMaxElement     -> Remove the key with max occurance
 pairWiseCombine      -> Does Pair wise combine in remove Max operation
 Fibonnaci_link       -> Links two Nodes of same degree to obtain higher level heap
 increaseKey          -> Increase the number of occurance by provided value for HashTag
 childCut             -> Incase the hasLostChild==True then put them in root level list
 cascadeChildCut      -> Do nested child cuts till hasLostChild==false
 Display              -> Display the F-Heap structure for debugging
 */

class fibHeap {
    private :
    int numberNode;
    fibNode * maxElement;
    public :
    fibNode * initFibHeap();
    void insertFibHeap(string key,int value,map<string,fibNode *> &);
    void restoreFibHeap(fibNode *);
    fibNode * joinCircularLinkList(fibNode * x,fibNode * y);
    fibNode * removeMaxElement();
    void pairWiseCombine();
    fibNode* Fibonnaci_link(fibNode* , fibNode* );
    void increaseKey(fibNode * maxElement,int incrValue);
    int childCut(fibNode *,fibNode * ,fibNode *);
    int cascadeChildCut(fibNode *,fibNode *);
    //just for debugging purpose
    void Display();
    
    //simple constructor for the fibHeap
    fibHeap() {
        maxElement =initFibHeap();
    }
};

//Single Global Instance of the Fibnonaci heap
fibHeap fh;
fibNode * maxElement = fh.initFibHeap();


/* initFibHeap
 Usage : Used to initialize the Fibnonaci Heap
 1. Create maxElementPtr=NULL
 return maxElementPtr
 */

fibNode * fibHeap::initFibHeap() {
    fibNode * nodePointer;
    nodePointer = NULL;
    return nodePointer;
}

/* Insert Algorithm :
 Usage : Used to insert a new node into Fibnonaci Heap
 1. Create temp node with all required values as required
 2.a if maxElement is null then set maxElement=temp
 2.b else insert temp maxElement List
 3. if temp is greater than current MaxElement than set maxElement as temp
 */

void fibHeap::insertFibHeap(string key,int value,map<string,fibNode *> & HashMap) {
    //1
    fibNode * temp = new fibNode;
    temp->value=value;
    temp->key=key;
    temp->degree=0;
    temp->parent=NULL;
    temp->child=NULL;
    temp->leftChild=temp;
    temp->rightChild=temp;
    temp->hasLostChild=false;
    HashMap[key]=temp;
    //2.b
    if(maxElement){
        (maxElement->leftChild)->rightChild=temp;
        temp->rightChild=maxElement;
        temp->leftChild=maxElement->leftChild;
        maxElement->leftChild=temp;
        //3
        if(temp->value > maxElement->value)
            maxElement=temp;
    }
    //2.a
    else {
        maxElement=temp;
    }
    numberNode++;
}

/* Restore Algorithm :
 Usage : Used to insert the node which were removed as removeMax operation during n top twit calculation
 1. if maxElement is null then set maxElement=temp
 2. else insert temp maxElement List
 2.a if temp is greater than current MaxElement than set maxElement as temp
 */

void fibHeap::restoreFibHeap(fibNode * temp) {
    //2.a
    if(maxElement){
        (maxElement->leftChild)->rightChild=temp;
        temp->rightChild=maxElement;
        temp->leftChild=maxElement->leftChild;
        maxElement->leftChild=temp;
        //2.b
        if(temp->value > maxElement->value)
            maxElement=temp;
    }
    //1
    else {
        maxElement=temp;
    }
}

/* Join Double ended Circular List Algorithm :
 Usage : Used to merge given two nodes into one
 Let y be node to be merged into x
 1. Insert y as the rightmost child in x List
 2.      Set Y left and right pointer to complete the circular list
 3.      if Y has greater value than x then set x as new max element
 4. return x
 */

fibNode * fibHeap::joinCircularLinkList(fibNode * xNode,fibNode * yNode) {
    //Check whether heap is empty or not
    (xNode->leftChild)->rightChild=yNode;
    yNode->rightChild=xNode;
    yNode->leftChild=xNode->leftChild;
    xNode->leftChild=yNode;
    if(yNode->value > xNode->value)
        xNode=yNode;
    
    return xNode;
}

/*
 Remove Max Algorithm
 Usage : Used to remove the Node with max Element from Fibnonaci Heap
 
 * 1. currMax = fh.maxElement
 * 2. if currMax != NULL
 * 2.a 	for each child currMaxChild of currMax
 * 2.b 		add currMaxChild to the root list of fh
 * 2.c 		currMaxChild.parent = NULL
 * 3. remove currMax from the root list of fh
 * 3.a		if currMax == currMax.right
 * 3.b 		fh.max = NULL
 * 3.c 	    else fh.max = currMax.right
 * 3.d      pairWiseCombine(maxElement)
 * 4. fh.numberNode--
 * 5. return currMax
 */

fibNode * fibHeap::removeMaxElement()
{
    fibNode *currMax, *currMaxChild, *next;
    fibNode ** childList;
    
    // 1
    currMax = maxElement;
    // 2
    if ( currMax != nullptr )
    {
        // 2.a
        currMaxChild = currMax->child;
        if ( currMaxChild != NULL )
        {
            childList = new fibNode*[currMax->degree];
            next = currMaxChild;
            for ( int i = 0; i < (int)currMax->degree; i++ )
            {
                childList[i] = next;
                next = next->rightChild;
            }
            for ( int i = 0; i < (int)currMax->degree; i++ )
            {
                currMaxChild = childList[i];
                // 2.b
                maxElement->leftChild->rightChild = currMaxChild;
                currMaxChild->leftChild = maxElement->leftChild;
                maxElement->leftChild = currMaxChild;
                currMaxChild->rightChild = maxElement;
                // 2.c
                currMaxChild->parent = nullptr;
            }
            delete [] childList;
        }
        // 3
        currMax->leftChild->rightChild = currMax->rightChild;
        currMax->rightChild->leftChild = currMax->leftChild;
        currMax->child=NULL;
        currMax->degree=0;
        // 3.a
        if ( currMax == currMax->rightChild )
        {
            // 3.b
            maxElement = NULL;
        }
        else
        {
            // 3.c
            maxElement = currMax->rightChild;
            // 3.d
            pairWiseCombine();
        }
        // 4
        //numberNode--;
    }
    // 5
    return currMax;
}

/*
 pairWiseCombine
 Usage : Combine all nodes of same degree following remove Max Operation
 * 1. let arrList[0 . . D(H.n)] be a new array used to store degree
 * 2. for i = 0 to fh.numberNodes
 * 2.a 	arrList[i] = NULL
 * 3. for each node w in the root list of fh
 * 3.a 	x = w
 * 3.b 	d = x.degree
 * 3.c 	while arrList[d] != NIL
 * 3.d 		y = A[d]
 * 3.e 		if x.value < y.value
 * 3.f			exchange x with y
 * 3.g 		FIB-HEAP-LINK(fh,y,x)
 * 3.h 		arrList[d] = NULL
 * 3.i 		d = d + 1
 * 3.j 	arrList[d] = x
 * 4. fh.max = NIL
 * 4.a for i = 0 to gh.numberNodes
 * 4.b 	if arrlist[i] != NULL
 * 4.c 		if fh.max == NULL
 * 4.d			create a root list for fh containing just arrList[i]
 * 4.e			fh.max = arrList[i]
 * 4.f 		else insert arrList[i] into fh's root list
 * 4.g			if arrList[i].value > fh.maxElement.value
 * 4.h				fh.max = arrList[i]
 * 5. free(arrList)
 */

void fibHeap::pairWiseCombine()
{
    fibNode* w, * next, * x, * y, * temp;
    fibNode** arrList, ** rootList;
    int d, rootSize;
    int max_degree = static_cast<int>(floor(log(static_cast<double>(numberNode))/log(static_cast<double>(1 + sqrt(static_cast<double>(5)))/2)));
    
    // 1
    arrList = new fibNode*[max_degree+2]; 
    // 2,2.a
    std::fill_n(arrList, max_degree+2, nullptr);
    // 3
    w = maxElement;
    rootSize = 0;
    next = w;
    do
    {
        rootSize++;
        next = next->rightChild;
    } while ( next != w );
    rootList = new fibNode*[rootSize];
    for ( int i = 0; i < rootSize; i++ )
    {
        rootList[i] = next;
        next = next->rightChild;
    }
    for ( int i = 0; i < rootSize; i++ )
    {
        w = rootList[i];
        // 3.a
        x = w;
        // 3.b
        d = x->degree;
        // 3.c
        while ( arrList[d] != nullptr )
        {
            // 3.d
            y = arrList[d];
            // 3.e
            if ( x->value < y->value )
            {
                // 3.f
                temp = x;
                x = y;
                y = temp;
            }
            // 3.g
            x=Fibonnaci_link(y,x);
            // 3.h
            arrList[d] = nullptr;
            // 3.i
            d++;
        }
        // 3.j
        arrList[d] = x;
    }
    delete [] rootList;
    // 4
    maxElement = nullptr;
    // 4.a
    for ( int i = 0; i < max_degree+2; i++ )
    {
        // 4.b
        if ( arrList[i] != nullptr )
        {
            // 4.c
            if ( maxElement == nullptr )
            {
                // 4.d,4.e
                maxElement = arrList[i]->leftChild = arrList[i]->rightChild = arrList[i];
            }
            else
            {
                // 4.f
                maxElement->leftChild->rightChild = arrList[i];
                arrList[i]->leftChild = maxElement->leftChild;
                maxElement->leftChild = arrList[i];
                arrList[i]->rightChild = maxElement;
                // 4.g
                if ( arrList[i]->value > maxElement->value )
                {
                    // 4.h
                    maxElement = arrList[i];
                }
            }
        }
    }
    //5
    delete [] arrList;
}

/*
 fib_heap_link(y,x)
 Usage : Used to merge the node into root List during PairWiseCombine Operation
 * 1. remove y from the root list of heap
 * 2. make y a child of x, incrementing x.degree
 * 3. set y.mark = FALSE
 */

fibNode* fibHeap::Fibonnaci_link( fibNode* y, fibNode* x )
{
    // 1
    y->leftChild->rightChild = y->rightChild;
    y->rightChild->leftChild = y->leftChild;
    // 2
    if ( x->child != nullptr )
    {
        x->child->leftChild->rightChild = y;
        y->leftChild = x->child->leftChild;
        x->child->leftChild = y;
        y->rightChild = x->child;
    }
    else
    {
        x->child = y;
        y->rightChild = y;
        y->leftChild = y;
    }
    y->parent = x;
    x->degree++;
    // 3
    y->hasLostChild = false;
    return x;
}

/*
 Increase_key(ptr,k)
 Usage : Used to increase the value by k of Node pointed by ptr
 * 1. if ptr = nullptr then return NULL
 * 2. else  tempPtr = ptr and tempPtr.value = tempPtr.value + k
 * 3. if tempPtr != NULL and tempPtr.value > ptr.value
 * 4. 	CUT(maxElement,ptr,tempPtr)
 * 5. 	CASCADING-CUT(maxElemenPtr,tempPtr)
 * 6. if ptr.value > maxElement.max.value
 * 7. 	maxElement = ptr
 */

void fibHeap::increaseKey(fibNode * elementPtr,int incrValue){
    fibNode * tempPtr;
    //1
    if(!elementPtr){
        return ;
    }
    //2
    elementPtr->value+=incrValue;
    tempPtr=elementPtr->parent;
    //3
    if(tempPtr!=NULL && elementPtr->value > tempPtr->value){
        //4
        childCut(maxElement,elementPtr,tempPtr);
        //5
        cascadeChildCut(maxElement,tempPtr);
    }
    //6
    if(elementPtr->value > maxElement->value)
        maxElement=elementPtr;
    
}

/*
 childCut(x,y,z)
 Used : used in increase key routine to remove node and adjust Heap to retain integrity
 * 1. remove x from the child list of y, decrementing y.degree
 * 2. add x to the root list of H
 * 3. x.p = NULL
 * 4. x.mark = FALSE
 */

int fibHeap::childCut(fibNode * maxElementPtr,fibNode * elementPtr,fibNode * tempPtr){
    //1
    if(elementPtr==elementPtr->rightChild)
        tempPtr->child=NULL;
    
    (elementPtr->leftChild)->rightChild = elementPtr->rightChild;
    (elementPtr->rightChild)->leftChild = elementPtr->leftChild;
    if (elementPtr == tempPtr->child)
        tempPtr->child = elementPtr->rightChild;
    tempPtr->degree--;
    //2
    elementPtr->leftChild=elementPtr;
    elementPtr->rightChild=elementPtr;
    (maxElementPtr->leftChild)->rightChild=elementPtr;
    elementPtr->rightChild=maxElementPtr;
    elementPtr->leftChild=maxElementPtr->leftChild;
    maxElementPtr->leftChild=elementPtr;
    //3
    elementPtr->parent = NULL;
    //4
    elementPtr->hasLostChild = false;
    
    return 0;
}

/*
 Cascading_cut(x,y)
 Used : Used to check parents node along the path from increase value @ node to maxElement
 * 1. z = y.p
 * 2. if z != NIL
 * 3. 	if y.mark == FALSE
 * 4. 		y.mark = TRUE
 * 5. 	else childCut(H,y,z)
 * 6. 		Cascading_cut(H,z)
 */

int fibHeap::cascadeChildCut(fibNode * maxElementPtr,fibNode * elementPtr){
    //1
    fibNode * temp=elementPtr->parent;
    if(temp){
        //2
        if(!temp->hasLostChild)
            //3
            temp->hasLostChild=true;
        //4
        else {
            childCut(maxElementPtr,elementPtr,temp);
            cascadeChildCut(maxElementPtr, temp);
        }
    }
    return 0;
}


//To be remove
void fibHeap::Display()
{
    queue<fibNode *> Queue;
    fibNode * p = maxElement;
    fibNode * x = maxElement;
    
    
    if (p == NULL)
    {
        cout<<"The Heap is Empty"<<endl;
        return;
    }
    cout<<"The root nodes of Heap are: "<<endl;
    Queue.push(maxElement);
    while(!Queue.empty()){
        p = x = Queue.front();
        Queue.pop();
        do {
            if(p!=NULL){
                cout<<p->key<<"("<<p->value<<")";
                if(p->child){
                    Queue.push(p->child);
                    cout<<"(Y)";
                }
                p=p->rightChild;
            }
            if(p!=maxElement)
                cout<<"->";
        }
        while(p!=x && p!= NULL);
        cout<<endl;
    }
    
}


/*
 main routine
 Usage : main routine of the program
 1. Set file pointed to input and outputfile
 2. Read the input File
 3. if input is for form <key,value> then check in hashtable with key
 3.a if key is not present in hashtable then call insert routine of F-Heap
 3.b else call increase routine(pointer from hashtable,incremental value) of F-Heap
 4. if input is STOP then nullify the files pointer and exit program
 5. else input is of form value x
 5.a remove MaxElement from F-Heap x times and write into file also push the removed pointer into Queue
 5.b Remove Pointers from Queue till Queue.isempty and call restoreHeap routine to insert them back
 */

//Main routine call of the program
int main(int argc, const char * argv[]) {
    map<string,fibNode *> HashMap;
    queue<fibNode *> Queue;
    fibNode*p;
    string key;
    int value;
    //1
    ifstream ifile(argv[1]);
    ofstream ofile("output_file.txt");
    //2
    while(ifile >> key) {
        //3
        if(key[0]=='#'){
            ifile >> value;
            fibNode * temp = HashMap[key];
            //3.a
            if(!temp)
                fh.insertFibHeap(key,value,HashMap);
            else
                //3.b
                fh.increaseKey(temp, value);
            //fh.Display();
            
        }
        //4
        else if(key!="stop" && key!="STOP")
        {
            //5
            value=stoi(key);
            if(value>0){
                int temp=value;
                //5.a
                p=fh.removeMaxElement();
                Queue.push(p);
                string keyHolder = p->key;
                //ofile << keyHolder.erase(0,1) << "(" << p->value << ")";;
                ofile << keyHolder.erase(0,1); 
		while(--temp){
                    p=fh.removeMaxElement();
                    Queue.push(p);
                    string keyHolder = p->key;
                    ofile << "," << keyHolder.erase(0,1);
                    //ofile << keyHolder.erase(0,1) << "(" << p->value << ")";
                    //cout<<"Delete key : "<<p->key<<" "<<p->value<<endl;
                }
                ofile << endl;
                //5.b
                while(!Queue.empty()){
                    fh.restoreFibHeap(Queue.front());
                    Queue.pop();
                }
                //fh.Display();
            }
        }
        
        else{
            ifile.close();
            ofile.close();
            return 0;
        }
    }
    return 0;
}



