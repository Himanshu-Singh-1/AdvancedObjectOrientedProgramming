//#include<stdio.h>
//#include "skiplist.hpp"
#include <iostream>
//#include<string.h>
#include<stdlib.h>
//#include<iostream>

//using namespace std;



namespace cs540{
//Skiplist begin


template<typename Key_T,typename Mapped_T>
class SkipNode_simple
{
	public:
	SkipNode_simple(int);
	SkipNode_simple();
	virtual ~SkipNode_simple();	
	
	int getHgt(void);
	int index=0;
	SkipNode_simple<Key_T,Mapped_T>** fwdNodes = nullptr;
	SkipNode_simple<Key_T,Mapped_T>*prev = nullptr;
	int nodeHieght=0;	
};

template<typename Key_T,typename Mapped_T>
class SkipNode: public SkipNode_simple<Key_T,Mapped_T>
{
	public:
	SkipNode(int,std::pair< const Key_T, Mapped_T>);
	//SkipNode(int);
	SkipNode(const SkipNode &);
	SkipNode &operator =(const SkipNode &);
	~SkipNode();
	
	const Key_T* getKey(void);
	Mapped_T* getObj(void);
	std::pair<const Key_T, Mapped_T>& getValue(void);
	//std::pair<const Key_T, Mapped_T> value;
	//int getHgt(void);
	//SkipNode** fwdNodes;
	//SkipNode<Key_T,Mapped_T>* prev =NULL;
	
	private:
	std::pair<const Key_T, Mapped_T> value;
};

template<typename Key_T,typename Mapped_T>
SkipNode_simple<Key_T,Mapped_T>::SkipNode_simple()
{
	fwdNodes = new SkipNode_simple<Key_T,Mapped_T>*[1]();
	nodeHieght =0;
}


template<typename Key_T,typename Mapped_T>
SkipNode_simple<Key_T,Mapped_T>::SkipNode_simple(int h)
{
	
	nodeHieght =h;
	//obj = (Mapped_T*)NULL;
	if(h==-1)
	fwdNodes = new SkipNode_simple<Key_T,Mapped_T>*[1]();
	else
	{
	fwdNodes = new SkipNode_simple<Key_T,Mapped_T>*[h+1]();
	
	//changed x from 1 to 0 
	for(int x=0;x<=nodeHieght;x++)
		fwdNodes[x] = nullptr;
	}
}


template<typename Key_T,typename Mapped_T>
SkipNode_simple<Key_T,Mapped_T>::~SkipNode_simple()
{
		//std::cout<<"inside skipnode_simple destructor\n";
	if(fwdNodes) {
		delete[] fwdNodes;
		}
}



template<typename Key_T,typename Mapped_T>
SkipNode<Key_T,Mapped_T>& SkipNode<Key_T,Mapped_T>::operator=(const SkipNode<Key_T,Mapped_T>&other)
{
	this->nodeHieght = other.Hieght;
	value = other.value;
	if(this->fwdNodes)
	delete[] this->fwdNodes;
	this->fwdNodes = new SkipNode_simple<Key_T,Mapped_T>*[this->nodeHieght+1]();
}

template<typename Key_T,typename Mapped_T>
SkipNode<Key_T,Mapped_T>::SkipNode(const SkipNode<Key_T,Mapped_T>&other):value(other.value)
{
	//printf("\n Inside SkipNode Copy Construxtor");
	this->nodeHieght =other.nodeHieght;
	//obj =o;
	delete[] this->fwdNodes;
	this->fwdNodes = new SkipNode_simple<Key_T,Mapped_T>*[this->nodeHieght+1]();
	//value.first.print();
}


template<typename Key_T,typename Mapped_T>
SkipNode<Key_T,Mapped_T>::~SkipNode()
{
	//std::cout<<"\ninside skipnode destructor\n";
	//if(this->fwdNodes != nullptr)
	 //delete[] this->fwdNodes;
}


template<typename Key_T,typename Mapped_T>
SkipNode<Key_T,Mapped_T>::SkipNode(int h,std::pair< const Key_T, Mapped_T> pair):value(pair),SkipNode_simple<Key_T,Mapped_T>(h)
{
	//this->nodeHieght =h;
	//obj =o;
	//fwdNodes = new SkipNode_simple<Key_T,Mapped_T>*[h+1];
}


template<typename Key_T,typename Mapped_T>
const Key_T* SkipNode<Key_T,Mapped_T>::getKey(void)
{
	return &value.first;
}

template<typename Key_T,typename Mapped_T>
Mapped_T* SkipNode<Key_T,Mapped_T>::getObj(void)
{
	return &value.second;
}

template<typename Key_T,typename Mapped_T>
std::pair<const Key_T, Mapped_T>& SkipNode<Key_T,Mapped_T>::getValue(void)
{
	return value;
}


template <typename Key_T,typename Mapped_T>
int SkipNode_simple<Key_T,Mapped_T>::getHgt(void)
{
	return nodeHieght;
}


template<typename Key_T,typename Mapped_T>
class SkipList
{
	public:
	SkipList(float,int);
	SkipList(const SkipList&);
	~SkipList();
	void clear();
	void insert(std::pair< const Key_T, Mapped_T> );
	bool remove(const Key_T*);
	Mapped_T* retrieve(const Key_T*);
	const Mapped_T* retrieve(const Key_T*) const;
	SkipNode<Key_T,Mapped_T>* begin_node() const;
	SkipNode<Key_T,Mapped_T>* head_node() const;
	SkipNode_simple<Key_T,Mapped_T>* end_node() const;
	SkipNode_simple<Key_T,Mapped_T>* end_node_actual() const;
	//SkipNode_simple<Key_T,Mapped_T>* start_node() const;
	
	SkipNode<Key_T,Mapped_T>*retrieve_node(const Key_T*) const;
	int randomLevel();
	size_t size() const;
	void print();
	bool empty() const;
	SkipList& operator=(const SkipList &); 
	
