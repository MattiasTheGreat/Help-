#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

bool isPlaceholder(string& input) {
	if (input.size() > 0 ) {
		return input[0] == '<';// && input[input.size() - 2] == '>';

	}
	return false;
}

bool isAlias(string placeholder, string other, map<string, string>& placeholders) {
	if (placeholder == other) {
		//cout << "found an alias" << endl;
		return true;
	}
	string alias = placeholders.at(placeholder);
	while (alias != placeholder) {
		if (alias == other) {
			//cout << "found an alias" << endl;
			return true;

		}
		alias = placeholders.at(alias);
	}
	//cout << other << " was not an alias for " << placeholder << endl;
	return false;
}

bool addPlaceholder(string placeholder, string equals, map<string, string>& placeholders) {

	if (isPlaceholder(equals)) {//Both are placeholders
		//cout << "Both are placeholders: " << placeholder << " " << equals << endl;
		if (placeholders.count(placeholder)) {
			if (placeholders.count(equals)) {//both of them are insterted before as placeholders-------
				//cout << "Both are inserted placeholders: " << placeholder << " " << equals << endl;

				if (isAlias(placeholder, equals, placeholders)) {
					return true;
				}
				else {
					string old1 = placeholders.at(placeholder);
					string old2 = placeholders.at(equals);

					//cout << "relations were before:\n placeholder: " << placeholders.at(placeholder) << "\n equals: " << placeholders.at(equals) << endl;

					placeholders[equals] = old1;
					placeholders[placeholder] = old2;
					//cout << "relations are after:\n placeholder: " << placeholders.at(placeholder) << "\n equals: " << placeholders.at(equals) << endl;
					return true;
				}
			}
			else {//one of them is insterted before as a placeholder-------
				//cout << "one is inserted placeholder: " << placeholder << " " << equals << endl;

				string old = placeholders.at(placeholder);
				if (isPlaceholder(old)) {
					placeholders[placeholder] = equals;
					placeholders[equals] = old;
					return true;
				}
				else {
					placeholders.insert(pair<string, string>(equals, old));
					return true;
				}
			}

		}else if (placeholders.count(equals)) {
			//cout << "one is inserted placeholders: " << placeholder << " " << equals << endl;

			addPlaceholder(equals, placeholder, placeholders);
			return true;
		}
		else {//none of the are added before as placeholders--------------------------------
			placeholders.insert(pair<string, string>(placeholder, equals));
			placeholders.insert(pair<string, string>(equals, placeholder));
			return true;
		}
	}
	else {//equals is not a placeholder
		//cout << "Equals is not a placeholder: " << placeholder << " " << equals << endl;

		if (placeholders.count(placeholder)) {
			//cout << "Placeholder was inserted before " << placeholder << " " << equals << endl;

			string old = placeholders.at(placeholder);
			//cout << "placeholder " << placeholder << " updated to " << equals << endl;
			placeholders[placeholder] = equals;
			if (!isPlaceholder(old)) {
				if (old != equals) {
					return false;
				}
				else {
					return true;
				}
			}
			else {
				//cout << "placeholder held a placeholder: " << placeholder << " " << equals << endl;

				return addPlaceholder(old, equals, placeholders);
			}
		}
		else {
			//cout << "placeholder was not inserted before: " << placeholder << " " << equals << endl;

			placeholders.insert(pair<string,string>(placeholder, equals));
			return true;
		}
	}
	//cout << "I dont want to reach here" << endl;
}

string cutFirstWord(string& sentence) {
	int pos = sentence.find(" ");
	string first_word;
	if (pos == string::npos) {
		first_word = sentence;
		sentence = "";
	}
	else {
		first_word = sentence.substr(0, pos);
		sentence.erase(0, pos + 1);
	}
	return first_word;
}

string validate(string& first, string& second, map<string, string>& placeholders) {

	if (first.empty()) {
		return  "";
	}
	else {
		//cout << "test: " << first << endl;
		string first_word = cutFirstWord(first);
		string second_word = cutFirstWord(second);

		if (isPlaceholder(first_word)) {
			first_word += "1";
			if (isPlaceholder(second_word)) {
				second_word += "2";
			}
			if(!addPlaceholder(first_word, second_word, placeholders))
				return "-";
		}
		else {
			if (isPlaceholder(second_word)) {
				second_word += "2";
				if(!addPlaceholder(second_word, first_word, placeholders))
					return "-";
			}
			else {
				if (first_word != second_word)
					return "-";
			}
		}

		string result = validate(first, second, placeholders);
		//cout << result << endl;

		if (result != "-") {
			if (isPlaceholder(first_word)) {
				string word = placeholders.at(first_word);
				if (!isPlaceholder(word)) {
					return word + " " + result;
				}
				else {
					return "-";
				}
			}
			else {
				
				return first_word + " " + result;
			}
		}
		else
			return "-";
	}

}


int main() {
	int testCases;

	cin >> testCases;
	{
		string throwaway;
		getline(cin, throwaway);
	}

	for (int i = 0; i < testCases; ++i) {
		string first;
		string second;
		getline(cin,first);
		getline(cin, second);

		map<string, string> placeholders;
		cout << validate(first, second, placeholders);
		cout << endl;
	}
	cout << endl;
}