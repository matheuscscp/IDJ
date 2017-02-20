/// @ingroup MOD_CONFIGFILE
/// @file configfile.cpp
/// @brief Implementation of the Configuration File Parser methods
/// @author Matheus Pimenta

#include "configfile.hpp"

// =============================================================================
// Configuration Class Member (Variable)
// =============================================================================

template <class T>
Configuration::Variable< T >::Variable (const string& name, const T& value) :
name_ ( name ), value_ ( value )
{
}

template <class T>
Configuration::Variable< T >::~Variable ()
{
}

template <class T>
const string& Configuration::Variable< T >::name () const
{
	return name_;
}

template <class T>
const T& Configuration::Variable< T >::value () const
{
	return value_;
}

template <class T>
void Configuration::Variable< T >::set (const T& value)
{
	value_ = value;
}

// =============================================================================
// Configuration File Parser Exceptions
// =============================================================================

Configuration::FileParserInvalidLine::FileParserInvalidLine
(unsigned int line) :
line_ ( line )
{
}

const unsigned int& Configuration::FileParserInvalidLine::line ()
{
	return line_;
}

Configuration::FParserValAssignmentMissing::FParserValAssignmentMissing
(unsigned int line) :
FileParserInvalidLine ( line )
{
}

Configuration::FParserVarNameInvalidToken::FParserVarNameInvalidToken
(unsigned int line) :
FileParserInvalidLine ( line )
{
}

Configuration::FParserClosingBraceMissing::FParserClosingBraceMissing
(unsigned int line) :
FileParserInvalidLine ( line )
{
}

// =============================================================================
// File Parser Class
// =============================================================================

Configuration::Parser::FileParser::FileParser (	const string& filename,
						Configuration* dest	)
{
	file_.open ( filename.c_str () );
	
	if ( !file_.is_open () )
		throw ( FileNotFound () );
	
	dest_.push ( dest );
	names_.push ( string () );
	
	parse ();
	
	file_.close ();
}

Configuration::Parser::FileParser::~FileParser ()
{
}

void Configuration::Parser::FileParser::parse ()
{
	line_ = 1;
	state_ = WAITING_NAME;
	
	// avoiding exceptions for empty files by setting eofbit
	input_ = file_.get ();
	
	while ( !file_.eof () )
	{
		switch ( state_ )
		{
			case WAITING_NAME:
				handleWaitingName ();
				break;
			
			case READING_NAME:
				handleReadingName ();
				break;
			
			case W8ING_EQLS_OR_BRACE:
				handleW8ingEqlsOrBrace ();
				break;
			
			case W8ING_OPENING_BRACE:
				handleW8ingOpeningBrace ();
				break;
			
			case WAITING_VALUE:
				handleWaitingValue ();
				break;
			
			case READING_VALUE:
				handleReadingValue ();
				break;
			
			case W8ING_NAME_OR_BRACE:
				handleW8ingNameOrBrace ();
				break;
			
			default:
				break;
		}
		
		if ( !file_.eof () )
			input_ = file_.get ();
		
		if ( file_.eof () )
			handleEof ();
	}
}

void Configuration::Parser::FileParser::insertConfigVar ()
{
	Configuration* tmp = dest_.top ();
	dest_.pop ();
	names_.pop ();
	
	try {
		dest_.top ()->insertConfig ( names_.top (), *tmp );
	}
	catch (VarAlreadyExisting& e)
	{
		dest_.top ()->setConfig ( names_.top (), *tmp );
	}
	
	delete tmp;
}

void Configuration::Parser::FileParser::insertDataVar ()
{
	try {
		dest_.top ()->insertStr ( names_.top (), value_ );
	}
	catch (VarAlreadyExisting& e)
	{
		dest_.top ()->setStr ( names_.top (), value_ );
	}
}

void Configuration::Parser::FileParser::avoidLine ()
{
	for ( bool end = false; ( !file_.eof () ) && ( !end ); )
	{
		input_ = file_.get ();
		
		if ( ( input_ == '\r' ) || ( input_ == '\n' ) )
		{
			end = true;
			handleLineBreak ();
		}
	}
}

void Configuration::Parser::FileParser::changeToW8ingNameState ()
{
	if ( dest_.size () == 1 )
		state_ = WAITING_NAME;
	else
		state_ = W8ING_NAME_OR_BRACE;
}