	private:
	SkipNode_simple<Key_T,Mapped_T>* head = nullptr;
	SkipNode_simple<Key_T,Mapped_T>* endnode = nullptr;
	float probability=0;
	int maxHeight=0;
	int curHeight=0;
	size_t nodecount;
};

template<typename Key_T,typename Mapped_T>
SkipList<Key_T,Mapped_T>:: SkipList(float p, int m)
{
	curHeight=0;
	nodecount=0;
	maxHeight=m;
	probability =p;
	//Create head
	head = new SkipNode_simple<Key_T,Mapped_T>(maxHeight);
	//end_node maxheight -1 to identufy endnode
	endnode = new SkipNode_simple<Key_T,Mapped_T>(-1);
}

/*
template<typename Key_T,typename Mapped_T>
SkipList<Key_T,Mapped_T>::~SkipList()
{
	SkipNode_simple<Key_T,Mapped_T>* tmp;
	SkipNode_simple<Key_T,Mapped_T>*nxt;
	tmp =head;
	while(tmp)
	{
		nxt =tmp->fwdNodes[0];
		if(tmp)
		delete tmp;
		tmp =nxt;
	}
}
*/


template<typename Key_T,typename Mapped_T>
SkipList<Key_T,Mapped_T>:: ~SkipList()
{	
	clear();
	if(head != nullptr)
		delete head;
	if(endnode != nullptr)
		delete endnode;
	//free(head);
	//free(endnode);
}



template<typename Key_T,typename Mapped_T>
void SkipList<Key_T,Mapped_T>:: clear()
{
	SkipNode_simple<Key_T,Mapped_T>* node = head;
	node = node->fwdNodes[0];
	SkipNode_simple<Key_T,Mapped_T>* travl_ptr = nullptr;
	//int max = maxHeight;
	while(node!=nullptr)
	{
		travl_ptr = node;
		node = node->fwdNodes[0];
		//delete dynamic_cast<SkipNode<Key_T,Mapped_T>*>(travl_ptr);
		//delete static_cast<SkipNode<Key_T,Mapped_T>*>(travl_ptr);
		delete travl_ptr;
	}
	
	
	if(head != nullptr)
		delete head;
	if(endnode != nullptr)
		delete endnode;
	//head = NULL;
	//Create head
	head = new SkipNode_simple<Key_T,Mapped_T>(maxHeight);
	//end_node maxheight -1 to identufy endnode
	endnode = new SkipNode_simple<Key_T,Mapped_T>(-1);	
	curHeight =0;
}


template<typename Key_T,typename Mapped_T>
SkipList<Key_T,Mapped_T>& SkipList<Key_T,Mapped_T>:: operator=(const SkipList<Key_T,Mapped_T> &other)
{
	if(this!=&other)
	{
	clear();
	curHeight=other.curHeight;
	maxHeight=other.maxHeight;
	probability =other.probability;
	nodecount = other.nodecount;
	//Create head
	//printf("\n Inside Map Copy");
	//endnode = new SkipNode_simple<Key_T,Mapped_T>(-1);
	endnode->prev = nullptr;
	//head = new SkipNode_simple<Key_T,Mapped_T>(maxHeight);
	SkipNode_simple<Key_T,Mapped_T>** update = new SkipNode_simple<Key_T,Mapped_T>*[maxHeight]();
	//memset(update,0,sizeof(SkipNode<Key_T,Mapped_T>*)*maxHeight);
	
	SkipNode_simple<Key_T,Mapped_T>*origTravPtr = other.head->fwdNodes[0];
	SkipNode<Key_T,Mapped_T>*copyTravPtr;
	SkipNode_simple<Key_T,Mapped_T>*ptr = head;
	
	while(origTravPtr!=NULL)
	{
		copyTravPtr = new SkipNode<Key_T,Mapped_T>(*(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(origTravPtr)));
		
		for(int level = 0 ;level < copyTravPtr->getHgt();level++)
		{
			if(update[level] == NULL)
				head->fwdNodes[level] = copyTravPtr;
			else
				(update[level]->fwdNodes)[level] = copyTravPtr;
			update[level] = copyTravPtr;
		}
		origTravPtr = origTravPtr->fwdNodes[0];
		if(ptr==NULL)
		{
			ptr = copyTravPtr;
			copyTravPtr->prev = NULL;
		}
		else
		{
			copyTravPtr->prev = ptr;
			ptr = copyTravPtr;
		}
	}
	delete[] update;
	}

	return *this;
}

template<typename Key_T,typename Mapped_T>
SkipList<Key_T,Mapped_T>::SkipList(const SkipList& other)
{
	curHeight=other.curHeight;
	maxHeight=other.maxHeight;
	probability =other.probability;
	nodecount = other.nodecount;
	//Create head
	//printf("\n Inside Map Copy");
	endnode = new SkipNode_simple<Key_T,Mapped_T>(-1);
	endnode->prev = nullptr;
	head = new SkipNode_simple<Key_T,Mapped_T>(maxHeight);
	SkipNode_simple<Key_T,Mapped_T>** update = new SkipNode_simple<Key_T,Mapped_T>*[maxHeight]();
	//memset(update,0,sizeof(SkipNode<Key_T,Mapped_T>*)*maxHeight);
	
	SkipNode_simple<Key_T,Mapped_T>*origTravPtr = other.head->fwdNodes[0];
	SkipNode<Key_T,Mapped_T>*copyTravPtr;
	SkipNode_simple<Key_T,Mapped_T>*ptr = head;
	
	while(origTravPtr!=NULL)
	{
		copyTravPtr = new SkipNode<Key_T,Mapped_T>(*(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(origTravPtr)));
		
		for(int level = 0 ;level < copyTravPtr->getHgt();level++)
		{
			if(update[level] == NULL)
				head->fwdNodes[level] = copyTravPtr;
			else
				(update[level]->fwdNodes)[level] = copyTravPtr;
			update[level] = copyTravPtr;
		}
		origTravPtr = origTravPtr->fwdNodes[0];
		if(ptr==NULL)
		{
			ptr = copyTravPtr;
			copyTravPtr->prev = NULL;
		}
		else
		{
			copyTravPtr->prev = ptr;
			ptr = copyTravPtr;
		}
	}
	delete[] update;
}


template<typename Key_T,typename Mapped_T>
SkipNode_simple<Key_T,Mapped_T>* SkipList<Key_T,Mapped_T>::end_node() const
{
	SkipNode_simple<Key_T,Mapped_T>* node = head;
	//while(node->fwdNodes[0]!=NULL)
	while(node->fwdNodes[0]!=NULL)	
	{
		node = node->fwdNodes[0];
	}
	if(node!=head)
		endnode->prev = node;
	//std::cout<<"Inside skiplist end_node hieght:"<<endnode->nodeHieght;
	//node->fwdNodes[0] = (SkipNode<Key_T,Mapped_T>*)endnode;
	return (endnode);
}

template<typename Key_T,typename Mapped_T>
SkipNode_simple<Key_T,Mapped_T>* SkipList<Key_T,Mapped_T>::end_node_actual() const
{
	SkipNode_simple<Key_T,Mapped_T>* node = head;
	//while(node->fwdNodes[0]!=NULL)
	while(node->fwdNodes[0]!=NULL)	
	{
		node = node->fwdNodes[0];
	}
	
	//std::cout<<"Inside skiplist end_node hieght:"<<endnode->nodeHieght;
	//node->fwdNodes[0] = (SkipNode<Key_T,Mapped_T>*)endnode;
	return (node);
}

/*
template<typename Key_T,typename Mapped_T>
SkipNode_simple<Key_T,Mapped_T>* SkipList<Key_T,Mapped_T>::start_node() const
{

	
	SkipNode_simple<Key_T,Mapped_T>* endnode = new SkipNode_simple<Key_T,Mapped_T>(-1);
	endnode->prev = nullptr;
	endnode->fwdNodes[0] = head;
	//std::cout<<"Inside skiplist end_node hieght:"<<endnode->nodeHieght;
	//node->fwdNodes[0] = (SkipNode<Key_T,Mapped_T>*)endnode;
	return (endnode);
}
*/

template<typename Key_T,typename Mapped_T>
SkipNode<Key_T,Mapped_T>* SkipList<Key_T,Mapped_T>::begin_node() const
{
	return(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(head->fwdNodes[0]));
}
template<typename Key_T,typename Mapped_T>
SkipNode<Key_T,Mapped_T>* SkipList<Key_T,Mapped_T>::head_node() const
{
	return dynamic_cast<SkipNode<Key_T,Mapped_T>*>(head);
}

template<typename Key_T,typename Mapped_T>
bool SkipList<Key_T,Mapped_T>:: empty() const
{
	if(size()>0)
		return false;
	else 
		return true;
}

template<typename Key_T,typename Mapped_T>
int SkipList<Key_T,Mapped_T>:: randomLevel()
{
	float r = (float)rand()/RAND_MAX;
	int lvl =1;
	while(r<probability && lvl<maxHeight)
	{
		lvl++;
		r = (float)rand()/RAND_MAX;
	}
	//printf("\n level generated : %5d",lvl);
	return lvl;
}



template<typename Key_T,typename Mapped_T>
void SkipList<Key_T,Mapped_T>:: print()
{
	printf("\n*****Skip List***** Current Hieght: %5d\n",curHeight);
    for(int i=0;i<=curHeight;i++)
    {
        SkipNode_simple<Key_T,Mapped_T> *node = head->fwdNodes[i];
		//head->getKey()->print();
        printf("level : %5d ",i);
        while(node != NULL)
        {
			printf("\nnode value:");
            //node->value.first.print();
			dynamic_cast<SkipNode<Key_T,Mapped_T>*>(node)->getKey()->print();
			if(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(node->prev)->getKey()!=NULL)
			{
			//printf("\nprev node value:");
			//dynamic_cast<SkipNode<Key_T,Mapped_T>*>(node->prev)->getKey()->print();
			}
            node = node->fwdNodes[i];
        }
        printf("\n");
    }
}


template<typename Key_T,typename Mapped_T>
void SkipList<Key_T,Mapped_T>::insert(std::pair< const Key_T, Mapped_T> pair)
{
	int lvl=0,h=0;
	SkipNode_simple<Key_T,Mapped_T>** updateVec = new SkipNode_simple<Key_T,Mapped_T>*[maxHeight+1]();
	//memset(updateVec,0,sizeof(SkipNode_simple<Key_T,Mapped_T>*)*(maxHeight+1));
	SkipNode_simple<Key_T,Mapped_T>* current = head;
	
	const Key_T* cmpKey =NULL;
	const Key_T* key = &pair.first;
	
	/*Start from highest lev//el of skip list move the current pointer fprward
	while key is greater than key of node next tp current otherwise inserted curren in updateVec
and move one level down and continue search	*/

for(int i= curHeight;i>=0;i--)
{
	while(current->fwdNodes[i]!=NULL 
	&& *(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(current->fwdNodes[i])->getKey())<*key)
	current = current->fwdNodes[i];
	updateVec[i] = current;
}

/* reached level 0 and forward poonter to right, which isi desired 
position to insert key*/
current = current->fwdNodes[0];

/*if current is NULL that means we have reached to end of level or current's key is not 
equal to key to insert that means we have to insert node between update[0] 
and current node*/
if(current == NULL || !(*(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(current)->getKey())==*key))
{
	int rlevel = randomLevel();
	/* if random level is greate than list's current level, initialize
	initialize update value with position to header for further use*/
	if(rlevel > curHeight)
	{
		for(int i= curHeight +1;i<rlevel+1;i++)
			updateVec[i] = head;
		curHeight = rlevel;
	}
	//printf("\nrlevel: %5d",rlevel);
	SkipNode<Key_T,Mapped_T>* n = new SkipNode<Key_T,Mapped_T>(rlevel,pair);
	//std::shared_ptr<SkipNode<Key_T,Mapped_T>> n(new SkipNode<Key_T,Mapped_T>(rlevel,pair));
	
	for(int i=0;i<rlevel;i++)
	{
		n->fwdNodes[i] = updateVec[i]->fwdNodes[i];
		updateVec[i]->fwdNodes[i] = n;
	}
	//printf("\nInserted:");
	nodecount++;
	n->prev =updateVec[0];
	if(n->fwdNodes[0]!=NULL)
	n->fwdNodes[0]->prev = n;
	n->index = n->prev->index+1;
	/*
	printf("\nInserted:");
	n->getKey()->print();
	printf("\nPrevious node key:");
	if(n->prev!=NULL && n->prev!=head)
    dynamic_cast<SkipNode<Key_T,Mapped_T>*>(n->prev)->getKey()->print();
	//n->getObj()->print();
	*/
	
}


delete[] updateVec;

}

template<typename Key_T,typename Mapped_T>
bool SkipList<Key_T,Mapped_T>::remove(const Key_T* key)
{
	//key->print();
	SkipNode_simple<Key_T,Mapped_T>** updateVec = new SkipNode_simple<Key_T,Mapped_T>* [maxHeight+1]();
	//memset(updateVec,0,sizeof(SkipNode<Key_T,Mapped_T>*)*(maxHeight+1));
	
	SkipNode_simple<Key_T,Mapped_T>* current = head;
	SkipNode_simple<Key_T,Mapped_T>* delnode = nullptr;
	/*Start from highest level of skip list move the current pointer
	forward while key is greater than key of npde next to current
	Otherwise insert current in update and move on level down and continue
	search*/
	
	for(int i = curHeight;i>=0;i--)
	{
		while(current->fwdNodes[i]!=NULL 
		&& *(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(current->fwdNodes[i])->getKey()) < *key)
		current = current->fwdNodes[i];
		updateVec[i] = current;
	}
	/* reached level 0 and forwrd pointer to right, which is possibly 
	our desired node*/
	current = current->fwdNodes[0];
	//If current node is target node
	if(current != NULL and *(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(current)->getKey()) == *key)
	{
		delnode  = current;
		if(current->fwdNodes[0]!=NULL)
		current->fwdNodes[0]->prev =current->prev;

	/*Start from lowest level and rearrange pointers like in a 
		singly linked list*/
		for(int i=0;i<=curHeight;i++)
		{
			/*If at level i, next node is not target node, break loop,
			no need to move further level*/
			if(updateVec[i]->fwdNodes[i] != current)
				break;
			updateVec[i]->fwdNodes[i] = current->fwdNodes[i];
		}
		//remove levels having no elements
		while(curHeight>0 && head->fwdNodes[curHeight]==NULL)
			curHeight--;
		//printf("\nDeleted key:");
		//key->print();
		nodecount--;
		delete delnode;
		delete[] updateVec;
		return true;
	}
	delete[] updateVec;

return false;	
}

template<typename Key_T,typename Mapped_T>
size_t SkipList<Key_T,Mapped_T>::size() const
{
	//std::cout<<nodecount<<std::endl;
	return nodecount;
}

template<typename Key_T,typename Mapped_T>
Mapped_T* SkipList<Key_T,Mapped_T>::retrieve(const Key_T* key)
{
	
	SkipNode_simple<Key_T,Mapped_T>* current = head;
	
	for(int i=curHeight;i>=0;i--)
	{
		while(current->fwdNodes[i] && 
		*(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(current->fwdNodes[i])->getKey()) < *key)
		current = current->fwdNodes[i];
	}
	/*reached level 0 and advance pointer to rigth, which is possibly
	the desired node*/
	current = current->fwdNodes[0];

	// If current node equal to key then node has been found
	if(current and *(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(current)->getKey()) == *key)
	{
		//printf("\n returning object\n");
		//std::cout<<current->getObj()<<std::endl;
		return dynamic_cast<SkipNode<Key_T,Mapped_T>*>(current)->getObj();
	}
	else
	{
		//printf("\n returning null\n");
		return nullptr;
	}
}



template<typename Key_T,typename Mapped_T>
const Mapped_T* SkipList<Key_T,Mapped_T>::retrieve(const Key_T* key) const
{
	int h=0;
	SkipNode_simple<Key_T,Mapped_T>** updateVec = new SkipNode_simple<Key_T,Mapped_T>*[maxHeight+1]();
	//memset(updateVec,0,sizeof(SkipNode<Key_T,Mapped_T>*)*(maxHeight+1));
	
	SkipNode_simple<Key_T,Mapped_T>* current = head;
	
	// find key and return node
	/*start from highest level of skip list move the current pointer
	forward while key is gretaer than key of node next to current 
	forward while key is gretaer than key of node next to current 
	otherwise insert current in update and move on level doen 
	and continue search*/
	for(int i=curHeight;i>=0;i--)
	{
		while(current->fwdNodes[i] && 
		*(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(current->fwdNodes[i])->getKey()) < *key)
		current = current->fwdNodes[i];
	}
	/*reached level 0 and advance pointer to rigth, which is possibly
	the desired node*/
	current = current->fwdNodes[0];
	// If current node equal to key then node has been found

	if(current and *(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(current)->getKey()) == *key)
	{
		delete[] updateVec;
		//printf("\n returning object");
		return dynamic_cast<SkipNode<Key_T,Mapped_T>*>(current)->getObj();
	}
	else
	{
		delete[] updateVec;
		
		//printf("\n returning null");
		return (Mapped_T*)NULL;
	}
	delete[] updateVec;
}

template<typename Key_T,typename Mapped_T>
SkipNode<Key_T,Mapped_T>* SkipList<Key_T,Mapped_T>::retrieve_node(const Key_T* key) const
{
	int h=0;
	SkipNode_simple<Key_T,Mapped_T>** updateVec = new SkipNode_simple<Key_T,Mapped_T>*[maxHeight+1]();
	//memset(updateVec,0,sizeof(SkipNode<Key_T,Mapped_T>*)*(maxHeight+1));
	
	SkipNode_simple<Key_T,Mapped_T>* current = head;
	
	// find key and return node
	/*start from highest level of skip list move the current pointer
	forward while key is gretaer than key of node next to current 
	otherwise insert current in update and move on level doen 
	and continue search*/
	for(int i=curHeight;i>=0;i--)
	{
		while(current->fwdNodes[i] && 
		*(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(current->fwdNodes[i])->getKey()) < *key)
		current = current->fwdNodes[i];
	}
	/*reached level 0 and advance pointer to rigth, which is possibly
	the desired node*/

	current = current->fwdNodes[0];
	// If current node equal to key then node has been found
	if(current and *(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(current)->getKey()) == *key)
	{
		delete[] updateVec;
		//printf("\n returning object");
		return dynamic_cast<SkipNode<Key_T,Mapped_T>*>(current);
	}
	else
	{
		delete[] updateVec;
		
		//printf("\n returning null");
		return (SkipNode<Key_T,Mapped_T>*)NULL;
		//throw std::out_of_range("Key not in map");
	}
	
	delete[] updateVec;
}














//Skiplist end
template <typename Key_T, typename Mapped_T>
class Map;
template <typename Key_T, typename Mapped_T>
bool operator==(const Map<Key_T,Mapped_T>& ,const Map<Key_T,Mapped_T>&);
template <typename Key_T, typename Mapped_T>
bool operator!=(const Map<Key_T,Mapped_T>& ,const Map<Key_T,Mapped_T>&);
template <typename Key_T, typename Mapped_T>
bool operator<(const Map<Key_T,Mapped_T>& ,const Map<Key_T,Mapped_T>&);



template <typename Key_T, typename Mapped_T>
class Map
{
	private:
	SkipList<Key_T,Mapped_T> skplist;
	
