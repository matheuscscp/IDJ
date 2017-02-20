/// @defgroup	MOD_CONFIGFILE "Configuration File Parser"
/// @{
/// 	@brief Module to define and implement a parser for configuration files
/// @}
/// @ingroup MOD_CONFIGFILE
/// @file configfile.hpp
/// @brief All Configuration File Parser classes and methods
/// @author Matheus Pimenta

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <vector>
#include <fstream>

using std::ios;
using std::string;
using std::stringstream;
using std::stack;
using std::vector;
using std::fstream;

/// Strong class with a system to manage and access configuration variables
/// for the client application, with the capacity of loading files and parsing
/// its words to generate those variables.
/// @brief Strong class to manage configuration variables
class Configuration
{
private:
	// =====================================================================
	// Configuration Class Member (Variable)
	// =====================================================================
	
	/// This class holds data of something like "dynamic type variables" as
	/// the configuration variables.
	/// @tparam T Variable's type.
	/// @brief Class for configuration variables
	template <class T>
	class Variable
	{
	private:
		/// @brief Variable's name
		string name_;
		
		/// @brief Variable's value
		T value_;
	public:
		/// Constructor assigning both the name and the value.
		/// @param name Variable's name.
		/// @param value Variable's value.
		/// @brief Constructor
		Variable (const string& name, const T& value);
		
		/// @brief Empty destructor
		~Variable ();
		
		/// Returns the variable's name.
		/// @return Variable's name.
		/// @brief Access method to variable's name
		const string& name () const;
		
		/// Returns the variable's value.
		/// @return Variable's value.
		/// @brief Access method to variable's value
		const T& value () const;
		
		/// Sets the variable's value.
		/// @param value Variable's new value.
		/// @brief Modifier method to variable's value
		void set (const T& value);
		
	};
public:
	// =====================================================================
	// Configuration Class Members (Specialized variables)
	// =====================================================================
	
	/// Class for configuration variables of configuration type.
	/// @see Variable
	/// @brief Class for configuration type
	typedef Variable< Configuration > ConfigVariable;
	
	/// Class for configuration variables of "primitive" types.
	/// @see Variable
	/// @brief Class for "primitive" types
	typedef Variable< string > DataVariable;
	
	// =====================================================================
	// Configuration File Parser Exceptions
	// =====================================================================
	
	/// @brief Exception for loading files
	class FileNotFound {};
	
	/// @brief Exception base class for parsing files
	class FileParserInvalidLine
	{
	private:
		/// @brief Specific invalid line
		unsigned int line_;
	public:
		/// @param line Specific invalid line.
		/// @brief Line assignment constructor
		FileParserInvalidLine (unsigned int line);
		
		/// @return Specific invalid line.
		/// @brief Access method to the specific invalid line
		const unsigned int& line ();
	};
	
	/// This exception will be thrown when the value assignment for
	/// an already name-assigned variable don't occurs.
	/// @brief Exception for value assignment missings
	class FParserValAssignmentMissing : public FileParserInvalidLine
	{
	public:
		/// @param line Specific invalid line.
		/// @brief Calls base class' constructor
		FParserValAssignmentMissing (unsigned int line);
	};
	
	/// This exception will be thrown when the parser finds an
	/// invalid token in a variable name.
	/// @brief Exception for name invalid tokens
	class FParserVarNameInvalidToken : public FileParserInvalidLine
	{
	public:
		/// @param line Specific invalid line.
		/// @brief Calls base class' constructor
		FParserVarNameInvalidToken (unsigned int line);
	};
	
	/// This exception will be thrown when the parser reaches the end of
	/// file before it unstack a Configuration type variable.
	/// @brief Exception for uncomplete Configuration type variable
	class FParserClosingBraceMissing : public FileParserInvalidLine
	{
	public:
		/// @param line Specific invalid line.
		/// @brief Calls base class' constructor
		FParserClosingBraceMissing (unsigned int line);
	};
	
	// =====================================================================
	// Configuration String Parser Exceptions
	// =====================================================================
	
	/// @brief Exception for parsing strings to variable name
	class SParserInvalidName {};
	
	/// @brief Exception for parsing strings to a real number
	class SParserInvConversionReal {};
	
	/// @brief Exception for parsing strings to an integer number
	class SParserInvConversionInt {};
	
