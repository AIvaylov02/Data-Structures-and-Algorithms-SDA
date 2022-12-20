#include <iostream>
#include <string>
#include <utility>
#include <map>

const char INVALID_DIR[] = "INVALID_DIRECTORY_HERE";

struct Directory
{
	Directory(std::string name, Directory* parentDirectory = nullptr);

	void AddSubFolder(const std::string& dirName);
	void PrintSubFolders() const;
	void PrintAbsolutePath() const;
	Directory* ChangeFolder(const std::string& changeTo) const;

private:
	std::string name;
	std::map<std::string, Directory> subdirectories;
	Directory* parentDirectory;

	void printDir(const Directory* current) const;
};

void CompleteRequests(int requests, Directory* current);

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	int requests;
	std::cin >> requests;
	Directory start(INVALID_DIR);
	Directory* current = &start;

	CompleteRequests(requests, current);

	return 0;
}

void CompleteRequests(int requests, Directory* current)
{
	char buff[108];
	for (int i = 0; i < requests; i++)
	{
		std::string firstCommand;
		std::cin >> firstCommand;

		if (firstCommand == "ls" || firstCommand == "pwd")
		{
			if (firstCommand == "ls")
				current->PrintSubFolders();
			else // == pwd
				current->PrintAbsolutePath();
		}
		else if (firstCommand == "mkdir" || firstCommand == "cd") // two-part commands, read an extra word
		{
			std::string secondCommand;
			std::cin >> secondCommand;

			if (firstCommand == "mkdir")
				current->AddSubFolder(secondCommand);
			else // == cd
				current = current->ChangeFolder(secondCommand);
		}
	}
}

Directory::Directory(std::string name, Directory* parentDirectory)
{
	this->name = name;
	this->parentDirectory = parentDirectory;
}

void Directory::AddSubFolder(const std::string& dirName)
{
	if (subdirectories.find(dirName) != subdirectories.end()) // the directory is present in our folder already
		std::cout << "Directory already exists\n";
	else
	{
		Directory newDir(dirName, this); // create a new subfolder and add it to our parent one
		subdirectories.insert(std::pair<std::string, Directory>(dirName, newDir));
	}
}

void Directory::PrintSubFolders() const
{
	for (auto iter = subdirectories.begin(); iter != subdirectories.end(); iter++)
		std::cout << (*iter).first << ' ';
	std::cout << '\n';
}

void Directory::printDir(const Directory* current) const
{
	if (current == nullptr)
		return;
	if (current->name == INVALID_DIR)
	{
		std::cout << '/';
		return;
	}
	printDir(current->parentDirectory);
	std::cout << current->name << '/';
}

void Directory::PrintAbsolutePath() const
{
	printDir(this);
	std::cout << '\n';
}

Directory* Directory::ChangeFolder(const std::string& changeTo) const
{
	if (changeTo == "..") // we want to go to parent folder
	{
		if (parentDirectory == nullptr)
			std::cout << "No such directory\n";
		else
			return parentDirectory;
	}
	else // we want to change to subfolder
	{
		auto wantedDir = subdirectories.find(changeTo);
		if (wantedDir == subdirectories.end())
			std::cout << "No such directory\n";
		else
		{
			Directory* result = const_cast<Directory*>(&((*wantedDir).second));
			return result;
		}
	}
	return const_cast<Directory*>(this);
}