	public:
	class Iterator;
	class ConstIterator;
	class ReverseIterator;
	Map();
	~Map();
	void clear();
	Map(const Map &);
	Map&operator=(const Map&);
	Map(std::initializer_list<std::pair<const Key_T, Mapped_T>>);
	//void insert_simple(const std::pair<const Key_T, Mapped_T> );
	std::pair<Iterator, bool>insert(const std::pair<const Key_T, Mapped_T> );
	size_t size() const;
	bool empty() const;
	//Mapped_T &at(const Key_T &);
	const Mapped_T &at(const Key_T &) const;
	void erase(const Key_T &);
	void erase(Iterator);
	void print();
	Iterator begin();
	Iterator end();
	ConstIterator begin() const;
	ConstIterator end() const;
	ReverseIterator rbegin();
	ReverseIterator rend();
	Iterator find(const Key_T&);
	ConstIterator find(const Key_T&) const;
	
	/*
	Approach 2 of friend fucntion inside template class. Both operators are template functions
	template class map and template function operators are declared at start as they have to be used 
within each other during definition. The definition of operators is outside class. Hence it will be generated
as a template. Inside class specific instantiation of template operators are made friends.
There has to be space between opeator== <>. Since the operator function is a non member template class
implicit conversions will not be done to deduce template parameters for these oprators 
map + other_type_variable  will not work even if conversion from other_type to map exists.
Done differently fro iterators. 	
	*/
	friend bool operator== <Key_T,Mapped_T>(const Map<Key_T,Mapped_T> &,const Map<Key_T,Mapped_T> &);
	friend bool operator!= <Key_T,Mapped_T>(const Map<Key_T,Mapped_T>&,const Map<Key_T,Mapped_T>&);	
	friend bool operator< <Key_T,Mapped_T>(const Map<Key_T,Mapped_T>&,const Map<Key_T,Mapped_T>&);	
	 
