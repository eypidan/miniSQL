#include "..\include\REPL.h"
#include "..\include\MetaData.h"
#include <fstream>

void Interpreter::REPL::run()
{
	isRunning = true;
	
	while (isRunning) {
		std::cout << "MiniSQL> ";
		buffer.clear();
		isReading = true;

		/* load query to stringstream from cin */
		while (isReading) {
			if (!std::getline(std::cin, line)) {
				isRunning = false;	
			}
			else if (line.find_first_not_of(' ') == std::string::npos) { // end of query
				isReading = false;
			}
			else {
				buffer << line << std::endl;
				if (line.find(';') != std::string::npos)	// meet ';'
					isReading = false;
				else std::cout << "      -> ";
			}
		}
		
		/* parse query */
		if (!isRunning) break;
		try {
			Parser parser(buffer);
			Statements statements = parser.parse();
			for (auto statement : statements) {
				auto quitStat = dynamic_cast<AST::QuitStatement *>(statement.get());
				auto execStat = dynamic_cast<AST::ExecfileStatement *>(statement.get());

				if (quitStat) {
					isRunning = false;
				}
				else if (execStat) {	// parse execfile
					std::string filePath = execStat->getFilePath();
					std::ifstream fin(filePath);
					if (!fin) {
						throw SQLException("Invalid file name. / File not found.");
					}
					Parser fileParser(fin);
					Statements fileStatements = fileParser.parse();
					for (auto fileStatement : fileStatements) {
						auto quitfileStat = dynamic_cast<AST::QuitStatement *>(fileStatement.get());
						auto execfileStat = dynamic_cast<AST::ExecfileStatement *>(fileStatement.get());

						if (quitfileStat) {
							throw SQLException("'quit' is not allowed in file input.");
						}
						else if (execfileStat) {
							throw SQLException("'execfile' is not allowed in file input.");
						}
						else { 
							fileStatement->callAPI(); 
						}
					}
				}
				else {
					statement->callAPI();
				}
			}
			if (!isRunning) break;

		}
		catch (SQLException & err) {
			std::cout << err.msg << std::endl;
		}
		catch (ParseError & err) {
			std::cout << err.what() << std::endl;
		}

		
		
	}
}
