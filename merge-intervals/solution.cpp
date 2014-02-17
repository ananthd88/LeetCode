# include <iostream>
# include <vector>
# include <algorithm>

using namespace std;

/* This struct if pre-defined by the problem and hence cannot be changed */
struct Interval
{
   int start;
   int end;
   Interval() : start(0), end(0) {}
   Interval(int s, int e) : start(s), end(e) {}
};

/* Structure for comparison operation (for sorting) 
   The comparison feature could have been part of the previous struct, however,
   since that was already defined by the problem, the feature had to be moved to
   this struct.
 */
struct LessThan
{
   inline bool operator () (const Interval& i1, const Interval& i2)
   {
      return i1.start < i2.start;
   }
};

class Solution
{
   public:
      /* i1 should be < i2
       */
      bool canMerge(Interval i1, Interval i2)
      {
         return (i1.end >= i2.start);
      }
      
      vector<Interval> merge(vector<Interval> &intervals)
      {
         /* Sort the intervals based on start */
         sort(intervals.begin(), intervals.end(), LessThan());
         
         /* Contains final output */
         vector<Interval> stack;
         for(vector<Interval>::iterator i = intervals.begin();
            i != intervals.end(); i++)
         {
            if(stack.size())
            {
               vector<Interval>::iterator j = stack.end(); j--;
               if(canMerge(*j, *i))
               {
                  (*j).end = (*j).end > (*i).end ? (*j).end : (*i).end;
               }
               else
               {
                  stack.push_back(*i);
               }
            }
            else
            {
               stack.push_back(*i);
            }
         }
         return stack;
      }
};

/* main() not needed for the problem.
   Included simply for testing purposes.
 */
int main()
{
   int numIntervals;
   cin >> numIntervals;
   vector<Interval> intervals(numIntervals);
   
   for(int i = 0; i < numIntervals; i++)
   {
      int start, end;
      cin >> start >> end;
      intervals[i].start = start;
      intervals[i].end = end;
   }
   Solution s;
   vector<Interval> solution = s.merge(intervals);
   for(vector<Interval>::iterator i = solution.begin(); i != solution.end(); 
      i++)
   {
      cout << (*i).start << " " << (*i).end << endl;
   }
   return 0;
}
