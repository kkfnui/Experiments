#include <iostream>
#include <list>
using namespace std;

/*
这个排序用的是stl中list的排序，list的sort方法用的是归并排序（http://blog.csdn.net/csufuyi/article/details/6540315）
本程序的用途：为加载多个图片并排列作准备
*/
struct rect{

public:
	int width;
	int height;

	rect(int x, int y)
	{
		width = x;
		height = y;
	}
};

typedef list<rect*> rect_list;
typedef rect_list::iterator rect_list_iter;

void print_list(rect_list list)
{
	rect_list_iter iter = list.begin();
	int i = 1;
	for(;iter != list.end(); iter++)
	{
		cout<<"\tNum"<< i;
		cout<<".x="<<(*iter)->width;
		cout<<",y="<<(*iter)->height<<endl;
		i++;
	}
}

rect_list init_list()
{
	rect_list list;
	rect* rect1 = new rect(5, 10);
	rect* rect2 = new rect(4, 9);
	rect* rect3 = new rect(5, 11);
	rect* rect4 = new rect(8, 14);
	rect* rect5 = new rect(6, 5);
	list.push_back(rect1);
	list.push_back(rect2);
	list.push_back(rect3);
	list.push_back(rect4);
	list.push_back(rect5);
	return list;
}

bool compare(rect* first, rect* second)
{
	if(first->width > second->width)
	{
		return true;
	}
	else if(first->width == second->width)
	{
		if(first->height >= second->height)
		{
			return true;
		}
	}
	return false;
}

int main()
{
	rect_list list;
	list = init_list();
	cout<<"list before sort:"<<endl;
	print_list(list);
	list.sort(compare);
	cout<<"list after sort:"<<endl;
	print_list(list);
    return 0;
}

