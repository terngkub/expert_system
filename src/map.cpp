#include "map.hpp"

map::map(std::string file) : filename(file)
{
}

map::map(const map & copy)
{
    *this = copy;
}

map & map::operator=(const map & var)
{
    if (this == &var)
    {
        /* to complete */
    }
    return (*this);
}

map::~map()
{

}

/* --------------------- Getter --------------------- */

int     getFact(int i)
{
    return (0);
}

/* --------------------- Setter --------------------- */
void    setFact(int state, char fact)
{
    return ;
}

/* --------------------- Others --------------------- */

/*
    read FILENAME and parse it to insert FACTS with VALUE
    Default value are FALSE:
*/
void    map::CreateMap()
{
    if (filename.empty())
        throw(std::runtime_error("Filename Cannnot be Empty"));
    
    std::ifstream   infile;
    infile.open(filename);
    if (infile.fail())
        throw(std::runtime_error("Failed to open file:" + filename));

    std::string     line;
    while(std::getline(infile, line))
    {
        chooseActions(line);
    }
}

/*
    Define Lines Behavior
    if comment, rules or or facts
*/
void    map::chooseActions(std::string line)
{
    std::string::iterator it = line.begin();
    std::string::iterator itend = line.end();

    while (it != itend && isspace(*it))
        it++;
    if (it != itend && *it == '#')
        return ;
    
}