	/// @brief Exception for parsing strings to one character
	class SParserInvConversionChar {};
private:
	// =====================================================================
	// Configuration Class Member (Parser)
	// =====================================================================
	
	/// Class to parse all things that the configuration needs.
	/// @see Configuration
	/// @brief Parser for everything in the configuration class
	class Parser
	{
	private:
		// =============================================================
		// File Parser Class
		// =============================================================
		
		/// Class dedicated entirely for parsing text files into a
		/// configuration instance.
		/// @brief Text file parser to a configuration instance
		class FileParser
		{
		private:
			/// Enumerating all the parsing process states.
			/// @brief Enumerating states
			enum State
			{
				/// @brief Waiting new variable name
				WAITING_NAME = 0,
				
				/// @brief Reading variable name
				READING_NAME,
				
				/// @brief Waiting equals or opening brace
				W8ING_EQLS_OR_BRACE,
				
				/// @brief Waiting opening brace symbol
				W8ING_OPENING_BRACE,
				
				/// @brief Waiting variable value
				WAITING_VALUE,
				
				/// @brief Reading variable value
				READING_VALUE,
				
				/// @brief Waiting name or closing brace
				W8ING_NAME_OR_BRACE
			};
			
			// =====================================================
			// File Parser Attributes
			// =====================================================
			
			/// @brief File stream being parsed
			fstream file_;
			
			/// @brief Configuration instance pointers to get vars
			stack< Configuration* > dest_;
			
			/// @brief File's current line being parsed
			unsigned int line_;
			
			/// @brief Current state of the parser
			State state_;
			
			/// @brief Current char to be parsed
			char input_;
			
			/// @brief Buffers to variable names
			stack< string > names_;
			
			/// @brief Buffer to variable values
			string value_;
			
			/// @brief Buffer to spaces between variable values
			string spaces_;
		public:
			// =====================================================
			// File Parser Methods
			// =====================================================
			
			/// Constructor to assign the configuration instance
			/// pointer and initialize some things for the parsing
			/// process.
			/// @param filename String with the file's name.
			/// @param dest Pointer to configuration instance.
			/// @throw FileNotFound If it was not possible to open
			/// the file.
			/// @brief Assignment and initializing constructor
			FileParser (	const string& filename,
					Configuration* dest	);
			
			/// @brief Empty destructor
			~FileParser ();
		private:
			/// This method parses a whole file, inserting (or
			/// re-setting) all its variables in the given
			/// configuration instance, using all the "handling"
			/// methods.
			/// @brief Parses a whole file
			void parse ();
			
			/// Inserts a new Configuration type variable in the
			/// given configuration instance.
			/// @see insertConfig
			/// @see setConfig
			/// @brief Inserts a variable in the config. instance
			void insertConfigVar ();
			
			/// Inserts a new data variable in the given
			/// configuration instance.
			/// @see insertStr
			/// @see setStr
			/// @brief Inserts a variable in the config. instance
			void insertDataVar ();
			
			/// This method ignores the current line of characters
			/// from the file, stopping and calling handleLineBreak
			/// if it hits CR or LF.
			/// @see handleLineBreak
			/// @brief Ignores a whole line
			void avoidLine ();
			
			/// This method checks which is the correct "waiting
			/// name" state to move on.
			/// @brief Changes to the next correct state
			void changeToW8ingNameState ();
			
			/// @brief Ignores Windows/DOS LF, if it's there
			void handleLineBreak ();
			
			/// Handles the current input character from the file,
			/// in the "waiting for var. name" state, throwing
			/// exception for invalid tokens, or calling avoidLine
			/// or handleLineBreak, respectively for '#' or 'r'/'n'
			/// found characters.
			/// @throw FParserVarNameInvalidToken
			/// If the method find any invalid character for a
			/// variable name.
			/// @see avoidLine
			/// @see handleLineBreak
			/// @brief Handles the input in "waiting name" state
			void handleWaitingName ();
			
			/// Handles the current input character from the file,
			/// in the "read variable name" state, throwing
			/// exception for invalid tokens, or changing to the
			/// "waiting value" state if space or '=' are found,
			/// or changing to the "waiting name or brace" state if
			/// '{' is found.
			/// @throw FParserVarNameInvalidToken
			/// If the method find any invalid character for a
			/// variable name.
			/// @see handleLineBreak
			/// @brief Handles the input in "reading name" state
			void handleReadingName ();
			
