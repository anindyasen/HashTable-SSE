#include <sys/time.h>
/*
#include "hash_table.hpp"
*/
#include "cntl_bit_sse_cmp_hash_table.hpp"
//Driver method to test map class
int main()
{
    HashMap<int, int> *h = new HashMap<int, int>(160,1);
    //cout << "h->getCapacity(): "<<h->getCapacity() <<endl;
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);
    for(int i =1;i<=100;i++){
       h->insertNode(130+i,i);
    }
    gettimeofday(&tv2, NULL);


    cout<< "Display :"<<endl;
    h->display();
//    cout << "h->getCapacity(): "<<h->getCapacity() <<endl;
    cout << "h->sizeofMap(): "<<h->sizeofMap() <<endl;
    cout << "h->deleteNode(150): "<<h->deleteNode(150)<< endl;
    cout << "h->sizeofMap(): "<<h->sizeofMap() <<endl;
    cout << "h->isEmpty(): "<<h->isEmpty() << endl;
    struct timeval  tv3, tv4;
    gettimeofday(&tv3, NULL);
    h->get(140);
 //   for(int i =1;i<=100000;i++){
        //cout << "h->get("<<(i+130)<<")"<<h->get(i+130)<<endl;
 //       h->get(i+130);
 //   }
    gettimeofday(&tv4, NULL);
    cout << "h->get(21): "<<h->get(21)<<endl;
    /* Cal Insertion/SearchTime 
    printf ("Total insertion time = %f ns\n",
         (((double) (tv2.tv_usec - tv1.tv_usec) / 1000000  +
         (double) (tv2.tv_sec - tv1.tv_sec)) * 1000000000));
    printf ("Total search time = %f ns\n",
         (((double) (tv4.tv_usec - tv3.tv_usec) / 1000000  +
         (double) (tv4.tv_sec - tv3.tv_sec)) * 1000000000));
    //cout<< "Display :"<<endl;
   // h->display();
   */
 
    return 0;
}