void Configuration::Parser::FileParser::handleLineBreak ()
{
	line_++;
	
	// avoiding Windows/DOS LF, or setting eofbit
	input_ = file_.get ();
	if ( !file_.eof () )
	{
		if ( input_ != '\n' )
			file_.seekg ( -1, ios::cur );
	}
}

void Configuration::Parser::FileParser::handleWaitingName ()
{
	if (	( input_ == '_' ) ||
		( ( input_ >= 'A' ) && ( input_ <= 'Z' ) ) ||
		( ( input_ >= 'a' ) && ( input_ <= 'z' ) )	)
	{
		names_.top () = input_;
		state_ = READING_NAME;
	}
	else if ( input_ == '#' )
		avoidLine ();
	else if ( ( input_ == '\r' ) || ( input_ == '\n' ) )
		handleLineBreak ();
	else if ( ( input_ != ' ' ) && ( input_ != '\t' ) )
		throw ( FParserVarNameInvalidToken ( line_ ) );
}

void Configuration::Parser::FileParser::handleReadingName ()
{
	if (	( input_ == '_' ) ||
		( ( input_ >= '0' ) && ( input_ <= '9' ) ) ||
		( ( input_ >= 'A' ) && ( input_ <= 'Z' ) ) ||
		( ( input_ >= 'a' ) && ( input_ <= 'z' ) )	)
		names_.top () += input_;
	else if ( ( input_ == ' ' ) || ( input_ == '\t' ) )
		state_ = W8ING_EQLS_OR_BRACE;
	else if ( ( input_ == '\r' ) || ( input_ == '\n' ) )
	{
		handleLineBreak ();
		state_ = W8ING_OPENING_BRACE;
	}
	else if ( input_ == '=' )
		state_ = WAITING_VALUE;
	else if ( input_ == '{' )
	{
		dest_.push ( new Configuration () );
		names_.push ( string () );
		state_ = W8ING_NAME_OR_BRACE;
	}
	else
		throw ( FParserVarNameInvalidToken ( line_ ) );
}

void Configuration::Parser::FileParser::handleW8ingEqlsOrBrace ()
{
	if ( input_ == '=' )
		state_ = WAITING_VALUE;
	else if ( input_ == '{' )
	{
		dest_.push ( new Configuration () );
		names_.push ( string () );
		state_ = W8ING_NAME_OR_BRACE;
	}
	else if ( input_ == '#' )
	{
		avoidLine ();
		state_ = W8ING_OPENING_BRACE;
	}
	else if ( ( input_ == '\r' ) || ( input_ == '\n' ) )
	{
		handleLineBreak ();
		state_ = W8ING_OPENING_BRACE;
	}
	else if ( ( input_ != ' ' ) && ( input_ != '\t' ) )
		throw ( FParserValAssignmentMissing ( line_ ) );
}

void Configuration::Parser::FileParser::handleW8ingOpeningBrace ()
{
	if ( input_ == '{' )
	{
		dest_.push ( new Configuration () );
		names_.push ( string () );
		state_ = W8ING_NAME_OR_BRACE;
	}
	else if ( input_ == '#' )
		avoidLine ();
	else if ( ( input_ == '\r' ) || ( input_ == '\n' ) )
		handleLineBreak ();
	else if ( ( input_ != ' ' ) && ( input_ != '\t' ) )
		throw ( FParserValAssignmentMissing ( line_ ) );
}

void Configuration::Parser::FileParser::handleWaitingValue ()
{
	if ( input_ == '#' )
	{
		value_ = "";
		insertDataVar ();
		
		avoidLine ();
		
		changeToW8ingNameState ();
	}
	else if ( ( input_ == '\r' ) || ( input_ == '\n' ) )
	{
		value_ = "";
		insertDataVar ();
		
		handleLineBreak ();
		
		changeToW8ingNameState ();
	}
	else if ( ( input_ != ' ' ) && ( input_ != '\t' ) )
	{
		if ( ( input_ == '}' ) && ( dest_.size () > 1 ) )
		{
			value_ = "";
			insertDataVar ();
			
			insertConfigVar ();
			
			changeToW8ingNameState ();
		}
		else
		{
			spaces_ = "";
			value_ = input_;
			
			state_ = READING_VALUE;
		}
	}
}

