#include <SDKDDKVer.h>

#include <stdio.h>
#include <tchar.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <algorithm>

#include <direct.h>	
#include <crtdbg.h>

#include <map>
#include <string>
#include <vector>

#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

vector<int> makeList(map<int, string> vMap, string strSlot)
{
	vector<int> temp;
	for (auto i = vMap.begin(); i != vMap.end(); i++)
	{
		if ((*i).second.compare(strSlot) == 0)
		{
			temp.push_back((*i).first);
		}
	}
	return temp;
}


int main()
{
	XMLDocument doc;
	if (doc.LoadFile("zitem.xml") != XML_SUCCESS)
	{
		cout << "Is not able to find zitem.xml\n";
	}
	else
	{
		XMLElement* xml = doc.FirstChildElement("XML");

		map<int, string> items;

		for (XMLElement* item = xml->FirstChildElement("ITEM"); item != 0; item = item->NextSiblingElement())
		{
			int nId = atoi(item->Attribute("id"));
			string strSlot = item->Attribute("slot");

			items.insert(pair<int, string>(nId, strSlot));
		}

		cout << "Found " << items.size() << " Items found \n\n";

		vector<int> vMeleeIds = makeList(items, "melee");
		vector<int> vRangeIds = makeList(items, "range");
		vector<int> vCustomIds = makeList(items, "custom");

		cout << "Melee Items: " << vMeleeIds.size() << "\n";
		cout << "Range Items: " << vRangeIds.size() << "\n";
		cout << "Custom Items: " << vCustomIds.size() << "\n";

		int nLists = 0;

		cout << "\nEnter the amout of list you want:\n";
		cin >> nLists;


		XMLDocument xmlDoc;

		XMLNode * pXml = xmlDoc.NewElement("XML");

		xmlDoc.InsertFirstChild(pXml);

		for (int i = 1; i != nLists; i++)
		{
			XMLElement * pGunGame = xmlDoc.NewElement("GUNGAME");
			pGunGame->SetAttribute("id", i);

			for (int j = 1; j != 100; j++)
			{
				XMLElement * pItemSet = xmlDoc.NewElement("ITEMSET");
				if (pItemSet)
				{
					pItemSet->SetAttribute("level", j);

					XMLElement * pItem = xmlDoc.NewElement("ITEM");
					if (pItem)
					{
						random_shuffle(vMeleeIds.begin(), vMeleeIds.end());
						random_shuffle(vRangeIds.begin(), vRangeIds.end());
						random_shuffle(vCustomIds.begin(), vCustomIds.end());

						pItem->SetAttribute("melee", vMeleeIds[0]);
						pItem->SetAttribute("primary", vRangeIds[0]);
						pItem->SetAttribute("secondary", vRangeIds[1]);
						pItem->SetAttribute("custom1", vCustomIds[0]);
						pItem->SetAttribute("custom2", vCustomIds[1]);
						pItemSet->InsertEndChild(pItem);
					}
					pGunGame->InsertEndChild(pItemSet);
				}
			}
			pXml->InsertEndChild(pGunGame);
		}

		xmlDoc.SaveFile("gungamelist.xml");
	}
	system("pause");
}

