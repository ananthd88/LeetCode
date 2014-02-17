class Solution 
{
public:
   int firstMissingPositive(int A[], int n) 
   {
      int minPos = 0;
      int minPosIndex = 0;
      for(int i = 0; i < n; i++)
      {
         
         if(!minPos && A[i] > 0)
         {
            minPos = A[i];
            minPosIndex = i;
            continue;
         }
         if(minPos > A[i] && A[i] > 0)
         {
            minPos = A[i];
            minPosIndex = i;
         }
      }
      if(minPos != 1)
      {
         return 1;         
      }
      else
         return restack(A, n, minPos);
      
   }
   int restack(int A[], int n, int minPos)
   {
      int source, restart = 0;
      int val = minPos;
      int temp;
      for(int i = 0; i < n; i++)
      {
         if(A[i] == minPos + i)
         {
            continue;
         }
         else
         {
            source = i;
            while((A[source] >= minPos) && (A[source] < minPos + n) && 
               (A[source] != minPos + source) && 
               (A[source] != A[A[source] - minPos]))
            {
               temp = A[A[source] - minPos];
               A[A[source] - minPos] = A[source];
               A[source] = temp;
            }
         }
         
      }
      for(int i = 0; i < n; i++)
      {
         if(A[i] != minPos + i)
         {
            return minPos + i;
         }
      }
      return minPos + n;
   }
};