	Mapped_T& operator[](const Key_T& key)
	{
	Mapped_T* obj = skplist.retrieve(&key);
	if(obj!=NULL)
		return *obj;
	else
	{
		skplist.insert(std::make_pair(key,Mapped_T()));
		return (*skplist.retrieve(&key));
	}
	}
	
};


template<typename Key_T, typename Mapped_T>
Map<Key_T,Mapped_T>::Map():skplist(0.6,1000)
{
	
}

template<typename Key_T, typename Mapped_T>
Map<Key_T,Mapped_T>::~Map()
{

}

template<typename Key_T, typename Mapped_T>
void Map<Key_T,Mapped_T>::clear()
{
	skplist.clear();
}

template<typename Key_T, typename Mapped_T>
Map<Key_T,Mapped_T>& Map<Key_T,Mapped_T>::operator=(const Map &other)
{
	//this->clear();
	skplist = other.skplist;
	return *this;
}

template<typename Key_T, typename Mapped_T>
Map<Key_T,Mapped_T>::Map(const Map &other):skplist(other.skplist)
{
	
	/*
	auto itr = other.begin();
	while(itr!=other.end())
	{
		std::pair<const Key_T, Mapped_T>val(itr.getPair());
		this->insert(val);
		itr++;
	}
	//Handling the last node pointed to by ***iterator
	std::pair<const Key_T, Mapped_T>val(itr.getPair());
		this->insert(val);
		*/
}


template <typename Key_T, typename Mapped_T>
bool operator==(const Map<Key_T,Mapped_T>& m1,const Map<Key_T,Mapped_T>& m2)
{
	//printf("\n Inside == map operator");
	//printf("\n Inside == map operator");
	int m1size =m1.size();
	int m2size =m2.size();
	if(m1size!=m2size)
		return false;

	else
	{	auto it1_end = m1.end();
		auto it2_end = m2.end();	
		auto itr1 = m1.begin();
		auto itr2 = m2.begin();
		while(itr1!=it1_end && itr2!=it2_end)
		{
			if(!(*itr1==*itr2))
			{
				//printf("*******Comparing values:");
				//(*itr1).first.print();
				//(*itr2).first.print();
				return false;
			}
			itr1++;
			itr2++;	
		}
		//if(!(*itr1==*itr2))
			//{
				//return false;
			//}
		return true;
	}
}

template <typename Key_T, typename Mapped_T>
bool operator<(const Map<Key_T,Mapped_T>& m1,const Map<Key_T,Mapped_T>& m2)
{
	auto itr1 = m1.begin();
	auto itr2 = m2.begin();
	if(m1.size() <= m2.size())
	{
		size_t sz = m1.size();
		while(sz!=0)
		{
		if(*itr1>*itr2)
			return false;
		sz--;
		itr1++;
		itr2++;
		}
		return true;
	}
	else if(m1.size() > m2.size())
	{
		size_t sz = m2.size();
		while(sz!=0)
		{
		if(*itr1>*itr2)
			return false;
		sz--;
		itr1++;
		itr2++;
		}
		return true;
	}
	return true;
	
}

template <typename Key_T, typename Mapped_T>
bool operator!=( Map<Key_T,Mapped_T> &m1, Map<Key_T,Mapped_T> &m2)
{
	if(m1==m2)
		return false;
	else
		return true;
}

template<typename Key_T, typename Mapped_T>
typename Map<Key_T,Mapped_T>::Iterator Map<Key_T,Mapped_T>::find(const Key_T& k)
{
	SkipNode<Key_T,Mapped_T>* n = skplist.retrieve_node(&k);
	if(n!=NULL)
	{
		Map<Key_T,Mapped_T>::Iterator itr(n);
		return itr;
	}
	else 
		return end();
}

template<typename Key_T, typename Mapped_T>
typename Map<Key_T,Mapped_T>::ConstIterator Map<Key_T,Mapped_T>::find(const Key_T& k) const
{
	SkipNode<Key_T,Mapped_T>* n = skplist.retrieve_node(&k);
	if(n!=NULL)
	{
		Map<Key_T,Mapped_T>::ConstIterator itr(n);
		return itr;
	}
	else 
		return end();
}


template<typename Key_T, typename Mapped_T>
typename Map<Key_T,Mapped_T>::Iterator Map<Key_T,Mapped_T>::begin()
{
	SkipNode<Key_T,Mapped_T>* n = skplist.begin_node();
	Map<Key_T,Mapped_T>::Iterator itr(n);
	return itr;
}



template<typename Key_T, typename Mapped_T>
typename Map<Key_T,Mapped_T>::ConstIterator Map<Key_T,Mapped_T>::begin() const
{
	SkipNode_simple<Key_T,Mapped_T>* n = skplist.begin_node();
	Map<Key_T,Mapped_T>::ConstIterator itr(n);
	return itr;
}

template<typename Key_T, typename Mapped_T>
typename Map<Key_T,Mapped_T>::ReverseIterator Map<Key_T,Mapped_T>::rbegin()
{
	SkipNode_simple<Key_T,Mapped_T>* n = skplist.end_node_actual();
	//printf("\n Inside map iteraor end");
	//n->getKey()->print();
	Map<Key_T,Mapped_T>::ReverseIterator itr(n);
	return itr;
}

template<typename Key_T, typename Mapped_T>
typename Map<Key_T,Mapped_T>::Iterator Map<Key_T,Mapped_T>::end()
{
	SkipNode_simple<Key_T,Mapped_T>* n = skplist.end_node();
	//printf("\n ***Inside map iteraor end : %5d",n->nodeHieght);
	//printf("\n Inside map iteraor end");
	//n->getKey()->print();
	Map<Key_T,Mapped_T>::Iterator itr(n);
	return itr;
}

//*******************************************************************
template<typename Key_T, typename Mapped_T>
typename Map<Key_T,Mapped_T>::ConstIterator Map<Key_T,Mapped_T>::end() const
{
	SkipNode_simple<Key_T,Mapped_T>* n = skplist.end_node();
	//printf("\n ***Inside map iteraor end : %5d",n->nodeHieght);
	
	//n->getKey()->print();
	Map<Key_T,Mapped_T>::ConstIterator itr(n);
	return itr;
}
//********************************************************************

template<typename Key_T, typename Mapped_T>
typename Map<Key_T,Mapped_T>::ReverseIterator Map<Key_T,Mapped_T>::rend()
{
	SkipNode_simple<Key_T,Mapped_T>* n = skplist.begin_node();
	
	Map<Key_T,Mapped_T>::ReverseIterator itr(n);
	return itr;
}



template<typename Key_T, typename Mapped_T>
void Map<Key_T,Mapped_T>::erase(const Key_T& k)
{
	skplist.remove(&k);
}

template<typename Key_T, typename Mapped_T>
bool Map<Key_T,Mapped_T>::empty() const
{
	if(skplist.size()>0)
		return false;
	else 
		return true;	
}


template<typename Key_T, typename Mapped_T>
size_t Map<Key_T,Mapped_T>::size() const
{
	return skplist.size();
}

template<typename Key_T, typename Mapped_T>
void Map<Key_T,Mapped_T>::print()
{
	skplist.print();
}



template<typename Key_T, typename Mapped_T>
Map<Key_T,Mapped_T>::Map(std::initializer_list<std::pair< const Key_T, Mapped_T>> pairs):skplist(0.6,5)
{
	//printf("\n*********Initializer list constructor hit*************");
	for(auto it = pairs.begin();it!=pairs.end();++it)
	{
		//std::pair< const Key_T, Mapped_T> tmp = new std::pair< const Key_T, Mapped_T>(*tmp);
	    skplist.insert(*it);
	}
}

template<typename Key_T, typename Mapped_T>
std::pair<typename Map<Key_T,Mapped_T>::Iterator,bool> Map<Key_T,Mapped_T>::insert(std::pair< const Key_T, Mapped_T> pair)
{
	
	//printf("\n**********inside map insert***********\n");
	//const Key_T *a = new Key_T(pair.first);
	//Mapped_T *b = new Mapped_T(pair.second);
    
	SkipNode<Key_T,Mapped_T>* n = skplist.retrieve_node(&pair.first);
	if(n!=NULL)
	{
	Map<Key_T,Mapped_T>::Iterator itr(n);
	return (std::make_pair(itr,true));
	}
	
	else
	{
	//SkipNode<Key_T,Mapped_T>* n = skplist.retrieve_node(&pair.first);
	skplist.insert(pair);
	SkipNode<Key_T,Mapped_T>* n = skplist.retrieve_node(&pair.first);
	Map<Key_T,Mapped_T>::Iterator itr(n);
	return (std::make_pair(itr,false));
	}
	
}

template<typename Key_T, typename Mapped_T>
const Mapped_T & Map<Key_T,Mapped_T>::at(const Key_T& key) const
{
	//printf("\n inside map at");
	//key.print();
	const Mapped_T* obj = skplist.retrieve(&key);
	if(obj == NULL)
		throw std::out_of_range("\n Key not in map");
	else
	return(*obj);
}



template<typename Key_T,typename Mapped_T>
class Map<Key_T,Mapped_T>::Iterator
{
	private:
	SkipNode_simple<Key_T,Mapped_T>* node;
	SkipNode_simple<Key_T,Mapped_T>* prev_node=NULL;
	public:
	Iterator(SkipNode_simple<Key_T,Mapped_T>*);
	const Key_T* getKey();
	Mapped_T* getValue();
	std::pair<const Key_T, Mapped_T>& getPair() const;
	void print();
	Iterator & operator ++ ();
	Iterator operator ++ (int);
	Iterator & operator -- ();
	Iterator operator -- (int);
	std::pair<const Key_T, Mapped_T>& operator *() const;
	std::pair<const Key_T, Mapped_T>* operator ->() const;
	//template<typename Key_T,typename Mapped_T>
	