			/// Handles the current input character from the file,
			/// in the "waiting for equals or brace" state, throwing
			/// exception for invalid tokens, or changing to the
			/// "waiting value" state if '=' is found, or changing
			/// to the "waiting name or brace" state if '{' is
			/// found.
			/// @throw FParserValAssignmentMissing
			/// If the method find any character different from
			/// both '=', '{', ' ' and '\t'.
			/// @see avoidLine
			/// @see handleLineBreak
			/// @brief Handles input to be equals or brace symbols
			void handleW8ingEqlsOrBrace ();
			
			/// Handles the current input character from the file,
			/// in the "waiting for opening brace" state, throwing
			/// exception for invalid tokens, or changing
			/// to the "waiting name or brace" state if '{' is
			/// found.
			/// @throw FParserValAssignmentMissing
			/// If the method find any character different from
			/// both '{', ' ' and '\t'.
			/// @see avoidLine
			/// @see handleLineBreak
			/// @brief Handles input to be opening brace symbol
			void handleW8ingOpeningBrace ();
			
			/// Handles the current input character from the file,
			/// in the "waiting for var. value" state, inserting
			/// an empty value in a new variable and then calling
			/// avoidLine or handleLineBreak, respectively for '#'
			/// or 'r'/'n' found characters, or changing to the next
			/// state, if a non-space (or tab) character was found.
			/// It also unstack a Configuration type variable if it
			/// finds a '}' character.
			/// @see insertDataVar
			/// @see insertConfigVar
			/// @see avoidLine
			/// @see handleLineBreak
			/// @see changeToW8ingNameState
			/// @brief Handles the input in "waiting value" state
			void handleWaitingValue ();
			
			/// Handles the current input character from the file,
			/// in the "reading variable value" state, inserting
			/// the new variable and then calling handleLineBreak or
			/// avoidLine, respectively for 'r'/'n' or '#' (if
			/// before the '#' there was a space) found characters,
			/// and changing back to the first state: waiting for a
			/// variable name.
			/// It also unstack a Configuration type variable if it
			/// finds spaces followed by '}' character.
			/// @see insertDataVar
			/// @see insertConfigVar
			/// @see handleLineBreak
			/// @see avoidLine
			/// @see changeToW8ingNameState
			/// @brief Handles the input in "reading value" state
			void handleReadingValue ();
			
			/// Unstack a Configuration type variable if closing
			/// brace is found, or calls the first handling method.
			/// @see insertConfigVar
			/// @see changeToW8ingNameState
			/// @see handleWaitingName
			/// @brief Handles input to be closing brace a name
			void handleW8ingNameOrBrace ();
			
			/// Handles the current state if the parser reached the
			/// end of the file, throwing exception if a name was
			/// parsed without equals sign in its end, or inserts
			/// the current name and value buffer content in a new
			/// variable.
			/// It also throws exception if a block of Configuration
			/// type variable isn't closed.
			/// @throw FParserValAssignmentMissing
			/// If a name was parsed without an equals sign in its
			/// end.
			/// @throw FParserClosingBraceMissing
			/// If "waiting"/"reading value" is found with a stack
			/// size greater than one, or "waiting name or brace"
			/// state is found.
			/// @see insertDataVar
			/// @brief Handles the current state in the end of file
			void handleEof ();
		};
	public:
		// =============================================================
		// Parser Methods
		// =============================================================
		
		/// This method instantiates a FileParser passing the following
		/// parameters.
		/// @param filename String with the file's name.
		/// @param dest Pointer to the configuration instance.
		/// @see FileParser
		/// @brief Parses a whole file to a config. instance
		void parseFile (const string& filename, Configuration* dest);
		
		/// This method parses a variable name, checking if all tokens
		/// are valid.
		/// @param src String with the variable name.
		/// @throw SParserInvalidName
		/// Thrown when an invalid token is found in the variable name.
		/// @brief Parses a variable name
		void parseName (const string& src);
		
		/// Parses a string to be a string type configuration variable,
		/// changing the ascii values of special characters to the
		/// composite structure: \< c >.
		/// @param src String with variable value.
		/// @return String with parsed and converted value.
		/// @brief Parser to string type variable values
		string parseInStr (const string& src);
		
