#include "BTreeNode.h"

using namespace std;

//We need a constructor here
BTLeafNode()::BTLeafNode()
{
	int zeros = 0;  // putting in a value of 0 as the first key
	/*for(int i =sizeof(RecordId); i< PageFile::PAGE_SIZE; i+=sizeof(int))  // add 0 for all the key values
	{
		//is this right? Should I be starting with an offset of RecordId
		memcpy(buffer[i],&zeros, sizeof(int));  //copy the bytes that hold that value
	}*/
	int size = sizeof(int) + sizeof(RecordId);
	RecordId *r = new RecordId(-1,0);//setting the PID to -1, not allowed for a valid PID
	for(int i =0; i<PageFile::PAGE_SIZE; i+=size)
	{
		memcpy(buffer[i], &r, sizeof(RecordId)); //inputing the sizeof(RecordId) into the buffer
	}
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read fr
 om
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{ 
	int code = pf.read(pid, buffer);  //this reads the page at pageID and puts it into the buffer
	return code; 
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{ 
	int code = pf.write(pid,buffer); // this writes from the buffer to the page at pid
	return code;  
	}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{ 
	int size = sizeof(RecordId) + sizeof(int);  // size of recordId and key pair
	int count = 0;
	for(int i = 0; i<PageFile::PAGE_SIZE -4; i+=size;) //check each position a key might be at starting at the offset of where the first key is at
	{
		//starting at sizeof(RecordId), is this correct?
		RecordId temp;
		memcpy(&temp,buffer[i],sizeof(RecordId));  //copy the keyValue of the node into the key
		if(temp.pid == -1) //if pageID =-1 then we have no more valid keys
		{return count;} //return the count of the keys
		else{count++;}
	}
	return count;
}

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{	
	RC nodeFull;
	int size = sizeof(RecordId) + sizeof(int)
	for(int i = 0; i<PageFile::PAGE_SIZE-4; i+=size)
	{
		RecordId temp;
		int keyValue;
		memcpy(&temp,buffer[i],sizeof(RecordId));
		if(temp.pid == -1) //not a valid key here so we will input the key, rid
		{
			memcpy(buffer[i], &rid, sizeof(RecordId)); //copy in the recordID
			memcpy(buffer[i+sizeof(RecordId)],&key, sizeof(int)); //copy in the Key
			return 0; //return 0 since the RecordId,key was successfully added
		}
		else
		{
			memcpy(&keyValue,buffer[i+sizeof(RecordId)], sizef(int)); // we need the keyValue here because we know this is a valid entry and we will need
																		//to compare keyValues in the next step
		}
		if(key < keyValue) //key we want to insert is less than the key that is already there so we want to insert it here
		{
			//create a temporary buffer, copy it intother and copy it all over
			char temp = buffer; //temporary buffer
			memcpy(buffer[i], &rid, sizeof(RecordId)); // copy the recordId over
			memcpy(buffer[i+sizeof(RecordId)], &key, sizeof(int)); //copy the key over
			for(int ii =i; ii<PageFile::PAGE_SIZE-size-4; ii+=size)
			{
				memcpy(buffer[ii+size],temp[ii],size);//copy over the buffer
			}
			return 0;
		}
	}
		return 0;
}

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{ return 0; }

/**
 * If searchKey exists in the node, set eid to the index entry
 * with searchKey and return 0. If not, set eid to the index entry
 * immediately after the largest index key that is smaller than searchKey,
 * and return the error code RC_NO_SUCH_RECORD.
 * Remember that keys inside a B+tree node are always kept sorted.
 * @param searchKey[IN] the key to search for.
 * @param eid[OUT] the index entry number with searchKey or immediately
                   behind the largest key smaller than searchKey.
 * @return 0 if searchKey is found. Otherwise return an error code.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{ 
int size = sizeof(RecordId) + sizeof(int)
	for(int i = 0; i<PageFile::PAGE_SIZE-4; i+=size)
	{
		RecordId temp;
		int keyValue;
		int keyCount = 0;
		memcpy(&temp,buffer[i],sizeof(RecordId));
		if(temp.pid == -1) //not a valid key here so we will input the key, rid
		{
			//memcpy(buffer[i], &rid, sizeof(RecordId)); //copy in the recordID
			//memcpy(buffer[i+sizeof(RecordId)],&key, sizeof(int)); //copy in the Key
			eid = keyCount;
			return RC_NO_SUCH_RECORD; //return 0 since the RecordId,key was successfully added
		
		}
		else
		{
			memcpy(&keyValue,buffer[i+sizeof(RecordId)], sizef(int)); // we need the keyValue here because we know this is a valid entry and we will need
																		//to compare keyValues in the next step
		}
		if(searchkey < keyValue) //key we want to insert is less than the key that is already there so we want to insert it here
		{
			eid = keyCount + 1;
			return 0;
		}
		keyCount++;
	}
	return 0; 
}

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{ 
	int size = sizeof(RecordId) + sizeof(int);
	int counter = 0;
	for(int i =0; i<PageFile::PAGE_SIZE-4; i+=size) // looping through the node
	{
		counter ++; //counts the amount of symbols, THE FIRST SYMBOL IS SYMBOL 1 NOT SYMBOL 0
		if(eid == counter)
		{
			RecordId temp;
			memcpy(&temp,buffer[i],sizeof(RecordId)); //copy the bytes at RecordID
			if(temp.pid == -1) // this means that no node exists with that eid
				return -1 ;
			else
			{
				memcpy(&rid,buffer[i],sizeof(RecordId)); // copy the bytes into rid
				memcpy(&key, buffer[i+sizeof(RecordId)], sizeof(int)); //copy the bytes into key 
				return 0;
			}
		}
	}
	return -1;
}

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{ 
	//return a pid holding the memory of the last 4 bytes
	PageId id;
	memcpy(&id,PageFile::PAGE_SIZE-sizeof(PageID),sizeof(PageID))
	return id;' }

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{ 
	// make sure the pid is valid aka it is greater than 0
	if(pid<0) //maybe I should return a negative number
		return -1;
	else
	{
		memcpy(buffer[PageFile::PAGE_SIZE-sizeof(PageID)],&pid, sizeof(PageId));
		return 0;
	}
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{ return 0; }
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf)
{ return 0; }

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{ return 0; }


/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{ return 0; }

/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey)
{ return 0; }

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{ return 0; }

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{ return 0; }
