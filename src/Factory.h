#ifndef LAB_2_FACTORY_H
#define LAB_2_FACTORY_H

#include <map>
#include <memory>
#include "Converter.h"


template<typename Converter_T, typename ID_T, typename Creator_T>
class Factory {
public:
    static Factory* getInstance() {
        static Factory f;
        return &f;
    }

    std::unique_ptr<Converter_T> create_converter(const ID_T& name) {
        auto st =  create_func.find(name);
        if (st == create_func.end()) {
            return nullptr;
        }
        return std::unique_ptr<Converter_T>(st->second());
    }

    bool register_converter(const ID_T& type, Creator_T creator) {
        create_func[type] = creator;
        return true;
    }


private:
    std::map<ID_T, Creator_T> create_func;
};
using Factory_Converter = Factory<Converter, std::string, Converter* (*)()>;

#endif