void Configuration::Parser::FileParser::handleReadingValue ()
{
	if ( ( input_ == ' ' ) || ( input_ == '\t' ) )
	{
		if ( spaces_.size () > 0 )
			spaces_ += input_;
		else
			spaces_ = input_;
	}
	else if ( ( input_ == '\r' ) || ( input_ == '\n' ) )
	{
		insertDataVar ();
		
		handleLineBreak ();
		
		changeToW8ingNameState ();
	}
	else if ( input_ == '#' )
	{
		if ( spaces_.size () > 0 )
		{
			insertDataVar ();
			
			avoidLine ();
			
			changeToW8ingNameState ();
		}
		else
			value_ += input_;
	}
	else if ( input_ == '}' )
	{
		if ( spaces_.size () > 0 )
		{
			insertDataVar ();
			
			insertConfigVar ();
			
			avoidLine ();
			
			changeToW8ingNameState ();
		}
		else
			value_ += input_;
	}
	else
	{
		if ( spaces_.size () > 0 )
		{
			value_ += spaces_;
			spaces_ = "";
		}
		value_ += input_;
	}
}

void Configuration::Parser::FileParser::handleW8ingNameOrBrace ()
{
	if ( input_ == '}' )
	{
		insertConfigVar ();
		
		changeToW8ingNameState ();
	}
	else
		handleWaitingName ();
}

void Configuration::Parser::FileParser::handleEof ()
{
	switch ( state_ )
	{
		case READING_NAME:
		case W8ING_EQLS_OR_BRACE:
		case W8ING_OPENING_BRACE:
			throw ( FParserValAssignmentMissing ( line_ ) );
			break;
		
		case WAITING_VALUE:
			if ( dest_.size () > 1 )
				throw ( FParserClosingBraceMissing ( line_ ) );
			value_ = "";
			insertDataVar ();
			break;
		
		case READING_VALUE:
			if ( dest_.size () > 1 )
				throw ( FParserClosingBraceMissing ( line_ ) );
			insertDataVar ();
			break;
		
		case W8ING_NAME_OR_BRACE:
			throw ( FParserClosingBraceMissing ( line_ ) );
			break;
		
		default:
			break;
	}
}

// =============================================================================
// Configuration Parser Methods
// =============================================================================

void Configuration::Parser::parseFile (	const string& filename,
					Configuration* dest	)
{
	FileParser ( filename, dest );
}

void Configuration::Parser::parseName (const string& src)
{
	size_t src_size = src.size ();
	
	if ( src_size == 0 )
		throw ( SParserInvalidName () );
	else if ( !(	( src[0] == '_' ) ||
			( ( src[0] >= 'A' ) && ( src[0] <= 'Z' ) ) ||
			( ( src[0] >= 'a' ) && ( src[0] <= 'z' ) )	) )
		throw ( SParserInvalidName () );
	
	for ( size_t i = 1; i < src_size; i++ )
	{
		
		if ( !(	( src[i] == '_' ) ||
			( ( src[i] >= '0' ) && ( src[i] <= '9' ) ) ||
			( ( src[i] >= 'A' ) && ( src[i] <= 'Z' ) ) ||
			( ( src[i] >= 'a' ) && ( src[i] <= 'z' ) )	) )
			throw ( SParserInvalidName () );
	}
}

string Configuration::Parser::parseInStr (const string& src)
{
	string s = "";
	
	stringstream ss;
	ss << src;
	char input = ss.get ();
	
	while ( !ss.eof () )
	{
		if (	( input != '#' ) &&
			( input != '}' )	)
		{
			switch ( input )
			{
				case '\t': s += "\\t"; break;
				case '\r': s += "\\r"; break;
				case '\n': s += "\\n"; break;
				case '\0': s += "\\0"; break;
				default:   s += input; break;
			}
		}
		else if ( input == '#' )
		{
			if ( s[ s.size () - 1 ] != '\\' )
				s += "\\#";
			else
				s += '#';
		}
		else
		{
			if ( s[ s.size () - 1 ] != '\\' )
				s += "\\}";
			else
				s += '}';
		}
		
		input = ss.get ();
	}
	
	return s;
}

string Configuration::Parser::parseInReal (const long double& src)
{
	stringstream ss;
	ss << src;
	return ss.str ();
}

