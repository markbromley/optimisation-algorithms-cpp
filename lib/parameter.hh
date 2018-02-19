#ifndef PARAMETER_HH
#define PARAMETER_HH

#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <utility>
#include <vector>


/**
 *  \class ConfigParameters
 *
 *  \brief class for accessing parameters from files and command line
 *
 *  The parameters are specified in a
 *  file with one key-value pair per line for example:
 \code{.unparsed}

  key1:value1
  key2:value2
  key3:value3

 \endcode
 *  Note that there are no spaces either side of the colon.
 */
class ConfigParameters
{
  typedef std::map< std::string, std::string > ParamDataType;

public:
    explicit ConfigParameters(){}

    explicit ConfigParameters( const std::string& filename)
    { 
        merge_file(filename);
    }

    void merge_file(const std::string& filename)
    {
        std::ifstream file;
        file.open( filename.c_str() );
        if( !file.is_open() ){ 
            throw "invalid parameter file";
        }
        else
        {
           merge(file);
        }
        file.close();
    }

    void merge(std::ifstream& file)
    {
        read_config_file( file );
        file.close();
    }

  /**
   * \brief set parameter with key
   */
    void set( const std::string& key, const std::string& value)
    {
        data_[key]=value;
    }

  /**
   * \brief get parameter with key
   */
    template< class T >
    T get( const std::string& key ) const
    {
        // get value from data
        const auto valueIt = data_.find( key );

        if( valueIt == data_.end() )
        {
          // "Can not find key"
           throw std::exception();
        }

        // extract data
        const std::string value = valueIt->second;

        // assign to string stream
        std::stringstream ss;
        ss << value;

        // extract from string stream
        T retValue;
        ss >> retValue;

        return retValue;
    }


    std::string getString( const std::string& key ) const
    {
        // get value from data
        const auto valueIt = data_.find( key );

        if( valueIt == data_.end() )
        {
            throw "unable to find key: " + key;
        }

        // extract data
        const std::string value = valueIt->second;
        return value;
    }

    /**
   * \brief add parameter (from command line arguments)
   */
    void add( const char* in )
    {
        std::istringstream is_line( in );
        std::string key;
        if( std::getline( is_line, key, ':' ) )
        {
            std::string value;
            if( std::getline( is_line, value ) )
            {
              store_line( key, value );
            }
        }
    }

protected:
  /**
   * \brief extract key-value pairs from stream
    * for store_line to read
   */
    void read_config_file( std::ifstream& stream )
    {
        std::string line;
        while( std::getline( stream, line) )
        {
            std::istringstream is_line(line);
            std::string key;
            if( std::getline(is_line, key, ':') )
            {
                std::string value;
                if( std::getline(is_line, value) )
                {
                    store_line(key, value);
                }
            }
        }
    }

  /**
   * \brief stores key-value pairs, over writes where necessary
   */
    void store_line( const std::string &key, const std::string& value )
    {
        auto it = data_.insert( std::pair< std::string, std::string >( key, value ) );

        if( it.second == true )
        {
            return;
        }

        // otherwise over writing
        it.first->second = value;
    }

private:
  ParamDataType data_;
};

#endif // #ifndef PARAMETER_HH
