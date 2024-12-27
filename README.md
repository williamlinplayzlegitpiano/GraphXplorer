# 🚀 GraphXplorer: A C++ Graph Library

Welcome to **GraphXplorer**, a powerful and flexible C++ library for working with undirected graphs featuring non-negative edge weights. This project allows you to load graphs from CSV files and provides various functionalities such as exploring graph properties, finding shortest paths, connected components, and more.

## 📂 Project Structure

- **Graph.h**: Header file containing the `Graph` class definition and its member functions.
- **Graph.cpp**: Implementation of the `Graph` class, including core algorithms and graph operations.
- **GraphTest.cpp**: A testing suite for validating the graph functionalities with various test cases.
- **Makefile**: Build system to compile the project and manage dependencies.

## 🔧 Features

1. **Graph Properties**
   - Get the number of nodes and edges.
   - Retrieve a list of node labels.
   - Query the weight of an edge and the neighbors of a node.

2. **Shortest Path Algorithms**
   - **Unweighted Shortest Path**: Find the shortest path using breadth-first search.
   - **Weighted Shortest Path**: Use Dijkstra's algorithm for edge-weighted graphs.

3. **Connected Components**
   - Identify connected components under a given edge weight threshold.

4. **Smallest Connecting Threshold**
   - Determine the minimum edge weight required to connect two nodes.

## 🚀 Getting Started

### Prerequisites
- C++ compiler supporting C++17 or later (e.g., g++, clang++)
- Make utility

### Installation
Clone this repository to your local machine:
```
git clone https://github.com/yourusername/GraphXplorer.git
cd GraphXplorer
```

Build
To build the project, use the provided Makefile:

```
make
```
This will create an executable named GraphTest.

### Run
You can execute the program with the following usage:
```
./GraphTest <edgelist_csv> <test>
```
<edgelist_csv>: Path to the CSV file containing the edge list (u,v,w format).
<test>: One of the following test options:
- graph_properties: Outputs basic graph properties.
- shortest_unweighted: Finds unweighted shortest paths between nodes.
- shortest_weighted: Finds weighted shortest paths using Dijkstra's algorithm.
- connected_components: Identifies connected components under a weight threshold.
- smallest_threshold: Determines the smallest edge weight to connect two nodes.
Example:
```
./GraphTest sample.csv graph_properties
```
## 📁 Example Edge List File
Create a CSV file sample.csv with the following content:

```
A,B,0.5
A,C,1.0
B,C,0.2
C,D,0.8
E,F,1.2
```

## 🛠 Development
### Debugging
To build the project with debugging information, use:

```
make gprof
```

### Cleanup
To remove the compiled files, run:

```
make clean
```

## 🧪 Testing
Test the program with various edge list inputs and tests to validate its correctness. Customize the provided test cases or add your own in GraphTest.cpp.

## 🤝 Contributing
Contributions are welcome! Feel free to open issues or submit pull requests with improvements, bug fixes, or new features.

## 📜 License
This project is licensed under the MIT License. See the LICENSE file for details.

## 💬 Contact
If you have any questions or suggestions, feel free to reach out at your-email@example.com or open an issue on GitHub.

Happy graph exploring with GraphXplorer! 🌟
