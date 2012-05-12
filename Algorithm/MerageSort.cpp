/*
归并算法
参考；http://zh.wikipedia.org/zh/%E5%BD%92%E5%B9%B6%E6%8E%92%E5%BA%8F
算法逻辑：
    1.申请空间，使其大小为两个已经排序序列之和，该空间用来存放合并后的序列
    2.设定两个指针，最初位置分别为两个已经排序序列的起始位置
    3.比较两个指针所指向的元素，选择相对小的元素放入到合并空间，并移动指针到下一位置
    4.重复步骤3直到某一指针达到序列尾
    5.将另一序列剩下的所有元素直接复制到合并序列尾
*/
//#只完成兩段之間歸併的功能#%
#include <iostream>
using namespace std;
void Merge(int a[], int b[], int low, int mid, int high)
{
    int k = low;
    int begin1 = low;
    int end1 = mid;
    int begin2 = mid + 1;
    int end2 = high;
    while(begin1 <= end1 && begin2 <= end2)
    {
        if(a[begin1] <= a[begin2])
            b[k++] = a[begin1++];
        else
            b[k++] = a[begin2++];
    }
    if(begin1 <= end1)
        for(int q = begin1; q <= end1; q++)
            b[k++] = a[q];
    else
        for(int q = begin2; q <= end2; q++)
            b[k++] = a[q];
}

void MergePass(int a[], int b[], int seg, int size)
{
    int seg_start_ind = 0;
    while(seg_start_ind <= size - 2 * seg) //#size - 2 * seg的意思是滿足可兩兩歸併的最低臨界值#%
    {
        Merge(a, b, seg_start_ind, seg_start_ind + seg - 1, seg_start_ind + seg * 2 - 1);
        seg_start_ind += 2 * seg;
    }
    //#如果一段是正好可歸併的數量而另一段則少於正好可歸併的數量#%
    if(seg_start_ind + seg < size)
        Merge(a, b, seg_start_ind, seg_start_ind + seg - 1, size - 1);
    else
        for(int j = seg_start_ind; j < size; j++) //#如果只剩下一段或者更少的數量#%
            b[j] = a[j];
}

void MergeSort(int a[], int size)
{
    int* temp = new int[size];
    int seg = 1;
    while(seg < size)
    {
        MergePass(a, temp, seg, size);
        seg += seg;
        MergePass(temp, a, seg, size);
        seg += seg;
    }
}

int main()
{
    int a[] = {3, 5, 3, 6, 4, 7, 5, 7, 4}; //#QQ#%
    MergeSort(a, sizeof(a) / sizeof(*a));
    //#輸出#%
    for(int i = 0; i < sizeof(a) / sizeof(*a); i++)
        cout << a[i] << ' ';
    cout << endl;

    return 0;
}

