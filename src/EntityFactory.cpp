#include "EntityFactory.h"

using namespace Nimbus;
using namespace std;

EntityFactory::EntityFactory(std::string entityDefinitionFile)
{
	// LOAD ENTITY DEFINITIONS
	string buffer;
	ifstream defFile(entityDefinitionFile);

	string currentCow;
	string currentEntityName;
	GameEntity* currentEntity = NULL;
	map<string, string> informationBuffer;

	if(defFile.is_open())
	{
		getline(defFile, buffer);

		// READ IN ENTITY DEFINITIONS
		do
		{
			int first = 0;
			int second = 0;
			// Check for beginning of `cow` definition.
			if((first = buffer.find('[')) != -1 && (second = buffer.find(']')) != -1)
			{
				if(!informationBuffer.empty())
				{
					if(currentCow == "Entity")
					{
						// Store previously created entity to entityInstances,
						// and prepare for the next entity.
						if(currentEntity != NULL)
						{
							this->entityInstances[currentEntityName] = currentEntity;

							currentEntity = NULL;
						}
						
						// Store attributes of the Entity from information in the map
						currentEntity = new GameEntity(informationBuffer);

						// Erase the information buffer from the previous entity
						informationBuffer.erase(informationBuffer.begin(), informationBuffer.end());
					}
					else if(currentCow == "Behaviour")
					{
					}
					else
					{
						cerr << "What...?" << endl;
					}
				}

				currentCow = buffer.substr(first, second - first);
			}
			// Check for beginning of `cow` property definition
			else  if((first = buffer.find('=')) != -1)
			{

			}

			// Dump information in information map into appropriate object.
		} while(getline(defFile, buffer));
	}
	else
	{
		cerr << "EntityFactory: NO DEFINITION FILE FOUND\"" << entityDefinitionFile << "\"." << endl;
	}
}

EntityFactory::~EntityFactory(void)
{
	// EMPTY DESTRUCTOR
}

GameEntity* EntityFactory::createEntity(std::string entityType)
{
	GameEntity* factorizedEntity = new GameEntity();

	return factorizedEntity;
}