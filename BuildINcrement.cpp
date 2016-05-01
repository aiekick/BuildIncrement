#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
using namespace std;

#include "tinyxml2.h"
using namespace tinyxml2;

template <typename T> string toStr(const T& t) { ostringstream os; os << t; return os.str(); } // sstream

// #define Build 4588
// #define Major 1
// #define Minor 0

static std::vector<std::string> splitString(const std::string& text, char delimiter)
{
	std::vector<std::string> result;

	std::string::size_type start = 0;
	std::string::size_type end = text.find(delimiter, start);

	while (end != string::npos)
	{
		std::string token = text.substr(start, end - start);

		result.push_back(token);

		start = end + 1;
		end = text.find(delimiter, start);
	}

	result.push_back(text.substr(start));

	return result;
}

#define BUILD_WORD "BuildNumber"
#define MAJOR_WORD "MajorNumber"
#define MINOR_WORD "MinorNumber"

static bool ReplaceString(std::string& str, const std::string& oldStr, const std::string& newStr)
{
	bool Finded = false;
	size_t pos = 0;
	while ((pos = str.find(oldStr, pos)) != std::string::npos){
		Finded = true;
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
	return Finded;
}

int main(int argc, char *argv[]) // Don't forget first integral argument 'argc'
{
	// check if there is more than one argument and use the second one
	//  (the first argument is the executable)
	if (argc > 1)
	{
		// file name Build.h
		std::string headerFileName(argv[1]);

		// read file if exist
		ifstream rHeaderfile(headerFileName, ios::in);
		if (rHeaderfile.is_open())
		{
			map<string, int> dico;
			
			dico[BUILD_WORD] = 0;
			dico[MAJOR_WORD] = 0;
			dico[MINOR_WORD] = 0;

			string buffer;

			for (int i = 0; i < 3; i++)
			{
				getline(rHeaderfile, buffer);

				vector<string> vec = splitString(buffer, ' ');
				if (vec.size() == 3)
				{
					if (vec[0] == "#define")
					{
						string typeStr = vec[1];
						string numStr = vec[2];

						if (dico.find(typeStr) != dico.end())
						{
							int num = stoi(numStr);
							if (typeStr == BUILD_WORD)
								num++;

							dico[typeStr] = num;
						}
					}
					else
					{
						cout << "'#define' not founded !\n";
					}
				}
			}

			rHeaderfile.close(); // close file

			// write build num to file
			ofstream wHeaderfile(headerFileName, ios::out | ios::trunc);
			if (wHeaderfile.is_open())
			{
				for (map<string, int>::iterator it = dico.begin(); it != dico.end(); ++it)
				{
					string str = it->first;
					int code = it->second;

					wHeaderfile << "#define" << " " << str << " " << code << endl;
					cout << "Current " << str << " : " << code << endl;
				}

				wHeaderfile.close();
			}
			else
			{
				cout << "Unable to write to header file";
			}

			if (argc > 2) // AndroidManifest.xml
			{
				string android_versionCode = toStr(dico[BUILD_WORD]);
				string android_versionName = toStr(dico[MAJOR_WORD]) + "." + toStr(dico[MINOR_WORD]) + "." + toStr(dico[BUILD_WORD]);
					
				tinyxml2::XMLDocument doc;
				tinyxml2::XMLError err = doc.LoadFile(argv[2]);

				if (err == tinyxml2::XMLError::XML_SUCCESS)
				{
					tinyxml2::XMLElement *manifest = doc.FirstChildElement("manifest");

					if (manifest != 0)
					{
						manifest->SetAttribute("android:versionCode", android_versionCode.c_str());
						cout << "android:versionCode" << " : " << android_versionCode << endl;

						manifest->SetAttribute("android:versionName", android_versionName.c_str());
						cout << "android:versionName" << " : " << android_versionName << endl;
					}

					if (doc.SaveFile(argv[2]) != tinyxml2::XMLError::XML_SUCCESS)
					{
						cout << "Unable to write to Android Manifest file ";
					}
				}
				else
				{
					cout << "Unable to read Android Manifest file ";
				}
			}
		}
		else
		{
			cout << "Unable to open header file";
		}
	}

	return 0;
}


