#include "pc.h"
#include "Converter.h"
#include <random>
default_random_engine dre;

Converter converter;
struct TreeNode
{

	int index;
	string name;
	TreeNode* parent;
	vector<TreeNode*> children;
};

class Tree
{
public:

	void MakeTree()
	{
		unordered_map<int32, TreeNode*> map;
		for (auto& i : converter._bones)
		{
			if (i->index == 0)
			{
				root = new TreeNode();
				root->index = i->index;
				root->name = i->name;

				map[root->index] = root;
			}

			else
			{

				TreeNode* newNode = new TreeNode();

				newNode->index = i->index;
				newNode->name = i->name;

				newNode->parent = map[i->parent];
				newNode->parent->children.push_back(newNode);
		
				map[newNode->index] = newNode;
			}
		}

	}

	void PrintTree(TreeNode* root , int32 depth =0)
	{

		for (int i = 0; i < depth; ++i) {
			cout << "-";
		}

		cout << root->name << endl;

		for (int i = 0; i < root->children.size(); ++i)
		{	
			PrintTree(root->children[i], depth + 1);
		}

	}


	TreeNode* root;
};





uniform_int_distribution<int> uid(0, 3);
int main()
{

	converter.ReadAssetFile(L"Tank/Tank.fbx");
	//converter.ExportMaterialData(L"instanceing/instanceing");
	converter.ExportModelData(L"Tank2/Tank2");
	

	cout << "-----------------------------------" << endl;

	Tree tree;

	tree.MakeTree();

	tree.PrintTree(tree.root, 0);

	cout << endl;


	cout << "---------------------------------" << endl;

	for (auto& i : converter._meshes)
	{
		cout << i->name << " " << i->boneIndex << endl;
	}
};
