
//
//  main.cpp
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
using namespace std;

//Node Structure for F-Heap Node
struct fibNode {
    string key;
    int value;//stores the number of occurance of twit hashtag
    int degree;//define the number of children of the node
    fibNode * parent;//points to the parent of the node
    fibNode * child;//points to one the children of the node
    fibNode * leftChild;//left child pointer of circular link list
    fibNode * rightChild;//right child pointer of circular link list
    bool hasLostChild;
};


//CLass declaration for F-Heap

class fibHeap {
    private :
    int numberNode;
    fibNode * maxElement;
    public :
    fibNode * initFibHeap();//routine to Initialize Heap
    fibNode * insertFibHeap(string key,int value,map<string,fibNode *> &);//routine to insert key,value into F-heap
    fibNode * removeMaxElement();//routine to remove max valued key from F-Heap
    void Consolidate();
    fibNode* Fibonnaci_link(fibNode* , fibNode* );
    //increase the value of key by incrValue
    fibNode * increaseKey(fibNode * maxElement,int incrValue);
    int childCut(fibNode *,fibNode * ,fibNode *);//push the new value up in the root CLL
    int cascadeChildCut(fibNode *,fibNode *);//perform cascade childcuts till hasLostChild!=false
    
    //tobe remove
    void Display();
    
    //simple constructor for the fibHeap
    fibHeap() {
        maxElement =initFibHeap();
    }
};


fibNode * fibHeap::initFibHeap() {
    fibNode * nodePointer;
    nodePointer = NULL;
    return nodePointer;
}

fibHeap fh;
fibNode * maxElement = fh.initFibHeap();

fibNode * fibHeap::insertFibHeap(string key,int value,map<string,fibNode *> & HashMap) {
    //Create simple node with fibHeap attributes
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
    //Check whether heap is empty or not
    if(maxElement){
        (maxElement->leftChild)->rightChild=temp;
        temp->rightChild=maxElement;
        temp->leftChild=maxElement->leftChild;
        maxElement->leftChild=temp;
        if(temp->value > maxElement->value)
            maxElement=temp;
    }
    else {
        maxElement=temp;
    }
    numberNode++;
    return maxElement;
}


/*
 * extract_min
 * 1. z = H.min
 * 2. if z != NIL
 * 3. 	for each child x of z
 * 4. 		add x to the root list of H
 * 5. 		x.p = NIL
 * 6. 	remove z from the root list of H
 * 7.		if z == z.right
 * 8. 		H.min = NIL
 * 9. 	else H.min = z.right
 *10. 		CONSOLIDATE(H)
 *11. 	H.n = H.n - 1
 *12. return z
 */
fibNode * fibHeap::removeMaxElement()
{
    fibNode *z, *x, *next;
    fibNode ** childList;
    
    // 1
    z = maxElement;
    // 2
    if ( z != nullptr )
    {
        // 3
        x = z->child;
        if ( x != nullptr )
        {
            childList = new fibNode*[z->degree];
            next = x;
            for ( int i = 0; i < (int)z->degree; i++ )
            {
                childList[i] = next;
                next = next->rightChild;
            }
            for ( int i = 0; i < (int)z->degree; i++ )
            {
                x = childList[i];
                // 4
                maxElement->leftChild->rightChild = x;
                x->leftChild = maxElement->leftChild;
                maxElement->leftChild = x;
                x->rightChild = maxElement;
                // 5
                x->parent = nullptr;
            }
            delete [] childList;
        }
        // 6
        z->leftChild->rightChild = z->rightChild;
        z->rightChild->leftChild = z->leftChild;
        // 7
        if ( z == z->rightChild )
        {
            // 8
            maxElement = nullptr;
        }
        else
        {
            // 9
            maxElement = z->rightChild;
            // 10
            Consolidate();
        }
        // 11
        numberNode--;
    }
    // 12
    return z;
}

/*
 * consolidate
 * 1. let A[0 . . D(H.n)] be a new array
 * 2. for i = 0 to D(H.n)
 * 3. 	A[i] = NIL
 * 4. for each node w in the root list of H
 * 5. 	x = w
 * 6. 	d = x.degree
 * 7. 	while A[d] != NIL
 * 8. 		y = A[d]
 * 9. 		if x.key > y.key
 *10.			exchange x with y
 *11. 		FIB-HEAP-LINK(H,y,x)
 *12. 		A[d] = NIL
 *13. 		d = d + 1
 *14. 	A[d] = x
 *15. H.min = NIL
 *16. for i = 0 to D(H.n)
 *17. 	if A[i] != NIL
 *18. 		if H.min == NIL
 *19. 			create a root list for H containing just A[i]
 *20. 			H.min = A[i]
 *21. 		else insert A[i] into H's root list
 *22. 			if A[i].key < H.min.key
 *23. 				H.min = A[i]
 */