		/// Inserts the given value in a string stream and returns the
		/// string value.
		/// @param src The real number variable value.
		/// @return String with the real number variable value.
		/// @brief Parser to real number variable values
		string parseInReal (const long double& src);
		
		/// Inserts the given value in a string stream and returns the
		/// string value.
		/// @param src The integer number variable value.
		/// @return String with the integer number variable value.
		/// @brief Parser to integer number variable values
		string parseInInt (const long int& src);
		
		/// This method convert the given character to a string.
		/// If it's a special character, this method converts with the
		/// composite form of the character: \< c >.
		/// @param src String with variable value.
		/// @return String with parsed and converted value.
		/// @brief Parser to char type variable values
		string parseInChar (const char& src);
		
		/// This method parses a string, creating and returning another
		/// string with composite (\< c >) special characters converted
		/// to their true (ascii) values.
		/// @param src String with the variable value.
		/// @return New string with true value special characters.
		/// @brief Parses a string only to convert special characters
		string parseOutStr (const string& src);
		
		/// This method parses a string, checking if it's possible to
		/// extract a real number from the given string.
		/// @param src String with the variable value.
		/// @return The real number.
		/// @throw SParserInvConversionReal
		/// Thrown if it was not possible to extract a real number
		/// from the given string.
		/// @brief Parses a string to be a real number
		long double parseOutReal (const string& src);
		
		/// This method parses a string, checking if it's possible to
		/// extract an integer number from the given string.
		/// @param src String with the variable value.
		/// @return The integer number.
		/// @throw SParserInvConversionInt
		/// Thrown if it was not possible to extract an integer number
		/// from the given string.
		/// @brief Parses a string to be an integer number
		long int parseOutInt (const string& src);
		
		/// This method parses a string, checking if the size is
		/// equal to one. If it's true, this method will return
		/// this single character. If it's false, the method will
		/// look for special characters if the size is equal to two.
		/// Otherwise, the method will throw an exception.
		/// @param src String with the variable value.
		/// @return The single character.
		/// @throw SParserInvConversionChar
		/// Thrown if the size is not equal to one.
		/// @brief Parses a string to be a single character
		char parseOutChar (const string& src);
	};
public:
	// =====================================================================
	// Configuration Exceptions
	// =====================================================================
	
	/// @brief Exception for finding variables
	class VarNotFound {};
	
	/// @brief Exception for inserting variables
	class VarAlreadyExisting {};
	
	/// @brief Exception for a configuration inserting itself
	class ConfInsertingItself {};
private:
	// =====================================================================
	// Configuration Attributes
	// =====================================================================
	
	/// Set of all configuration variables.
	/// @see ConfigVariable
	/// @brief Vector of configuration variables
	vector< ConfigVariable > cvars;
	
	/// Set of all configuration variables.
	/// @see ConfigVariable
	/// @brief Vector of configuration variables
	vector< DataVariable > vars;
	
	/// The parser for configuration formatted files.
	/// @see Parser
	/// @brief Parser instance
	Parser parser;
public:
	// =====================================================================
	// Configuration Methods
	// =====================================================================
	
	/// @brief Empty constructor
	Configuration ();
	
	/// @brief Empty destructor
	~Configuration ();
	
	/// This method returns a vector with all the Configuration type
	/// variables of the configuration instance.
	/// @return Vector with all Configuration type variables.
	/// @brief Access method to all Configuration type variables
	const vector< ConfigVariable >& configVars () const;
	
	/// This method returns a vector with all the data variables of the
	/// configuration instance.
	/// @return Vector with all data variables.
	/// @brief Access method to all data variables
	const vector< DataVariable >& dataVars () const;
	
