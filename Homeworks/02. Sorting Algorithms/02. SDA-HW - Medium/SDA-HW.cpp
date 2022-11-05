#include <iostream>
#include <cstring>
#pragma warning (disable:4996)
// 10^5 means at worst O(N*sqrt(N)) complexity so we will have to use a fast sorting mechanism

class Student
{
public:
	// Because we will not use the names in any other part of the programme, there is no point in copying them and deleting the old ones -> we will just steal the pointers
	Student(char* name, int score);

	Student(const Student& other);
	Student& operator=(const Student& other);
	~Student();

	bool operator<(const Student& other);
	
	friend std::ostream& operator<< (std::ostream& outputStream, const Student& st);
private:
	char* name = nullptr;
	int score;

	void free();
	void copyFrom(const Student& other);
};

void fillArrays(char** stNames, int* scores, int count);
void fillCollection(Student** students,char* const* names, const int* scores, int count);
void freeCollection(Student** students, int studentsCount);

void PrintStudents(Student** students, int studentsCount);

void MergeArrays(Student** students, int startIndex, int mid, int endIndex);
void MergeSort(Student** students, int startIndex, int endIndex);

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	int studentsCount;
	std::cin >> studentsCount;
	char** stNames = new char*[studentsCount];
	for (int i = 0; i < studentsCount; i++)
		stNames[i] = new char[10];
	int* scores = new int[studentsCount];
	fillArrays(stNames, scores, studentsCount);

	// we use collection of ptr to objects, in order to not worry about default constructor and be able to move objects at high speeds, a.k.a just move the ptr-s
	Student** students = new Student*[studentsCount];
	fillCollection(students, stNames, scores, studentsCount);
	delete[] stNames; // we delete the outer ptr only, because we will still use the names in our studentsCollection
	delete[] scores;

	MergeSort(students, 0, studentsCount - 1);
	PrintStudents(students, studentsCount);
	freeCollection(students, studentsCount);
	return 0;
}

void fillArrays(char** stNames, int* scores, int count)
{
	for (int currName = 0; currName < count; currName++)
		std::cin >> stNames[currName];

	for (int currScore = 0; currScore < count; currScore++)
		std::cin >> scores[currScore];
}

void fillCollection(Student** students,char* const* names, const int* scores, int count)
{
	for (int i = 0; i < count; i++)
		students[i] = new Student(names[i], scores[i]);
}

void freeCollection(Student** students, int studentsCount)
{
	for (int i = 0; i < studentsCount; i++)
		delete students[i]; // we delete each student
	delete[] students; // we free up the collection
	students = nullptr;
}

Student::Student(char* name, int score)
{
	this->name = name;
	/*this->name = new char[strlen(name) + 1];  -> if we were to copy the names
	strcpy(this->name, name);*/
	this->score = score;
}

void Student::free()
{
	delete[] name;
}
void Student::copyFrom(const Student& other)
{
	name = new char[strlen(other.name) + 1];
	strcpy(name, other.name);
	score = other.score;
}

Student::Student(const Student& other)
{
	copyFrom(other);
}
Student& Student::operator=(const Student& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}
Student::~Student()
{
	free();
}

bool Student::operator<(const Student& other) // Az - 10, Mika - 20, Kiryakov - 30
{
	if (score < other.score) // if the right student score is higher than the current Student, swap them (we want to have the biggest at the front)
		return false;
	else if (score > other.score)
		return true;
	
	// both scores are equal, we should sort by name
	else if (strcmp(name, other.name) < 0) // if the second name is alphabetically after the first one
		return true;
	else
		return false;
}

std::ostream& operator<< (std::ostream& outputStream, const Student& st)
{
	outputStream << st.name << " " << st.score << "\n";
	return outputStream;
}

void PrintStudents(Student** students, int studentsCount)
{
	for (int i = 0; i < studentsCount; i++)
		std::cout << *students[i];
}

void MergeSort(Student** students, int startIndex, int endIndex)
{
	if (startIndex >= endIndex)
		return;

	int mid = startIndex + (endIndex - startIndex) / 2;

	MergeSort(students, startIndex, mid);
	MergeSort(students, mid + 1, endIndex);

	MergeArrays(students, startIndex, mid, endIndex);
}

void MergeArrays(Student** students, int startIndex, int mid, int endIndex)
{
	const int LEFT_SIZE = mid - startIndex + 1;
	const int RIGHT_SIZE = endIndex - mid;

	Student** leftArr = new Student*[LEFT_SIZE];
	Student** rightArr = new Student*[RIGHT_SIZE];

	// we split up the array in half and just fill it up on both sides
	for (int i = 0; i < LEFT_SIZE; i++)
	{
		leftArr[i] = new Student(*students[startIndex + i]);
	}
	for (int i = 0; i < RIGHT_SIZE; i++)
	{
		rightArr[i] = new Student(*students[mid + 1 + i]);
	}

	// We have valid copies of the objects, we can start to overlap the original array.
	// We need to make copies and not just go with ptr-s because at some point some same memory slot will be overwritten simultaneously 
	// by the sub-array and the big array, which will not be ideal

	// time to start sorting
	int leftIndex, rightIndex;
	leftIndex = rightIndex = 0;
	while (leftIndex < LEFT_SIZE && rightIndex < RIGHT_SIZE)
	{
		// we should operator= the objects
		if (*leftArr[leftIndex] < *rightArr[rightIndex])
		{
			*students[startIndex++] = *leftArr[leftIndex++];
		}
		else
		{
			*students[startIndex++] = *rightArr[rightIndex++];
		}
	}

	// one of the two arrays has already ended, we have to foreach the remaining elements
	while (leftIndex < LEFT_SIZE)
	{
		*students[startIndex++] = *leftArr[leftIndex++];
	}
	while (rightIndex < RIGHT_SIZE)
	{
		*students[startIndex++] = *rightArr[rightIndex++];
	}
	
	for (int i = 0; i < LEFT_SIZE; i++)
		delete leftArr[i];
	delete[] leftArr;
	for (int i = 0; i < RIGHT_SIZE; i++)
		delete rightArr[i];
	delete[] rightArr;
}