	friend bool operator ==(const Iterator &itr1, const Iterator &itr2)
	{
	if(itr1.node == itr2.node)
		return true;
	else
		return false;	
	}
	
	friend bool operator ==(const Iterator &itr1, const ConstIterator &itr2)
	{
	if(itr1.node == itr2.node)
		return true;
	else
		return false;	
	}
	
	friend bool operator !=(const Iterator &itr1, const ConstIterator &itr2)
	{
	if(itr1.node != itr2.node)
		return true;
	else
		return false;	
	}
	
	friend bool operator !=(const Iterator &itr1, const Iterator &itr2)
	{
	if(itr1.node != itr2.node)
		return true;
	else
		return false;	
	}
	
};

	

template<typename Key_T,typename Mapped_T>
std::pair<const Key_T, Mapped_T>& Map<Key_T,Mapped_T>::Iterator:: operator *() const
{
return (getPair());	
}


template<typename Key_T,typename Mapped_T>
std::pair<const Key_T, Mapped_T>* Map<Key_T,Mapped_T>::Iterator:: operator ->() const
{
return (&getPair());	
}

template<typename Key_T,typename Mapped_T>
typename Map<Key_T,Mapped_T>::Iterator Map<Key_T,Mapped_T>::Iterator:: operator --(int)
{
	Iterator itr(*this);
	//node->prev->getKey()->print();
	//node = node->prev;
	
		if(node!=nullptr)
		node =node->prev;
	else
		node = prev_node;
	/*
	if(prev_node!=NULL)
	{
		//std::cout<<"\n\nPrev Node not null"<<std::endl;
		node = prev_node;
		//prev_node=prev_node->prev;
		prev_node = nullptr;
	}
	else
	{		
		node = node->prev;
	}
	*/
	return itr;
}

template<typename Key_T,typename Mapped_T>
typename Map<Key_T,Mapped_T>::Iterator & Map<Key_T,Mapped_T>::Iterator:: operator --()
{
	
	if(node!=nullptr)
		node =node->prev;
	else
		node = prev_node;
	

/*
	 if(prev_node!=NULL)
	{
		node = prev_node;
		//prev_node=prev_node->prev;
		prev_node = NULL;
	}
	else
		node = node->prev;
	*/
	return *this; 
}

template<typename Key_T,typename Mapped_T>
typename Map<Key_T,Mapped_T>::Iterator Map<Key_T,Mapped_T>::Iterator:: operator ++(int)
{
	Iterator itr(*this);
	node = node->fwdNodes[0];
	
	return itr;
}
/*
template<typename Key_T,typename Mapped_T>
typename Map<Key_T,Mapped_T>::Iterator Map<Key_T,Mapped_T>::Iterator:: operator ++(int)
{
	Iterator itr(*this);
	if(prev_node!=NULL)
	{
		node = prev_node;
		prev_node=prev_node->prev;
	}
	else
	node = node->fwdNodes[0];
	
	return itr;
}
*/
template<typename Key_T,typename Mapped_T>
typename Map<Key_T,Mapped_T>::Iterator & Map<Key_T,Mapped_T>::Iterator:: operator ++()
{
	node = node->fwdNodes[0];
	return *this;
}

template<typename Key_T,typename Mapped_T>
const Key_T* Map<Key_T,Mapped_T>::Iterator::getKey()
{
	//printf("\nInside iterator print");
	return(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(node)->getKey());
}

template<typename Key_T,typename Mapped_T>
Mapped_T* Map<Key_T,Mapped_T>::Iterator::getValue() 
{
	//printf("\nInside iterator print");
	return(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(node)->getObj());
}

template<typename Key_T,typename Mapped_T>
std::pair<const Key_T, Mapped_T>& Map<Key_T,Mapped_T>::Iterator::getPair() const
{
	//printf("\nInside iterator print");
	return(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(node)->getValue());
}

template<typename Key_T,typename Mapped_T>
void Map<Key_T,Mapped_T>::Iterator::print()
{
	//printf("\nInside iterator print");
	dynamic_cast<SkipNode<Key_T,Mapped_T>*>(node)->getKey()->print();
}

template<typename Key_T,typename Mapped_T>
Map<Key_T,Mapped_T>::Iterator::Iterator(SkipNode_simple<Key_T,Mapped_T>* n)
{
	node =n;
	if(n!=NULL && n->getHgt()==-1)
	{
		node=NULL;
		prev_node=n->prev;
	}
	
	
}
//Iterator

//This function defined after iterator as nested templated class has to be defined before its first use.
template<typename Key_T, typename Mapped_T>
void Map<Key_T,Mapped_T>::erase(Iterator iter)
{
	skplist.remove(iter.getKey());
}


//Const Iterator 
template<typename Key_T,typename Mapped_T>
class Map<Key_T,Mapped_T>::ConstIterator
{
	private:
	SkipNode_simple<Key_T,Mapped_T>* node;
	SkipNode_simple<Key_T,Mapped_T>* prev_node;
	public:
	ConstIterator(SkipNode_simple<Key_T,Mapped_T>*);
	const Key_T* getKey();
	Mapped_T* getValue();
	std::pair<const Key_T, Mapped_T>& getPair() const;
	//void print();
	ConstIterator & operator ++ ();
	ConstIterator operator ++ (int);
	ConstIterator & operator -- ();
	ConstIterator operator -- (int);
	void print();
	const std::pair<const Key_T, Mapped_T>& operator *() const;
	const std::pair<const Key_T, Mapped_T>* operator ->() const;
	
