#ifndef MAP_HPP
#define MAP_HPP

#include <map> /* For std::map container */
#include <string> /* For std::string */
#include <fstream> /* For File input, used int CreateMap */
#include <exception>
class map
{
    private:
        std::map<char, int> ArrayOfFacts;
        std::string         filename;

    public:
        /* Coplien form */
        map(std::string file);
        map(const map & copy);
        map & operator=(const map & var);
        ~map();

        /* Getter */
        int             getFact(int iterator);

        /* Setter */
        void            setFact(int state, char fact);

        /* Others */
        void            CreateMap();
        void            chooseActions(std::string line);
};

#endif