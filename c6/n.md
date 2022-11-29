container 
serial container & associative container
serial : vector / list
associative : map & set.  for stroage and searching.

map && set

usage of set:
sort & deduplication ;
can use iterator and auto for to do iteration.
iterator : *biodirection iterator*;
erase : position / value_type value/ iterator first--->iterator final;
if the position is invalid, it will report an error.
if the value is invalid, (not in the set), won't report errors.
*erase has a return value : return the number of erasing*

swap : swap the node;

*multiset*: allow inserting when the value has same value in the set. (no deduplication)
When finding , find the first node in inorder.
(the far right node in the tree)

there is no modification in set & multiset; because if modifying, the positions of nodes will be changed.

map:

using pair<first(key), second(val)>;
bio-direction iterator.
operator[];

insert : 3 ways;
pair 
anonymous object;
make_pair;  *most recommended*

the return value of insert is pair<iterator, bool > ;
when inserting ,if successful, return pair<newly inserted element iterator, true>;
		if unsuccessful, return pari<existed element iterator, false>;


it->first & it->second for iteration. (range for or iterator)