void fibHeap::Consolidate()
{
    fibNode* w, * next, * x, * y, * temp;
    fibNode** A, ** rootList;
    // Max degree <= log base golden ratio of n
    int d, rootSize;
    int max_degree = static_cast<int>(floor(log(static_cast<double>(numberNode))/log(static_cast<double>(1 + sqrt(static_cast<double>(5)))/2)));
    
    // 1
    A = new fibNode*[max_degree+2]; // plus two both for indexing to max degree and so A[max_degree+1] == NIL
    // 2, 3
    std::fill_n(A, max_degree+2, nullptr);
    // 4
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
        // 5
        x = w;
        // 6
        d = x->degree;
        // 7
        while ( A[d] != nullptr )
        {
            // 8
            y = A[d];
            // 9
            if ( x->value < y->value )
            {
                // 10
                temp = x;
                x = y;
                y = temp;
            }
            // 11
            x=Fibonnaci_link(y,x);
            // 12
            A[d] = nullptr;
            // 13
            d++;
        }
        // 14
        A[d] = x;
    }
    delete [] rootList;
    // 15
    maxElement = nullptr;
    // 16
    for ( int i = 0; i < max_degree+2; i++ )
    {
        // 17
        if ( A[i] != nullptr )
        {
            // 18
            if ( maxElement == nullptr )
            {
                // 19, 20
                maxElement = A[i]->leftChild = A[i]->rightChild = A[i];
            }
            else
            {
                // 21
                maxElement->leftChild->rightChild = A[i];
                A[i]->leftChild = maxElement->leftChild;
                maxElement->leftChild = A[i];
                A[i]->rightChild = maxElement;
                // 22
                if ( A[i]->value > maxElement->value )
                {
                    // 23
                    maxElement = A[i];
                }
            }
        }
    }
    delete [] A;
}

/*
 * fib_heap_link(y,x)
 * 1. remove y from the root list of heap
 * 2. make y a child of x, incrementing x.degree
 * 3. y.mark = FALSE
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





fibNode * fibHeap::increaseKey(fibNode * elementPtr,int incrValue){
    fibNode * tempPtr;
    if(!elementPtr){
        return elementPtr;
    }
    elementPtr->value+=incrValue;
    tempPtr=elementPtr->parent;
    if(tempPtr!=NULL && elementPtr->value > tempPtr->value){
        childCut(maxElement,elementPtr,tempPtr);
        cascadeChildCut(maxElement,tempPtr);
    }
    if(elementPtr->value > maxElement->value)
        maxElement=elementPtr;
    
    return maxElement;
}


int fibHeap::childCut(fibNode * maxElementPtr,fibNode * elementPtr,fibNode * tempPtr){
    if(elementPtr==elementPtr->rightChild)
        tempPtr->child=NULL;
    
    (elementPtr->leftChild)->rightChild = elementPtr->rightChild;
    (elementPtr->rightChild)->leftChild = elementPtr->leftChild;
    
    if (elementPtr == tempPtr->child)
        tempPtr->child = elementPtr->rightChild;
    tempPtr->degree--;
    
    elementPtr->leftChild=elementPtr;
    elementPtr->rightChild=elementPtr;
    (maxElementPtr->leftChild)->rightChild=elementPtr;
    elementPtr->rightChild=maxElementPtr;
    elementPtr->leftChild=maxElementPtr->leftChild;
    maxElementPtr->leftChild=elementPtr;
    elementPtr->parent = NULL;
    elementPtr->hasLostChild = false;
    
    return 0;
}



int fibHeap::cascadeChildCut(fibNode * maxElementPtr,fibNode * elementPtr){
    fibNode * temp=elementPtr->parent;
    if(temp){
        if(!temp->hasLostChild)
            temp->hasLostChild=true;
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




//Main routine call of the program
int main(int argc, const char * argv[]) {
    map<string,fibNode *> HashMap;
    fibNode*p;
    string str;
    int value;
    while(1){
        int op;
        cout<<"Enter Opertion type :";
        cin>>op;
        if(op==1){
            cout<<"Enter the key value ";
            cin>>str>>value;
            fibNode * temp = HashMap[str];
            if(!temp)
                p=fh.insertFibHeap(str,value,HashMap);
            else
                p=fh.increaseKey(temp, value);
            
            fh.Display();
        }
        else{
            p=fh.removeMaxElement();
            cout<<"Delete key : "<<p->key<<" "<<p->value<<endl;
            fh.Display();
        }
        
    }
    
}