	/// Method to load a configuration text file into the configuration
	/// instance, calling the parser's method.
	/// If there is some variable in the file with the same name of any
	/// in the configuration instance, they will replace all of the already
	/// existing variables.
	/// @param filename String with the name of the configuration file.
	/// @see parseFile
	/// @brief Loads a text configuration file
	void readTxt (const string& filename);
private:
	/// Method to load a configuration binary file into the configuration
	/// instance.
	/// If there is some variable in the file with the same name of any
	/// in the configuration instance, they will replace all of the already
	/// existing variables.
	/// This method is also recursive to load each of configuration sets of
	/// the file.
	/// @param f Stream of the configuration file.
	/// @param dest Current configuration instance that's being read.
	/// @throw FileNotFound If it was not possible to open the file.
	/// @brief Recursive method to load a binary configuration file
	void readBin_ (fstream& f, Configuration& dest);
public:
	/// Method to load a configuration binary file into the configuration
	/// instance.
	/// If there is some variable in the file with the same name of any
	/// in the configuration instance, they will replace all of the already
	/// existing variables.
	/// @param filename String with the name of the configuration file.
	/// @throw FileNotFound If it was not possible to open the file.
	/// @brief Loads a binary configuration file
	void readBin (const string& filename);
private:
	/// Writes in a text file, named with "filename", all the variables in
	/// the configuration instance, using recursion to write all
	/// configuration instances within the main ("this" pointer)
	/// configuration instance.
	/// @param f Stream of the configuration file.
	/// @param src Current configuration instance that's being written.
	/// @param tab Number of indentation tabs of the current configuration
	/// instance.
	/// @brief Recursive method to write all variables in a text file
	void writeTxt_ (	fstream& f,
				const Configuration& src,
				unsigned short tab	);
public:
	/// Writes in a text file, named with "filename", all the variables in
	/// the configuration instance.
	/// @param filename String with the name of the configuration file.
	/// @brief Writes all variables in a text file
	void writeTxt (const string& filename);
private:
	/// Writes in a binary file, named with "filename", all the variables in
	/// the configuration instance, using recursion to write all
	/// configuration instances within the main ("this" pointer)
	/// configuration instance.
	/// @param f Stream of the configuration file.
	/// @param src Current configuration instance that's being written.
	/// @brief Recursive method to write all variables in a binary file
	void writeBin_ (fstream& f, const Configuration& src);
public:
	/// Writes in a binary file, named with "filename", all the variables in
	/// the configuration instance.
	/// @param filename String with the name of the configuration file.
	/// @brief Writes all variables in a binary file
	void writeBin (const string& filename);
	
	/// Clear all variables in the configuration instance.
	/// @brief Clear all variables
	void clear ();
	
	/// Returns the amount of variables in the configuration instance.
	/// @return The amount of variables in the configuration instance.
	/// @brief Returns the number of all existing variables
	size_t size () const;
private:
	/// Makes a binary search for any variable, named with the "name"
	/// parameter, assigning the position to the storage pointed by "mid"
	/// parameter, returning code of success.
	/// @param mid Storage which will hold the returned position.
	/// @param vec Reference to the vector in which the var. will be sought.
	/// @param name String with the variable's name.
	/// @throw VarNotFound If the variable is not present in the
	/// configuration instance.
	/// @brief Searches a variable in the configuration instance
	template <class T>
	void find (long int& mid, const T& vec, const string& name);
public:
	/// Method to insert a new variable of configuration type in the
	/// configuration instance, checking the validity of the name with the
	/// parser's method.
	/// @param name String with the variable's name.
	/// @param val ConfigVariable's value.
	/// @throw VarAlreadyExisting
	/// If new variable's name is found in another variable.
	/// @throw ConfInsertingItself
	/// If the configuration instance is trying to insert itself.
	/// @see parseName
	/// @brief Inserts a new config. variable in the configuration instance
	void insertConfig (const string& name, const Configuration& val);
	
	/// Method to insert a new variable of string type in the
	/// configuration instance, checking the validity of the name with the
	/// parser's method, and converting special characters (like '\r', '\n',
	/// etc.) to two characters: '\\' with 'r', for instance (also with a
	/// parser's method).
	/// @param name String with the variable's name.
	/// @param val DataVariable's value.
	/// @throw VarAlreadyExisting
	/// If new variable's name is found in another variable.
	/// @see parseName
	/// @see parseInStr
	/// @brief Inserts a new string variable in the configuration instance
	void insertStr (const string& name, const string& val);
	
	/// Method to insert a new variable of real number type in the
	/// configuration instance, checking the validity of the name with the
	/// parser's method.
	/// @param name String with the variable's name.
	/// @param val DataVariable's value.
	/// @throw VarAlreadyExisting
	/// If new variable's name is found in another variable.
	/// @see parseName
	/// @see parseInReal
	/// @brief Inserts a new real num variable in the configuration instance
	void insertReal (const string& name, const long double& val);
	
