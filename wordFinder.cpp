#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

class Dictionary
{
private:
	std::unordered_set<std::string> words;
public:
	Dictionary()
	{}

	bool loadFromFile(std::string file_name)
	{
		std::ifstream infile(file_name.c_str());
		if (!infile.is_open())
		{
			std::cout << file_name << " is not opened." << std::endl;
			return false;
		}

		//just push_back 
		std::string line;
		while(getline(infile, line))
		{
			std::transform(line.begin(), line.end(), line.begin(), ::tolower);
			words.insert(line);
		}

		infile.close();
		return true;
	}

	bool checkExist(std::string word)
	{
		return words.find(word) != words.end();

	}
};

void rec_permutation(const std::vector<char>& chars, std::vector<char>& chars_so_far, 
					 size_t cur_index, size_t min_char_size,
	                 std::vector<size_t>& visited, std::unordered_set<size_t>& unvisited, 
	                 std::unordered_set<std::string>& words)
{
	//add the current char to chars_so_far and add words
	chars_so_far.push_back(chars[cur_index]);
	if (chars_so_far.size() >= min_char_size)
		words.insert(std::string(chars_so_far.begin(), chars_so_far.end()));

	//if the last letter
	if (unvisited.size() == 1)
	{
		chars_so_far.pop_back();
		return;
	}
	else
	{
		visited.push_back(cur_index);
		unvisited.erase(cur_index);
		std::vector<size_t> cur_unvisited(unvisited.begin(), unvisited.end());
		for (size_t i: cur_unvisited)
		{
			rec_permutation(chars, chars_so_far,
							i, min_char_size,
							visited, unvisited,
							words);
		}
		unvisited.insert(cur_index);
		visited.pop_back();
		chars_so_far.pop_back();
		return;
	}
}
void createPermutations(const std::vector<char>& chars, size_t min_char_size, std::unordered_set<std::string>& words)
{
	std::vector<char> chars_so_far;
	std::vector<size_t> visited;
	std::unordered_set<size_t> unvisited;
	for (size_t i = 0; i < chars.size(); ++i)
	{
		unvisited.insert(i);
	}

	for (size_t i = 0; i < chars.size(); ++i)
	{
		rec_permutation(chars, chars_so_far, i, min_char_size, visited, unvisited, words);
	}
}

int main(int argv, char* argc[])
{
	if (argv < 2)
	{
		std::cout << "need to enter at least 2 charactors" << std::endl;
		return 0;
	}

	int min_char_size = (int)argc[1][0] - 48;
	//get charactors
	std::vector<char> chars;
	std::string arg_str = argc[2];
	for (size_t i = 0; i < arg_str.length(); ++i)
	{
		chars.push_back(std::tolower(arg_str[i]));
	}

	// for (int i = 2; i < argv; ++i)
	// {
	// 	std::string str = argc[i];
	// 	chars.push_back(std::tolower(str[0]));
	// }

	std::unordered_set<std::string> words;
	createPermutations(chars, (size_t)min_char_size, words);

	Dictionary dict;
	std::string file_name = "dictionary.txt";
	dict.loadFromFile(file_name);

	std::unordered_map<size_t, std::vector<std::string> > true_words;
	std::vector<size_t> lengths;
	for (std::string w: words)
	{
		if(dict.checkExist(w))
		{
			size_t len = w.length();
			if (true_words.find(len) == true_words.end())
			{
				lengths.push_back(len);
				std::vector<std::string> temp;
				temp.push_back(w);
				true_words[len] = temp;
			}
			else
			{
				true_words[len].push_back(w);
			}
		}
	}

	std::sort(lengths.begin(), lengths.end());
	std::ofstream outfile("found_words.txt");
	size_t total_num_words = 0;
	for (size_t i : lengths)
	{
		std::vector<std::string> temp = true_words[i];
		std::sort(temp.begin(), temp.end());
		outfile << "length[" << i << "]: " << temp.size() << "\n";
		std::cout << "length[" << i << "]: " << temp.size() << std::endl;
		total_num_words += temp.size();
		for (std::string str: temp)
		{
			outfile << str << "\n";
			std::cout << str << "\n";
		}
		outfile << "\n";
		std::cout << std::endl;
	}
	outfile << "total number of words: " <<total_num_words << std::endl;
	std::cout << "total number of words: " <<total_num_words << std::endl;
	outfile.close();

	return 0;

}
