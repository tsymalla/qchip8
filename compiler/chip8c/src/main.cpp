#include <fstream>
#include <lexer.hpp>
#include <parser.hpp>
#include <string>

using namespace compiler;

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: chip8c filename";
		return 1;
	}

	std::ifstream inputFile{argv[1], std::ios_base::in};
	if (!inputFile.is_open())
	{
		std::cerr << "Could not open input file \"" << argv[1] << ".\n";
		return 1;
	}

	std::string inputBuffer{std::istreambuf_iterator<char>(inputFile), std::istreambuf_iterator<char>()};

	Lexer lexer(std::move(inputBuffer));
	const auto& tokens = lexer.Lex();
	Parser parser(tokens);
	parser.Parse();

	return 0;
}