string Configuration::Parser::parseInInt (const long int& src)
{
	stringstream ss;
	ss << src;
	return ss.str ();
}

string Configuration::Parser::parseInChar (const char& src)
{
	string s;
	
	switch ( src )
	{
		case '\t': s = "\\t"; break;
		case '\r': s = "\\r"; break;
		case '\n': s = "\\n"; break;
		case '\0': s = "\\0"; break;
		case '#':  s = "\\#"; break;
		case '}':  s = "\\}"; break;
		default:   s = src;   break;
	}
	
	return s;
}

string Configuration::Parser::parseOutStr (const string& src)
{
	string s = "";
	
	stringstream ss;
	ss << src;
	char input = ss.get ();
	char tmp;
	
	while ( !ss.eof () )
	{
		if ( input != '\\' )
			s += input;
		else
		{
			tmp = ss.get ();
			if ( ss.eof () )
				s += input;
			else if ( tmp == 't' )
				s += '\t';
			else if ( tmp == 'r' )
				s += '\r';
			else if ( tmp == 'n' )
				s += '\n';
			else if ( tmp == '0' )
				s += '\0';
			else if ( tmp == '#' )
				s += '#';
			else if ( tmp == '}' )
				s += '}';
			else
			{
				s += input;
				ss.seekg ( -1, ios::cur );
			}
		}
		
		if ( !ss.eof () )
			input = ss.get ();
	}
	
	return s;
}

long double Configuration::Parser::parseOutReal (const string& src)
{
	stringstream ss;
	long double tmp;
	
	ss << src;
	ss >> tmp;
	
	if ( ( !ss.eof () ) || ( ss.fail () ) )
		throw ( SParserInvConversionReal () );
	
	return tmp;
}

long int Configuration::Parser::parseOutInt (const string& src)
{
	stringstream ss;
	long int tmp;
	
	ss << src;
	ss >> tmp;
	
	if ( ( !ss.eof () ) || ( ss.fail () ) )
		throw ( SParserInvConversionInt () );
	
	return tmp;
}

char Configuration::Parser::parseOutChar (const string& src)
{
	if ( src.size () != 1 )
	{
		if ( src.size () != 2 )
			throw ( SParserInvConversionChar () );
		else if ( src[0] == '\\' )
		{
			if ( src[1] == 't' )
				return '\t';
			else if ( src[1] == 'r' )
				return '\r';
			else if ( src[1] == 'n' )
				return '\n';
			else if ( src[1] == '0' )
				return '\0';
			else if ( src[1] == '#' )
				return '#';
			else if ( src[1] == '}' )
				return '}';
			else
				throw ( SParserInvConversionChar () );
		}
		else
			throw ( SParserInvConversionChar () );
	}
	
	return src[0];
}

// =============================================================================
// Configuration Methods
// =============================================================================

Configuration::Configuration ()
{
}

Configuration::~Configuration ()
{
}

const vector< Configuration::ConfigVariable >&
Configuration::configVars () const
{
	return cvars;
}

const vector< Configuration::DataVariable >& Configuration::dataVars () const
{
	return vars;
}

void Configuration::readTxt (const string& filename)
{
	parser.parseFile ( filename, this );
}

void Configuration::readBin_ (fstream& f, Configuration& dest)
{
	size_t tmp1, tmp2;
	char* s;
	string name;
	Configuration buf;
	string value;
	
	// reading all Configuration type variables
	f.read ( (char*) &tmp1, sizeof ( tmp1 ) );
	for ( size_t i = 0; i < tmp1; i++ )
	{
		// reading the size and then the name itself
		f.read ( (char*) &tmp2, sizeof ( tmp2 ) );
		s = new char[ tmp2 + 1 ];
		f.read ( s, tmp2 );
		s[ tmp2 ] = '\0';
		name = s;
		delete[] s;
		
		readBin_ ( f, buf );
		
		try {
			dest.insertConfig ( name, buf );
		}
		catch (VarAlreadyExisting& e)
		{
			dest.setConfig ( name, buf );
		}
	}
	
	// reading all data variables
	f.read ( (char*) &tmp1, sizeof ( tmp1 ) );
	for ( size_t i = 0; i < tmp1; i++ )
	{
		// reading the size and then the name itself
		f.read ( (char*) &tmp2, sizeof ( tmp2 ) );
		s = new char[ tmp2 + 1 ];
		f.read ( s, tmp2 );
		s[ tmp2 ] = '\0';
		name = s;
		delete[] s;
		
		// reading the size and then the value itself of data variable
		f.read ( (char*) &tmp2, sizeof ( tmp2 ) );
		s = new char[ tmp2 + 1 ];
		f.read ( s, tmp2 );
		s[ tmp2 ] = '\0';
		value = s;
		delete[] s;
		
		try {
			dest.insertStr ( name, value );
		}
		catch (VarAlreadyExisting& e)
		{
			dest.setStr ( name, value );
		}
	}
}

