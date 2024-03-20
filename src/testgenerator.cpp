/**
 * @file testgenerator.cpp
 * @author Mihail Croitor (mcroitor@gmail.com)
 * @brief This is a simple test data generator that reads a configuration file and
 * generates random test data
 * @version 0.1
 * @date 2024-02-20
 *
 * @copyright Copyright (c) 2024
 *
 */

/*
 * The configuration file is a YAML file with the following structure:
 * filename: "test%.txt"
 * nr_tests: 10
 * description: "Test description"
 * lines:
 *   - - type: integer
 *       min: 1
 *       max: 100
 *       name: N
 *   - - type: array
 *       size: N
 *       element:
 *         type: integer
 *         min: -100
 *         max: 100
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>

#include "generators.hpp"
#include "formatparser.hpp"

extern std::map<std::string, node_to_type> convertor;

/**
 * @brief Generate a test based on the given structure
 *
 * @param structure
 * @return std::string
 */
std::string generate_test(const YAML::Node &lines)
{
  std::string result;
  for (const auto &line : lines)
  {
    for (const auto &node : line)
    {
      auto type = node["type"].as<std::string>();

      if (convertor.find(type) != convertor.end())
      {
        result += convertor.at(type)(node) + " ";
      }
      else if (type == "array")
      {
        result += node_to_vector(node) + " ";
      }
      else
      {
        std::cerr << "Unknown type: " << type << std::endl;
      }
    }
    result += "\n";
  }
  return result;
}

/**
 * @brief Generate a sample test description file
 *
 */
void sample_test_description_generator()
{
  std::ofstream file("sample_test_description.yaml");
  if (!file)
  {
    std::cerr << "Error: could not open file sample_test_description.yaml" << std::endl;
    return;
  }

  YAML::Node config;

  config["filename"] = "input%.txt";
  config["from"] = 1;
  config["to"] = 10;
  config["description"] = "Sample test description";
  config["lines"].push_back(YAML::Load("- type: integer\n  min: 1\n  max: 100\n  name: N"));
  config["lines"].push_back(YAML::Load("- type: float\n  min: 0\n  max: 1"));
  config["lines"].push_back(YAML::Load("- type: array\n  size: N\n  element:\n    type: integer\n    min: -100\n    max: 100"));

  file << config;
  file.close();

  std::cout << "Sample test description file generated" << std::endl;
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::string filename = std::filesystem::path(argv[0]).filename().string();
    std::cerr << "Usage: " << filename << " <config_file> | --sample" << std::endl;
    std::cerr << "\t--sample \t- generate a sample test description file" << std::endl;
    return 1;
  }

  std::string config{argv[1]};

  if (config == "--sample")
  {
    sample_test_description_generator();
    return 0;
  }

  // Load the configuration file
  const std::vector<YAML::Node> config_nodes = YAML::LoadAllFromFile(config);

  for (const auto &config_node : config_nodes)
  {
    std::string filename_template = config_node["filename"].as<std::string>();
    int from = config_node["from"].as<int>();
    int to = config_node["to"].as<int>();
    const auto &lines = config_node["lines"];

    std::cout << "Generating " << (to - from + 1) << " tests." << std::endl;

    for (int i = from; i <= to; ++i)
    {
      std::string filename = filename_template;
      std::string test_number = std::to_string(i);
      filename.replace(filename.find("%"), 1, test_number);
      std::ofstream file(filename);

      std::cerr << "Generating test " << filename << std::endl;

      if (!file)
      {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return 1;
      }

      file << generate_test(lines);

      file.close();
      std::cout << "Test " << filename << " generated." << std::endl;
    }
  }

  return 0;
}
