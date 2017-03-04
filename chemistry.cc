// MIT License

// Copyright (c) 2017 Thibault Dupuis

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <vector>


// Container for elements
struct AtomicElement {

  AtomicElement(std::string short_name, std::string full_name, double weight)
    : short_name(short_name), full_name(full_name), weight(weight) {}

  std::string short_name;
  std::string full_name;
  double weight;
};


// Function who extract atomic elements from the CSV File
std::unordered_map<std::string, AtomicElement> ExtractAtomElements(
    const std::string &path_to_file) {

  // Init an empty map
  std::unordered_map<std::string, AtomicElement> atom_elements;

  // Read the input file
  std::ifstream data_stream(path_to_file);

  if(!data_stream)
    return atom_elements;


  std::string line_readed;

  while(std::getline(data_stream, line_readed)) {

    size_t pos = 0;

    // We check for the short name of atomic element
    pos = line_readed.find(',', line_readed.find(',', 0));

    // If pos = npos, there is an error in the data file
    if(pos == std::string::npos)
      return atom_elements;

    std::string short_name = line_readed.substr(
                               pos + 1,
                               line_readed.find(',', pos + 1) - pos - 1);


    // Check for errors
    if(short_name.empty())
      return atom_elements;

    // Process the short name
    std::string lower_short_name = short_name;
    std::transform(lower_short_name.begin(),
                   lower_short_name.end(),
                   lower_short_name.begin(),
                   ::tolower);

    // Replace the cursor
    pos = line_readed.find(',', pos + 1);

    // Get the full name
    std::string full_name = line_readed.substr(
                              pos + 1,
                              line_readed.find(',', pos + 1) - pos - 1);
    // Check for errors
    if(full_name.empty())
      return atom_elements;

    // Replace the cursor
    pos = line_readed.find(',', pos + 1);


    // Get the atom weight
    std::string temp_weight = line_readed.substr(pos + 1);

    // If temp_weight is empty, value is = 0.0
    double weight = 0.0;

    if(!temp_weight.empty())
      weight = std::stod(temp_weight);


    // Store the atom values
    AtomicElement atom(short_name, full_name, weight);
    atom_elements.insert(std::make_pair(lower_short_name, atom));
  }


  return atom_elements;
}




int main(int argc, char *argv[])
{
  // Normally, the data file is located at the root path
  const std::string kPath_to_csv_file = "data.csv";


  // Extract atom elements from the data file
  std::unordered_map<std::string, AtomicElement> elements = ExtractAtomElements(
                                                             kPath_to_csv_file);
  // Exit the program is the array is empty
  if(elements.empty()) {
    std::cout << "Fatal: Error with the date file: "
              << kPath_to_csv_file
              << '\n';

    return 0;
  }



  return 0;
}