void Configuration::readBin (const string& filename)
{
	fstream f ( filename.c_str (), fstream::in | fstream::binary );
	
	if ( !f.is_open () )
		throw ( FileNotFound () );
	
	readBin_ ( f, *this );
	
	f.close ();
}

void Configuration::writeTxt_ (	fstream& f,
				const Configuration& src,
				unsigned short tab	)
{
	string inden = "";
	for ( unsigned short i = 0; i < tab; i++ )
		inden += '\t';
	
	// writing all the Configuration type variables
	for ( size_t i = 0; i < src.configVars ().size (); i++ )
	{
		f << inden;
		f << src.configVars ()[i].name ();
		f << "\r\n";
		f << inden;
		f << "{\r\n";
		writeTxt_ ( f, src.configVars ()[i].value (), tab + 1 );
		f << inden;
		f << "}\r\n";
		
		// extra line break to separate blocks, if the last block
		// written isn't the last variable of its configuration instance
		if ( !(	( i + 1 == src.configVars ().size () ) &&
			( !src.dataVars ().size () )	) )
		{
			f << inden;
			f << "\r\n";
		}
	}
	
	// writing all the data variables
	for ( size_t i = 0; i < src.dataVars ().size (); i++ )
	{
		f << inden;
		f << src.dataVars ()[i].name ();
		f << " = ";
		f << src.dataVars ()[i].value ();
		f << "\r\n";
	}
}

void Configuration::writeTxt (const string& filename)
{
	fstream f ( filename.c_str (), fstream::out );
	
	writeTxt_ ( f, *this, 0 );
	
	f.close ();
}

void Configuration::writeBin_ (fstream& f, const Configuration& src)
{
	size_t tmp1, tmp2;
	
	// writing all the Configuration type variables
	tmp1 = src.configVars ().size ();
	f.write ( (const char*) &tmp1, sizeof ( tmp1 ) );
	for ( size_t i = 0; i < tmp1; i++ )
	{
		// writing the size and the name itself
		tmp2 = src.configVars ()[i].name ().size ();
		f.write ( (const char*) &tmp2, sizeof ( tmp2 ) );
		f.write ( src.configVars ()[i].name ().c_str (), tmp2 );
		
		writeBin_ ( f, src.configVars ()[i].value () );
	}
	
	// writing all the data variables
	tmp1 = src.dataVars ().size ();
	f.write ( (const char*) &tmp1, sizeof ( tmp1 ) );
	for ( size_t i = 0; i < tmp1; i++ )
	{
		// writing the size and the name itself
		tmp2 = src.dataVars ()[i].name ().size ();
		f.write ( (const char*) &tmp2, sizeof ( tmp2 ) );
		f.write ( src.dataVars ()[i].name ().c_str (), tmp2 );
		
		// writing the size and the value itself
		tmp2 = src.dataVars ()[i].value ().size ();
		f.write ( (const char*) &tmp2, sizeof ( tmp2 ) );
		f.write ( src.dataVars ()[i].value ().c_str (), tmp2 );
	}
}

void Configuration::writeBin (const string& filename)
{
	fstream f ( filename.c_str (), fstream::out | fstream::binary );
	
	writeBin_ ( f, *this );
	
	f.close ();
}

void Configuration::clear ()
{
	cvars.clear ();
	vars.clear ();
}

size_t Configuration::size () const
{
	return ( cvars.size () + vars.size () );
}

