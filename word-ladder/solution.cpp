# include <string>
# include <queue>
# include <list>
# include <unordered_set>
# include <iostream>

using namespace std;
class Node
{
   list<Node *> neighbors;
   string word;
   unsigned int visited;
   unsigned int depth[3];
   public:
      Node()
      {
         visited = false;
         depth[0] = 0;
         depth[1] = 0;
         depth[2] = 0;
      }
      list<Node *> *getNeighbors()
      {
         return &neighbors;
      }
      void addNeighbor(Node *n)
      {
         neighbors.push_back(n);
      }
      void setWord(const string& w)
      {
         word = w;
      }
      string *getWord()
      {
         return &word;
      }
      unsigned int getVisited()
      {
         return visited;
      }
      void setVisited(unsigned int v)
      {
         visited = v;
      }
      void setDepth(unsigned int d)
      {
         depth[visited] = d;
      }
      unsigned int getDepth()
      {
         return depth[visited];
      }
      unsigned int getDepth(unsigned int v)
      {
         return depth[v];
      }
      void clearDepths()
      {
         for(int i = 0; i < 3; i++)
         {
            depth[i] = 0;
         }
      }
};

class Graph
{
   Node * nodes;
   unsigned int capacity, numNodes;
   unsigned int numConnections;
   public:
   /* Constructor
    */
   Graph(unsigned int n)
   {
      nodes = new Node[n];
      numNodes = 0;
      capacity = n;
      numConnections = 0;
   }
   
   /* Accessor Methods
    */
   unsigned int getNumNodes()
   {
      return numNodes;
   }
   unsigned int getNumConnections()
   {
      return numConnections;
   }
   unsigned int getCapacity()
   {
      return capacity;
   }
   Node *getNode(unsigned int index)
   {
      return nodes + index;
   }
   
   /* Destructor
    */
   ~Graph()
   {
      delete nodes;
   }
   
   /* Methods to add words to the graph
    */
   bool addWord(const string& w)
   {
      if(numNodes < capacity)
      {
         nodes[numNodes].setWord(w);
         numNodes++;
         return true;
      }
      return false;
   }
   void addWords(unordered_set<string>& dict)
   {
      /*for(unordered_set<string>::iterator i = dict.begin(); i != dict.end(); 
         i++)*/
      for(const auto& s : dict)
      {
         addWord(s);
      }
   }
   
   /* Methods used to find & make connections between 'adjacent' nodes/words
      Words are adjacent if they differ in only on of their characters.
    */
   bool isAdjacentWord(string* s1, string* s2)
   {
      bool isOneOff = false;
      for (unsigned int i = 0; i < s1->length(); ++i)
      {
         if((*s1)[i] != (*s2)[i])
         {
            if(isOneOff)
            {
               return false;
            }
            else
            {
               isOneOff = true;
            }
         }
      }
      return isOneOff;
   }
   bool isAdjacent(unsigned int index1, unsigned int index2)
   {
      
      string *s1 = nodes[index1].getWord();
      string *s2 = nodes[index2].getWord();
      bool result = isAdjacentWord(s1, s2);
      return result;
   }
   void connectNodes(unsigned int index1, unsigned int index2)
   {
      nodes[index1].addNeighbor(nodes + index2);
      nodes[index2].addNeighbor(nodes + index1);
      numConnections++;
   }
   void makeConnections()
   {
      for(unsigned int i = 0; i < numNodes - 1; i++)
      {
         for(unsigned int j = i + 1; j < numNodes; j++)
         {
            if(isAdjacent(i, j))
            {
               connectNodes(i, j);
            }
         }
      }
   }
   
   /* Traversal Methods
    */
   void clearFlags()
   {
      for(int i = 0; i < capacity; i++)
      {
         nodes[i].setVisited(0);
         nodes[i].clearDepths();
      }
   }
   /* BFS Step function
    * Returns 0 - if the queue is empty.
    * Returns 1 - if the goal node was found in the queue.
    if the BFS Step was completed normally without finding a node
    *             visited by the other BFS frontier.
    * Returns 2 - if the BFS Step was interrupted by the finding of a node 
    *             visited by the other BFS frontier. Pushes the found node into
    *             the queue before returning.
    */
   unsigned int BFSStep(queue<Node *>& q, unsigned int visitedFlag)
   {
      /* Case 0:  The queue is empty (The start and end nodes are in disjoint 
       *          graphs
       */
      if(q.empty())
      {
         return 0;
      }

      Node *n = q.front();
      q.pop();
      unsigned int v = n->getVisited();
      unsigned int d = n->getDepth();
      if(v == 0)
      {
         n->setVisited(visitedFlag);
      }
      /* Case 2:  The popped node has already been visited from the other side 
       *          The midpoint has been found. Push it back to the queue and 
       *          return.
       */
      else if(v != visitedFlag)
      {
         q.push(n);
         return 1;
      }
      
      /* Add children of this node to the queue */
      list<Node *> *l = n->getNeighbors();
      for(list<Node *>::iterator i = l->begin(); i != l->end(); i++)
      {
         v = (*i)->getVisited();
         (*i)->setVisited(visitedFlag);
         (*i)->setDepth(d + 1);            
         if(v == 0)
         {
            q.push(*i);
         }
         /* Case 2:  The child node has already been visited from the other side
          */
         else if(v != visitedFlag)
         {
            q.push(*i);
            return 1;
         }
      }
      /* Case 3:  Normal run of BFSStep() with every child node being added to 
       *          the queue has not been visited before.
       */
      return 2;
   }
   unsigned int bidirectionalSearch(unsigned int start, unsigned int end)
   {
      queue<Node *> startQ, endQ;
      unsigned int result, pathLength;
      clearFlags();
      bool select = true;
      startQ.push(nodes + start);
      endQ.push(nodes + end);
      unsigned int startLength = 0, endLength = 0;
      Node *midpoint = NULL;
      do
      {
         result = select ? BFSStep(startQ, 1) : BFSStep(endQ, 2);
         switch(result)
         {
            case 0:  return 0; break;
            case 1:  midpoint = select ? startQ.back() : endQ.back(); 
                     //select ? startQ.pop() : endQ.pop();
                     break;
            case 2:  select = select ? false : true; break;
         }
      }
      while(midpoint == NULL);
      pathLength = midpoint->getDepth();
      select ? midpoint->setVisited(2) : midpoint->setVisited(1);
      pathLength += midpoint->getDepth();
      return pathLength;
   }
};

class Solution
{
   public:
      int ladderLength(string start, string end, unordered_set<string> &dict)
      {
         unsigned int size =  dict.size();
         if(!start.compare(end))
         {
            return 0;
         }
         Graph *g = new Graph(size + 2);
         if(g->isAdjacentWord(&start, &end))
         {
            return 1;
         }
         g->addWords(dict);
         g->addWord(start);
         g->addWord(end);
         g->makeConnections();
         return g->bidirectionalSearch(size, size + 1);         
      }
};

/* main() not needed for the solution
   Included only for testing purposes
 */
 /*
int main()
{
   Solution s;
   unordered_set<string> dict;
   dict.insert( {"hot","dot","dog","lot","log"} );
   string start("hit");
   string end("cog");
   cout << s.ladderLength(start, end, dict) << endl;
}
*/
