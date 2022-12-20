#include "cli.h"
#include <iostream>
#include <string>

#include "..\testing/display.h"
#include "..\io\fileio.h"
#include "..\manager\mStack.h"
#include "..\passwordGen/passwordGen.h"
#include "..\encryption\hash\Sha.h"

int takeInputs()
{
	/*
	* COMMANDS:
	* cls - clear console
	* cd (name) - move to that manager
	* cd .. - pop from stack
	* dis - display everything from current manager
	* key - display key of current manager
	* add (name) - adds new child of name to current selected manager
	* del (optional: name) - removes child
	* save - saves to file
	* load (file name) - loads from file
	* gen (size) - generates password with cached settings
	* enable (lower/upper/etc.) - enables password gen stuff
	* disable (lower/upper/etc.) - disables password gen stuff
	* setcustom (str) - sets the custom characters
	* exit - exits
	*/
	Mstack m;
	Manager* base = nullptr;
	passSettings settings = { true,true,true,true,false,2 };

	std::string raw = "";
	std::string com;
	std::string pre = ">";

	std::string pathInstance = "";

	passwordGenerator passInstance;
	std::string passPath = "PasswordGeneratorSettings.pswd";

	std::cout << pre;
	bool changes = false;
	unsigned int cycles = 10000;
	for(;;)
	{
		std::getline(std::cin, raw);
		// split first word out of raw by " ", then add to com
		if (raw.find(" ") == std::string::npos)
		{
			com = raw;
			raw = "";
		}
		else
		{
			int pos = raw.find(" ");
			com = raw.substr(0, pos);
			raw = raw.substr(pos + 1, raw.size() - pos);
		}

		if (com == "clear")
		{
			std::cout << "not happening rn" << std::endl;
		}
		else if (com == "cd")
		{
			// only gonna be able to do 1 step at a time
			if (m.empty())
			{
				std::cout << "no file loaded" << std::endl;
			}
			else
			{
				if (raw == "..")
				{
					if (m.size() != 1)
						m.pop();
				}
				else if (raw.empty())
				{
					std::cout << "specify key, (or .. to go up)" << std::endl;
				}
				else
				{
					// see if top has child with raw as name
					bool found = false;

					for (int i = 0; i < m.top()->size(); i++)
					{
						if ((*m.top())[i].getKey() == raw)
						{
							found = true;
							m.push(&(*m.top())[i]);
							break;
						}
					}

					if (!found)
					{
						std::cout << "unknown key" << std::endl;
					}
				}
			}
		}
		else if (com == "cdi")
		{
			if (raw.empty())
			{
				std::cout << "specify index, or -1 to go up" << std::endl;
			}
			else if (raw == "-1")
			{
				if (m.size() != 1)
					m.pop();
			}
			else
			{
				try
				{
					int i = stoi(raw);
					if (i < 0)
					{
						std::cout << "index must be at least -1" << std::endl;
					}
					else if (i > m.top()->size())
					{
						std::cout << "index too large, must be less than " << m.top()->size() << std::endl;
					}
					else
					{
						m.push(&(*m.top())[i]);
					}
				}
				catch (...)
				{
					std::cout << "enter numeric values for cdi, use cd for strings" << std::endl;
				}
			}
		}
		else if (com == "dis")
		{
			// display top of stack
			if (m.empty() || m.top() == nullptr)
			{
				std::cout << "cannot display; nothing loaded" << std::endl;
			}
			else
			{
				displayManager(*m.top(), 0, "");
			}
		}
		else if (com == "key")
		{
			// display key of top of stack, this one is kinda useless
			if (m.empty() || m.top() == nullptr)
			{
				std::cout << "cannot display; nothing loaded" << std::endl;
			}
			else
			{
				std::cout << m.top()->getKey() << std::endl;
			}
		}
		else if (com == "add")
		{
			// add raw as a new manager
			if (raw.empty())
			{
				std::cout << "specify key" << std::endl;
			}
			else
			{
				changes = true;
				if (m.empty())
				{
					m.push(new Manager(raw));
					base = m.top();
					std::cout << "successfully added " << raw << std::endl;
				}
				else
				{
					m.top()->addItem(Manager(raw));
					std::cout << "successfully added " << raw << std::endl;
				}
			}
		}
		else if (com == "del")
		{
			if (raw.empty())
			{
				std::cout << "specify key" << std::endl;
			}
			else
			{
				switch (m.top()->removeItem(raw))
				{
					case -1:
						std::cout << "failed to delete: key not found" << std::endl;
						break;
					case 0:
						std::cout << "failed to delete: \"" << raw << "\" has children" << std::endl;
						break;
					case 1:
						changes = true;
						std::cout << "successfully deleted \"" << raw << "\"" << std::endl;
						break;
				}

			}
		}
		else if (com == "deli")
		{
			if (raw.empty())
			{
				std::cout << "specify index" << std::endl;
			}
			else
			{
				try
				{
					int i = stoi(raw);
					if (i < 0)
					{
						std::cout << "index must be at least 0" << std::endl;
					}
					else if (i >= m.top()->size())
					{
						std::cout << "index too large, must be less than " << m.top()->size() << std::endl;
					}
					else
					{
						switch (m.top()->removeItem(i))
						{
						case -1:
							std::cout << "failed to delete: index does not exist" << std::endl;
							break;
						case 0:
							std::cout << "failed to delete: has children" << std::endl;
							break;
						case 1:
							changes = true;
							std::cout << "successfully deleted index [" << raw << "]" << std::endl;
							break;
						}
					}
				}
				catch (...)
				{
					std::cout << "enter numeric values for deli, use del for strings" << std::endl;
				}
			}
		}
		else if (com == "del")
		{
			if (raw.empty())
			{
				Manager temp = *m.top();
				m.pop();
				m.top()->removeItem(temp);
				std::cout << "successfully deleted " << temp.getKey() << std::endl;
			}
			else
			{
				// delete child with raws name
				if (m.top()->removeItem(raw))
				{
					std::cout << "successfully deleted " << raw << std::endl;
				}
				else
				{
					std::cout << "unable to find " << raw << std::endl;
				}
			}
		}
		else if (com == "save")
		{
			if (m.empty() || m.top() == nullptr)
			{
				std::cout << "no file open" << std::endl;
			}
			else
			{
				// pop everything from stack then save to file

				// if no saved path and no specified path, error
				// raw takes priority over path instance
				//mInstance = m.top();

				if (raw.empty())
				{
					if (pathInstance.empty())
						std::cout << "ERROR: enter file name" << std::endl;
					else
					{
						// get password
						std::cout << "enter password: ";
						std::string pass;
						std::getline(std::cin, pass);

						// get data for iv and salt
						std::cout << "enter random data: ";
						std::string extra;
						std::getline(std::cin, extra);

						uint8_t* saltSeed = sha256("abc");
						uint8_t* IVSeed = sha256("cba");
						
						if (managerToFile(
							*base, // m
							pathInstance, // path
							pass, // pass
							cycles, // cycles
							genSalt(sha256(extra.substr(0, extra.size() / 2)),32),//salt
							genIV(sha256(extra.substr(extra.size() / 2, extra.size() / 2)), 16) // iv
						)) // */
						{
							std::cout << "saved as " << pathInstance << std::endl;
							changes = false;
						}
						else
						{
							std::cout << "failed to save file" << std::endl;
						}
						
					}
				}
				else
				{

					// get password
					std::cout << "enter password: ";
					std::string pass;
					std::getline(std::cin, pass);

					// get data for iv and salt
					std::cout << "enter random data: ";
					std::string extra;
					std::getline(std::cin, extra);
					
					pathInstance = raw;
					

					if (managerToFile(
						*base, // m
						pathInstance, // path
						pass, // pass
						cycles, // cycles
						genSalt(sha256(extra.substr(0, extra.size() / 2)), 32),//salt
						genIV(sha256(extra.substr(extra.size() / 2, extra.size() / 2)), 16) // iv
					))
					{
						std::cout << "saved as " << pathInstance << std::endl;
						changes = false;
					}
					else
					{
						std::cout << "failed to save file" << std::endl;
					}
				}
			}
			
			
			//*
			if (passToFile(settings, passPath))
			{
				std::cout << "saved password generator settings" << std::endl;
			}
			else
			{
				std::cout << "failed to save password generator settings" << std::endl;
			}
			//*/
		}
		else if (com == "load")
		{
			// load manager from file
			if (raw.empty())
			{
				std::cout << "ERROR: specify file" << std::endl;
			}
			else
			{
				// ask for password
				std::cout << "enter password: ";
				std::string pass;
				std::getline(std::cin, pass);
				managerAndGenerator temp = fileToManager(raw, pass);
				cycles = temp.cycles;
				// Manager* temp = 
				if (temp.manager == nullptr)
				{
					std::cout << "unable to open file (invalid path or password)" << std::endl;
				}
				else
				{
					std::cout << "successfully opened file" << std::endl;
					pathInstance = raw;

					while (!m.empty())
					{
						m.pop();
					}
					m.push(temp.manager);
					base = m.top();
				}
			}
			// load password generator settings
			settings = fileToPass(passPath);
			std::cout << "loaded password generator settings" << std::endl;
			
		}
		else if (com == "gen")
		{
			if (raw.empty())
			{
				std::cout << "missing length" << std::endl;
			}
			else
			{
				try
				{
					int size = stoi(raw);
					std::cout << passInstance.generatePassword(size, 
						settings.maxRepeat,
						settings.lower,
						settings.upper,
						settings.number,
						settings.special,
						settings.custom) << std::endl;
				}
				catch (...)
				{
					std::cout << "invalid length" << std::endl;
				}
			}
		}
		else if (com == "cycles")
		{
			if (raw.empty())
			{
				std::cout << "cycles: " << cycles << std::endl;
				std::cout << "use \"cycles {int}\" to set the cycles" << std::endl;
			}
			else
			{
				changes = true;
				try
				{
					cycles = stoi(raw);
					std::cout << "encryption cycles set to " << raw << std::endl;
				}
				catch (...)
				{
					std::cout << "invalid number" << std::endl;
				}
		}
		}
		else if (com == "maxrepeat")
		{
			if (raw.empty())
			{
				std::cout << "missing length" << std::endl;
			}
			else
			{
				changes = true;
				try
				{
					settings.maxRepeat = stoi(raw);
					std::cout << "max repeats for password gen set to " << raw << std::endl;
				}
				catch (...)
				{
					std::cout << "invalid number" << std::endl;
				}
			}
		}
		else if (com == "enable")
		{
			if (raw.empty())
			{
				std::cout << "specify what to enable" << std::endl;
				std::cout << "options: lower, upper, number, special, custom" << std::endl;
			}
			else
			{
				changes = true;
				if (raw.find("lower") != std::string::npos)
				{
					settings.lower = true;
					std::cout << "enabled lower" << std::endl;
				}
				if (raw.find("upper") != std::string::npos)
				{
					settings.upper = true;
					std::cout << "enabled upper" << std::endl;
				}
				if (raw.find("number") != std::string::npos)
				{
					settings.number = true;
					std::cout << "enabled number" << std::endl;
				}
				if (raw.find("special") != std::string::npos)
				{
					settings.special = true;
					std::cout << "enabled special" << std::endl;
				}
				if (raw.find("custom") != std::string::npos)
				{
					settings.custom = true;
					std::cout << "enabled custom" << std::endl;
				}
			}
			
		}
		else if (com == "disable")
		{
			if (raw.empty())
			{
				std::cout << "specify what to disable" << std::endl;
				std::cout << "options: lower, upper, number, special, custom" << std::endl;
			}
			else
			{
				changes = true;
				if (raw.find("lower") != std::string::npos)
				{
					settings.lower = false;
					std::cout << "disabled lower" << std::endl;
				}
				if (raw.find("upper") != std::string::npos)
				{
					settings.upper = false;
					std::cout << "disabled upper" << std::endl;
				}
				if (raw.find("number") != std::string::npos)
				{
					settings.number = false;
					std::cout << "disabled number" << std::endl;
				}
				if (raw.find("special") != std::string::npos)
				{
					settings.special = false;
					std::cout << "disabled special" << std::endl;
				}
				if (raw.find("custom") != std::string::npos)
				{
					settings.custom = false;
					std::cout << "disabled custom" << std::endl;
				}
			}
			
		}
		else if (com == "setcustom")
		{
			passInstance.custom = raw;
			std::cout << "custom characters set to \"" << raw << "\"" << std::endl;
			changes = true;
		}
		else if (com == "help")
		{
			std::cout << "clear\ndis\nkey\nadd\nsave\nload\ngen\nmaxrepeat\nenable\ndisable\nsetcustom\nhelp\ncensor\npeek\ndel\ndeli\ncd\ncdi\nexit\n" << std::endl;
		}
		else if (com == "censor")
		{
			if (raw.empty())
			{
				std::cout << "missing entry to censor" << std::endl;
			}
			else
			{
				
				bool found = false;
				int i = 0;
				for (; i < m.top()->size(); i++)
				{
					if ((*m.top())[i].getKey() == raw)
					{
						found = true;
						(*m.top())[i].censor = !(*m.top())[i].censor;
						break;
					}
				}

				if (!found)
				{
					std::cout << "unknown key" << std::endl;
				}
				else
				{
					changes = true;
					std::cout << "changed censor status for key to " << ((*m.top())[i].censor ? "true" : "false");
				}
			}
		}
		else if (com == "peek")
		{
			if (raw.empty())
			{
				std::cout << "missing index" << std::endl;
			}
			else
			{
				try
				{
					int i = stoi(raw);
					if (i < 0)
						std::cout << "negative indexes are not allowed" << std::endl;
					else if (i >= m.top()->size())
					{
						std::cout << "invalid index, num too high" << std::endl;
					}
					else
					{
						std::cout << (*m.top())[i].getKey() << std::endl;
					}
				}
				catch(...)
				{
					std::cout << "must enter numeric value" << std::endl;
				}
			}
		}
		else if (com == "exit")
		{
			if (changes)
			{
				std::cout << "there are unsaved changes, enter \"y\" to exit" << std::endl;
				std::cout << "exit (y/n): ";
				std::getline(std::cin, raw);
				if (raw == "y" || raw == "Y")
				{
					std::cout << "exiting..." << std::endl;
					return 0;
				}
				std::cout << "cancelling exit attempt" << std::endl;
			}
			else
			{
				std::cout << "exiting..." << std::endl;
				return 0;
			}
		}
		else
		{
			if (com.empty())
			{
				std::cout << m.toStr() << pre;
				continue;
			}
			std::cout << "unknown command, type \"help\" for a list of commands";

		}
		std::cout << std::endl << m.toStr() << pre;
	}
	return 0;
}