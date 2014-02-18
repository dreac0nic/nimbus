#include "NimbusApplication.h"
#include <iostream>

/** The main function to start the Nimbus application.
 */
int main(int argc, char* argv[])
{
	// This used to be a placeholder for a render window so I knew the application ran...
	// Now I just like the dragon... :)
	std::cout << "Nimbus Application... in development" << std::endl << std::endl;
	std::cout << "Meanwhile, here is a dragon:" << std::endl;
	std::cout << "\n                                                 /===-_---~~~~~~~~~------____\n                                                |===-~___                _,-\'\n                 -==\\\\                         `//~\\\\   ~~~~`---.___.-~~\n             ______-==|                         | |  \\\\           _-~`\n       __--~~~  ,-/-==\\\\                        | |   `\\        ,\'\n    _-~       /\'    |  \\\\                      / /      \\      /\n  .\'        /       |   \\\\                   /\' /        \\   /\'\n /  ____  /         |    \\`\\.__/-~~ ~ \\ _ _/\'  /          \\/\'\n/-\'~    ~~~~~---__  |     ~-/~         ( )   /\'        _--~`\n                  \\_|      /        _)   ;  ),   __--~~\n                    \'~~--_/      _-~/-  / \\   \'-~ \\\n                   {\\__--_/}    / \\\\_>- )<__\\      \\\n                   /\'   (_/  _-~  | |__>--<__|      | \n                  |0  0 _/) )-~     | |__>--<__|      |\n                  / /~ ,_/       / /__>---<__/      |  \n                 o o _//        /-~_>---<__-~      /\n                 (^(~          /~_>---<__-      _-~              \n                ,/|           /__>--<__/     _-~                 \n             ,//(\'(          |__>--<__|     /                  .----_ \n            ( ( \'))          |__>--<__|    |                 /\' _---_~\\\n         `-)) )) (           |__>--<__|    |               /\'  /     ~\\`\\\n        ,/,\'//( (             \\__>--<__\\    \\            /\'  //        ||\n      ,( ( ((, ))              ~-__>--<_~-_  ~--____---~\' _/\'/        /\'\n    `~/  )` ) ,/|                 ~-_~>--<_/-__       __-~ _/ \n  ._-~//( )/ )) `                    ~~-\'_/_/ /~~~~~~~__--~ \n   ;\'( \')/ ,)(                              ~~~~~~~~~~ \n  \' \') \'( (/\n    \'   \'  `   ";
	std::cout << std::endl;

	// Begin the application
	NimbusApplication::begin();

	// Pause the application
	//std::cin.get();

	return 0;
}