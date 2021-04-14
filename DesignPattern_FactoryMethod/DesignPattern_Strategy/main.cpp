#include <iostream>
#include <algorithm>

using namespace std;

class SortInterface
{
public:
	SortInterface() {}
	virtual ~SortInterface() {}

	virtual void DoSort(int* arr) = 0;
};

class QuickSort : public SortInterface
{
private:
	void QSort(int* arr, int s, int e)
	{
		if (s >= e) return;

		int p = s;
		int l = s+1, r = e;

		do
		{
			while (arr[l] < arr[p]) ++l;
			while (arr[r] > arr[p]) --r;
			if (l < r) swap(arr[l], arr[r]);
		} while (l < r);

		swap(arr[r], arr[p]);

		QSort(arr, s, r-1);
		QSort(arr, r+1, e);
	}

public:
	QuickSort() {}
	~QuickSort() {}


	virtual void DoSort(int* arr) override
	{
		QSort(arr, 0, 9);
		cout << "퀵정렬 알고리즘 사용" << endl;
	}
};

class BubbleSort : public SortInterface
{
public:
	BubbleSort() {}
	~BubbleSort() {}

	virtual void DoSort(int* arr) override
	{
		for (int i = 0; i < 9; ++i)
		{
			for (int j = i + 1; j < 10; ++j)
			{
				if (arr[j] < arr[i]) swap(arr[j], arr[i]);
			}
		}
		//for (int i = 9; i > 0; --i)
		//{
		//	for (int j = 0; j < i; ++j)
		//	{
		//		if (arr[j+1] < arr[j]) swap(arr[j], arr[j+1]);
		//	}
		//}
		cout << "버블정렬 알고리즘 사용" << endl;
	}
};

class SelectionSort : public SortInterface
{
public:
	SelectionSort() {}
	~SelectionSort() {}

	virtual void DoSort(int* arr) override
	{
		cout << "선택정렬 알고리즘 사용" << endl;
	}
};

class InsertionSort : public SortInterface
{
public:
	InsertionSort() {}
	~InsertionSort() {}

	virtual void DoSort(int* arr) override
	{
		for (int i = 1; i < 10; ++i)
		{
			for (int j = 0; j < i; ++j)
			{
				if (arr[i] < arr[j])
				{
					int num = arr[i];
					memcpy(&arr[j + 1], &arr[j], (i - j) * sizeof(int));
					arr[j] = num;
					break;
				}
			}
		}

		cout << "삽입정렬 알고리즘 사용" << endl;
	}
};

class MergeSort : public SortInterface
{
public:
	MergeSort() {}
	~MergeSort() {}

	virtual void DoSort(int* arr) override
	{
		cout << "병합정렬 알고리즘 사용" << endl;
	}
};

class HeapSort : public SortInterface
{
public:
	HeapSort() {}
	~HeapSort() {}

	virtual void DoSort(int* arr) override
	{
		cout << "힙정렬 알고리즘 사용" << endl;
	}
};

class SortManager
{
private:
	SortInterface* sort;

public:
	SortManager():sort(nullptr) {}
	~SortManager() { if (sort) delete sort; }

	void DoSort(int* arr)
	{
		if (sort)
		{
			sort->DoSort(arr);
		}
	}

	void ChangeSort(SortInterface* otherSort)
	{
		if (sort) delete sort;
		sort = otherSort;
	}
};

int main()
{
	int testArray[10] = {4, 3, 3, 1, 2, 9, 0, 7, 6 ,5};

	SortManager sortMgr;
	
	//sortMgr.ChangeSort(new BubbleSort());
	//sortMgr.DoSort(testArray);

	//sortMgr.ChangeSort(new InsertionSort());
	//sortMgr.DoSort(testArray);

	//sortMgr.ChangeSort(new SelectionSort());
	//sortMgr.DoSort(testArray);
	//
	sortMgr.ChangeSort(new QuickSort());
	sortMgr.DoSort(testArray);
	//
	//sortMgr.ChangeSort(new MergeSort());
	//sortMgr.DoSort(testArray);
	//
	//sortMgr.ChangeSort(new HeapSort());
	//sortMgr.DoSort(testArray);

	return 0;
}