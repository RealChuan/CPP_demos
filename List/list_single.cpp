#include"iostream"

using namespace std;

struct student
{
	int id;
	int grades;
	student *next;
	student(int x,int y):id(x),grades(y),next(nullptr){}
};

student* createList()
{
	student *head, *p, *q;
	head=p=nullptr;
	for(int i=0;i<10;i++)
	{
		q=new student(i+1,100-i);
		if(nullptr==head)
		{
			head=q;
		}
		else
		{
			p->next=q;
		}
		p=q;
	}
	p->next=nullptr;
	return head;
}

void showList(student* head)
{
	if(head==nullptr)
	{
		cout<<"链表为空"<<endl;
		return;
	}
	cout << "学号\t分数" << endl;
	student* p;
	cout << head->id <<"\t"<<head->grades<< endl;
	p = head->next;
	while (p)
	{
		cout << p->id <<"\t"<<p->grades<< endl;
		p = p->next;
	}
}

void delList(student* head)
{
	student *p;
	p=head;
	while (p)
	{
		head=head->next;
		delete p;
		p=nullptr;
		p=head;
	}
	cout<<"链表释放完成！"<<endl;
}

student* reverseList1(student* head)
{
	if (nullptr == head || nullptr == head->next)
		return head;
	student *p,*q,*r;
	p = head;
	q = head->next;
	head->next = nullptr;
	while (q)
	{
		r = q->next;
		q->next = p;
		p = q;
		q = r;
	}
	head = p;
	return head;
}

student* reverseList2(student* head)
{
	student *p,*q;
	p = head->next;
	while (p->next != nullptr)
	{
		q = p->next;
		p->next = q->next;
		q->next = head->next;
		head->next = q;
	}
	p->next = head;				//相当于成环
	head = p->next->next;		//新head变为原head的next
	p->next->next = nullptr;	//断掉环
	return head;
}

int main()
{
	student* s1=createList();
	showList(s1);
	cout<<"链表反转一次"<<endl;
	s1=reverseList1(s1);
	showList(s1);
	cout<<"链表反转二次"<<endl;
	s1=reverseList2(s1);
	showList(s1);
	delList(s1);
	return 0;
}