	/*Approach 3 for friend functions inside template classes. In this approach the friend operator 
	functions are not template functions. They are generated for each template instantiation of iterator class
	inlined in class definition because operator doesnt exist outside
	class definition of iterator until a template instantiation of iterator happens
	so there is no way to declrae that function friend here, it has to be 
	defined inline here so that it is generated for each iterator otherwise 
	what we made friend doesnt exist. Only specific definitions of operators can be written outside 
	of the class definition and thses will become friends even if not inlined, but then will have to 
	write specific definitions for each template type instantiation of iterator
	
	The advantage of this is thay implicit type conversions will happen when calling the operator 
	if the type of iterator can be deduced from the arguments stdandard implicit conversion rules will apply 
	for non-template non-member functions iterator<>++ will cause ADL??
	ADL is argument dependent lookup the operator functions are not in global scope not memeber function 
	of iterator class but exists in class or namespace suggested by the type of argument(the template instantiation of iterator class)
	*/
	
	friend bool operator ==(const ConstIterator &itr1, const ConstIterator &itr2)
	{
	if(itr1.node == itr2.node)
		return true;
	else
		return false;	
	}
	
	friend bool operator ==(const ConstIterator &itr1, const Iterator &itr2)
	{
	if(itr1.node == itr2.node)
		return true;
	else
		return false;	
	}
	
