#include "pc.h"
#include "Converter.h"
#include <random>
default_random_engine dre;

uniform_int_distribution<int> uid(0, 3);
int main()
{
	Converter converter;
	converter.ReadAssetFile(L"Tank/Tank.fbx");
	converter.ExportMaterialData(L"333/33");
	converter.ExportModelData(L"333/333");
	

	cout << "-----------------------------------" << endl;

	for (auto& i : converter._bones)
	{
		cout << i->name << " " << i->parent << " " << i->index << endl;
	}

	


	cout << "---------------------------------" << endl;

	for (auto& i : converter._meshes)
	{
		cout << i->name << " " << i->boneIndex << endl;
	}
};