	/// Method to insert a new variable of integer number type in the
	/// configuration instance, checking the validity of the name with the
	/// parser's method.
	/// @param name String with the variable's name.
	/// @param val DataVariable's value.
	/// @throw VarAlreadyExisting
	/// If new variable's name is found in another variable.
	/// @see parseName
	/// @see parseInInt
	/// @brief Inserts a new int num. variable in the configuration instance
	void insertInt (const string& name, const long int& val);
	
	/// Method to insert a new variable of char type in the
	/// configuration instance, checking the validity of the name with the
	/// parser's method, and converting special characters (like '\r', '\n',
	/// etc.) to two characters: '\\' with 'r', for instance (also with a
	/// parser's method).
	/// @param name String with the variable's name.
	/// @param val DataVariable's value.
	/// @throw VarAlreadyExisting
	/// If new variable's name is found in another variable.
	/// @see parseName
	/// @see parseInChar
	/// @brief Inserts a new char variable in the configuration instance
	void insertChar (const string& name, const char& val);
	
	/// Returns a configuration of some existing variable in the
	/// configuration instance.
	/// @param name String with the variable's name.
	/// @return The value of the required configuration type variable.
	/// @brief Access method to a configuration type variable
	const Configuration& getConfig (const string& name);
private:
	/// Returns the "true" (non-parsed) value of an existing variable.
	/// @param name String with the variable's name.
	/// @return Non-parsed variable's value.
	/// @brief Returns non-parsed variable value
	const string& get (const string& name);
public:
	/// Returns a string value of some existing variable in the
	/// configuration instance using the string source and the parser's
	/// method.
	/// @param name String with the variable's name.
	/// @return The value of the required variable.
	/// @see get
	/// @see parseOutStr
	/// @brief Access method to a string variable
	string getStr (const string& name);
	
	/// Returns a real number (long double) value of some existing variable
	/// in the configuration instance using the string source and the
	/// parser's method.
	/// @param name String with the variable's name.
	/// @return The value of the required variable.
	/// @see get
	/// @see parseOutReal
	/// @brief Access method to a real number variable
	long double getReal (const string& name);
	
	/// Returns an integer number (long int) value of some existing variable
	/// in the configuration instance using the string source and the
	/// parser's method.
	/// @param name String with the variable's name.
	/// @return The value of the required variable.
	/// @see get
	/// @see parseOutInt
	/// @brief Access method to an integer number variable
	long int getInt (const string& name);
	
	/// Returns a character (char) value of some existing variable in the
	/// configuration instance using the string source and the parser's
	/// method.
	/// @param name String with the variable's name.
	/// @return The value of the required variable.
	/// @see get
	/// @see parseOutChar
	/// @brief Access method to a character variable
	char getChar (const string& name);
	
	/// Sets the value with "val" parameter of config. type variable named
	/// with "name" parameter.
	/// @param name String with the variable's name.
	/// @param val New value to be assigned.
	/// @brief Modifier method to a configuration type variable
	void setConfig (const string& name, const Configuration& val);
	
	/// Sets the value with "val" parameter of string variable named with
	/// "name" parameter.
	/// @param name String with the variable's name.
	/// @param val New value to be assigned.
	/// @see parseInStr
	/// @brief Modifier method to a string variable
	void setStr (const string& name, const string& val);
	
	/// Sets the value with "val" parameter of real num. variable named with
	/// "name" parameter.
	/// @param name String with the variable's name.
	/// @param val New value to be assigned.
	/// @see parseInReal
	/// @brief Modifier method to a real number variable
	void setReal (const string& name, const long double& val);
	
	/// Sets the value with "val" parameter of int num. variable named with
	/// "name" parameter.
	/// @param name String with the variable's name.
	/// @param val New value to be assigned.
	/// @see parseInInt
	/// @brief Modifier method to an integer number variable
	void setInt (const string& name, const long int& val);
	
	/// Sets the value with "val" parameter of character variable named with
	/// "name" parameter.
	/// @param name String with the variable's name.
	/// @param val New value to be assigned.
	/// @see parseInChar
	/// @brief Modifier method to a character variable
	void setChar (const string& name, const char& val);
	
	/// Erases a configuration type variable named with "name" parameter.
	/// @param name String with the configuration type variable's name.
	/// @brief Erases a configuration type variable
	void eraseSub (const string& name);
	
	/// Erases a data variable named with "name" parameter.
	/// @param name String with the data variable's name.
	/// @brief Erases a data variable
	void erase (const string& name);
};

#endif