	friend bool operator !=(const ConstIterator &itr1, const Iterator &itr2)
	{
	if(itr1.node != itr2.node)
		return true;
	else
		return false;	
	}
	
	friend bool operator !=(const ConstIterator &itr1, const ConstIterator &itr2)
	{
	if(itr1.node != itr2.node)
		return true;
	else
		return false;	
	}
	
};


template<typename Key_T,typename Mapped_T>
void Map<Key_T,Mapped_T>::ConstIterator::print()
{
	//printf("\nInside iterator print");
	dynamic_cast<SkipNode<Key_T,Mapped_T>*>(node)->getKey()->print();
}
	
template<typename Key_T,typename Mapped_T>
const std::pair<const Key_T, Mapped_T>& Map<Key_T,Mapped_T>::ConstIterator:: operator *() const
{
return (getPair());	
}


template<typename Key_T,typename Mapped_T>
const std::pair<const Key_T, Mapped_T>* Map<Key_T,Mapped_T>::ConstIterator:: operator ->() const
{
return (&getPair());	
}

template<typename Key_T,typename Mapped_T>
typename Map<Key_T,Mapped_T>::ConstIterator Map<Key_T,Mapped_T>::ConstIterator:: operator --(int)
{
	ConstIterator itr(*this);
	//printf("\n Inside decrement");
	//node->prev->getKey()->print();
	//node = node->prev;
	if(prev_node!=NULL)
	{
		node = prev_node;
		//prev_node=prev_node->prev;
		prev_node = NULL;
	}
	else
		node = node->prev;
	return itr;
}

template<typename Key_T,typename Mapped_T>
typename Map<Key_T,Mapped_T>::ConstIterator & Map<Key_T,Mapped_T>::ConstIterator:: operator --()
{
	//node = node->prev;
	if(prev_node!=NULL)
	{
		node = prev_node;
		//prev_node=prev_node->prev;
		prev_node = NULL;
	}
	else
		node = node->prev;
	return *this;
}

template<typename Key_T,typename Mapped_T>
typename Map<Key_T,Mapped_T>::ConstIterator Map<Key_T,Mapped_T>::ConstIterator:: operator ++(int)
{
	ConstIterator itr(*this);
	node = node->fwdNodes[0];
	return itr;
}

template<typename Key_T,typename Mapped_T>
typename Map<Key_T,Mapped_T>::ConstIterator & Map<Key_T,Mapped_T>::ConstIterator:: operator ++()
{
	node = node->fwdNodes[0];
	return *this;
}

template<typename Key_T,typename Mapped_T>
const Key_T* Map<Key_T,Mapped_T>::ConstIterator::getKey()
{
	//printf("\nInside iterator print");
	return(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(node)->getKey());
}

template<typename Key_T,typename Mapped_T>
Mapped_T* Map<Key_T,Mapped_T>::ConstIterator::getValue() 
{
	//printf("\nInside iterator print");
	return(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(node)->getObj());
}

template<typename Key_T,typename Mapped_T>
std::pair<const Key_T, Mapped_T>& Map<Key_T,Mapped_T>::ConstIterator::getPair() const
{
	//printf("\nInside iterator print");
	return(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(node)->getValue());
}

/*
template<typename Key_T,typename Mapped_T>
void Map<Key_T,Mapped_T>::ConstIterator::print()
{
	printf("\nInside iterator print");
	node->getKey()->print();
}
*/

template<typename Key_T,typename Mapped_T>
Map<Key_T,Mapped_T>::ConstIterator::ConstIterator(SkipNode_simple<Key_T,Mapped_T>* n)
{
	node =n;
	if(n!=NULL && n->getHgt()==-1)
	{
		node=NULL;
		prev_node=n->prev;
	}
}
//Const Iterator



//Reverse Iterator 
template<typename Key_T,typename Mapped_T>
class Map<Key_T,Mapped_T>::ReverseIterator
{
	private:
	SkipNode_simple<Key_T,Mapped_T>* node;
	
