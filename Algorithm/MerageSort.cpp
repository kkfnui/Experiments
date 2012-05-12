/*
�鲢�㷨
�ο���http://zh.wikipedia.org/zh/%E5%BD%92%E5%B9%B6%E6%8E%92%E5%BA%8F
�㷨�߼���
    1.����ռ䣬ʹ���СΪ�����Ѿ���������֮�ͣ��ÿռ�������źϲ��������
    2.�趨����ָ�룬���λ�÷ֱ�Ϊ�����Ѿ��������е���ʼλ��
    3.�Ƚ�����ָ����ָ���Ԫ�أ�ѡ�����С��Ԫ�ط��뵽�ϲ��ռ䣬���ƶ�ָ�뵽��һλ��
    4.�ظ�����3ֱ��ĳһָ��ﵽ����β
    5.����һ����ʣ�µ�����Ԫ��ֱ�Ӹ��Ƶ��ϲ�����β
*/
//#ֻ��Ƀɶ�֮�g�w��Ĺ���#%
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
    while(seg_start_ind <= size - 2 * seg) //#size - 2 * seg����˼�ǝM��ɃɃɚw�������R��ֵ#%
    {
        Merge(a, b, seg_start_ind, seg_start_ind + seg - 1, seg_start_ind + seg * 2 - 1);
        seg_start_ind += 2 * seg;
    }
    //#���һ�������ÿɚw��Ĕ�������һ�΄t������ÿɚw��Ĕ���#%
    if(seg_start_ind + seg < size)
        Merge(a, b, seg_start_ind, seg_start_ind + seg - 1, size - 1);
    else
        for(int j = seg_start_ind; j < size; j++) //#���ֻʣ��һ�λ��߸��ٵĔ���#%
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
    //#ݔ��#%
    for(int i = 0; i < sizeof(a) / sizeof(*a); i++)
        cout << a[i] << ' ';
    cout << endl;

    return 0;
}

