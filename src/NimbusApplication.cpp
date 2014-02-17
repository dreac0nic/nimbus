#include "NimbusApplication.h"
#include <OgreConfigFile.h>

NimbusApplication NimbusApplication::app = NimbusApplication();

NimbusApplication::NimbusApplication(void):
	mRoot(0)
{
}

NimbusApplication::~NimbusApplication(void)
{
	delete mRoot;
}

void NimbusApplication::begin(void)
{
	try
	{
		app.loadConfiguration();
	}
	catch(Exception e)
	{
		std::cerr << "Ogre encountered an error" << std::endl;
		std::cerr << e.getFullDescription() << std::endl;
	}
}

bool NimbusApplication::loadConfiguration(void)
{
	// Config file names (consider unifying config files http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Practical+Application+-+Initialization)
	String pluginsCfg = "../../assets/plugins_d.cfg";
	String resourcesCfg = "../../assets/resources_d.cfg";

	// For loading resource config options
	ConfigFile confFile;
	String sectionName, typeName, valueName;

	//////////
	// Create the ogre root object
	mRoot = new Root(pluginsCfg);

	//////////
	// Initialize resource groups

	// Load the resource config file
	confFile.load(resourcesCfg);

	// Iterate through each section
	ConfigFile::SectionIterator sectionIter = confFile.getSectionIterator();
	while(sectionIter.hasMoreElements())
	{
		sectionName = sectionIter.peekNextKey();

		// Iterate through each setting
		ConfigFile::SettingsMultiMap* settings = sectionIter.getNext();
		ConfigFile::SettingsMultiMap::iterator settingIterator;
		for(settingIterator = settings->begin(); settingIterator != settings->end(); settingIterator++)
		{
			typeName = settingIterator->first;
			valueName = settingIterator->second;

			// Add the location to the current section
			ResourceGroupManager::getSingleton().addResourceLocation(
				valueName, typeName, sectionName);
		}
	}

	// Right now this doesn't cost much, but will need to be moved as the application grows
	//TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// In fact, this should likely be done on a per run mode basis in the RunMode initialization
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	//////////
	// Create the rendering system

	// TODO: Replace this with custom code for determining which RenderSystem should be used
	// The config dialog simply isn't a releasable option
	if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
	{
		return false;
	}

	//////////
	// Create a RenderWindow

	// Consider manually creating a window so we have more control...
	// (not necessary with above config options)
	mWindow = mRoot->initialise(true, "Nimbus[wt]");

	return true;
}

bool NimbusApplication::createScene(void)
{
	return true;
}