template <class T>
void Configuration::find (long int& mid, const T& vec, const string& name)
{
	long int beg = 0;
	long int end = ( ( (long int) vec.size () ) - 1 );
	bool found = false;
	
	mid = 0;
	
	while ( ( beg <= end ) && ( !found ) )
	{
		mid = ( beg + end ) / 2;
		
		if ( vec[ mid ].name () < name )
		{
			beg = mid + 1;
			mid = beg;
		}
		else if ( vec[ mid ].name () > name )
			end = mid - 1;
		else
			found = true;
	}
	
	if ( !found )
		throw ( VarNotFound () );
}

void Configuration::insertConfig (const string& name, const Configuration& val)
{
	long int i;
	
	if ( &val == this )
		throw ( ConfInsertingItself () );
	
	try {
		find ( i, cvars, name );
		
		throw ( VarAlreadyExisting () );
	}
	catch (VarNotFound& e)
	{
		parser.parseName ( name );
		
		cvars.insert
		(
			cvars.begin () + i,
			ConfigVariable ( name, val )
		);
	}
}

void Configuration::insertStr (const string& name, const string& val)
{
	long int i;
	
	try {
		find ( i, vars, name );
		
		throw ( VarAlreadyExisting () );
	}
	catch (VarNotFound& e)
	{
		parser.parseName ( name );
		
		vars.insert
		(
			vars.begin () + i,
			DataVariable ( name, parser.parseInStr ( val ) )
		);
	}
}

void Configuration::insertReal (const string& name, const long double& val)
{
	long int i;
	
	try {
		find ( i, vars, name );
		
		throw ( VarAlreadyExisting () );
	}
	catch (VarNotFound& e)
	{
		parser.parseName ( name );
		
		vars.insert
		(
			vars.begin () + i,
			DataVariable ( name, parser.parseInReal ( val ) )
		);
	}
}

void Configuration::insertInt (const string& name, const long int& val)
{
	long int i;
	
	try {
		find ( i, vars, name );
		
		throw ( VarAlreadyExisting () );
	}
	catch (VarNotFound& e)
	{
		parser.parseName ( name );
		
		vars.insert
		(
			vars.begin () + i,
			DataVariable ( name, parser.parseInInt ( val ) )
		);
	}
}

void Configuration::insertChar (const string& name, const char& val)
{
	long int i;
	
	try {
		find ( i, vars, name );
		
		throw ( VarAlreadyExisting () );
	}
	catch (VarNotFound& e)
	{
		parser.parseName ( name );
		
		vars.insert
		(
			vars.begin () + i,
			DataVariable ( name, parser.parseInChar ( val ) )
		);
	}
}

const Configuration& Configuration::getConfig (const string& name)
{
	long int i;
	
	find ( i, cvars, name );
	
	return cvars[i].value ();
}

const string& Configuration::get (const string& name)
{
	long int i;
	
	find ( i, vars, name );
	
	return vars[i].value ();
}

string Configuration::getStr (const string& name)
{
	return parser.parseOutStr ( get ( name ) );
}

long double Configuration::getReal (const string& name)
{
	return parser.parseOutReal ( get ( name ) );
}

long int Configuration::getInt (const string& name)
{
	return parser.parseOutInt ( get ( name ) );
}

char Configuration::getChar (const string& name)
{
	return parser.parseOutChar ( get ( name ) );
}

void Configuration::setConfig (const string& name, const Configuration& val)
{
	long int i;
	
	find ( i, cvars, name );
	
	cvars[i].set ( val );
}

void Configuration::setStr (const string& name, const string& val)
{
	long int i;
	
	find ( i, vars, name );
	
	vars[i].set ( parser.parseInStr ( val ) );
}

void Configuration::setReal (const string& name, const long double& val)
{
	long int i;
	
	find ( i, vars, name );
	
	vars[i].set ( parser.parseInReal ( val ) );
}

void Configuration::setInt (const string& name, const long int& val)
{
	long int i;
	
	find ( i, vars, name );
	
	vars[i].set ( parser.parseInInt ( val ) );
}

void Configuration::setChar (const string& name, const char& val)
{
	long int i;
	
	find ( i, vars, name );
	
	vars[i].set ( parser.parseInChar ( val ) );
}

void Configuration::eraseSub (const string& name)
{
	long int i;
	
	find ( i, cvars, name );
	
	cvars.erase ( cvars.begin () + i );
}

void Configuration::erase (const string& name)
{
	long int i;
	
	find ( i, vars, name );
	
	vars.erase ( vars.begin () + i );
}