	public:
	ReverseIterator(SkipNode_simple<Key_T,Mapped_T>*);
	const Key_T* getKey();
	Mapped_T* getValue();
	std::pair<const Key_T, Mapped_T>& getPair() const;
	void print();
	ReverseIterator & operator ++ ();
	ReverseIterator operator ++ (int);
	ReverseIterator & operator -- ();
	ReverseIterator operator -- (int);
	std::pair<const Key_T, Mapped_T>& operator *() const;
	std::pair<const Key_T, Mapped_T>* operator ->() const;
	//template<typename Key_T,typename Mapped_T>
	
	friend bool operator ==(const ReverseIterator &itr1, const ReverseIterator &itr2)
	{
	if(itr1.node == itr2.node)
		return true;
	else
		return false;	
	}
	
	friend bool operator !=(const ReverseIterator &itr1, const ReverseIterator &itr2)
	{
	if(itr1.node != itr2.node)
		return true;
	else
		return false;	
	}
	
};

template<typename Key_T,typename Mapped_T>
std::pair<const Key_T, Mapped_T>& Map<Key_T,Mapped_T>::ReverseIterator:: operator *() const
{
return (getPair());	
}

template<typename Key_T,typename Mapped_T>
std::pair<const Key_T, Mapped_T>* Map<Key_T,Mapped_T>::ReverseIterator:: operator ->() const
{
return (&getPair());	
}

template<typename Key_T,typename Mapped_T>
typename Map<Key_T,Mapped_T>::ReverseIterator Map<Key_T,Mapped_T>::ReverseIterator:: operator ++(int)
{
	//Latest change
	ReverseIterator itr(*this);
	//printf("\n Inside decrement");
	//node->prev->getKey()->print();
	node = node->prev;
	return itr;
}

template<typename Key_T,typename Mapped_T>
typename Map<Key_T,Mapped_T>::ReverseIterator & Map<Key_T,Mapped_T>::ReverseIterator:: operator ++()
{
	node = node->prev;
	return *this;
}

template<typename Key_T,typename Mapped_T>
typename Map<Key_T,Mapped_T>::ReverseIterator Map<Key_T,Mapped_T>::ReverseIterator:: operator --(int)
{
	ReverseIterator itr(*this);
	node = node->fwdNodes[0];
	return itr;
}

template<typename Key_T,typename Mapped_T>
typename Map<Key_T,Mapped_T>::ReverseIterator & Map<Key_T,Mapped_T>::ReverseIterator:: operator --()
{
	node = node->fwdNodes[0];
	return *this;
}

template<typename Key_T,typename Mapped_T>
const Key_T* Map<Key_T,Mapped_T>::ReverseIterator::getKey()
{
	//printf("\nInside iterator print");
	return(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(node)->getKey());
}

template<typename Key_T,typename Mapped_T>
Mapped_T* Map<Key_T,Mapped_T>::ReverseIterator::getValue() 
{
	//printf("\nInside iterator print");
	return(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(node)->getObj());
}

template<typename Key_T,typename Mapped_T>
std::pair<const Key_T, Mapped_T>& Map<Key_T,Mapped_T>::ReverseIterator::getPair() const
{
	//printf("\nInside iterator print");
	return(dynamic_cast<SkipNode<Key_T,Mapped_T>*>(node)->getValue());
}

template<typename Key_T,typename Mapped_T>
void Map<Key_T,Mapped_T>::ReverseIterator::print()
{
	//printf("\nInside iterator print");
	dynamic_cast<SkipNode<Key_T,Mapped_T>*>(node)->getKey()->print();
}

template<typename Key_T,typename Mapped_T>
Map<Key_T,Mapped_T>::ReverseIterator::ReverseIterator(SkipNode_simple<Key_T,Mapped_T>* n)
{
	node =n;
}

//Reverse Iterator


}





