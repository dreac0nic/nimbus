#include "NimbusApplication.h"
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "TestMode.h"

NimbusApplication NimbusApplication::app = NimbusApplication();

NimbusApplication::NimbusApplication(void):
	mRoot(0)
{
}

NimbusApplication::~NimbusApplication(void)
{
	delete runMode;
	delete mRoot;
}

void NimbusApplication::begin(void)
{
	try
	{
		if(!app.loadConfiguration())
		{
			std::cerr << "Failed to load configuration files" << std::endl;
			return;
		}

		// Create the initial run mode
		app.runMode = new TestMode(app.mWindow);

		// Start the rendering process
		app.mRoot->addFrameListener(&app);
		app.mRoot->startRendering();
	}
	catch(Exception e)
	{
		std::cerr << "Ogre encountered an error" << std::endl;
		std::cerr << e.getFullDescription() << std::endl;
	}
}

bool NimbusApplication::frameRenderingQueued(const FrameEvent& evt)
{
	if(mWindow->isClosed())
	{
		return false;
	}

	// Run the current RunMode
	runMode = runMode->run(evt);

	// Check that the we should continue running
	if(runMode == 0)
	{
		LogManager::getSingletonPtr()->logMessage("(Nimbus) Received 0 RunMode... Terminating");
		return false;
	}

	return true;
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
	// Initialize the application

	// Initialize the root
	mWindow = mRoot->initialise(true, "Nimbus[wt]");

	// TODO: We need to be passing the RenderWindow around so we only ever
	// have one of them. One solution would be to make NimbusApplication a
	// Singleton... but that's just basically making it global. We could also be
	// sure to explicitly call initialize() with RenderWindow as a parameter... or
	// it could be a parameter of run(). Neither of those sound very good either.
	// I'm currently out of ideas on this one.

	return true;
}
