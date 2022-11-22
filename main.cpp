#include <iostream>
#include <fstream>
#include <vector>
#include <set>

/*====================================================================================================================*/

std::vector <std::vector <int> > inputAdjacencyList();

std::vector <int> encodePruferCode(const std::vector <std::vector <int> >& adjacencyList);

std::vector <std::pair <int, int> > decodePruferCode(const std::vector <int>& code);

/*====================================================================================================================*/

int main()
{
    std::vector <std::vector <int> > adjacencyList = inputAdjacencyList();

    std::vector <int> pruferCode = encodePruferCode(adjacencyList);

    for (const auto& elem : pruferCode)
        std::cout << elem << " ";
    std::cout << "\n\n";

    std::vector <std::pair <int, int> > tree = decodePruferCode(pruferCode);

    for (const auto& edge : tree)
        std::cout << edge.first << " " << edge.second << std::endl;
}

/*====================================================================================================================*/

std::vector <std::vector <int> > inputAdjacencyList()
{
    std::ifstream reader("../input.txt");

    if (reader.is_open())
    {
        int numOfVertexes;

        reader >> numOfVertexes;

        std::vector <std::vector <int> > adjacencyList(numOfVertexes, std::vector <int>());

        for (int i = 0; i < numOfVertexes; ++i)
            for (int j = 0; j < numOfVertexes; ++j)
                if (!reader.eof())
                {
                    int temp;

                    reader >> temp;

                    if (temp)
                        adjacencyList[i].emplace_back(j);
                }
                else
                    break;

        reader.close();

        return adjacencyList;
    }
    else
    {
        reader.close();

        std::cout << "File opening error!\n";

        std::exit(2);
    }
}

std::vector <int> encodePruferCode(const std::vector <std::vector <int> >& adjacencyList)
{
    std::set <int> leafs;

    std::vector <int> degrees(adjacencyList.size()), code;

    std::vector <bool> visited(adjacencyList.size(), false);

    for (std::size_t i = 0; i < adjacencyList.size(); ++i)
    {
        degrees[i] = (int) adjacencyList[i].size();

        if (degrees[i] == 1)
            leafs.insert((int) i);
    }

    for (std::size_t i = 0; i < adjacencyList.size() - 2; ++i)
    {
        auto leaf = leafs.begin();

        leafs.erase(leaf);

        visited[*leaf] = true;

        int vertex;

        for (const auto& currentVertex : adjacencyList[*leaf])
            if (!visited[currentVertex])
                vertex = currentVertex;

        code.emplace_back(vertex);

        if (--degrees[vertex] == 1)
            leafs.insert(vertex);
    }

    return code;
}

std::vector <std::pair <int, int> > decodePruferCode(const std::vector <int>& code)
{
    int treeSize = (int) code.size() + 2;

    std::vector <std::pair <int, int> > tree;

    std::vector <int> degrees(treeSize, 1);

    for (const auto& elem : code)
        ++degrees[elem];

    int head = 0;

    while (degrees[head] != 1)
        ++head;

    int leaf = head;

    for (const auto& vertex : code)
    {
        tree.emplace_back(leaf, vertex);

        if (--degrees[vertex] == 1 && vertex < head)
            leaf = vertex;
        else
        {
            ++head;

            while (degrees[head] != 1)
                ++head;

            leaf = head;
        }
    }

    tree.emplace_back(leaf, treeSize - 1);

    return tree;
}